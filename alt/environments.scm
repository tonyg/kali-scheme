
; don't put a copyright notice, silly shell script

(define (*structure-ref struct name)
  (eval name (interaction-environment)))

