; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; A compact table is an encoding of a very large vector that has lots
; of recurring patterns.  It was written for encoding Unicode tables.

; The vector is partitioned into blocks, and the blocks get assembled
; into a new compressed vector.  Each time a new block gets added, the
; algorithm looks if the same block is already present in the
; compressed vector, or the compressed vector ends with a prefix of
; the new block.  In the former case, nothing needs to get added.  In
; the latter case, only the suffix needs to get added.  At the same
; time, the algorithm computes a table with indices of the block
; beginnings.

; The algorithm can take a long time; little attempt at optimization
; has been made.  It's mainly intended for offline computation as part
; of a build process.

; This tries to merge BLOCK onto REVERSE-BASE, sharing the prefix of
; BLOCK.

; returns new reverse list + index offset

(define (compact-block block reverse-base)
  (let* ((block-size (length block))
	 (base-block (reverse (take-upto reverse-base block-size)))
	 (base-block-size (length base-block)))
    (let loop ((base-block base-block)
	       (offset 0))
      (if (list-prefix? base-block block)
	  (values (append (reverse (list-tail block (- base-block-size offset)))
			  reverse-base)
		  offset)
	  (loop (cdr base-block) (+ 1 offset))))))

; GET-VALUE is a thunk that returns the next value of the input vector
; every time it gets called.  BLOCK-SIZE is the size of the blocks in
; the algorithm.

; The procedure returns two valuesthe indices vector and a vector of
; the actual values.

(define (compute-compact-table get-value block-size)
  
  (define (get-block)
    (let loop ((i 0) (rev-block '()))
      (cond
       ((>= i block-size)
	(reverse rev-block))
       ((get-value)
	=> (lambda (value)
	     (loop (+ 1 i) (cons value rev-block))))
       (else
	(reverse rev-block)))))
      
  (let loop ((reverse-values '())
	     (reverse-indices '())
	     (last-index 0)
	     ;; cache for blocks that have already shown up twice
	     ;; (reduces run time *a lot*)
	     (bingo-block-alist '()))

    (let ((block (get-block)))
      (if (< (length block) block-size)
	  (begin
	    (write block) 
	    (newline)))
      (cond
       ((null? block)
	(values (list->vector (reverse reverse-indices))
		(list->vector (reverse reverse-values))))
       ((assoc block bingo-block-alist)
	=> (lambda (pair)
	     (loop reverse-values
		   (cons (cdr pair) reverse-indices)
		   last-index
		   bingo-block-alist)))
       ((sublist-index (reverse block) reverse-values)
	=> (lambda (rev-index)
	     (loop reverse-values
		   (cons (+ (- block-size (length block)) (- last-index rev-index))
			 reverse-indices)
		   last-index
		   (cons (cons block (- last-index rev-index)) bingo-block-alist))))
       (else
	(call-with-values
	    (lambda () (compact-block block reverse-values))
	  (lambda (reverse-values offset)
	    (loop reverse-values
		  (cons (+ last-index offset) reverse-indices)
		  (+ last-index offset)
		  bingo-block-alist))))))))

; List utilities

(define (sublist-index sublist list)
  (let loop ((list list)
	     (index 0))
    (cond
     ((list-prefix? sublist list)
      index)
     ((null? list)
      #f)
     (else (loop (cdr list) (+ 1 index))))))

(define (list-prefix? list-1 list-2)
  (cond
   ((null? list-1) #t)
   ((null? list-2) #f)
   ((equal? (car list-1) (car list-2))
    (list-prefix? (cdr list-1) (cdr list-2)))
   (else #f)))

(define (take-upto list count)
  (let loop ((list list) (count count) (rev-result '()))
    (if (or (zero? count)
	    (null? list))
	(reverse rev-result)
	(loop (cdr list) (- count 1) (cons (car list) rev-result)))))

