# Scheme 48 Makefile
# Last updated May 1993 by JAR
# More documentation in file INSTALL

# To change the set of features included in the standard image, edit
# the definition of usual-features in more-packages.scm.

# If you want to install Scheme 48 somewhere, set BIN, LIB, and MAN as
# appropriate for your site.  RUNNABLE is what the command by which
# you invoke Scheme 48 will be called.  LIB is the directory tree in
# which files are sought when doing ,load-package of one of the
# "more-packages" (big/pp.scm, etc.; see more-packages.scm).

# If you definitely won't be installing Scheme 48, you should set LIB
# to the directory that contains this Makefile.  This will make the
# ,load-package command work for the "extra" packages defined in
# more-packages.scm. 

BIN = /usr/local/bin
LIB = /usr/local/lib/scheme48
MAN = /usr/local/man/man1

# You might want to change RUNNABLE to "s48"
RUNNABLE = scheme48

# -N is needed to allow dynamic loading (at least on a DEC MIPS)
# VMLINKFLAGS = -N

CC = gcc
CFLAGS = -O2 -g
# On an HP "snake," you'll probably want:
#  CC = cc -Aa
#  CFLAGS = -D_HPUX_SOURCE -O +Obb1500
CP = cp -p
INSTALL_METHOD = install_method_2
DISTDIR = /users/ftp/pub/jar

# The static linker can use a lot of space.
BIG_HEAP = -h 4000000


###
### Avoid changing anything below this point.
###


# Targets:

IMAGE = scheme48.image
VM = scheme48vm
OBJFILES = main.o unix.o dynload.o error.o extension.o scheme48vm.o


# Linker:

# You gots your choice here; the linker can be run in just about any
# Scheme implementation.  It doesn't matter a whole lot which one you
# use unless of course the implementation that you would have used is
# broken or unavailable.  LINKER_RUNNABLE = $(RUNNABLE) requires you
# to already have a "scheme48" (i.e. $(RUNNABLE)) command (as you
# would if you had already done "make install").  Using ./$(VM) -i
# ./$(IMAGE) can be bad if you're making changes that might break the
# system.  You can run the linker in a Scheme other than Scheme 48;
# see e.g. the rule for link/linker-in-lucid, below.

#LINKER_RUNNABLE = ./$(VM) -i ./$(IMAGE)
LINKER_IMAGE = link/linker.image
START_LINKER_RUNNABLE = echo ,batch; echo ,bench on;

# or,
LINKER_RUNNABLE = $(RUNNABLE)
# and the other two as above

# Temporary while debugging version 0.22 (10/11/93)
#LINKER_RUNNABLE = ../0.21/scheme48vm -i ../0.21/scheme48.image

# or,
#LINKER_RUNNABLE = ./$(VM) -i initial.image
#LINKER_IMAGE = link/linker.image
#START_LINKER_RUNNABLE = 
# To make this work, you'll probably have to change the line 
#   ,open features ascii bitwise signals record
# to
#   ,load alt/record.scm alt/vanilla-features.scm alt/more-features.scm 
# in the rule for $(LINKER).  This hasn't been tested.

# or,
OLD_RUNNABLE = s48.10-19-92-with-patches
#LINKER_RUNNABLE = $(OLD_RUNNABLE)
#LINKER_IMAGE = link/old-linker.image
#START_LINKER_RUNNABLE = echo :batch;


# Sources:

CFILES = main.c unix.c dynload.c error.c extension.c scheme48vm.c \
         prescheme.h scheme48.h
CONFIG_FILES = signatures.scm rts-packages.scm comp-packages.scm


### Rules

# This is the first rule and therefore the make command's default target.
enough: $(VM) $(IMAGE)

$(VM): $(OBJFILES)
	$(CC) $(VMLINKFLAGS) -o $(VM) $(OBJFILES)

scheme48vm.o: scheme48vm.c prescheme.h
extension.o: extension.c scheme48.h

# The following hack traces the module dependencies described in the
# various configuration files and converts them into dependency lists
# that "make" can use for its purposes.
# You can actually feed these forms to any Scheme implementation.
# It doesn't have to be the LINKER_RUNNABLE.
# The line "(define syntactic 0) (define table 0)" exists only to suppress
# an annoying "undefined" warning for some forward references.

