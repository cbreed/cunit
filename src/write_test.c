/**********************************************
 * Program for Generating Test Files Using
 * The CUnit Library
 *
 *********************************************/
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdbool.h>
 #include <dirent.h> 
 
 #define VERSION 0.01
 #define COMMENT_BLOCK_WIDTH 56
void write_testfile(char* filename)
{

  char buf[256];
  memset(buf, '\0', sizeof(buf));
  strcat(buf, "test_");
  strcat(buf, filename);
  
  DIR* dir;
	struct dirent* ent;
	dir = opendir("./");
	if(dir != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
			//printf("Found: %s\n", ent->d_name);			
			char* ext = strrchr(ent->d_name, '.');
			//printf("ext = %s\n", ext);			
			if(ext != NULL && strcmp(ext, ".c") == 0 && strcmp(ent->d_name, buf) == 0)
			{	
				printf("Ignoring File: %s\n", ent->d_name);
				return; //Ignore existing test_* files
			}
		}
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
			  if(ext != NULL && strcmp(ext, ".c") == 0 && strcspn(ent->d_name, "test_") == strlen(ent->d_name) && i < 256)
			  {					
				  files[i++] = ent->d_name;
			  }
		  }
	  }
	  if(i > 1)
	  {	
		  printf("Found the Following C Files:\n");
		  for(int j = 0; j < i; j++) 
			  printf("%s\n", files[j]);
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
    
   
    if(strcmp(argv[1], "-v") == 0)
      printf("Version %f\n", VERSION);
      
    if(strcmp(argv[1], "-h") == 0)
      printf("Usage: write_test [-v | -h ] [-l <file1, file2,...> ] \n");
  }
  
  if(files != NULL)
  {
    write_makefile(files);
  }
  
  else
    write_makefile(NULL);
} 
      
      
