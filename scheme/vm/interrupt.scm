; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.
; Code for handling interrupts.

; New interrupt handler vector in *val*

(define-opcode set-interrupt-handlers!
  (cond ((or (not (vm-vector? *val*))
	     (< (vm-vector-length *val*) interrupt-count))
	 (raise-exception wrong-type-argument 0 *val*))
	(else
	 (let ((temp *interrupt-handlers*))
	   (set! *interrupt-handlers* *val*)
	   (set! *val* temp)
	   (goto interpret)))))

; New interrupt mask as fixnum in *val*

(define-opcode set-enabled-interrupts!
  (let ((old *enabled-interrupts*))
    (set-enabled-interrupts! (extract-fixnum *val*))
    (set! *val* (enter-fixnum old))
    (goto interpret)))

(define (handle-interrupt)
  (save-interrupt-state)
  (goto call-interrupt-handler (get-highest-priority-interrupt!)))

; Should this use the stack's exception space?
; And doesn't the caller know the number of arguments on the stack?

(define (save-interrupt-state)
  (let* ((args-on-stack (arguments-on-stack))
	 (key (ensure-stack-space stack-continuation-size ensure-space)))
    (push *val*)
    (push *template*)
    (push (enter-fixnum *nargs*))
    (push (enter-fixnum *enabled-interrupts*))
    (set-template! *interrupt-template* (enter-fixnum 0))
    (push-continuation! *code-pointer* (+ args-on-stack 5) key)))

; For alarm interrupts the interrupted template is passed to the handler
;  for use by code profilers.
; For gc interrupts we push the list of things to be finalized.
; For i/o-completion we push the channel and its status.

(define (call-interrupt-handler pending-interrupt)
  (set! *nargs* (push-interrupt-args pending-interrupt))
  (if (not (vm-vector? *interrupt-handlers*))
      (error "interrupt handler is not a vector"))
  (set-enabled-interrupts! 0)
  (set! *val* (vm-vector-ref *interrupt-handlers* pending-interrupt))
  (if (not (closure? *val*))
      (error "interrupt handler is not a closure" pending-interrupt))
  (goto perform-application 0))

(define (push-interrupt-args pending-interrupt)
  (cond ((eq? pending-interrupt (enum interrupt alarm))
	 (push *interrupted-template*)
	 (set! *interrupted-template* false)
	 (push (enter-fixnum *enabled-interrupts*))
	 2)
	((eq? pending-interrupt (enum interrupt post-gc))
	 (push *finalize-these*)
	 (push (enter-fixnum *enabled-interrupts*))
	 (set! *finalize-these* null)
	 2)
	((eq? pending-interrupt (enum interrupt i/o-completion))
	 (let ((channel (dequeue-channel!)))
	   (if (not (channel-queue-empty?))
	       (note-interrupt! (enum interrupt i/o-completion)))
	   (push channel)
	   (push (channel-os-status channel))
	   (push (enter-fixnum *enabled-interrupts*))
	   3))
	(else
	 (push (enter-fixnum *enabled-interrupts*))
	 1)))

(define-opcode return-from-interrupt
  (set-enabled-interrupts! (extract-fixnum (pop)))
  (set! *nargs* (extract-fixnum (pop)))
  (set-template! (pop) (enter-fixnum 0)) ; always called with a fresh template
  (set! *val* (pop))
  (goto interpret))

; Do nothing much until something happens.  To avoid race conditions this
; opcode is called with all interrupts disabled, so it has to return if
; any interrupt occurs, even a disabled one.

(define-primitive wait (fixnum-> boolean->)
  (lambda (max-wait minutes?)
    (if (not (any-pending-interrupts?))
	(wait-for-event max-wait minutes?))
    (goto return-unspecific 0)))