filenames.make: rts-packages.scm alt-packages.scm comp-packages.scm \
	   initial-packages.scm link-packages.scm more-packages.scm
	($(START_LINKER_RUNNABLE)                       \
	 echo ,load alt/config.scm alt/flatload.scm ;   \
	 echo "(load \"bcomp/defpackage.scm\")" ;       \
	 echo "(load-configuration \"packages.scm\")" ; \
	 echo "(define syntactic 0) (define table 0)" ; \
	 echo "(flatload linker-structures)" ; \
	 echo "(define q-f (all-file-names link-config))" ; \
	 echo "(flatload usual-structures)" ; \
	 echo "(define i-f (all-file-names initial-system))" ; \
	 echo "(define u-f (all-file-names usual-features initial-system))" ; \
	 echo "(write-file-names \"filenames.make\"    \
		 'initial-files i-f 'usual-files u-f 'linker-files q-f)") \
	| $(LINKER_RUNNABLE)
include filenames.make

# The linker is capable of rebuilding an image from sources, even
# across an incompatible change in VM data representations.
# The ,bench command here turns benchmark mode on.

#	 echo ,open features ;  echo ,open ascii ;   \
#	 echo ,open bitwise ;   echo ,open signals ; \
#	 echo ,open record ;    echo ,open handle ;  \

link/linker.image: $(linker-files)
	(echo ,batch ; echo ,bench on ;              \
	 echo ,open features ascii bitwise signals record handle ; \
	 echo ,open code-vectors ; \
	 echo ,load $(linker-files) ;		     \
	 echo ,load alt/init-defpackage.scm ;        \
	 echo ,dump link/linker.image)		     \
	| $(LINKER_RUNNABLE)

# The initial Scheme 48 heap image is built by the linker.  It
# contains Scheme, the byte-code compiler, and a minimal
# command processor, and no debugging environment to speak of.

initial.image: \
    $(LINKER_IMAGE) $(CONFIG_FILES) $(initial-files) initial.scm
	($(START_LINKER_RUNNABLE)               \
	 echo "(load \"initial.scm\")" ;  \
	 echo "(link-initial-system)")   \
	| $(LINKER_RUNNABLE) $(BIG_HEAP) -i $(LINKER_IMAGE)


# In the following, "make" passes $$USER to the shell as $USER, which
# the shell sees as an environment variable reference.
#
# For bootstrap reasons, initial.image is *not* listed as a source,
# even though it really is.
#
# This rule ought to work regardless of whether the system starts in
# the mini-command processor or the full command processor.

$(IMAGE): $(VM) more-signatures.scm more-packages.scm $(usual-files) \
	  initial.debug
	(echo ,load env/init-defpackage.scm ; \
	 echo "((*structure-ref filenames 'set-translation!)         \
	        \"=scheme48/\" \"./\")" ;			     \
	 echo ,load more-signatures.scm =scheme48/link-packages.scm ; \
	 echo ,load =scheme48/more-packages.scm ; \
	 echo "(ensure-loaded package-commands)" ;		     \
	 echo ",go ((*structure-ref command 'command-processor) \"batch\")" ; \
	 echo "(ensure-loaded usual-features)" ;		     \
	 echo ,structure more-structures more-structures-signature ; \
	 echo ,in debuginfo "(read-debug-info \"initial.debug\")" ;  \
	 echo ,keep maps source files ;				     \
	 echo ,translate =scheme48/ $(LIB)/  ;			     \
	 echo ,build "((*structure-ref package-commands		     \
				       'new-command-processor)	     \
		       \"(made by $$USER on `date`)\"		     \
		       built-in-structures more-structures)" $(IMAGE) ) \
	| ./$(VM) -i initial.image

main.o: main.c
	$(CC) $(CFLAGS) -c -DDEFAULT_IMAGE_NAME=\"$(LIB)/$(IMAGE)\" main.c

# Unix man page...

$(RUNNABLE).1: scheme48.man Makefile
	sed 's=LBIN=$(BIN)=g' scheme48.man \
	| sed 's=LLIB=$(LIB)=g' \
	| sed 's=LS48=$(RUNNABLE)=g' >$@

# Distribution...

# DISTFILES should include all sources.  initial.image should appear
# late in the list, so that when the distribution is extracted from the
# tar file, the file dates get set so that initial.image is newer than
# the files it depends on.

