(FORM 0 random okay #f
  ()

    (P #f_0 (c_1)
      (LET* (((loop_2)   (allocate 'single-set 'ptr))
             (()         (set-contents loop_2 'cell '0 '0 ^loop_3)))
        (jump 0 (contents loop_2 'cell '0 '0) '5 '1)))

     (J loop_3 (i_4 r_5)
       (LET* (((c_6)*     ^c_7))
         (test 2 ^g_8 ^g_9 (less? '1 i_4))))

      (C g_8 ()
        (return 0 c_6 '#f))

      (C g_9 ()
        (return 0 c_6 '#t))

      (C c_7 (v_10)
        (test 2 ^g_11 ^g_12 v_10))

       (C g_11 ()
         (unknown-return 0 c_1 '1 r_5))

       (C g_12 ()
         (jump 0 (contents loop_2 'cell '0 '0) (subtract i_4 '1) (multiply i_4 r_5))))

