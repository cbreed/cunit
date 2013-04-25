#!/usr/bin/perl
#Credit for this file goes to James Blew (jhblew@ncsu.edu).
# Contact him with questions.
#All types to match.  >>>MUST BE USER-DEFINED<<<
@types=('int','void','float','bool','long','double','char');

#Check for number of args
die "Prints out file for C unit testing.  Also updates makefiles.\nUsage: funcparse.pl <file1.c file2.c ...>\n"
                        if($#ARGV==-1);

#Opens and prepares Makefile
open(MAKE, ">>Makefile");
print MAKE "test: ".join(' ',@ARGV)."\n";


#Gets functions and prepares testfiles
foreach $file(@ARGV){ #Loop through arguments as C source
        #Check that file is *.c and opens if yes.
        warn "File $file is not C source code?\n" unless($file=~/\.c$/);
        #Make the Makefile
        $file=~s/.c//;
        #print MAKE "\tgcc -Wall -std=c99 $file.c -o $file\n";
        print MAKE "\tgcc -Wall -std=c99 -c -fPIC $file.c -o $file.o\n";
        print MAKE "\tgcc $file.o -shared -o lib$file.so\n";
        print MAKE "\tgcc $file.o -o $file\n";
        print MAKE "\tgcc -Wall -Wno-implicit-function-declaration -std=c99 test_$file.c -L$ENV{'HOME'}/lib/ -L./ -ldl -lcunit -l$file -o test_$file\n";
        print MAKE "clean:\n";
        print MAKE "\trm -f *.o\n\trm -f $file\n\trm -f lib$file.so\n\trm -f test_$file\n";
        
        $file = $file .'.c';
        open(IN, $file);
        #Open and prepare output source
        open(OUT, ">test_$file");
        print OUT "#include <stdio.h>\n#include <stdlib.h>\n#include <dlfcn.h>\n#include \"cunit.h\"\n\n";
        foreach $type(@types){ #Repeat for each type. Not efficient, but it works
                while(<IN>){ #Loop through the lines and write things
                        chomp;
                        if (/^(un)?(signed )?($type\**) (.*)\((.*)\)/){
                                next if($4 eq "main");
                                push @funcs,[$1,$2,$3,$4,$5];
                        }
                }
                #Rewind the file to loop through it again
                seek(IN,0,0);
        }
        foreach (@funcs){ #Writes function pointers
                $un=$_->[0];$signed=$_->[1];$type=$_->[2];$name=$_->[3];$args=$_->[4];
                print OUT "$un$signed" if $signed=='signed';
                print OUT "$type (*$name) (";
                @temp=split(/, /,$args);
                foreach (@temp){
                        @temp2=split(/ /);
                        push @temp3,$temp2[0];
                }
                if($#temp3==-1){
                        print OUT 'void';
                } else {
                        print OUT join(', ',@temp3);
                }
                @temp3=();
                print OUT ");\n";
        }
        $file=~s/.c//;
        print OUT "\n";
        print OUT "void* handle;\nvoid startup()\n{\n\thandle = dlopen(\"./lib$file.so\", RTLD_NOW);\n\tif(!handle)
        perror(\"Library didn't open\"), exit(1);\n";
        foreach (@funcs){
                $name=$_->[3];
                print OUT "\t$name = dlsym(handle, \"$name\");\n";
        }
        print OUT "}\n";
        print OUT "void tear_down()\n{\n\tdlclose(handle);\n}\n\n";
        foreach (@funcs){
                $un=$_->[0];$signed=$_->[1];$type=$_->[2];$name=$_->[3];
                print OUT "$un$signed" if $signed=='signed';
                print OUT "$type test_$name(){\n";
                print OUT "\tstartup();\n\t//TODO: TEST CODE HERE\n\tteardown();\n";
                print OUT "}\n";
        }
        print OUT "int main(int argc, char** argv){\n";
        foreach (@funcs){
                $name=$_->[3];
                print OUT "\ttest_$name();\n";
        }
        print OUT "\tprintResults(false);\n}";
}



=begin
test: test.c 
	gcc -Wall -std=c99 -c -fPIC test.c -o test.o
	gcc test.o -shared -o libtest.so
	gcc -Wall -Wno-implicit-function-declaration -std=c99 test_test.c -L./ -lcunit -o test_test
=cut
