/*!
 *  \file Header File
 *  \brief Header File
 *  More.
 */
/**************************************************************************************
 * Connor Reed (cbreed2@ncsu.edu)
 * 
 * This file is part of the CUnit Library.
 *  
 * The CUnit Library is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *  The CUnit Library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.

 *   You should have received a copy of the GNU General Public License
 *   along with the CUnit Library.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *  Copyright 2012
 *
 * The CUnit Library is meant to be a JUnit-like framework
 *   for C programs. 
 * Version 0.6: Beta Stage 2
 *
 **************************************************************************************/
 
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
  extern void printResults(bool verbose);
  extern int* end_test(int* num_elements, int* allpass);
#endif
