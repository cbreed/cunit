/*****************************************
 * Connor Reed (cbreed2@ncsu.edu)
 * January 9, 2013
 * Creates a Standard Header for a C file.
 * Version: 2.5.1
 ****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 

#define VERSION 2.5
#define COMMENT_BLOCK_WIDTH 56

#if GEN_MAKEFILE
void write_makefile(); //Prototype
void write_testfile(char* filename);
#endif

int gen_test = 0;

int main(int argc, char** argv)
{

	char filename[256] = {};
	char buf[1024] = {};
	char assignment[1024] = {};
	char description[1024] = {};
#if USE_VI || USE_GEDIT
	char cmd[512] = {};
#endif


	if(argc > 1)
	{
		if(strcmp(argv[1],"-v") == 0)
		{
			printf("Version: %f\n", VERSION);
			return 0;
		}
		else if(strcmp(argv[1],"-h") == 0)
		{
			printf("newfile [-v | -h | filename]\n");
			return 0;
		}
		else
			strncpy(filename, argv[1], sizeof(filename));
	}
	else
	{
		printf("Enter File Name: ");
		scanf("%s", filename);
		getchar(); //Eat Newline
	}
	
	FILE* newFile = fopen(filename, "w+");
	if(newFile == NULL)
	{
	  printf("Null Pointer to file: %s\n", filename);
	  exit(1);
	}
	
	fputs("/**", newFile);
	for(int i = 0; i <= COMMENT_BLOCK_WIDTH; i++)
	{
		fputs("*", newFile);
	}
	fputs("\n", newFile);
	fputs(" * ", newFile);
	
	//Pull User's Name from Environment Variables
  char* name = getenv("NAME");
  if(name == NULL)
    name = " ";
  
  fputs(name, newFile);
	fputs(" \n", newFile);
	fputs(" * CSC230, Spring 2013 \n", newFile);
	fputs(" * ", newFile);
	fputs(filename, newFile);
	fputs("\n", newFile);
	

	printf("Enter Assignment: ");
	int c = getchar();
	int i = 0; 
	while(c != '\n')
	{
		buf[i++] = c;
		c = getchar();
	}
	
	strcat(assignment, " * ");
	strcat(assignment, buf);
	strcat(assignment, "\n");
	fputs(assignment, newFile);
	
	fputs(" * \n", newFile);

	printf("Enter Description: ");	
	//Zero out array
	
	memset(buf, '\0', 1024);
	c = 0;
	i = 0;
	c = getchar();
	while(c != '\n')
	{
		buf[i++] = c;
		c = getchar();
	}
	
	strcat(description, " * ");
	char line[1024] = {};
	
	
	/***************************************************************
	 * Line-Wrapping Algorithm By Gerald Pinder (gmpinder@ncsu.edu)
	 **************************************************************/
	 
	int j;
	int k = 0;
	for(j = 0; j < i; j++)
	{
	    if((k > 56) && (buf[j] == ' ') && (buf[j + 1] != '\0'))
	    {
            strcat(description, line);
            strcat(description, "\n * ");
            k = 0;
            memset(line, '\0', 1024);
        }
        else
        {
		    line[k] = buf[j];
		    k++;
	    }
		if(j == (i - 1))
		{
		    strcat(description, line);
		}
	}
	strcat(description, "\n");
		
	fputs(description, newFile);
	fputs(" **", newFile);
	for(i = 0; i <= COMMENT_BLOCK_WIDTH; i++)
	{
		fputs("*", newFile);
	}
	fputs("/\n\n", newFile);
	fputs("#include <stdio.h>\n", newFile);
	fputs("#include <stdlib.h>\n\n", newFile);
	fputs("int main(int argc, char** argv)\n", newFile);
	fputs("{\n", newFile);
	fputs("    \n", newFile);
	fputs("}\n", newFile);
	fclose(newFile);
	

	printf("Generate Testfile? [y/n]: ");
	c = getchar();

	
	if(c == 'y' || c == 'Y')
	{
		gen_test = 1;
	}
		
#if GEN_MAKEFILE
	write_makefile();
#endif

#if USE_VI
	strcat(cmd, "vi ");
	strcat(cmd, filename);
	system(cmd);
#endif

#if USE_GEDIT
	strcat(cmd, "gedit ");
	strcat(cmd, filename);
	strcat(cmd, " &");
	system(cmd);
#endif

	return 0;
}	

#if GEN_MAKEFILE
void write_makefile()
{
	char makecmd[1024] = {};
	char objname[256] = {};
	char** files = (char**) malloc(sizeof(char*) * 256); //MAJOR TODO. Need to count files. Buffer overflow possible.
	int i = 0;
	DIR* dir;
	struct dirent* ent;
	dir = opendir("./");
	if(dir != NULL)
	{
		while((ent = readdir(dir)) != NULL && i < 256)
		{
			//printf("Found: %s\n", ent->d_name);			
			char* ext = strrchr(ent->d_name, '.');
			//printf("ext = %s\n", ext);			
			if(ext != NULL && strcmp(ext, ".c") == 0 && strcspn(ent->d_name, "test_") >= 5 && i < 256)
			{	
				//printf("Found C File: %s\n", ent->d_name);				
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
	
	//Make the File's Makefile
	FILE* make = fopen("Makefile", "w");
	
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
	fputs("\n", make);
	fputs("clean:\n", make);
	if(gen_test)
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
		
		if(gen_test)
		{  
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
		}
    
		memset(makecmd, '\0', sizeof(makecmd));
		memset(objname, '\0', sizeof(objname));
		
	}

	if(gen_test)
	{
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
			//strcat(makecmd, files[j]);
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
	}
	
	fclose(make);
}

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
#endif
