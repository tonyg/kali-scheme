
#SYSTEMS=Gambit-C Bigloo Chicken MzScheme Scheme48 Scheme48-jit
SYSTEMS=MzScheme Gambit-C Chicken Scheme48 Scheme48-opt-only Scheme48-jit-noopt Scheme48-jit
BENCHMARKS="all"
SETTINGS="r5rs r6rs r6rs-fixflo r6rs-fixflo-unsafe"
COMP_S48=../go

bench-%.html: $(wildcard results-*)
	rm -f all-results.scm
	echo "(define all-results '(" > all-results.scm	&&			\
	for SYSTEM in $(SYSTEMS); do						\
	      for result in results.$$SYSTEM-$* ; do				\
		echo $$result;							\
	        echo ";---------------------------------------------------" >> all-results.scm; \
	        echo "($$SYSTEM" >> all-results.scm; 				\
	          ./summarize $$result >> all-results.scm;			\
	          echo ")" >> all-results.scm;					\
	      done;								\
	done &&									\
	echo "))" >> all-results.scm &&						\
	gsi generate-html-from-all-results.scm &&				\
	mv bench.html $@

result-Scheme48-jit-r6rs:
	./bench -s r6rs scheme48-jit all

comp.image:
	WD=`pwd`;								\
	cd ~/sw/trunk/s48-compiler;						\
	(									\
		echo ',batch';							\
		echo ',exec ,load load-all.scm';				\
		echo ',open compile-structure';					\
		echo ",dump $$WD/$@";						\
		echo ',exit';							\
	) | $(COMP_S48)