DISTFILES = COPYING README INSTALL NEWS TODO Makefile $(CFILES) \
	    filenames.make scheme48.man \
	    doc/*.txt doc/*.tex *.scm \
	    rts/*.scm bcomp/*.scm env/*.scm big/*.scm misc/*.scm \
	    link/*.scm opt/*.scm vm/*.scm alt/*.scm debug/*.scm infix/*.scm \
	    initial.image initial.debug \
	    link/lucid-script.lisp \
	    emacs/*.el emacs/README .gdbinit

dist:
	tar cf - $(DISTFILES) | gzip -c >$(DISTDIR)/$(RUNNABLE)-0-`cat minor-version-number`.tar.gz
	$(MAKE) increment-minor-version

increment-minor-version:
	(cat minor-version-number; echo 1+p) | dc >minor-version-number.tmp
	mv minor-version-number.tmp minor-version-number
	echo "(define version-info \"0.`cat minor-version-number`\")" \
	  >env/version-info.scm



### Fake targets:  all clean install man dist

all: vm linker
	$(MAKE) image
vm: $(VM)
linker: $(LINKER_IMAGE)
image: initial.image
	$(MAKE) $(IMAGE)

clean:
	-rm -f $(VM) *.o *.1 TAGS $(IMAGE) *.tmp \
	    link/*.image debug/*.image debug/*.debug

install: $(VM) $(INSTALL_METHOD) $(RUNNABLE).1 install_misc
	$(CP) $(VM) $(LIB)/
	chmod +x $(BIN)/$(RUNNABLE)
	if test -d $(MAN);  \
	  then $(CP) $(RUNNABLE).1 $(MAN)/; chmod +r $(MAN)/$(RUNNABLE).1; \
          else echo "No man directory, not installing man page" ; fi

install_misc: $(LIB)/rts $(LIB)/env $(LIB)/big $(LIB)/opt $(LIB)/misc
	$(CP) env/*.scm $(LIB)/env/
	$(CP) big/*.scm $(LIB)/big/
	$(CP) opt/*.scm $(LIB)/opt/
	$(CP) rts/jar-defrecord.scm $(LIB)/rts/
	$(CP) rts/*num.scm $(LIB)/rts/
	$(CP) misc/*.scm $(LIB)/misc/
$(LIB)/rts:
	mkdir -p $(LIB)/rts
$(LIB)/env:
	mkdir -p $(LIB)/env
$(LIB)/opt:
	mkdir -p $(LIB)/opt
$(LIB)/big:
	mkdir -p $(LIB)/big
$(LIB)/misc:
	mkdir -p $(LIB)/misc
$(LIB):
	mkdir $(LIB)

# There are two ways to make an executable file that will invoke the
# VM on the image.  "Method 1" uses the Unix kernel's shell script
# execution feature (#! at the beginning of an exec'ed file), while
# "method 2" just uses a /bin/sh script.  Method 1 is preferable,
# because it's more efficient, less sensitive to wired file names, and
# is a generally cool hack, but due to a bug ("limitation") in the
# Unix implementations I know about, it fails if that first #! line is
# more than about 32 characters, and for Scheme 48 it almost certainly
# will be.  So, method 2 is the default.

install_method_1: $(IMAGE)
	(echo '#!$(LIB)/$(VM)' -i; cat $(IMAGE)) >$(BIN)/$(RUNNABLE)

install_method_2: $(IMAGE)
	$(CP) $(IMAGE) $(LIB)/$(IMAGE)
	(echo '#!/bin/sh'; \
	 echo exec '$(LIB)/$(VM)' -o '$(LIB)/$(VM)' -i '$(LIB)/$(IMAGE)' \$$\@) \
	   > $(BIN)/$(RUNNABLE)

man:	$(RUNNABLE).1

tags:
	etags rts/*.scm bcomp/*.scm *.scm env/*.scm big/*.scm \
	  link/*.scm opt/*.scm debug/*.scm misc/*.scm


### To bootstrap from Lucid Common Lisp:

PSEUDODIR = ../pseudo

link/linker-in-lucid: link/lucid-script.lisp $(linker-files) \
	    alt/pseudoscheme-features.scm alt/pseudoscheme-record.scm
	(echo "(defvar pseudoscheme-directory \"$(PSEUDODIR)/\")"; \
	 cat link/lucid-script.lisp; \
	 echo "(dump-linker) (lcl:quit)") \
	| lisp

# Various things for debugging

debug/medium.image: $(LINKER_IMAGE) $(CONFIG_FILES) $(medium-files)
	($(START_LINKER_RUNNABLE) \
	 echo "(load \"initial.scm\")";  \
	 echo "(load-configuration \"debug-packages.scm\")";  \
	 echo "(link-medium-system)") \
	| $(LINKER_RUNNABLE) $(BIG_HEAP) -i $(LINKER_IMAGE)

debug/little.image: \
    $(LINKER_IMAGE) $(CONFIG_FILES) debug-packages.scm initial.scm
	($(START_LINKER_RUNNABLE)               \
	 echo "(load \"initial.scm\")" ;  \
	 echo "(load-configuration \"debug-packages.scm\")" ; \
	 echo "(link-little-system)")   \
	| time $(LINKER_RUNNABLE) $(BIG_HEAP) -i $(LINKER_IMAGE)

l.image: l.s48
	(echo ",batch"; \
	 echo ",take l.s48"; \
	 echo ",dump l.image") \
	| $(RUNNABLE)
