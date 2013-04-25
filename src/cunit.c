
/**************************************************************************************
 * Connor Reed (cbreed2@ncsu.edu)
 * 
 * This file is part of the CUnit Library.
 *  
 * The CUnit Library is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *  The CUnit Library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
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
/*!
 *  \file Header File
 *  \brief Header File
 *  More.
 */

#include "cunit.h"

/*!
 * \var Global variable to keep track of the ID for each of the
 * 	Nodes.
 */
int g_counter = 0;

/*!
 * \var Global variable to keep track of the size of the list
 */
int size = 0;

/**
 * Struct for a Node in the Linked List that
 * 	represents a single test that was executed
 */
struct Node
{
  void* expected; /* \var The expected value */
  void* actual; /* \var The actual value we received */
  enum dataType type; /* \var Enum used to denote the type for expected and actual */
  char* test_name; /* \var A description or name of the test */
  bool pass; /* \var Keeps track if this particualr test passed */
  int id; /* \var A unique ID */
  struct Node* next; /* \var A pointer to the next node in the list */
};

 
struct Node* head = NULL;


static void _dump_node(struct Node* n)
{
  
  printf("\tTest \"%s\": %s\n", n->test_name, n->pass ? "Pass" : "Fail");
  if(!n->pass)
  {
    if(n->type == type_int)
      printf("\t\tExpected %d but got %d\n", *((int*)n->expected), *(int*) n->actual);
      
    if(n->type == type_double)
      printf("\t\tExpected %f but got %f\n", *(double*)n->expected, *(double*)n->actual); 
      
    if(n->type == type_char)
      printf("\t\tExpected %c but got %c\n", *(char*)n->expected, *(char*)n->actual); 
    
    if(n->type == type_charpointer)
      printf("\t\tExpected %s but got %s\n", (char*) n->expected, (char *)n->actual);     
  }
}

static void _dump_list(bool verbose)
{
  bool allpass = true;
  printf("Results: \n");
  struct Node* temp = head;
  struct Node* next = NULL;
  while(temp != NULL)
  {
    if(verbose)
    {
      _dump_node(temp);
      if(!temp->pass)
        allpass = false;
    }
    else
    {
      
      if(!temp->pass)
      {
        allpass = false;
        _dump_node(temp);
      }
    }
    free(temp->expected);
    free(temp->actual);
    next = temp->next;
    free(temp);
    temp = next;
  }
  if(allpass)
    printf("\tAll tests passed.\n");
}

static void _add(struct Node* new)
{
  if(new->next != NULL)
  {
    //Ensure the next pointer is NULL, not garbage.
    new->next = NULL;
  }
  
  if(head ==  NULL)
  {
    //Add the new Node to the head if it's empty.
    head = new;
    size++;
    return;
  }   
    
  struct Node* temp;
  int i = 0;
  for(temp = head; temp->next != NULL; temp = temp->next)
  { //Do Nothing. Just loop around.
  }
  
  temp->next = new;
  size++;
}

void cunit_test()
{
  //Sanity Test for the library
  
  static int a = 1;
  static int b = 1;
  static double c = 1.0;
  static double d = 1.0;
  static char e = 'a';
  static char f = 'a';
  static char* g = "Hello";
  static char* h = "Hello";
  static int i = 2;
  static double j = 2.0;
  static char k = 'b';
  static char* l = "Goodbye";
  
  printf("1 = 1: %s\n", assertEquals((void*) &a, (void*) &b, type_int, "Sanity Test Pass int") ? "true" : "false");
  printf("1.0 = 1.0: %s\n", assertEquals((void*) &c, (void*) &d, type_double, "Sanity Test Pass double") ? "true" : "false");
  printf("a = a: %s\n", assertEquals((void*) &e, (void*) &f, type_char,  "Sanity Test Pass char") ? "true" : "false");
  printf("Hello = Hello: %s\n", assertEquals((void*) &g, (void*) &h, type_charpointer, "Sanity Test Pass char*") ? "true" : "false");
  
  printf("1 = 2: %s\n", assertEquals((void*) &a, (void*) &i, type_int, "Sanity Test Fail int") ? "true" : "false");
  printf("1.0 = 2.0: %s\n", assertEquals((void*) &c, (void*) &j, type_double, "Sanity Test Fail double") ? "true" : "false");
  printf("a = b: %s\n", assertEquals((void*) &e, (void*) &k, type_char, "Sanity Test Fail char") ? "true" : "false");
  printf("Hello = Goodbye: %s\n", assertEquals((void*) g, (void*) l, type_charpointer, "Sanity Test Fail char*") ? "true" : "false");
}


