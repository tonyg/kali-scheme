# Scheme 48 Makefile
# Last updated January 1994 by JAR
# Documentation in file INSTALL

# One of these days maybe we'll figure out how to use Gnu autoconf.

# If you want to install Scheme 48 somewhere, set BIN, LIB, and MAN as
# appropriate for your site.  RUNNABLE is what the command by which
# you invoke Scheme 48 will be called.  LIB is the directory tree in
# which files are sought when doing ,load-package of one of the
# "more-packages" (big/pp.scm, etc.; see more-packages.scm).

# If you definitely won't be installing Scheme 48, you should set LIB
# to the directory that contains this Makefile.  This will make the
# ,load-package command work for the "extra" packages defined in
# more-packages.scm. 

TOPDIR = /usr/local
BIN = $(TOPDIR)/bin
LIB = $(TOPDIR)/lib/scheme48
MAN = $(TOPDIR)/man/man1

# You might want to change RUNNABLE to "s48"
RUNNABLE = scheme48

# -N is needed to allow dynamic loading (at least on a DEC MIPS)
# VMLINKFLAGS = -N

# Libraries used by the VM:
#on most systems
VMLIBS= -lm
#on SGI
#VMLIBS = -lm -lmld
#on SunOS5
#VMLIBS= -lnsl -lgen -ldl -lsocket
#on SunOS4
#VMLIBS= -ldl

CC = gcc
#CFLAGS = -g
CFLAGS = -O2 -g
# On an HP "snake," you may instead want:
#   CC = cc -Aa
#   CFLAGS = -D_HPUX_SOURCE -O +Obb1500
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

LINKER_VM = ./$(VM) $(BIG_HEAP)
LINKER_RUNNABLE = $(LINKER_VM) -i ./$(IMAGE)
LINKER_IMAGE = link/linker.image
LINKER = $(LINKER_VM) -i $(LINKER_IMAGE)
START_LINKER = echo ,batch; echo ,bench on;

# or,
LINKER_RUNNABLE = $(RUNNABLE)
# and the other two as above


# Sources:

CFILES = main.c unix.c dynload.c error.c extension.c scheme48vm.c \
         prescheme.h scheme48.h
CONFIG_FILES = interfaces.scm low-packages.scm rts-packages.scm \
	       comp-packages.scm


### Rules

# This is the first rule and therefore the make command's default target.
enough: $(VM) $(IMAGE) .notify

# The developers are curious to know.
.notify:
	echo Another 0.`cat minor-version-number` installation. \
	   | mail scheme-48-notifications@martigny.ai.mit.edu 
	touch .notify

$(VM): $(OBJFILES)
	$(CC) $(VMLINKFLAGS) -o $(VM) $(OBJFILES) $(VMLIBS)

scheme48vm.o: scheme48vm.c prescheme.h
extension.o: extension.c scheme48.h
dynload.o: dynload.c scheme48.h

# The following hack traces the module dependencies described in the
# various configuration files and converts them into dependency lists
# that "make" can use for its purposes.
# You can actually feed these forms to any Scheme implementation.
# It doesn't have to be the initial.image.
# The line "(define syntactic 0) (define tables 0)" exists only to suppress
# an annoying "undefined" warning for some forward references.

# If there are errors running this script, and you need to debug,
# don't use the initial.image, use something that has a reasonable
# environment, e.g. any version of the full Scheme 48 system.

# If this fails and you don't feel like debugging or fixing the problem,
# try "touch filenames.make" and hope for the best.

