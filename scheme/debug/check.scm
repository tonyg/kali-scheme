; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


; The barest skeleton of a test suite.
; Mostly it makes sure that many of the external packages load without
; error.

; ,exec ,load debug/check.scm
; (done)

(load-package 'testing)

(config '(run 
	  (define-structure bar (export)
	    (open scheme testing))))

(in 'bar '(bench off))
(in 'bar '(run (define (foo) (cadr '(a b)))))
(in 'bar '(run (define cadr list)))
(in 'bar '(run (test "non-bench" equal? '((a b)) (foo))))

(in 'bar '(bench on))
(in 'bar '(run (define (foo) (car '(a b)))))
(in 'bar '(run (define car list)))
(in 'bar '(run (test "bench" equal? 'a (foo))))

(config '(run
	  (define-structure test1 (export x)
	    (open scheme testing)
	    (begin (define x 10)
		   (define (z) x)))))
(config '(run 
	  (define-structure test2 (export)
	    (open scheme test1 testing)
	    (begin (define (z) x)))))
(config '(run
	  (define-structure test3 (export)
	    (open scheme test1 testing)
	    (begin (define (z) x)))))
(load-package 'test2)
(load-package 'test3)
(in 'test3 '(run (define x 20)))
(in 'test3 '(open test2))
(in 'test2 '(run (test "shadowing" = 10 (z))))
(in 'test3 '(run (test "shadowing" = 20 (z))))
(in 'test1 '(run (test "shadowing" = 10 (z))))

(config '(run 
(define-structure foo (export)
  (open scheme testing
	;assembler
	code-vectors
	ports
	queues
	random
	sort
	big-scheme
	arrays
	dump/restore
	search-trees
	threads
	placeholders
	locks
	interrupts
	mask-types
	masks
	finite-types
	sockets
	(subset i/o-internal (open-input-port?))
	sicp)
  (begin

(test "1000 args"
      equal?
      '(0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 
	100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 
	200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256 257 258 259 260 261 262 263 264 265 266 267 268 269 270 271 272 273 274 275 276 277 278 279 280 281 282 283 284 285 286 287 288 289 290 291 292 293 294 295 296 297 298 299 
	300 301 302 303 304 305 306 307 308 309 310 311 312 313 314 315 316 317 318 319 320 321 322 323 324 325 326 327 328 329 330 331 332 333 334 335 336 337 338 339 340 341 342 343 344 345 346 347 348 349 350 351 352 353 354 355 356 357 358 359 360 361 362 363 364 365 366 367 368 369 370 371 372 373 374 375 376 377 378 379 380 381 382 383 384 385 386 387 388 389 390 391 392 393 394 395 396 397 398 399 
	400 401 402 403 404 405 406 407 408 409 410 411 412 413 414 415 416 417 418 419 420 421 422 423 424 425 426 427 428 429 430 431 432 433 434 435 436 437 438 439 440 441 442 443 444 445 446 447 448 449 450 451 452 453 454 455 456 457 458 459 460 461 462 463 464 465 466 467 468 469 470 471 472 473 474 475 476 477 478 479 480 481 482 483 484 485 486 487 488 489 490 491 492 493 494 495 496 497 498 499 
	500 501 502 503 504 505 506 507 508 509 510 511 512 513 514 515 516 517 518 519 520 521 522 523 524 525 526 527 528 529 530 531 532 533 534 535 536 537 538 539 540 541 542 543 544 545 546 547 548 549 550 551 552 553 554 555 556 557 558 559 560 561 562 563 564 565 566 567 568 569 570 571 572 573 574 575 576 577 578 579 580 581 582 583 584 585 586 587 588 589 590 591 592 593 594 595 596 597 598 599 
	600 601 602 603 604 605 606 607 608 609 610 611 612 613 614 615 616 617 618 619 620 621 622 623 624 625 626 627 628 629 630 631 632 633 634 635 636 637 638 639 640 641 642 643 644 645 646 647 648 649 650 651 652 653 654 655 656 657 658 659 660 661 662 663 664 665 666 667 668 669 670 671 672 673 674 675 676 677 678 679 680 681 682 683 684 685 686 687 688 689 690 691 692 693 694 695 696 697 698 699 
	700 701 702 703 704 705 706 707 708 709 710 711 712 713 714 715 716 717 718 719 720 721 722 723 724 725 726 727 728 729 730 731 732 733 734 735 736 737 738 739 740 741 742 743 744 745 746 747 748 749 750 751 752 753 754 755 756 757 758 759 760 761 762 763 764 765 766 767 768 769 770 771 772 773 774 775 776 777 778 779 780 781 782 783 784 785 786 787 788 789 790 791 792 793 794 795 796 797 798 799 
	800 801 802 803 804 805 806 807 808 809 810 811 812 813 814 815 816 817 818 819 820 821 822 823 824 825 826 827 828 829 830 831 832 833 834 835 836 837 838 839 840 841 842 843 844 845 846 847 848 849 850 851 852 853 854 855 856 857 858 859 860 861 862 863 864 865 866 867 868 869 870 871 872 873 874 875 876 877 878 879 880 881 882 883 884 885 886 887 888 889 890 891 892 893 894 895 896 897 898 899 
	900 901 902 903 904 905 906 907 908 909 910 911 912 913 914 915 916 917 918 919 920 921 922 923 924 925 926 927 928 929 930 931 932 933 934 935 936 937 938 939 940 941 942 943 944 945 946 947 948 949 950 951 952 953 954 955 956 957 958 959 960 961 962 963 964 965 966 967 968 969 970 971 972 973 974 975 976 977 978 979 980 981 982 983 984 985 986 987 988 989 990 991 992 993 994 995 996 997 998 999 
	)
      (let ((f (lambda (x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 x10 x11 x12 x13 x14 x15 x16 x17 x18 x19 x20 x21 x22 x23 x24 x25 x26 x27 x28 x29 x30 x31 x32 x33 x34 x35 x36 x37 x38 x39 x40 x41 x42 x43 x44 x45 x46 x47 x48 x49 x50 x51 x52 x53 x54 x55 x56 x57 x58 x59 x60 x61 x62 x63 x64 x65 x66 x67 x68 x69 x70 x71 x72 x73 x74 x75 x76 x77 x78 x79 x80 x81 x82 x83 x84 x85 x86 x87 x88 x89 x90 x91 x92 x93 x94 x95 x96 x97 x98 x99 
			x100 x101 x102 x103 x104 x105 x106 x107 x108 x109 x110 x111 x112 x113 x114 x115 x116 x117 x118 x119 x120 x121 x122 x123 x124 x125 x126 x127 x128 x129 x130 x131 x132 x133 x134 x135 x136 x137 x138 x139 x140 x141 x142 x143 x144 x145 x146 x147 x148 x149 x150 x151 x152 x153 x154 x155 x156 x157 x158 x159 x160 x161 x162 x163 x164 x165 x166 x167 x168 x169 x170 x171 x172 x173 x174 x175 x176 x177 x178 x179 x180 x181 x182 x183 x184 x185 x186 x187 x188 x189 x190 x191 x192 x193 x194 x195 x196 x197 x198 x199 
			x200 x201 x202 x203 x204 x205 x206 x207 x208 x209 x210 x211 x212 x213 x214 x215 x216 x217 x218 x219 x220 x221 x222 x223 x224 x225 x226 x227 x228 x229 x230 x231 x232 x233 x234 x235 x236 x237 x238 x239 x240 x241 x242 x243 x244 x245 x246 x247 x248 x249 x250 x251 x252 x253 x254 x255 x256 x257 x258 x259 x260 x261 x262 x263 x264 x265 x266 x267 x268 x269 x270 x271 x272 x273 x274 x275 x276 x277 x278 x279 x280 x281 x282 x283 x284 x285 x286 x287 x288 x289 x290 x291 x292 x293 x294 x295 x296 x297 x298 x299 
			x300 x301 x302 x303 x304 x305 x306 x307 x308 x309 x310 x311 x312 x313 x314 x315 x316 x317 x318 x319 x320 x321 x322 x323 x324 x325 x326 x327 x328 x329 x330 x331 x332 x333 x334 x335 x336 x337 x338 x339 x340 x341 x342 x343 x344 x345 x346 x347 x348 x349 x350 x351 x352 x353 x354 x355 x356 x357 x358 x359 x360 x361 x362 x363 x364 x365 x366 x367 x368 x369 x370 x371 x372 x373 x374 x375 x376 x377 x378 x379 x380 x381 x382 x383 x384 x385 x386 x387 x388 x389 x390 x391 x392 x393 x394 x395 x396 x397 x398 x399 
			x400 x401 x402 x403 x404 x405 x406 x407 x408 x409 x410 x411 x412 x413 x414 x415 x416 x417 x418 x419 x420 x421 x422 x423 x424 x425 x426 x427 x428 x429 x430 x431 x432 x433 x434 x435 x436 x437 x438 x439 x440 x441 x442 x443 x444 x445 x446 x447 x448 x449 x450 x451 x452 x453 x454 x455 x456 x457 x458 x459 x460 x461 x462 x463 x464 x465 x466 x467 x468 x469 x470 x471 x472 x473 x474 x475 x476 x477 x478 x479 x480 x481 x482 x483 x484 x485 x486 x487 x488 x489 x490 x491 x492 x493 x494 x495 x496 x497 x498 x499 
			x500 x501 x502 x503 x504 x505 x506 x507 x508 x509 x510 x511 x512 x513 x514 x515 x516 x517 x518 x519 x520 x521 x522 x523 x524 x525 x526 x527 x528 x529 x530 x531 x532 x533 x534 x535 x536 x537 x538 x539 x540 x541 x542 x543 x544 x545 x546 x547 x548 x549 x550 x551 x552 x553 x554 x555 x556 x557 x558 x559 x560 x561 x562 x563 x564 x565 x566 x567 x568 x569 x570 x571 x572 x573 x574 x575 x576 x577 x578 x579 x580 x581 x582 x583 x584 x585 x586 x587 x588 x589 x590 x591 x592 x593 x594 x595 x596 x597 x598 x599 
			x600 x601 x602 x603 x604 x605 x606 x607 x608 x609 x610 x611 x612 x613 x614 x615 x616 x617 x618 x619 x620 x621 x622 x623 x624 x625 x626 x627 x628 x629 x630 x631 x632 x633 x634 x635 x636 x637 x638 x639 x640 x641 x642 x643 x644 x645 x646 x647 x648 x649 x650 x651 x652 x653 x654 x655 x656 x657 x658 x659 x660 x661 x662 x663 x664 x665 x666 x667 x668 x669 x670 x671 x672 x673 x674 x675 x676 x677 x678 x679 x680 x681 x682 x683 x684 x685 x686 x687 x688 x689 x690 x691 x692 x693 x694 x695 x696 x697 x698 x699 
			x700 x701 x702 x703 x704 x705 x706 x707 x708 x709 x710 x711 x712 x713 x714 x715 x716 x717 x718 x719 x720 x721 x722 x723 x724 x725 x726 x727 x728 x729 x730 x731 x732 x733 x734 x735 x736 x737 x738 x739 x740 x741 x742 x743 x744 x745 x746 x747 x748 x749 x750 x751 x752 x753 x754 x755 x756 x757 x758 x759 x760 x761 x762 x763 x764 x765 x766 x767 x768 x769 x770 x771 x772 x773 x774 x775 x776 x777 x778 x779 x780 x781 x782 x783 x784 x785 x786 x787 x788 x789 x790 x791 x792 x793 x794 x795 x796 x797 x798 x799 
			x800 x801 x802 x803 x804 x805 x806 x807 x808 x809 x810 x811 x812 x813 x814 x815 x816 x817 x818 x819 x820 x821 x822 x823 x824 x825 x826 x827 x828 x829 x830 x831 x832 x833 x834 x835 x836 x837 x838 x839 x840 x841 x842 x843 x844 x845 x846 x847 x848 x849 x850 x851 x852 x853 x854 x855 x856 x857 x858 x859 x860 x861 x862 x863 x864 x865 x866 x867 x868 x869 x870 x871 x872 x873 x874 x875 x876 x877 x878 x879 x880 x881 x882 x883 x884 x885 x886 x887 x888 x889 x890 x891 x892 x893 x894 x895 x896 x897 x898 x899 
			x900 x901 x902 x903 x904 x905 x906 x907 x908 x909 x910 x911 x912 x913 x914 x915 x916 x917 x918 x919 x920 x921 x922 x923 x924 x925 x926 x927 x928 x929 x930 x931 x932 x933 x934 x935 x936 x937 x938 x939 x940 x941 x942 x943 x944 x945 x946 x947 x948 x949 x950 x951 x952 x953 x954 x955 x956 x957 x958 x959 x960 x961 x962 x963 x964 x965 x966 x967 x968 x969 x970 x971 x972 x973 x974 x975 x976 x977 x978 x979 x980 x981 x982 x983 x984 x985 x986 x987 x988 x989 x990 x991 x992 x993 x994 x995 x996 x997 x998 x999 
			)
		 (list x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 x10 x11 x12 x13 x14 x15 x16 x17 x18 x19 x20 x21 x22 x23 x24 x25 x26 x27 x28 x29 x30 x31 x32 x33 x34 x35 x36 x37 x38 x39 x40 x41 x42 x43 x44 x45 x46 x47 x48 x49 x50 x51 x52 x53 x54 x55 x56 x57 x58 x59 x60 x61 x62 x63 x64 x65 x66 x67 x68 x69 x70 x71 x72 x73 x74 x75 x76 x77 x78 x79 x80 x81 x82 x83 x84 x85 x86 x87 x88 x89 x90 x91 x92 x93 x94 x95 x96 x97 x98 x99 
		       x100 x101 x102 x103 x104 x105 x106 x107 x108 x109 x110 x111 x112 x113 x114 x115 x116 x117 x118 x119 x120 x121 x122 x123 x124 x125 x126 x127 x128 x129 x130 x131 x132 x133 x134 x135 x136 x137 x138 x139 x140 x141 x142 x143 x144 x145 x146 x147 x148 x149 x150 x151 x152 x153 x154 x155 x156 x157 x158 x159 x160 x161 x162 x163 x164 x165 x166 x167 x168 x169 x170 x171 x172 x173 x174 x175 x176 x177 x178 x179 x180 x181 x182 x183 x184 x185 x186 x187 x188 x189 x190 x191 x192 x193 x194 x195 x196 x197 x198 x199 
		       x200 x201 x202 x203 x204 x205 x206 x207 x208 x209 x210 x211 x212 x213 x214 x215 x216 x217 x218 x219 x220 x221 x222 x223 x224 x225 x226 x227 x228 x229 x230 x231 x232 x233 x234 x235 x236 x237 x238 x239 x240 x241 x242 x243 x244 x245 x246 x247 x248 x249 x250 x251 x252 x253 x254 x255 x256 x257 x258 x259 x260 x261 x262 x263 x264 x265 x266 x267 x268 x269 x270 x271 x272 x273 x274 x275 x276 x277 x278 x279 x280 x281 x282 x283 x284 x285 x286 x287 x288 x289 x290 x291 x292 x293 x294 x295 x296 x297 x298 x299 
		       x300 x301 x302 x303 x304 x305 x306 x307 x308 x309 x310 x311 x312 x313 x314 x315 x316 x317 x318 x319 x320 x321 x322 x323 x324 x325 x326 x327 x328 x329 x330 x331 x332 x333 x334 x335 x336 x337 x338 x339 x340 x341 x342 x343 x344 x345 x346 x347 x348 x349 x350 x351 x352 x353 x354 x355 x356 x357 x358 x359 x360 x361 x362 x363 x364 x365 x366 x367 x368 x369 x370 x371 x372 x373 x374 x375 x376 x377 x378 x379 x380 x381 x382 x383 x384 x385 x386 x387 x388 x389 x390 x391 x392 x393 x394 x395 x396 x397 x398 x399 
		       x400 x401 x402 x403 x404 x405 x406 x407 x408 x409 x410 x411 x412 x413 x414 x415 x416 x417 x418 x419 x420 x421 x422 x423 x424 x425 x426 x427 x428 x429 x430 x431 x432 x433 x434 x435 x436 x437 x438 x439 x440 x441 x442 x443 x444 x445 x446 x447 x448 x449 x450 x451 x452 x453 x454 x455 x456 x457 x458 x459 x460 x461 x462 x463 x464 x465 x466 x467 x468 x469 x470 x471 x472 x473 x474 x475 x476 x477 x478 x479 x480 x481 x482 x483 x484 x485 x486 x487 x488 x489 x490 x491 x492 x493 x494 x495 x496 x497 x498 x499 
		       x500 x501 x502 x503 x504 x505 x506 x507 x508 x509 x510 x511 x512 x513 x514 x515 x516 x517 x518 x519 x520 x521 x522 x523 x524 x525 x526 x527 x528 x529 x530 x531 x532 x533 x534 x535 x536 x537 x538 x539 x540 x541 x542 x543 x544 x545 x546 x547 x548 x549 x550 x551 x552 x553 x554 x555 x556 x557 x558 x559 x560 x561 x562 x563 x564 x565 x566 x567 x568 x569 x570 x571 x572 x573 x574 x575 x576 x577 x578 x579 x580 x581 x582 x583 x584 x585 x586 x587 x588 x589 x590 x591 x592 x593 x594 x595 x596 x597 x598 x599 
		       x600 x601 x602 x603 x604 x605 x606 x607 x608 x609 x610 x611 x612 x613 x614 x615 x616 x617 x618 x619 x620 x621 x622 x623 x624 x625 x626 x627 x628 x629 x630 x631 x632 x633 x634 x635 x636 x637 x638 x639 x640 x641 x642 x643 x644 x645 x646 x647 x648 x649 x650 x651 x652 x653 x654 x655 x656 x657 x658 x659 x660 x661 x662 x663 x664 x665 x666 x667 x668 x669 x670 x671 x672 x673 x674 x675 x676 x677 x678 x679 x680 x681 x682 x683 x684 x685 x686 x687 x688 x689 x690 x691 x692 x693 x694 x695 x696 x697 x698 x699 
		       x700 x701 x702 x703 x704 x705 x706 x707 x708 x709 x710 x711 x712 x713 x714 x715 x716 x717 x718 x719 x720 x721 x722 x723 x724 x725 x726 x727 x728 x729 x730 x731 x732 x733 x734 x735 x736 x737 x738 x739 x740 x741 x742 x743 x744 x745 x746 x747 x748 x749 x750 x751 x752 x753 x754 x755 x756 x757 x758 x759 x760 x761 x762 x763 x764 x765 x766 x767 x768 x769 x770 x771 x772 x773 x774 x775 x776 x777 x778 x779 x780 x781 x782 x783 x784 x785 x786 x787 x788 x789 x790 x791 x792 x793 x794 x795 x796 x797 x798 x799 
		       x800 x801 x802 x803 x804 x805 x806 x807 x808 x809 x810 x811 x812 x813 x814 x815 x816 x817 x818 x819 x820 x821 x822 x823 x824 x825 x826 x827 x828 x829 x830 x831 x832 x833 x834 x835 x836 x837 x838 x839 x840 x841 x842 x843 x844 x845 x846 x847 x848 x849 x850 x851 x852 x853 x854 x855 x856 x857 x858 x859 x860 x861 x862 x863 x864 x865 x866 x867 x868 x869 x870 x871 x872 x873 x874 x875 x876 x877 x878 x879 x880 x881 x882 x883 x884 x885 x886 x887 x888 x889 x890 x891 x892 x893 x894 x895 x896 x897 x898 x899 
		       x900 x901 x902 x903 x904 x905 x906 x907 x908 x909 x910 x911 x912 x913 x914 x915 x916 x917 x918 x919 x920 x921 x922 x923 x924 x925 x926 x927 x928 x929 x930 x931 x932 x933 x934 x935 x936 x937 x938 x939 x940 x941 x942 x943 x944 x945 x946 x947 x948 x949 x950 x951 x952 x953 x954 x955 x956 x957 x958 x959 x960 x961 x962 x963 x964 x965 x966 x967 x968 x969 x970 x971 x972 x973 x974 x975 x976 x977 x978 x979 x980 x981 x982 x983 x984 x985 x986 x987 x988 x989 x990 x991 x992 x993 x994 x995 x996 x997 x998 x999 
		       ))))
	(f 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 
	   100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 
	   200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256 257 258 259 260 261 262 263 264 265 266 267 268 269 270 271 272 273 274 275 276 277 278 279 280 281 282 283 284 285 286 287 288 289 290 291 292 293 294 295 296 297 298 299 
	   300 301 302 303 304 305 306 307 308 309 310 311 312 313 314 315 316 317 318 319 320 321 322 323 324 325 326 327 328 329 330 331 332 333 334 335 336 337 338 339 340 341 342 343 344 345 346 347 348 349 350 351 352 353 354 355 356 357 358 359 360 361 362 363 364 365 366 367 368 369 370 371 372 373 374 375 376 377 378 379 380 381 382 383 384 385 386 387 388 389 390 391 392 393 394 395 396 397 398 399 
	   400 401 402 403 404 405 406 407 408 409 410 411 412 413 414 415 416 417 418 419 420 421 422 423 424 425 426 427 428 429 430 431 432 433 434 435 436 437 438 439 440 441 442 443 444 445 446 447 448 449 450 451 452 453 454 455 456 457 458 459 460 461 462 463 464 465 466 467 468 469 470 471 472 473 474 475 476 477 478 479 480 481 482 483 484 485 486 487 488 489 490 491 492 493 494 495 496 497 498 499 
	   500 501 502 503 504 505 506 507 508 509 510 511 512 513 514 515 516 517 518 519 520 521 522 523 524 525 526 527 528 529 530 531 532 533 534 535 536 537 538 539 540 541 542 543 544 545 546 547 548 549 550 551 552 553 554 555 556 557 558 559 560 561 562 563 564 565 566 567 568 569 570 571 572 573 574 575 576 577 578 579 580 581 582 583 584 585 586 587 588 589 590 591 592 593 594 595 596 597 598 599 
	   600 601 602 603 604 605 606 607 608 609 610 611 612 613 614 615 616 617 618 619 620 621 622 623 624 625 626 627 628 629 630 631 632 633 634 635 636 637 638 639 640 641 642 643 644 645 646 647 648 649 650 651 652 653 654 655 656 657 658 659 660 661 662 663 664 665 666 667 668 669 670 671 672 673 674 675 676 677 678 679 680 681 682 683 684 685 686 687 688 689 690 691 692 693 694 695 696 697 698 699 
	   700 701 702 703 704 705 706 707 708 709 710 711 712 713 714 715 716 717 718 719 720 721 722 723 724 725 726 727 728 729 730 731 732 733 734 735 736 737 738 739 740 741 742 743 744 745 746 747 748 749 750 751 752 753 754 755 756 757 758 759 760 761 762 763 764 765 766 767 768 769 770 771 772 773 774 775 776 777 778 779 780 781 782 783 784 785 786 787 788 789 790 791 792 793 794 795 796 797 798 799 
	   800 801 802 803 804 805 806 807 808 809 810 811 812 813 814 815 816 817 818 819 820 821 822 823 824 825 826 827 828 829 830 831 832 833 834 835 836 837 838 839 840 841 842 843 844 845 846 847 848 849 850 851 852 853 854 855 856 857 858 859 860 861 862 863 864 865 866 867 868 869 870 871 872 873 874 875 876 877 878 879 880 881 882 883 884 885 886 887 888 889 890 891 892 893 894 895 896 897 898 899 
	   900 901 902 903 904 905 906 907 908 909 910 911 912 913 914 915 916 917 918 919 920 921 922 923 924 925 926 927 928 929 930 931 932 933 934 935 936 937 938 939 940 941 942 943 944 945 946 947 948 949 950 951 952 953 954 955 956 957 958 959 960 961 962 963 964 965 966 967 968 969 970 971 972 973 974 975 976 977 978 979 980 981 982 983 984 985 986 987 988 989 990 991 992 993 994 995 996 997 998 999 
	   )))

(test "* 1" = 6 (* 1 2 3))
(test "* 2" = (* 214760876 10) 2147608760)
(test "* 3" = (* 47123 46039) 2169495797)

(test "internal define" equal? '(20 20 20 20)
      (list (let ()
	      (define a 5)
	      (define b 15)
	      (+ a b))
      
	    (let ((b 10))
	      (define (a) b)
	      (define b 20)
	      (a))

	    (let ((b 10))
	      (define b 20)
	      (define (a) b)
	      (a))

	    (let ((b 10)
		  (a (lambda () 20))
		  (define (lambda args 30)))
	      (define b 40)
	      (define (a) b)
	      (a))))

(define (identity x) x)		; handy for preventing inlining

(let ((port (make-port #f -1 0 0 (make-code-vector 1024 25) 0 1023 #f)))
  (set-port-index! port 0)
  (test "read-char0" = 25 (char->ascii (read-char port)))
  (test "read-char1" = 25 (char->ascii ((identity read-char) port))))

(test "apply" equal? '(1 2 3 4) (apply list 1 2 '(3 4)))

(test "char<->integer" eq? #\a (integer->char (char->integer #\a)))

(test "number->string" equal?
      '("12" "14" "-12" "-14")
      (list (number->string 12)
	    (number->string 12 8)
	    (number->string -12)
	    (number->string -12 8)))

(test "string->number" equal?
      '(12 10 -12 -10)
      (list (string->number "12")
	    (string->number "12" 8)
	    (string->number "-12")
	    (string->number "-12" 8)))

;(test "lap" equal? #f ((lap #f () (protocol 0) (false) (return))))
(let ((q (make-queue)))
  (enqueue! q 'a)
  (test "q" eq? 'a (dequeue! q)))

(test "random" <= 0 ((make-random 7)))

(test "sort" equal? '(1 2 3 3) (sort-list '(2 3 1 3) <))

(test "bigbit" = (expt 2 100) (arithmetic-shift 1 100))

(test "format" string=? "x(1 2)" (format #f "x~s" '(1 2)))

(test "fancy input ports" equal? '(#f 3 5)
      (let ((p1 (make-tracking-input-port (make-string-input-port "


    2 3"))))
	(read p1)
	(let ((res (list (current-row p1) (current-column p1))))
	  (close-input-port p1)
	  (cons (open-input-port? p1) res))))

(test "fancy output ports" equal? '(1 4 "8
   9")
      (let* ((r 0)
	     (c 0)
	     (s (call-with-string-output-port
		 (lambda (out)
		   (let ((out (make-tracking-output-port out)))
		     (write 8 out)
		     (fresh-line out)
		     (fresh-line out)
		     (display "   9" out)
		     (set! r (current-row out))
		     (set! c (current-column out))
		     (close-output-port out))))))
	(list r c s)))

(test "limit-output" string=? "(1 2 3 4 5"
      (call-with-string-output-port
       (lambda (out)
	 (limit-output out
		       10
		       (lambda (out)
			 (display '(1 2 3 4 5 6) out))))))

(test "destructure" eq? 'a (destructure (((x (y) z) '(b (a) c))) y))

(test "array" eq? 'a
      (let ((a (make-array 'b 3 4)))
	(array-set! a 'a 1 2)
	(array-ref a 1 2)))

(test "R4RS delay" = 3 (letrec ((p (delay (if c 3 (begin (set! c #t)
							 (+ (force p) 1)))))
				(c #f))
			 (force p)))

(test "receive" equal? '(a b) (receive stuff (values 'a 'b) stuff))
(let ((z '(a "b" 3 #t)))
  (test "dump" equal? z
	(let ((q (make-queue)))
	  (dump z (lambda (c) (enqueue! q c)) -1)
	  (restore (lambda () (dequeue! q))))))
(let ((r 0)
      (l1 (make-lock))
      (l2 (make-lock))
      (ph (make-placeholder)))
  (let ((f (lambda (i lock)
	     (spawn (lambda ()
		      (let ((v (placeholder-value ph)))
			(with-interrupts-inhibited
			 (lambda ()
			   (set! r (+ i v r))))
			(release-lock lock)))))))
    (obtain-lock l1)
    (obtain-lock l2)
    (f 1 l1)
    (f 2 l2)
    (placeholder-set! ph 10)
    (obtain-lock l1)
    (obtain-lock l2)
    (test "placeholders" = r 23)))

(test "explode" equal? 'ab3 (implode (explode 'ab3)))

(test "get/put" equal? 'a (begin (put 'foo 'prop 'a)
				 (get 'foo 'prop)))

(test "search-trees" eq? 'a
      (let ((t (make-search-tree = <)))
	(search-tree-set! t 3 'b)
	(search-tree-set! t 4 'a)
	(search-tree-set! t 5 'c)
	(search-tree-ref t 4)))

(define-enumerated-type color :color
  color?
  colors
  color-name
  color-index
  (black white purple maroon))

(test "enumerated-types" equal? '(black white 2)
      (list (color-name (vector-ref colors 0))
	    (color-name (color white))
	    (color-index (color purple))))

(test "masks" equal? '((#t #f #f #t)
		       (#t #t #t #f)
		       ((black white purple)
			#o11)
		       ((black maroon)
			(black maroon)
			(black white maroon))
		       ((black maroon)
			(maroon)
			(maroon))
		       ((black white purple)
			(black white purple maroon)
			(black white purple))
		       ((black white purple)
			(black)
			())
		       (()
			(white purple)
			(black white purple))
		       ((maroon)
			(white purple)
			(black white purple maroon)))
      (let ((type (make-mask-type 'color-mask
				  color?
				  (lambda (i) (vector-ref colors i))
				  color-index
				  (vector-length colors)))
	    (show (lambda (mask)
		    (map color-name (mask->list mask)))))
	(let ((m0 (list->mask type (list (color black)
					 (color white)
					 (color purple))))
	      (m1 (integer->mask type #o11))
	      (m2 (list->mask type '())))
	  (list (list (mask-type? type)
		      (mask-type? m0)
		      (mask? type)
		      (mask? m0))
		(list (eq? (mask-type m0) type)
		      (mask-has-type? m0 type)
		      (mask-member? m0 (color black))
		      (mask-member? m0 (color maroon)))
		(list (map color-name (mask->list m0))
		      (mask->integer m1))
		(list (show (mask-set m1))
		      (show (mask-set m1 (color black)))
		      (show (mask-set m1 (color black) (color white))))
		(list (show (mask-clear m1))
		      (show (mask-clear m1 (color black)))
		      (show (mask-clear m1 (color black) (color white))))
		(list (show (mask-union m0 m0))
		      (show (mask-union m0 m1))
		      (show (mask-union m2 m0)))
		(list (show (mask-intersection m0 m0))
		      (show (mask-intersection m0 m1))
		      (show (mask-intersection m2 m0)))
		(list (show (mask-subtract m0 m0))
		      (show (mask-subtract m0 m1))
		      (show (mask-subtract m0 m2)))
		(list (show (mask-negate m0))
		      (show (mask-negate m1))
		      (show (mask-negate m2)))))))

(test "sockets" equal? '(303 202 101)
      (let ((server (open-socket))
	    (name (get-host-name)))
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
		 (socket-client name (socket-port-number server)))
	       (lambda (in out)
		 (write i out)
		 (write-char #\space out)
		 (let ((result (cons (read in) result)))
		   (close-input-port in)
		   (close-output-port out)
		   (loop (+ i 1) result))))))))

))))

(load-package 'foo)

(load-package 'floatnums)

(in 'foo '(run (let* ((one (exact->inexact 1))
		      (three (exact->inexact 3))
		      (third (/ one three))
		      (xthird (inexact->exact third)))
		 (test "float" eq? #f (= 1/3 xthird))
		 (test "exact<->inexact" = third (exact->inexact xthird)))))

(in 'foo '(run (let ((arg0 '(1.0 1.0 2.0)) (arg1 '(2.0 1.0 1.0)))
		 (test "float-comparisons"
		       equal?
		       (list (map = arg0 arg1)
			     (map < arg0 arg1)
			     (map > arg0 arg1)
			     (map <= arg0 arg1)
			     (map >= arg0 arg1))
		       '((#f #t #f)
			 (#t #f #f)
			 (#f #f #t)
			 (#t #t #f)
			 (#f #t #t))))))

(in 'foo '(run (test "float-ops"
                     equal?
                     (list (+ 2.0 3.0) (- 1.0 2.0) (* 2.0 3.0) (/ 1.0 2.0))
                     '(5.0 -1.0 6.0 0.5))))

(in 'foo '(run (let ((arg0 '(1 1 2)) (arg1 '(2 1 1)))
		 (test "integer-comparisons"
		       equal?
		       (list (map = arg0 arg1)
			     (map < arg0 arg1)
			     (map > arg0 arg1)
			     (map <= arg0 arg1)
			     (map >= arg0 arg1))
		       '((#f #t #f)
			 (#t #f #f)
			 (#f #f #t)
			 (#t #t #f)
			 (#f #t #t))))))

; All done.

(if (in 'testing '(run (lost?)))
    (display "Some tests failed.")
    (display "All tests succeeded."))
(newline)

(define (done)
  (exit (if (in 'testing '(run (lost?))) 1 0)))
