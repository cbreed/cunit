/**
 * @file
 * @author Connor Reed (cbreed2@ncsu.edu)
 * @version 0.6 Beta Stage 2
 * 
 * 
 * @section LICENSE
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
 * @section DESCRIPTION
 * The CUnit Library is meant to be a JUnit-like framework
 *   for C programs. This file is part of the CUnit Library.
 *
 */
 
#ifndef _CUINT_H_
#define _CUINT_H_

  #include <stdio.h>
  #include <stdlib.h>
  #include <stdbool.h>
  #include <string.h>
  
  /**
   * Enumeration for assertEquals that indicates the type
   */
  enum dataType 
  { 
	  /** A int */
	  type_int = 1, 
	  /** A double */
	  type_double, 
	  /** A char */
	  type_char, 
	  /** A char* */ 
	  type_charpointer 
  };
  
  /**
   * External function that preforms a Sanity test on the library
   */	
  extern void cunit_test();
  
  /**
   * The main logic behind the library. Preforms the equality test and adds the result node
   * 	to the list.
   * 	
   * @param expected Pointer to the expected value
   * @param actual Pointer to the actual value 
   * @param type An enumeration indcating the type
   * @param testname A descriptive name for the test
   * 
   * @returns Indicates if the test passed (true if passed)
   */
  extern bool assertEquals(void* expected, void* actual, enum dataType type, char* testname);
  
  /**
   * Wrapper function for the assertEquals method for int's
   * 
   * @param expected The expected value
   * @param actual The actual value
   * @param testname A descriptive name for the test
   * 
   * @returns Indicates if the test passed (true if passed)
   */
  extern bool assertEqualsInt(int expected, int actual, char* testname);
  
  /**
   * Wrapper function for the assertEquals method for double's
   * 
   * @param expected The expected value
   * @param actual The actual value
   * @param testname A descriptive name for the test
   * 
   * @returns Indicates if the test passed (true if passed)
   */
  extern bool assertEqualsDouble(double expected, double actual, char* testname);
  
  /**
   * Wrapper function for the assertEquals method for char's
   * 
   * @param expected The expected value
   * @param actual The actual value
   * @param testname A descriptive name for the test
   * 
   * @returns Indicates if the test passed (true if passed)
   */
  extern bool assertEqualsChar(char expected, char actual, char* testname);
  
  /**
   * Wrapper function for the assertEquals method for char*'s
   * 
   * @param expected The expected value
   * @param actual The actual value
   * @param testname A descriptive name for the test
   * 
   * @returns Indicates if the test passed (true if passed)
   */
  extern bool assertEqualsCharP(char* expected, char* actual, char* testname);
  
  /**
   * Calls _dump_list and passes verbose on.
   */
  extern void printResults(bool verbose);
  
  /**
   * Clears out the list and starts a new test session
   */
  extern void start_test();
  
  /**
   * Reads the contents of the list and creates a dynamically allocated
   * array of int's that represents the result of each test (1 is a pass)
   * 
   * @param num_elements Return buffer that will be filled with the number of 
   * 	elements in the array
   * @param allpass Return buffer that will be 1 if all the tests pass
   * @returns A pointer to an array of int's that contains the results
   * 	of each test. This will need to be freed.
   */
  extern int* end_test(int* num_elements, int* allpass);
#endif
