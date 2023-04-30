CC=gcc
CFLAGS=-Wall -Werror -g #-fopenmp
LIBS=-lcunit -lpthread
INCLUDE_HEADERS_DIRECTORY=-Iinclude

# Rule to build the main program sp
sp: sp.c src/shortest_path.o src/files.o src/graph.o #include/portable_endian.o include/portable_semaphore.o
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LIBS)

# Rule to compile object files
%.o: %.c
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $<

# Rule to clean the project
clean:
	rm -f src/*.o
	rm -f sp
	rm -f tests
	rm -f perf
	rm -f test/*.txt
	rm -f *.txt

# Rule to build and run CUnit tests
tests: test/bellman_ford_tests.c src/shortest_path.o src/files.o src/graph.o #include/portable_endian.o include/portable_semaphore.o
	@echo "Compiling and executing CUnit tests..."
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS)  -o $@ $^ $(LIBS)

#Testing sp with 4 threads
	./sp -f test/OutputFiles/petitTest1.bin test/InputFiles/petit.bin
	./sp -f test/OutputFiles/grandTest2.bin test/InputFiles/grand.bin
	./sp -f test/OutputFiles/denseTest3.bin test/InputFiles/dense.bin
	./sp -f test/OutputFiles/IsoleTest4.bin test/InputFiles/Isole.bin
	./sp -f test/OutputFiles/NonConnexeTest5.bin test/InputFiles/Nonconnexe.bin

#Testing sp with 1 thread
	./sp -f test/OutputFiles/petitTest1thread.bin test/InputFiles/petit.bin
	./sp -f test/OutputFiles/grandTest1thread.bin test/InputFiles/grand.bin
	./sp -f test/OutputFiles/denseTest1thread.bin test/InputFiles/dense.bin
	./sp -f test/OutputFiles/IsoleTest1thread.bin test/InputFiles/Isole.bin
	./sp -f test/OutputFiles/NonConnexeTest1thread.bin test/InputFiles/Nonconnexe.bin

#On mets dans ./test des méthode pour vérifier que les fichiers outputfile1.bin, outputfile2.bin et outputfile3.bin sont bien correctes
	./tests

# Rule to build and run performance tests
perf: test/performance_tests.c src/shortest_path.o src/files.o src/graph.o #include/portable_endian.o include/portable_semaphore.o 
	@echo "Compiling and executing performance tests..."
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS)  -o $@ $^ $(LIBS) 
	./perf

# a .PHONY target forces make to execute the command even if the target already exists
.PHONY: clean tests perf
