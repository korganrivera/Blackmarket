/*
    Reads prefixes from a text file into an array, using a bunch of malloc.
    start:  2015.9.16.12:07
    end:    2015.9.16.01:06

    Next:   combine the two parts into one function to make it more trim.
            do the thing where i smush words together, or don't.

*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    FILE *pre, *su;
    unsigned n, m, len, i=0;
    char c;
    char **prefixes, **suffixes;    //  array of strings; prefixes and suffixes.
    char buffer[100];               //  to temporary hold string I read from the file.
                                    //  when this all moves to a function, I won't lose mem with buffer because it will evaporate.
                                    //  which also makes my double mallocs sort of pointless, but whatevs.

    //  open prefix file in text mode.
    if((pre=fopen("prefixes.txt","r")) == NULL){ puts("\ncan't open prefix file."); exit(1);}

    //  if empty prefix file, exit.
    if(fgetc(pre)==EOF) { puts("\nprefix file is empty."); exit(1); }
    rewind(pre);

    //  open suffix file in text mode.
    if((su=fopen("suffixes.txt","r")) == NULL){ puts("\ncan't open suffix file."); exit(1);}

    //  if empty suffix file, exit.
    if(fgetc(su)==EOF) { puts("\nsuffix file is empty."); exit(1); }
    rewind(su);
////
    //  counts lines in file. prefix file has to end with a newline!
    //  add a bit to correct files if not done, later.
    n=0;
    while((c=fgetc(pre))!=EOF) if(c=='\n') n++;
    rewind(pre);

    printf("\nI counted %u prefixes.", n);

    //  malloc an array of n char pointers.
    if((prefixes = malloc(n*sizeof(char*))) == NULL){ puts("\nmalloc failed."); exit(1); }

    puts("\narray of prefix pointers malloced successfully.");

    //  read strings into buffer. for each, malloc space for it, copy it over and repeat.
    while(EOF != fscanf(pre, "%s\n", buffer)){
        len = strlen(buffer);

        //  make space for this string.
        if((prefixes[i] = malloc(len*sizeof(char))) == NULL){ puts("\nmalloc failed."); exit(1); }

        //  move the string I read into the space I just made.
        strcpy(prefixes[i++], buffer);
    }
    fclose(pre);
    puts("\nprefix file successfully read.");
////
    //  do the same thing for suffix file.
    m=i=0;
    while((c=fgetc(su))!=EOF) if(c=='\n') m++;
    rewind(su);

    printf("\nI counted %u suffixes.", m);

    //  malloc an array of n char pointers.
    if((suffixes = malloc(m*sizeof(char*))) == NULL){ puts("\nmalloc failed."); exit(1); }

    puts("\narray of suffix pointers malloced successfully.");

    //  read strings into buffer. for each, malloc space for it, copy it over and repeat.
    while(EOF != fscanf(su, "%s\n", buffer)){
        len = strlen(buffer);

        //  make space for this string.
        if((suffixes[i] = malloc(len*sizeof(char))) == NULL){ puts("\nmalloc failed."); exit(1); }

        //  move the string I read into the space I just made.
        strcpy(suffixes[i++], buffer);
    }
    fclose(su);
    puts("\nsuffix file successfully read.");

    puts("\ndone.  here's the prefix list:");
    for(i=0; i<n; i++) printf("\n%s", prefixes[i]);
    puts("\npress a key to continue");
    getchar();
    puts("\ndone.  here's the suffix list:");
    for(i=0; i<m; i++) printf("\n%s", suffixes[i]);

    free(prefixes);
    free(suffixes);
}



