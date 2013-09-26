cunit
=====

CUnit - A Open-Source Library based on JUnit for Unit-Testing C programs.

INSTALLATION:
  The usual make && make install applies, like always. After compiling everything,
   the install.sh script will run, and prompt you for your name. It will export the 
   following variables in your ~/.bashrc file:
   
    export NAME=<the name you entered>
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/lib
    export CPATH=$CPATH:$HOME/lib
    export PATH=$PATH:$HOME/bin
    
  If you use a C-Shell, you can move this to your ~/.mycshrc file.
  
  Installed Files:
    - $HOME/bin/new_file: Creates a new file using NCSU's file commenting standards
    - $HOME/bin/write_test: Generates an empty testfile and creates a Makefile that
          links against the library (More on this in the usage section)
    - $HOME/bin/funcparse.pl: Similar to write_test, but instead of generating a    
          Makefile and a testfile with just a main method, the script will generate 
          a file with test method for each method declared in the file under test 
          (More in the usage section)
    - $HOME/lib/libcunit.so: The library itself
    - $HOME/lib/cunit.h: The header file for the library
          
USAGE:

  For the sake of example, we'll say you have a C file you want to test called
    myfile.c:
    
      #include <stdio.h>
      #include <stdlib.h>

      int add(int a, int b) {return a+b;}
      int subtract(int a, int b) {return a-b;}
      int multiply(int a, int b) {return a*b;}
      int divide(int a, int b) {return a/b;}

      int main(int argc, char** argv)
      {
        printf("3 + 2 = %d\n", add(3,2));
        printf("3 - 2 = %d\n", subtract(3,2));
        printf("3 * 2 = %d\n", multiply(3,2));
        printf("3 / 2 = %d\n", divide(3,2));
      }
      
  If you use the write_test program, two new files will be created in the same
    directory: Makefile and test_myfile.c. The Makefile will contain:
      
      
      test: myfile.c 
	      gcc -Wall -std=c99 -c -fPIC myfile.c -o myfile.o
	      gcc HW1.o -shared -o libmyfile.so
	      gcc -Wall -Wno-implicit-function-declaration -std=c99 test_myfile.c -L$HOME/lib/ -L./ -lmyfile -lcunit -o test_myfile

      clean:
	      rm -f *.o
	      rm -f myfile
	      rm -f libmyfile.so
	      rm -f test_myfile
	      
	      (NOTE: $HOME will be replaced with the actual value of $HOME)
	      
	  The C file will contain:
	  
	#include <stdio.h>
        #include <stdlib.h>
        #include "cunit.h"

        int main(int argc, char** argv)
        {
	      
	}
	      
	If you use the funcparse.pl script instead, the Makefile will look like:
	
	    test: myfile.c
	      gcc -Wall -std=c99 -c -fPIC myfile.c -o myfile.o
	      gcc myfile.o -shared -o libmyfile.so
	      gcc myfile.o -o myfile
	      gcc -Wall -std=c99 test_myfile.c -L$HOME/lib/ -L./ -ldl -lcunit -lmyfile -o test_myfile
      clean:
	      rm -f *.o
	      rm -f myfile
	      rm -f libmyfile.so
	      rm -f test_myfile
	      
	  The C file will look like:
	  
	    #include <stdio.h>
      #include <stdlib.h>
      #include <dlfcn.h>
      #include "cunit.h"

      int (*add) (int, int);
      int (*subtract) (int, int);
      int (*multiply) (int, int);
      int (*divide) (int, int);

      void* handle;
      void startup()
      {
	      handle = dlopen("./libHW1.so", RTLD_NOW);
	      if(!handle)
              perror("Library didn't open"), exit(1);
	      add = dlsym(handle, "add");
	      subtract = dlsym(handle, "subtract");
	      multiply = dlsym(handle, "multiply");
	      divide = dlsym(handle, "divide");
      }
      void teardown()
      {
	      dlclose(handle);
      }

      void test_add(){
	      startup();
	      //TODO: TEST CODE HERE
	      teardown();
      }
      void test_subtract(){
	      startup();
	      //TODO: TEST CODE HERE
	      teardown();
      }
      void test_multiply(){
	      startup();
	      //TODO: TEST CODE HERE
	      teardown();
      }
      void test_divide(){
	      startup();
	      //TODO: TEST CODE HERE
	      teardown();
      }
      int main(int argc, char** argv){

	      test_add();
	      test_subtract();
	      test_multiply();
	      test_divide();
	      printResults(false);
      }
      
  The main difference between the two methods is that the write_test method links
    against the shared object compiled from the code under test, and the funcparse
    methodology dynamically opens it with dlopen(). There are benefits and 
    downfalls to both, but in general follow these guidelines:
    
      -Use write_test if:
        
        -You have complicated source or a complicated build system, and don't
          need much pre-generation
          
        -You're an advanced programmer who will end up writing a bunch of
          custom test code anyways
          
        -Have a testing environment that will depend on rapid or accurate
          timings in functions
          
      -Use funcparse.pl if:
      
        -You want simple and easy to write test code
        
        -You have no requirement for a highly customized makefile
        
        
        
        
	        
      
    
        
