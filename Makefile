.PHONY: all clean test compile0 compile1 compile2 compile3 test0 test1 test2 test3

all: directories compile0 compile1 compile2 compile3

clean:
	rm -rf ./Build/*
	rm -rf ./Informations
	rm -rf ./Results
	rm -rf ./Tables
	rm -rf ./Plots
	clear

test: test0 test1 test2 test3 statistics

maxThread=8 # max number of threads
it=10000 # max number of iterations
 
directories:
	python3 ./Source/directories.py

compile0:
	gcc ./Source/alpha_beta_tris.c -o ./Build/alpha_beta_tris_seq0 -O0
	gcc -fopenmp ./Source/alpha_beta_tris.c -o ./Build/alpha_beta_tris0 -O0

	# gcc-12 -o ./Build/alpha_beta_tris_seq0 ./Source/alpha_beta_tris.c -O0
	# gcc-12 -fopenmp -o ./Build/alpha_beta_tris0 ./Source/alpha_beta_tris.c -O0
	
compile1:
	gcc ./Source/alpha_beta_tris.c -o ./Build/alpha_beta_tris_seq1 -O1
	gcc -fopenmp ./Source/alpha_beta_tris.c -o ./Build/alpha_beta_tris1 -O1

	# gcc-12 -o ./Build/alpha_beta_tris_seq1 ./Source/alpha_beta_tris.c -O1
	# gcc-12 -fopenmp -o ./Build/alpha_beta_tris1 ./Source/alpha_beta_tris.c -O1

compile2:
	gcc ./Source/alpha_beta_tris.c -o ./Build/alpha_beta_tris_seq2 -O2
	gcc -fopenmp ./Source/alpha_beta_tris.c -o ./Build/alpha_beta_tris2 -O2

	# gcc-12 -o ./Build/alpha_beta_tris_seq2 ./Source/alpha_beta_tris.c -O2
	# gcc-12 -fopenmp -o ./Build/alpha_beta_tris2 ./Source/alpha_beta_tris.c -O2
	
compile3:
	gcc ./Source/alpha_beta_tris.c -o ./Build/alpha_beta_tris_seq3 -O3
	gcc -fopenmp ./Source/alpha_beta_tris.c -o ./Build/alpha_beta_tris3 -O3

	# gcc-12 -o ./Build/alpha_beta_tris_seq3 ./Source/alpha_beta_tris.c -O3
	# gcc-12 -fopenmp -o ./Build/alpha_beta_tris3 ./Source/alpha_beta_tris.c -O3

statistics:
	python3 ./Source/analize.py
	python3 ./Source/tableCreator.py
	python3 ./Source/plotCreator.py

test0:
	i=1; while [ $$i -le $(it) ] ; do \
		./Build/alpha_beta_tris_seq0 0 ;\
		numThread=1; while [ $$numThread -le $(maxThread) ] ; do \
			export OMP_NUM_THREADS=$$numThread && \
			./Build/alpha_beta_tris0 0 $$numThread ;\
		numThread=$$((numThread+1)); \
		done; \
	i=$$((i+1)); \
	done; \

test1:
	i=1; while [ $$i -le $(it) ] ; do \
		./Build/alpha_beta_tris_seq1 1 ;\
		numThread=1; while [ $$numThread -le $(maxThread) ] ; do \
			export OMP_NUM_THREADS=$$numThread && \
			./Build/alpha_beta_tris1 1 $$numThread ;\
		numThread=$$((numThread+1)); \
		done; \
	i=$$((i+1)); \
	done; \

test2:
	i=1; while [ $$i -le $(it) ] ; do \
		./Build/alpha_beta_tris_seq2 2 ;\
		numThread=1; while [ $$numThread -le $(maxThread) ] ; do \
			export OMP_NUM_THREADS=$$numThread && \
			./Build/alpha_beta_tris2 2 $$numThread ;\
		numThread=$$((numThread+1)); \
		done; \
	i=$$((i+1)); \
	done; \

test3:
	i=1; while [ $$i -le $(it) ] ; do \
		./Build/alpha_beta_tris_seq3 3 ;\
		numThread=1; while [ $$numThread -le $(maxThread) ] ; do \
			export OMP_NUM_THREADS=$$numThread && \
			./Build/alpha_beta_tris3 3 $$numThread ;\
		numThread=$$((numThread+1)); \
		done; \
	i=$$((i+1)); \
	done; \