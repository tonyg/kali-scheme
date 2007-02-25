(define-test-suite rendezvous-channels-tests)

(define-test-case channel-1 rendezvous-channels-tests
  (let ((channel (make-channel)))
    (spawn
     (lambda ()
       (sleep 500)
       (send channel 'message)))
    (check (receive channel) => 'message)))

(define-test-case channel-2 rendezvous-channels-tests
  (let ((channel (make-channel)))
    (spawn
     (lambda ()
       (send channel 'message)))
    (sleep 500)
    (check (receive channel) => 'message)))

(define-test-case channel-3 rendezvous-channels-tests
  (let ((channel (make-channel)))
    (spawn
     (lambda ()
       (let loop ((i 0))
	 (if (not (= i 1000))
	     (begin
	       (send channel i)
	       (loop (+ 1 i)))))))
    (spawn
     (lambda ()
       ;; (sleep 500)
       (let loop ((i 1000))
	 (if (not (= i 2000))
	     (begin
	       (send channel i)
	       (loop (+ 1 i)))))))

    
    (let loop ((count 0)
	       (values '()))
      (if (= count 2000)
	  (check values
		 (=> lset=)
		 (iota 2000))
	  (loop (+ 1 count 1)
		(cons (receive channel) values))))))

(define-test-case select-1 rendezvous-channels-tests
  (let ((channel-1 (make-channel))
	(channel-2 (make-channel)))
    (spawn
     (lambda ()
       (let loop ((i 0))
	 (if (not (= i 1000))
	     (begin
	       (send channel-1 i)
	       ;;(sleep 1000)
	       (loop (+ 1 i)))))))
    (spawn
     (lambda ()
       ;; (sleep 500)
       (let loop ((i 1000))
	 (if (not (= i 2000))
	     (begin
	       (send channel-2 i)
	       (loop (+ 1 i)))))))
    (sleep 500)
    (let loop ((count 0)
	       (values '()))
      (if (= count 2000)
	  (check values
		 (=> lset=)
		 (iota 2000))
	  (loop (+ 1 count 1)
		(cons (select (receive-rv channel-1)
			      (receive-rv channel-2))
		      values))))))

(define-test-case wrap-1 rendezvous-channels-tests
  (let ((channel-1 (make-channel))
	(channel-2 (make-channel)))
    (spawn
     (lambda ()
       (let loop ((i 0))
	 (if (not (= i 1000))
	     (begin
	       (send channel-1 i)
	       ;;(sleep 1000)
	       (loop (+ 1 i)))))))
    (spawn
     (lambda ()
       ;; (sleep 500)
       (let loop ((i 1000))
	 (if (not (= i 2000))
	     (begin
	       (send channel-2 i)
	       (loop (+ 1 i)))))))
    (sleep 500)
    (let loop ((count 0)
	       (values '()))
      (if (= count 2000)
	  (check values
		 (=> lset=)
		 (iota 2000))
	  (let* ((val
		  (select (wrap (receive-rv channel-1)
				(lambda (n)
				  (cons 1 n)))
			  (wrap (receive-rv channel-2)
				(lambda (n)
				  (cons 2 n)))))
		 (chan (car val))
		 (n (cdr val)))
	    (if (< n 1000)
		(check chan => 1)
		(check chan => 2))
	    (loop (+ 1 count 1)
		  (cons n values)))))))

(define (make-rv-notifier rv ack-message nack-message callback)
  (with-nack
   (lambda (nack)
     (spawn
      (lambda ()
	(sync nack)
	(callback nack-message)))
     (wrap rv
	   (lambda (ignore)
	     (callback ack-message))))))

(define-test-case with-nack-1 rendezvous-channels-tests
  (let ((channel-1 (make-channel))
	(channel-2 (make-channel))
	(message-1 #f)
	(message-2 #f))
    (spawn
     (lambda ()
       (sleep 500)
       (send channel-1 'ignore)))
    (select (make-rv-notifier (receive-rv channel-1)
			      "ch1" "not ch1"
			      (lambda (message)
				(set! message-1 message)))
	    (make-rv-notifier (receive-rv channel-2)
			      "ch2" "not ch2"
			      (lambda (message)
				(set! message-2 message))))
    (sleep 500)
    (check message-1 => "ch1")
    (check message-2 => "not ch2")))

(define-test-case with-nack-2 rendezvous-channels-tests
  (let ((channel-1 (make-channel))
	(channel-2 (make-channel))
	(message-1 #f)
	(message-2 #f))
    (spawn
     (lambda ()
       (send channel-1 'ignore)))
    (sleep 500)
    (select (make-rv-notifier (receive-rv channel-1)
			      "ch1" "not ch1"
			      (lambda (message)
				(set! message-1 message)))
	    (make-rv-notifier (receive-rv channel-2)
			      "ch2" "not ch2"
			      (lambda (message)
				(set! message-2 message))))
    (sleep 500)
    (check message-1 => "ch1")
    (check message-2 => "not ch2")))

(define-test-suite rendezvous-jars-tests)

(define-test-case take rendezvous-jars-tests
  (let ((jar (make-jar)))
    (jar-put! jar 1)
    (check (jar-take jar) => 1)
    (jar-put! jar 2)
    (check (jar-take jar) => 2)
    (jar-put! jar 3)
    (check (jar-take jar) => 3)
    (jar-put! jar 4)
    (check (jar-take jar) => 4)))

(define-test-case select rendezvous-jars-tests
  (let ((jar-1 (make-jar))
	(jar-2 (make-jar))
	(result-channel (make-channel)))
    
    (spawn
     (lambda ()
       (let ((contents (select (jar-take-rv jar-1)
			       (jar-take-rv jar-2))))
	 (send result-channel (cons 1 contents)))))
    (spawn
     (lambda ()
       (let ((contents (select (jar-take-rv jar-1)
			       (jar-take-rv jar-2))))
	 (send result-channel (cons 2 contents)))))
    (spawn
     (lambda ()
       (let ((contents (select (jar-take-rv jar-1)
			       (jar-take-rv jar-2))))
	 (send result-channel (cons 3 contents)))))
 
    (sleep 500)
    (jar-put! jar-1 17)
    (jar-put! jar-2 23)
    (let ((res-1 (receive result-channel))
	  (res-2 (receive result-channel))
	  (ensure
	   (lambda (res)
	     (check (member res '((1 . 17)
				  (2 . 17)
				  (3 . 17)
				  (1 . 23)
				  (2 . 23)
				  (3 . 23)))))))
      (ensure res-1)
      (ensure res-2))))

(define-test-case multi rendezvous-jars-tests

  (let ((jar-1 (make-jar 1))
	(jar-2 (make-jar 2)))

    (spawn
     (lambda ()
       (let loop ((i 1))
	 (if (< i 1000)
	     (begin
	       (check (jar-take jar-1) => (cons 1 (- i 1)))
	       (jar-put! jar-2 (cons 2 i))
	       (loop (+ i 1)))))))

    (jar-put! jar-2 (cons 2 0))

    (let loop ((i 0))
      (if (< i 1000)
	  (begin
	    (check (jar-take jar-2) => (cons 2 i))
	    (jar-put! jar-1 (cons 1 i))
	    (loop (+ i 1)))))))


(define-test-suite rendezvous-placeholders-tests)

(define-test-case placeholder-1 rendezvous-placeholders-tests
  (let ((placeholder-1 (make-placeholder))
	(placeholder-2 (make-placeholder))
	(results (make-channel)))
    
    
    (spawn
     (lambda ()
       (let ((contents (select (placeholder-value-rv placeholder-1)
			       (placeholder-value-rv placeholder-1))))
	 (send results contents))))
    (spawn
     (lambda ()
       (let ((contents (select (placeholder-value-rv placeholder-1)
			       (placeholder-value-rv placeholder-2))))
	 (send results contents))))
    (spawn
     (lambda ()
       (let ((contents (select (placeholder-value-rv placeholder-1)
			       (placeholder-value-rv placeholder-2))))
	 (send results contents))))
 
    (sleep 500)
    (placeholder-set! placeholder-1 17)
    (placeholder-set! placeholder-2 23)
    (let ((vals (list (receive results) (receive results) (receive results))))
      (check (lset<= vals '(17 23))))))

(define-test-suite with-nack-tests)

(define-test-case dummy with-nack-tests
  (let ((ch (make-channel)))
    (spawn
     (lambda ()
       (send ch 23)))
    (check
     (sync
      (with-nack (lambda (nack) 
		   (receive-rv ch))))
     => 23)))

(define-test-case dummy-guard with-nack-tests
  (let ((ch (make-channel)))
    (spawn
     (lambda ()
       (send ch 23)))
    (check
     (sync
      (guard (lambda ()
	       (with-nack (lambda (nack) 
			    (receive-rv ch))))))
     => 23)))


(define-test-case no-nack-1 with-nack-tests
  (let ((ch (make-channel))
	(no #f))
    (spawn
     (lambda ()
       (sleep 500)
       (send ch 23)))
    (sync
     (wrap (with-nack (lambda (nack)
			(spawn (lambda ()
				 (sync nack)
				 (set! no #t)))
			(receive-rv ch)))
	   (lambda (value)
	     (check value => 23))))
    (sleep 500)
    (check (not no))))

(define-test-case no-nack-2 with-nack-tests
  (let ((ch (make-channel))
	(no #f))
    (spawn 
     (lambda ()
       (sync
	(wrap (with-nack (lambda (nack)
			   (spawn (lambda ()
				    (sync nack)
				    (set! no 1)))
			   (with-nack (lambda (nack)
					(spawn (lambda ()
						 (sync nack)
						 (set! no 2)))
					(with-nack (lambda (nack)
						     (spawn (lambda ()
							      (sync nack)
							      (set! no 3)))
						     (receive-rv ch)))))))
	      (lambda (value) 
		(check value => 10))))))
    (sleep 10)
    (send ch 10)
    (sleep 200)
    (check no => #f)))

(define-test-case nack-1 with-nack-tests
  (let ((ch (make-channel))
	(results (make-channel)))
    (spawn 
     (lambda ()
       (select
	(receive-rv ch)
	(wrap (with-nack (lambda (nack)
			   (spawn (lambda ()
				    (sync nack)
				    (send results 1)))
			   (with-nack (lambda (nack)
					(spawn (lambda ()
						 (sync nack)
						 (send results 2)))
					(with-nack (lambda (nack)
						     (spawn (lambda ()
							      (sync nack)
							      (send results 3)))
						     (send-rv ch 'jo)))))))
	      (lambda (value) 
		(check #f))))))
    (sleep 10)
    (send ch 10)
    (let ((vals (list (receive results) (receive results) (receive results))))
      (check vals
	     (=> lset=) '(1 2 3)))))

(define-test-case no-nack-3 with-nack-tests
  (let ((ch (make-channel))
	(results (make-channel)))
    (spawn 
     (lambda ()
       (select
	(send-rv ch 'tralala)
	(with-nack (lambda (nack)
		     (spawn (lambda ()
			      (sync nack)
			      (send results 1)))
		     (guard (lambda ()
			      (with-nack (lambda (nack)
					   (spawn (lambda ()
						    (sync nack)
						    (send results 2)))
					   (send-rv ch 'jo-man)))))))
	(send-rv ch 'dudel-di-dudel))))

    (sleep 10)
    (check (memq (receive ch) '(tralala dudel-di-dudel)))
    (sleep 200)
    (check (list (receive results) (receive results))
	   (=> lset=) '(1 2))))

(define-test-case nack-2 with-nack-tests
  (let* ((ch-1 (make-channel))
	 (ch-2 (make-channel))
	 (rv-1 (receive-rv ch-1))
	 (rv-2 (receive-rv ch-2))
	 (results (make-channel)))
    (spawn 
     (lambda ()
       (select 
	(wrap (with-nack (lambda (nack)
			   (choose
			    (with-nack (lambda (nack)
					 (spawn (lambda ()
						  (sync nack)
						  (send results 'nack-1)))
					 rv-1))
			    (with-nack (lambda (nack)
					 (spawn (lambda ()
						  (send results 'nack-2)))
					 rv-2))
			    rv-1)))
	      (lambda (value)
		(send results 'rv-1)))
	(wrap rv-2 
	      (lambda (value)
		(send results 'rv-2))))))
    (sleep 10)
    (send ch-1 'jo)
    (sleep 200)
    (check
     (list (receive results) (receive results) (receive results))
     (=> lset=) '(nack-1 nack-2 rv-1))))
 	   
(define (make-channels channels)
  (let loop ((res '()) (i channels))
    (if (= i 0)
	res
	(loop (cons (cons i (make-channel)) res) (- i 1)))))

(define-test-case nack-3 with-nack-tests
  (let* ((channel-count 10)
	 (channels (make-channels channel-count))
	 (rvs (map (lambda (pair) 
		     (cons (car pair) (receive-rv (cdr pair)))) 
		   channels))
	 (results (make-channel)))

    (spawn 
     (lambda ()
       (let ((select-rvs (map (lambda (rv)
				(wrap 
				 (with-nack
				  (lambda (another-rv)
				    (spawn (lambda ()
					     (sync another-rv)))
				    (cdr rv)))
				 (lambda (val)
				   (send results val))))
			      rvs)))
	 (let loop ()
	   (apply select select-rvs)
	   (loop)))))

    (sleep 50)

    (let loop ((channels channels))
      (if (not (null? channels))
	  (let ((channel-no (car (car channels)))
		(channel (cdr (car channels))))
	    (send channel channel-no)
	    (sleep 100)
	    (check (receive results) => channel-no)
	    (loop (cdr channels)))))))

(define-test-suite cml-tests
  (rendezvous-channels-tests
   rendezvous-jars-tests
   rendezvous-placeholders-tests
   with-nack-tests))
  
  
