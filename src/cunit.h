#ifndef _CUINT_H_
#define _CUINT_H_

  #include <stdio.h>
  #include <stdlib.h>
  #include <stdbool.h>
  #include <string.h>
  
   //Variable Declarations:
  enum dataType { type_int = 1, type_double, type_char, type_charpointer } ;
  
  //Function Prototypes:	
  extern void cunit_test();
  extern bool assertEquals(void* expected, void* actual, enum dataType type, char* testname);
  extern bool assertEqualsInt(int expected, int actual, char* testname);
  extern bool assertEqualsDouble(double expected, double actual, char* testname);
  extern bool assertEqualsChar(char expected, char actual, char* testname);
  extern bool assertEqualsCharP(char* expected, char* actual, char* testname);
  void printResults(bool verbose);
#endif
