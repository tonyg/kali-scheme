# arch-tag: Tom Lord Tue Dec  4 14:47:18 2001 (scm-library.mk)
#
# scm-library.mk -
#
################################################################
# Copyright (C) 2001 Tom Lord
# 
# See the file "COPYING" for further information about
# the copyright and warranty status of this work.
# 

ifndef dotx-rules-mk
dotx-rules-mk	:= 1

xfiles		:=	$(patsubst %.c, %.x, $(libsource))

.SUFFIXES:	.x

$(xfiles):%.x:%.c
	set -e ; \
	touch $@; \
	rm -f ,$@; \
	$(CC) $(CFLAGS) -DS48_MAGIC_SNARFER -E $< | sed -f $(makefiles)/dotx.sed > ,$@ ; \
	rm -f $@; \
	cp ,$@ $@ ; \
	$(CC) $(CFLAGS) -DS48_MAGIC_SNARFER2 -E $< | sed -f $(makefiles)/dotx.sed >> ,$@ ; \
	rm -f $@; \
	mv ,$@ $@ ;

clean: clean-xfiles

clean-xfiles:
	for f in $(xfiles) ,fake.x ; do \
	  test ! -e $$f || rm -f $$f ; \
	done

$(libdepfiles): %.d: %.x

endif