void printResults(bool verbose)
{
    _dump_list(verbose);
}
void start_test()
{
  /* Begin by freeing the list */
  struct Node* temp = head;
  struct Node* next = NULL;
  while(temp != NULL)
  {
    free(temp->expected);
    free(temp->actual);
    next = temp->next;
    free(temp);
    temp = next;
  }
  
  head = NULL; //Set head to null so that we create a new list
}

int* end_test(int* num_elements, int* allpass)
{
  int* ret = malloc(size * sizeof(int)); //An array to hold the results
  memset(ret, 0, size * sizeof(int));
  struct Node* temp = head;
  struct Node* next = NULL;
  int pass = 1;
  int i = 0;
  while(temp != NULL)
  {
    if(temp->pass)
      ret[i] = 1; //If the node passed saet it to one (a "true" value)
    else
      pass = 0;
    i++;
    temp = temp->next;
  }

  *num_elements = size;
  *allpass = pass;
  return ret;
}
  
bool assertEquals(void* expected, void* actual, enum dataType type, char* testname)
{
  bool pass = true;
 
  if(type == type_int)
  {
    //Cast to ints
    int* e = (int*) expected;
    int* a = (int*) actual;
    if(*e != *a)
      pass = false;
  }
  
  if(type == type_double)
  {
    double* e = (double*) expected;
    double* a = (double*) actual;
    
    if(*e != *a)
      pass = false;
  }
  
  if(type == type_char)
  {
    char* e = (char*) expected;
    char* a = (char*) actual;
    if(*e != *a)
      pass = false;
  }
  
  if(type == type_charpointer)
  {
    char* e = (char*) expected;
    char* a = (char*) actual;
    if(strcmp(e, a) != 0)
      pass = false;
  }

  
  struct Node* n = (struct Node*) malloc(sizeof(struct Node));
  
  n->expected = expected;
  n->actual = actual;
  n->type = type;
  n->test_name = testname;
  n->pass = pass;
  n->next = NULL;
  n->id = g_counter++;
  _add(n);
  
  return pass;
}

bool assertEqualsInt(int expected, int actual, char* testname)
{
  //Malloc storage for the data in memory
  
  int* e = (int*) malloc(sizeof(int));
  *e = expected;
  
  int* a = (int*) malloc(sizeof(int));
  *a = actual;
  
  bool value = assertEquals((void*) e, (void*) a, type_int, testname);
  
  
  
  if(value)
    return value;
  else
    return false;
  
}

bool assertEqualsDouble(double expected, double actual, char* testname)
{
  double* e = (double*) malloc(sizeof(double));
  *e = expected;
  
  double* a = (double*) malloc(sizeof(double));
  *a = actual;
  
  bool value = assertEquals((void*) &e, (void*) &a, type_double, testname);
  
  if(value)
    return value;
  else
    return false;
}

bool assertEqualsChar(char expected, char actual, char* testname)
{
  //Malloc storage for the data in memory
  
  char* e = (char*) malloc(sizeof(char));
  *e = expected;
  
  char* a = (char*) malloc(sizeof(char));
  *a = actual;
  
  bool value = assertEquals((void*) &e, (void*) &a, type_char, testname);
  
  if(value)
    return value;
  else
    return false;
}

bool assertEqualsCharP(char* expected, char* actual, char* testname)
{
  //Malloc storage for the data in memory
  
  char* e = strdup(expected);
  char* a = strdup(actual);
  
  bool value = assertEquals((void*) e, (void*) a, type_charpointer, testname);
  
  if(value)
    return value;
  else
    return false;
}


