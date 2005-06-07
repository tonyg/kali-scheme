
(define control-names
  '#("NUL" "SOH" "STX" "ETX" "EOT" "ENQ" "ACK" "BEL"
     "BS"  "HT"  "LF"  "VT"  "FF"  "CR"  "SO"  "SI"
     "DLE" "DC1" "DC2" "DC3" "DC4" "NAK" "SYN" "ETB"
     "CAN" "EM"  "SUB" "ESC" "FS"  "GS"  "RS"  "US"
     "SPACE"))

(define unicode-latin-1-names
  '#("NULL"
     "START OF HEADING"
     "START OF TEXT"
     "END OF TEXT"
     "END OF TRANSMISSION"
     "ENQUIRY"
     "ACKNOWLEDGE"
     "BELL"
     "BACKSPACE"
     "HORIZONTAL TABULATION"
     "LINE FEED"
     "VERTICAL TABULATION"
     "FORM FEED"
     "CARRIAGE RETURN"
     "SHIFT OUT"
     "SHIFT IN"
     "DATA LINK ESCAPE"
     "DEVICE CONTROL ONE"
     "DEVICE CONTROL TWO"
     "DEVICE CONTROL THREE"
     "DEVICE CONTROL FOUR"
     "NEGATIVE ACKNOWLEDGE"
     "SYNCHRONOUS IDLE"
     "END OF TRANSMISSION BLOCK"
     "CANCEL"
     "END OF MEDIUM"
     "SUBSTITUTE"
     "ESCAPE"
     "FILE SEPARATOR"
     "GROUP SEPARATOR"
     "RECORD SEPARATOR"
     "UNIT SEPARATOR"
     "SPACE"
     "EXCLAMATION MARK"
     "QUOTATION MARK"
     "NUMBER SIGN"
     "DOLLAR SIGN"
     "PERCENT SIGN"
     "AMPERSAND"
     "APOSTROPHE"
     "LEFT PARENTHESIS"
     "RIGHT PARENTHESIS"
     "ASTERISK"
     "PLUS SIGN"
     "COMMA"
     "HYPHEN-MINUS"
     "FULL STOP"
     "SOLIDUS"
     "DIGIT ZERO"
     "DIGIT ONE"
     "DIGIT TWO"
     "DIGIT THREE"
     "DIGIT FOUR"
     "DIGIT FIVE"
     "DIGIT SIX"
     "DIGIT SEVEN"
     "DIGIT EIGHT"
     "DIGIT NINE"
     "COLON"
     "SEMICOLON"
     "LESS-THAN SIGN"
     "EQUALS SIGN"
     "GREATER-THAN SIGN"
     "QUESTION MARK"
     "COMMERCIAL AT"
     "LATIN CAPITAL LETTER A"
     "LATIN CAPITAL LETTER B"
     "LATIN CAPITAL LETTER C"
     "LATIN CAPITAL LETTER D"
     "LATIN CAPITAL LETTER E"
     "LATIN CAPITAL LETTER F"
     "LATIN CAPITAL LETTER G"
     "LATIN CAPITAL LETTER H"
     "LATIN CAPITAL LETTER I"
     "LATIN CAPITAL LETTER J"
     "LATIN CAPITAL LETTER K"
     "LATIN CAPITAL LETTER L"
     "LATIN CAPITAL LETTER M"
     "LATIN CAPITAL LETTER N"
     "LATIN CAPITAL LETTER O"
     "LATIN CAPITAL LETTER P"
     "LATIN CAPITAL LETTER Q"
     "LATIN CAPITAL LETTER R"
     "LATIN CAPITAL LETTER S"
     "LATIN CAPITAL LETTER T"
     "LATIN CAPITAL LETTER U"
     "LATIN CAPITAL LETTER V"
     "LATIN CAPITAL LETTER W"
     "LATIN CAPITAL LETTER X"
     "LATIN CAPITAL LETTER Y"
     "LATIN CAPITAL LETTER Z"
     "LEFT SQUARE BRACKET"
     "REVERSE SOLIDUS"
     "RIGHT SQUARE BRACKET"
     "CIRCUMFLEX ACCENT"
     "LOW LINE"
     "GRAVE ACCENT"
     "LATIN SMALL LETTER A"
     "LATIN SMALL LETTER B"
     "LATIN SMALL LETTER C"
     "LATIN SMALL LETTER D"
     "LATIN SMALL LETTER E"
     "LATIN SMALL LETTER F"
     "LATIN SMALL LETTER G"
     "LATIN SMALL LETTER H"
     "LATIN SMALL LETTER I"
     "LATIN SMALL LETTER J"
     "LATIN SMALL LETTER K"
     "LATIN SMALL LETTER L"
     "LATIN SMALL LETTER M"
     "LATIN SMALL LETTER N"
     "LATIN SMALL LETTER O"
     "LATIN SMALL LETTER P"
     "LATIN SMALL LETTER Q"
     "LATIN SMALL LETTER R"
     "LATIN SMALL LETTER S"
     "LATIN SMALL LETTER T"
     "LATIN SMALL LETTER U"
     "LATIN SMALL LETTER V"
     "LATIN SMALL LETTER W"
     "LATIN SMALL LETTER X"
     "LATIN SMALL LETTER Y"
     "LATIN SMALL LETTER Z"
     "LEFT CURLY BRACKET"
     "VERTICAL LINE"
     "RIGHT CURLY BRACKET"
     "TILDE"
     "DELETE"))

(define (display-char i)
  (for-each display
	    (list "\\code{" (number->string i 10) "} & \\code{"
		  (string-upcase (number->string i 16)) "} & "
		  (cond ((<= i 32)
			 (vector-ref control-names i))
			((= i 127)
			 "DEL")
			((= i (char->ascii #\|))
			 "\\verb$|$")
			(else
			 (string-append "\\verb|"
					(list->string (list (ascii->char i)))
					"|")))
		   " & "
		   (string-downcase (vector-ref unicode-latin-1-names i)))))

(define (string-upcase string)
  (list->string (map char-upcase (string->list string))))

(define (string-downcase string)
  (list->string (map char-downcase (string->list string))))

(define (write-ascii-table)
  (do ((i 0 (+ i 1)))
      ((= i 64))
    (display-char i)
    (display " & ")
    (display-char (+ i 64))
    (display " \\\\")
    (newline)))

