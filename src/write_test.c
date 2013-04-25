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
 * The write_test program generates test files (test_filename.c) from existing C files
 * and updates a Makefile to facilitate automated compilation.
 *
 * Version 0.6: Beta Stage 2
 *
 **************************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h> 

#define VERSION 0.5
#define COMMENT_BLOCK_WIDTH 56


void write_testfile(char* filename)
{

  char buf[256];
  memset(buf, '\0', sizeof(buf));
  strcat(buf, "test_");
  strcat(buf, filename);

  FILE* check = fopen(buf, "r");
  if(check != NULL)
  {
    fclose(check);
    printf("Not writing test for %s Testfile exists.\n", filename);
    return;
  }
  
  FILE* test = fopen(buf, "w+");
  
  fputs("/**", test);
	for(int i = 0; i <= COMMENT_BLOCK_WIDTH; i++)
	{
		fputs("*", test);
	}
	fputs("\n", test);
	fputs(" * ", test);
	fputs("Automated Testing For File: ", test);
	fputs(filename, test);
	fputs("\n * \n *", test);
	for(int i = 0; i <= COMMENT_BLOCK_WIDTH; i++)
	{
		fputs("*", test);
	}
	fputs("/\n\n", test);
	fputs("#include <stdio.h>\n", test);
	fputs("#include <stdlib.h>\n", test);
	fputs("#include \"cunit.h\"\n\n", test);
	fputs("int main(int argc, char** argv)\n", test);
	fputs("{\n", test);
	fputs("    \n", test);
	fputs("}\n", test);
	fclose(test);
}

