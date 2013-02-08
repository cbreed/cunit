/****************************************************************
 * Connor Reed (cbreed2@ncsu.edu)
 *
 * The CUnit Library is meant to be a JUnit-like framework
 *  for C programs. 
 * Version 0.01: Pre-Alpha
 *
 ****************************************************************/


#include "cunit.h"

char* g_testName = " ";
int g_counter = 0; 

struct Node
{
  struct Node* prev;
  void* expected;
  void* actual;
  enum dataType type;
  char* test_name;
  bool pass;
  int id;
  struct Node* next;
};

 
struct Node* head = NULL;


void _dump_node(struct Node* n)
{
  
  printf("\tTest \"%s\": %s\n", n->test_name, n->pass ? "Pass" : "Fail");
  if(!n->pass)
  {
    if(n->type == type_int)
    {
      int* e = (int*) n->expected;
      int* a = (int*) n->actual;
      printf("\t\tExpected %d but got %d\n", *e, *a);
    } 
      
    if(n->type == type_double)
      printf("\t\tExpected %f but got %f\n", *(double*)n->expected, *(double*)n->actual); 
      
    if(n->type == type_char)
      printf("\t\tExpected %c but got %c\n", *(char*)n->expected, *(char*)n->actual); 
    
    if(n->type == type_charpointer)
      printf("\t\tExpected %s but got %s\n", (char*) n->expected, (char *)n->actual);     
  }
}

void _dump_list(bool verbose)
{
  bool allpass = true;
  printf("Results: \n");
  struct Node* temp = head;
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
    free(temp);
    temp = temp->next;
  }
  if(allpass)
    printf("All tests passed.\n");
}

void _add(struct Node* new)
{
  if(new->next != NULL)
  {
    //Ensure the next pointer is NULL, not garbage.
    new->next = NULL;
  }
  
  if(head ==  NULL)
  {
    //Add the new Node t othe head if it's empty.
    head = new;
    return;
  }   
    
  struct Node* temp;
  int i = 0;
  for(temp = head; temp->next != NULL; temp = temp->next)
  { //Do Nothing. Just loop around.
  }
  
  temp->next = new;
  temp->prev = temp;

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
  {
    //printf("Test \"%s\" Failed. Expected: %d, but got: %d\n", testname, expected, actual);
    return false;
  }
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
  {
    //printf("Test \"%s\" Failed. Expected: %lf, but got: %lf\n", testname, expected, actual);
    return false;
  }
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
  {
    //printf("Test \"%s\" Failed. Expected: %c, but got: %c\n", testname, expected, actual);
    return false;
  }
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
  {
    //printf("Test \"%s\" Failed. Expected: %s, but got: %s\n", testname, expected, actual);
    return false;
  }
}