; The players:
;   *pending-interrupts*      A bit mask of pending interrupts
;   *enabled-interrupts*      A bit mask of enabled interrupts
;   *pending-interrupt?*      True if either an event or interrupt is pending
;   *pending-events?*         True if an event is pending
;
; When an asynchronous event occurs the OS sets *PENDING-EVENTS?* and
; *PENDING-INTERRUPT?* to true.
;
; When *PENDING-EVENTS?* is true the VM calls (CURRENT-EVENTS) to get the
; pending events.
;
; The goals of all this mucking about are:
;   - no race conditions
;   - the VM operates synchronously; only the OS is asynchronous
;   - polling only requires testing *PENDING-INTERRUPT?*

(define *pending-events?* #f)

; Called asynchronously by the OS

(define (note-event)
  (set! *pending-events?* #t)       ; order required by non-atomicity
  (set! *pending-interrupt?* #t))

; The polling procedure

(define (pending-interrupt?)
  (cond ((not *pending-interrupt?*)
	 #f)
	(*pending-events?*
	 (set! *pending-events?* #f)
	 (check-events))
	(else
	 #t)))

; Same, except we include disabled interrupts.

(define (any-pending-interrupts?)
  (or (pending-interrupt?)
      (< 0 *pending-interrupts*)))

(define (note-interrupt! interrupt)
  (set! *pending-interrupts*
	(bitwise-ior *pending-interrupts* (interrupt-bit interrupt)))
  (check-for-enabled-interrupt!))

(define (clear-interrupt! interrupt)
  (set! *pending-interrupts*
	(bitwise-and *pending-interrupts*
		     (bitwise-not (interrupt-bit interrupt))))
  (check-for-enabled-interrupt!))

(define (set-enabled-interrupts! enabled)
  (set! *enabled-interrupts* enabled)
  (check-for-enabled-interrupt!))

(define (disable-interrupts!)
  (set! *pending-interrupt?* #f)
  (set! *enabled-interrupts* 0))

(define (disable-interrupt! interrupt)
  (set! *enabled-interrupts*
	(bitwise-and *enabled-interrupts*
		     (bitwise-not (interrupt-bit interrupt)))))

(define (interrupt-bit interrupt)
  (shift-left 1 interrupt))

; Whenever *PENDING-INTERRUPTS* or *ENABLED-INTERRUPTS* changes we have to
; set *PENDING-INTERRUPT?* to the correct value.

(define (check-for-enabled-interrupt!)
  (if (= 0 (bitwise-and *pending-interrupts* *enabled-interrupts*))
      (begin
	(set! *pending-interrupt?* #f)
	(if *pending-events?*
	    (set! *pending-interrupt?* #t)))
      (set! *pending-interrupt?* #t)))

; We don't need to mess with *PENDING-INTERRUPT?* because all interrupts
; are about to be disabled.

(define (get-highest-priority-interrupt!)
  (let ((n (bitwise-and *pending-interrupts* *enabled-interrupts*)))
    (let loop ((i 0) (m 1))
      (cond ((= 0 (bitwise-and n m))
	     (loop (+ i 1) (* m 2)))
	    (else
	     (set! *pending-interrupts*
		   (bitwise-and *pending-interrupts* (bitwise-not m)))
	     i)))))

(define (check-events)
  (receive (type channel status)
      (get-next-event)
    (set! *pending-interrupts*
	  (bitwise-ior (process-event type channel status)
		       *pending-interrupts*))
    (if (eq? type (enum events no-event))
	(begin
	  (check-for-enabled-interrupt!)
	  (pending-interrupt?))
	(check-events))))

(define (process-event event channel status)
  (cond ((eq? event (enum events alarm-event))
	 ;; Save the interrupted template for use by profilers.
	 (if (false? *interrupted-template*)
	     (set! *interrupted-template* *template*))
	 (interrupt-bit (enum interrupt alarm)))
	((eq? event (enum events keyboard-interrupt-event))
	 (interrupt-bit (enum interrupt keyboard)))
	((eq? event (enum events io-completion-event))
	 (enqueue-channel! channel status)
	 (interrupt-bit (enum interrupt i/o-completion)))
	((eq? event (enum events no-event))
	 0)
	((eq? event (enum events error-event))
	 (error-message "OS error while getting event")
	 (error-message (error-string status))
	 0)
	(else
	 (error-message "unknown type of event")
	 0)))
