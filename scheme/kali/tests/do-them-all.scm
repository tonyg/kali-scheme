
(define (test-all machine-name port)
  (encode-decode-test)
  (remote-test machine-name port)
  (record-test machine-name port)
  (fluid-test  machine-name port))
