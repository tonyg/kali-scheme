; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-test-suite tcp-sockets-tests)

(define-test-case tcp-sockets tcp-sockets-tests
  (check
   (let ((server (open-socket)))
     (spawn (lambda ()
	      (let loop ((i 100))
		(if (= i 400)
		    (close-socket server)
		    (call-with-values (lambda ()
					(socket-accept server))
		      (lambda (in out)
			(spawn (lambda ()
				 (write (+ i (read in)) out)
				 (write-char #\space out)
				 (close-input-port in)
				 (close-output-port out)))
			(loop (+ i 100))))))))
     (let loop ((i 1) (result '()))
       (if (= i 4)
	   result
	   (call-with-values
	       (lambda ()
		 (socket-client "localhost" (socket-port-number server)))
	     (lambda (in out)
	       (write i out)
	       (write-char #\space out)
	       (let ((result (cons (read in) result)))
		 (close-input-port in)
		 (close-output-port out)
		 (loop (+ i 1) result)))))))
   => '(303 202 101)))

(define-test-suite udp-sockets-tests)

(define-test-case udp-sockets udp-sockets-tests
  (check
   ;; we hope this is reliable on localhost
   (call-with-values
       (lambda () (open-udp-socket))
     (lambda (server-input-socket server-output-socket)
       (spawn (lambda ()
		(let ((rec-buffer (make-byte-vector 100 0))
		      (send-buffer (make-byte-vector 100 0)))
		  (let loop ((i 100))
		    (if (= i 400)
			(begin
			  (close-socket server-input-socket)
			  (close-socket server-output-socket))
			(call-with-values (lambda ()
					    (udp-receive server-input-socket rec-buffer))
			  (lambda (count udp-address)
			    (byte-vector-set! send-buffer 0
					      (+ (byte-vector-ref rec-buffer 0) 1))
			    (udp-send server-output-socket
				      (lookup-udp-address "localhost"
							  (udp-address-port udp-address))
				      send-buffer 1)
			    (loop (+ 1 i)))))))))
       (let ((server-port (socket-port-number server-input-socket)))
	 (call-with-values
	     (lambda () (open-udp-socket))
	   (lambda (client-input-socket client-output-socket)
	     (let ((rec-buffer (make-byte-vector 100 0))
		   (send-buffer (make-byte-vector 100 0)))
	       (let loop ((i 100) (result '()))
		 (if (= i 400)
		     (begin
		       (close-socket client-input-socket)
		       (close-socket client-output-socket)
		       result)
		     (let ((val (modulo i 256)))
		       (byte-vector-set! send-buffer 0 val)
		       (udp-send client-output-socket
				 (lookup-udp-address "localhost" server-port)
				 send-buffer 1)
		       (call-with-values
			   (lambda ()
			     (udp-receive client-input-socket rec-buffer))
			 (lambda (count udp-address)
			   (loop (+ i 1)
				 (cons (byte-vector-ref rec-buffer 0)
				       result)))))))))))))
   =>
   '(144 143 142 141 140 139 138 137 136 135 134 133 132 131 130 129 128 127 126 125 124 123 122 121 120 119 118 117 116 115 114 113 112 111 110 109 108 107 106 105 104 103 102 101 100 99 98 97 96 95 94 93 92 91 90 89 88 87 86 85 84 83 82 81 80 79 78 77 76 75 74 73 72 71 70 69 68 67 66 65 64 63 62 61 60 59 58 57 56 55 54 53 52 51 50 49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34 33 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 255 254 253 252 251 250 249 248 247 246 245 244 243 242 241 240 239 238 237 236 235 234 233 232 231 230 229 228 227 226 225 224 223 222 221 220 219 218 217 216 215 214 213 212 211 210 209 208 207 206 205 204 203 202 201 200 199 198 197 196 195 194 193 192 191 190 189 188 187 186 185 184 183 182 181 180 179 178 177 176 175 174 173 172 171 170 169 168 167 166 165 164 163 162 161 160 159 158 157 156 155 154 153 152 151 150 149 148 147 146 145 144 143 142 141 140 139 138 137 136 135 134 133 132 131 130 129 128 127 126 125 124 123 122 121 120 119 118 117 116 115 114 113 112 111 110 109 108 107 106 105 104 103 102 101)))
