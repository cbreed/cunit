all: libcunit new_file write_test
new_file: src/new_file.c
	gcc -Wall -std=c99 src/new_file.c -o bin/new_file -D USE_VI -D GEN_MAKEFILE
write_test: src/write_test.c	
	gcc -Wall -std=c99 src/write_test.c -o bin/write_test 
libcunit: cunit.o
	gcc lib/cunit.o -shared -o lib/libcunit.so
cunit.o: src/cunit.c
	gcc -c -fPIC src/cunit.c -o lib/cunit.o 
clean: 
	rm lib/* bin/*
	rm -f *.o
install:
	cp bin/new_file bin/write_test ~/bin
	cp lib/libcunit.so src/cunit.h ~/lib

	