void write_makefile(char** filelist)
{
	char makecmd[1024] = {};
	char objname[256] = {};
	char** files;
	int i = 0;
	if(filelist == NULL)
	{
	  files = (char**) malloc(sizeof(char*) * 256); //MAJOR TODO. Need to count files. Buffer overflow possible.
	  DIR* dir;
	  struct dirent* ent;
	  dir = opendir("./");
	  if(dir != NULL)
	  {
		  while((ent = readdir(dir)) != NULL && i < 256)
		  {			
			  char* ext = strrchr(ent->d_name, '.');
			  //strcspn(ent->d_name, "test_") == strlen(ent->d_name) && 			
			  if(ext != NULL && strcmp(ext, ".c") == 0 && i < 256)
			  {					
				  if(strncmp(ent->d_name, "test_", 5) == 0)
				  {
				    printf("File Ignored: %s\n", ent->d_name);
				    continue;
				  } 
				  else
				    files[i++] = ent->d_name;
			  }
		  }
	  }
	  if(i > 1)
	  {	
		  printf("\nFound the Following C Files:\n");
		  for(int j = 0; j < i; j++) 
			  printf("\t%s\n", files[j]);
	  }
	}
	
	else
	{
	  files = filelist;
	  while(files[i] != NULL)
      i++; //Count elements in list
	}
  
 
	//Make the File's Makefile
	FILE* make = fopen("Makefile", "a+");
#if GEN_MAKEFILE	
	fputs("all:\n", make);
	for(int j = 0; j < i; j++)
	{	
		strcat(makecmd, "	gcc -Wall -std=c99 ");
		strcat(makecmd, files[j]);
		strcat(makecmd, " -o "); 
		strncpy(objname, files[j], strlen(files[j]) - 2);
		objname[strlen(files[j])-2]='\0';
		strcat(makecmd, objname);
		strcat(makecmd, "\n");
		fputs(makecmd, make);
		memset(makecmd, '\0', sizeof(makecmd));
		memset(objname, '\0', sizeof(objname));
		
	}
	
	
#endif
	fputs("test: ", make);
	
	for(int j = 0; j < i; j++)
	{
	  fputs(files[j], make);
	  fputs(" ", make);
	}
	
	fputs("\n", make);
	memset(makecmd, '\0', sizeof(makecmd));
	memset(objname, '\0', sizeof(objname));
	for(int j = 0; j < i; j++)
	{	
		strcat(makecmd, "	gcc -Wall -std=c99 -c -fPIC ");
		strcat(makecmd, files[j]);
		strcat(makecmd, " -o "); 
		strncpy(objname, files[j], strlen(files[j]) - 2);
		objname[strlen(files[j])-2]='\0';
		strcat(makecmd, objname);
		strcat(makecmd, ".o");
		strcat(makecmd, "\n");
		fputs(makecmd, make);
		memset(makecmd, '\0', sizeof(makecmd));
		memset(objname, '\0', sizeof(objname));
		
	}

	memset(makecmd, '\0', sizeof(makecmd));
	memset(objname, '\0', sizeof(objname));
	for(int j = 0; j < i; j++)
	{	
		strcat(makecmd, "	gcc ");
		strncpy(objname, files[j], strlen(files[j]) - 2);
		objname[strlen(files[j])-2]='\0';
		strcat(makecmd, objname);
		strcat(makecmd, ".o");
		strcat(makecmd, " -shared -o ");
		strncpy(objname, files[j], strlen(files[j]) - 2);
		objname[strlen(files[j])-2]='\0';
		strcat(makecmd, "lib");
		strcat(makecmd, objname);
		strcat(makecmd, ".so");
		strcat(makecmd, "\n");
		fputs(makecmd, make);
		memset(makecmd, '\0', sizeof(makecmd));
		memset(objname, '\0', sizeof(objname));
		
	}
	
	memset(makecmd, '\0', sizeof(makecmd));
	memset(objname, '\0', sizeof(objname));
	for(int j = 0; j < i; j++)
	{	
		strcat(makecmd, "	gcc -Wall -Wno-implicit-function-declaration -std=c99 ");
		strcat(makecmd, "test_");
		strcat(makecmd, files[j]);
		strcat(makecmd, " -L");
		strcat(makecmd, getenv("HOME"));
		strcat(makecmd, "/lib/ -L./ -l"); 
		strncpy(objname, files[j], strlen(files[j]) - 2);
		objname[strlen(files[j])-2]='\0';
		strcat(makecmd, objname);
		strcat(makecmd, " -lcunit -o ");
		strcat(makecmd, "test_");
		strncpy(objname, files[j], strlen(files[j]) - 2);
		objname[strlen(files[j])-2]='\0';
		strcat(makecmd, objname);
		strcat(makecmd, "\n");
		fputs(makecmd, make);
		memset(makecmd, '\0', sizeof(makecmd));
		memset(objname, '\0', sizeof(objname));
		
		write_testfile(files[j]);
	}
	
	fputs("\n", make);
	
  fputs("\n", make);
	fputs("clean:\n", make);
	fputs("	rm -f *.o\n", make);
	memset(makecmd, '\0', sizeof(makecmd));
	memset(objname, '\0', sizeof(objname));
	for(int j = 0; j < i; j++)
	{
		strncpy(objname, files[j], strlen(files[j]) - 2);
		objname[strlen(files[j])-2]='\0';
		
		fputs("	rm -f ", make);
		fputs(objname, make);
		fputs("\n", make);
		
		
		strcat(makecmd, "lib");
		strcat(makecmd, objname);
		strcat(makecmd, ".so");
		
		fputs("	rm -f ", make);
		fputs(makecmd, make);
		fputs("\n", make);

		memset(makecmd, '\0', sizeof(makecmd));

		strcat(makecmd, "test_");
		strcat(makecmd, objname);
		
		fputs("	rm -f ", make);
		fputs(makecmd, make);
		fputs("\n", make);
		
		memset(makecmd, '\0', sizeof(makecmd));
		memset(objname, '\0', sizeof(objname));
		
	}
	fclose(make);
}

int main(int argc, char** argv)
{
  char** files = NULL;
  if(argc > 1)
  {
    if(strcmp(argv[1], "-l") == 0)
    {
      //User has selected to input his own list
      files = (char**) malloc(sizeof(char*) * (argc - 2));
      int j = 0;
      for(int i = 2; i < argc; i++) 
      {
        files[j++] = argv[i];
      }
      
      files[j] = NULL; //Null-terminate the list
    }
    
    else if(strcmp(argv[1], "-v") == 0)
      printf("Version %f\n", VERSION);
      
    else
    {
      printf("Usage: write_test [-v | -h ] [-l <file1, file2,...> ] \n");
      exit(1);
    }
  }
  
  if(files != NULL)
  {
    write_makefile(files);
  }
  
  else
    write_makefile(NULL);
} 
      
      
