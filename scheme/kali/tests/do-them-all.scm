;; there should be two servers running,
;; the "local" one an one other on: 
;; MASCHINE-NAME listening on port PORT

(define (test-all machine-name port)
  (encode-decode-test)
  (remote-test machine-name port)
  (record-test machine-name port)
  (fluid-test  machine-name port)
  (gc-test machine-name port))
