; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Code to print out module dependencies in a format readable by the
; graph layout program AT&T DOT Release 1.0. (for information on DOT call
; the AT&T Software Technology Center Common Support Hotline (908) 582-7009)

; Follow link script up to the actual linking
;(load-configuration "scheme/interfaces.scm") 
;(load-configuration "scheme/packages.scm") 
;(flatload initial-structures) 
;(load "build/initial.scm")
;
; Load this and run it
;(load "scheme/debug/graph.scm")
;(dependency-graph (initial-packages)
; (map structure-package (list scheme-level-1 scheme-level-0))
; "graph.dot")
;
; Run the graph layout program
; setenv SDE_LICENSE_FILE /pls/local/lib/DOT/LICENSE.dot
; /pls/local/lib/DOT/dot -Tps graph.dot -o graph.ps

; Returns a list of the packages in the initial system.

(define (initial-packages)
  (map (lambda (p)
	 (structure-package (cdr p)))
       (append (struct-list scheme
			    environments
			    module-system
			    ensures-loaded
			    packages
			    packages-internal)
	       (desirable-structures))))

; Write the dependency graph found by rooting from PACKAGES to FILENAME.
; Packages in the list IGNORE are ignored.
;
; Each configuration file's packages are done as a separate subgraph.

(define (dependency-graph packages ignore filename)
  (call-with-output-file filename
    (lambda (out)
      (display prelude out)
      (newline out)
      (let ((subgraphs (do-next-package packages ignore '() ignore out)))
	(for-each (lambda (sub)
		    (note-subgraph sub out))
		  subgraphs)
	(display "}" out)
	(newline out)))))

; Do the first not-yet-done package, returning the subgraphs if there are
; no packages left.  TO-DO, DONE, and IGNORE are lists of packages.
; SUBGRAPHS is an a-list indexed by source-file-name.

(define (do-next-package to-do done subgraphs ignore out)
  (let loop ((to-do to-do))
    (if (null? to-do)
	subgraphs
	(let ((package (car to-do)))
	  (if (memq package done)
	      (loop (cdr to-do))
	      (do-package package (cdr to-do) (cons package done)
			  subgraphs ignore out))))))

; Find the correct subgraph, add PACKAGE to it, note any edges, and continue
; with the rest of the graph.

(define (do-package package to-do done subgraphs ignore out)
  (let* ((source-file (package-file-name package))
	 (opens (map structure-package
		     ((package-opens-thunk package))))
	 (old-subgraph (assq source-file subgraphs))
	 (subgraph (or old-subgraph
		       (list source-file))))
    (set-cdr! subgraph (cons package (cdr subgraph)))
    (do-edges package opens source-file ignore out)
    (do-next-package (append opens to-do)
		     done
		     (if old-subgraph
			 subgraphs
			 (cons subgraph subgraphs))
		     ignore
		     out)))

; Add an edge from each package in OPENS to PACKAGE, provided that the
; two were defined in the same file.

(define (do-edges package opens source-file ignore out)
  (let loop ((opens opens) (done ignore))
    (if (not (null? opens))
	(loop (cdr opens)
	      (let ((p (car opens)))
		(if (or (memq p done)
			(not (string=? source-file (package-file-name p))))
		    done
		    (begin
		      (note-edge p package out)
		      (cons p done))))))))

; Writing out the package name as a string (actually, its the name of
; the first of the package's clients).

(define (package-name package out)
  (let ((clients (population->list (package-clients package))))
    (write-char #\" out)
    (display (structure-name (car clients)) out)
    (write-char #\" out)))

; Header for DOT files

(define prelude
  "digraph G {
   orientation=landscape;
   size =\"10,7.5\";
   page =\"8.5,11\";
   ratio =fill;")

; Writing out edges and subgraphs

(define (note-edge from to out)
  (display "   " out)
  (package-name from out)
  (display " -> " out)
  (package-name to out)
  (write-char #\; out)
  (newline out))

(define (note-subgraph subgraph out)
  (display "   subgraph \"cluster_" out)
  (display (car subgraph) out)
  (display "\" { label=\"" out)
  (display (car subgraph) out)
  (display "\"; " out)
  (for-each (lambda (p)
	      (package-name p out)
	      (display "; " out))
	    (cdr subgraph))
  (display "}" out)
  (newline out))