filenames.make: packages.scm rts-packages.scm alt-packages.scm \
		comp-packages.scm initial-packages.scm link-packages.scm \
		more-packages.scm
	touch filenames.make
	$(MAKE) $(VM)
	(echo ,load alt/config.scm alt/flatload.scm bcomp/module-language.scm;\
	 echo \(load-configuration \"packages.scm\"\);  \
	 echo \(define syntactic 0\) \(define tables 0\); \
	 echo \(flatload linker-structures\);		 \
	 echo \(define q-f \(all-file-names link-config\)\); \
	 echo \(flatload usual-structures\);		 \
	 echo \(define i-f \(all-file-names initial-system\)\); \
	 echo \(define u-f \(all-file-names usual-features initial-system\)\);\
	 echo \(write-file-names \"filenames.make\"      \
		 \'initial-files i-f \'usual-files u-f \'linker-files q-f\)) \
	| ./$(VM) -i initial.image -a batch
# or:	| $(RUNNABLE) -a batch

# If make barfs on this include line, just comment it out.  It's only
# really needed if you want to build the linker or rebuild initial.image.
include filenames.make
#NetBSD make wants to see this instead:
#.include "filenames.make"

# The linker is capable of rebuilding an image from sources, even
# across an incompatible change in VM data representations.
# The ,bench command here turns benchmark mode on.
# (set! *package-uid* 0) is a kludge.

link/linker.image: $(linker-files)
	(echo ,batch; echo ,bench on;                  \
	 echo ,open signals handle features;           \
	 echo ,open bitwise ascii code-vectors record; \
	 echo ,load $(linker-files);		       \
	 echo ,load alt/init-defpackage.scm;	       \
	 echo \(set! *package-uid* 0\);		       \
	 echo ,dump link/linker.image)		       \
	| $(LINKER_RUNNABLE)

# The initial Scheme 48 heap image is built by the linker.  It
# contains Scheme, the byte-code compiler, and a minimal
# command processor, and no debugging environment to speak of.

initial.image: $(LINKER_IMAGE) $(CONFIG_FILES) initial.scm $(initial-files)
	($(START_LINKER)               \
	 echo \(load-configuration \"interfaces.scm\"\); \
	 echo \(load-configuration \"packages.scm\"\); \
	 echo \(flatload initial-structures\); \
	 echo \(load \"initial.scm\"\);  \
	 echo \(link-initial-system\))   \
	| $(LINKER)


# In the following, "make" passes $$USER to the shell as $USER, which
# the shell sees as an environment variable reference.
#
# For bootstrap reasons, initial.image is *not* listed as a source,
# even though it really is.
#
# This rule ought to work regardless of whether the system starts in
# the mini-command processor or the full command processor.

$(IMAGE): $(VM) more-interfaces.scm more-packages.scm $(usual-files) \
	  initial.debug
	(echo ,load env/init-defpackage.scm; \
	 echo \(\(\*structure-ref filenames \'set-translation!\)     \
	        \"=scheme48/\" \"./\"\);			     \
	 echo ,load more-interfaces.scm =scheme48/link-packages.scm; \
	 echo ,load =scheme48/more-packages.scm;		     \
	 echo \(ensure-loaded command-processor\);		     \
	 echo \(ensure-loaded usual-commands\);			     \
	 echo \(\(\*structure-ref command-processor \'set-command-structure!\)\
	        usual-commands\); \
	 echo ,go \(\(*structure-ref command-processor \'command-processor\) \
	            \"batch\"\); \
	 echo \(ensure-loaded usual-features\);		            \
	 echo ,structure more-structures more-structures-interface; \
	 echo ,in debuginfo \(read-debug-info \"initial.debug\"\);  \
	 echo ,keep maps source files;				     \
	 echo ,translate =scheme48/ $(LIB)/ ;			     \
	 echo ,build \(\(*structure-ref package-commands-internal    \
				        \'new-command-processor\)    \
		       \"\(made by $$USER on `date`\)\"		     \
		       built-in-structures more-structures\) $(IMAGE) ) \
	| ./$(VM) -i initial.image -a batch

main.o: main.c
	$(CC) $(CFLAGS) -c -DDEFAULT_IMAGE_NAME=\"$(LIB)/$(IMAGE)\" main.c

# Unix man page...

$(RUNNABLE).1: scheme48.man Makefile
	sed 's=LBIN=$(BIN)=g' scheme48.man \
	| sed 's=LLIB=$(LIB)=g' \
	| sed 's=LS48=$(RUNNABLE)=g' >$@


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

install: enough $(INSTALL_METHOD) $(RUNNABLE).1 install_misc
	install-vm
	chmod +x $(BIN)/$(RUNNABLE)
	if test -d $(MAN);  \
	  then $(CP) $(RUNNABLE).1 $(MAN)/; chmod +r $(MAN)/$(RUNNABLE).1; \
          else echo "No man directory, not installing man page"; fi

install-vm:
	$(CP) $(VM) $(LIB)/

install_misc: $(LIB)/rts $(LIB)/env $(LIB)/big $(LIB)/opt $(LIB)/misc $(LIB)/link
	$(CP) env/*.scm $(LIB)/env/
	$(CP) big/*.scm $(LIB)/big/
	$(CP) opt/*.scm $(LIB)/opt/
	$(CP) rts/jar-defrecord.scm $(LIB)/rts/
	$(CP) rts/*num.scm $(LIB)/rts/
	$(CP) link/*.scm $(LIB)/link/
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
$(LIB)/link:
	mkdir -p $(LIB)/link
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


# Distribution...

# DISTFILES should include all sources.  initial.image should appear
# late in the list, so that when the distribution is extracted from the
# tar file, the file dates get set so that initial.image is newer than
# the files it depends on.
# Also, filenames.make must follow *.scm in this list, or else it will
# appear to be out of date.

DISTFILES = COPYING README INSTALL NEWS TODO Makefile $(CFILES) \
	    scheme48.man doc/*.txt doc/*.tex doc/*.ps \
	    *.scm filenames.make \
	    rts/*.scm bcomp/*.scm env/*.scm big/*.scm misc/*.scm \
	    link/*.scm opt/*.scm vm/*.scm alt/*.scm debug/*.scm infix/*.scm \
	    initial.image initial.debug \
	    link/*.lisp link/*.exec \
	    emacs/*.el emacs/README .gdbinit *-version-number

dist: initial.image
	tar cf - $(DISTFILES) | gzip -c >$(DISTDIR)/$(RUNNABLE)-0-`cat minor-version-number`.tar.gz
#	$(MAKE) new-version

inc:
	(cat minor-version-number; echo 1+p) | dc >minor-version-number.tmp
	mv minor-version-number.tmp minor-version-number
	echo \(define version-info \"0.`cat minor-version-number`\"\) \
	  >env/version-info.scm


### To bootstrap from Lucid Common Lisp:

PSEUDODIR = ../pseudo

link/linker-in-lucid: link/lucid-script.lisp $(linker-files) \
	    alt/pseudoscheme-features.scm alt/pseudoscheme-record.scm
	(echo \(defvar pseudoscheme-directory \"$(PSEUDODIR)/\"\); \
	 cat link/lucid-script.lisp; \
	 echo \(dump-linker\) \(lcl:quit\)) \
	| lisp

# Various things for debugging

debug/medium.image: $(LINKER_IMAGE) $(CONFIG_FILES) $(medium-files)
	($(START_LINKER)                 \
	 echo \(load-configuration \"interfaces.scm\"\); \
	 echo \(load-configuration \"packages.scm\"\); \
	 echo \(flatload initial-structures\); \
	 echo \(load-configuration \"debug-packages.scm\"\);  \
	 echo \(link-medium-system\))    \
	| $(LINKER)

debug/mini.image: $(LINKER_IMAGE) $(CONFIG_FILES)
	($(START_LINKER)                 \
	 echo \(load-configuration \"interfaces.scm\"\); \
	 echo \(load-configuration \"packages.scm\"\); \
	 echo \(flatload initial-structures\); \
	 echo \(load-configuration \"debug-packages.scm\"\);  \
	 echo \(link-mini-system\))    \
	| $(LINKER)

debug/little.image: $(LINKER_IMAGE) $(CONFIG_FILES) debug-packages.scm
	($(START_LINKER)                 \
	 echo \(load-configuration \"interfaces.scm\"\); \
	 echo \(load-configuration \"packages.scm\"\); \
	 echo \(flatload initial-structures\); \
	 echo \(load-configuration \"debug-packages.scm\"\); \
	 echo \(link-little-system\))    \
	| time $(LINKER)
