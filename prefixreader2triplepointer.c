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

unsigned filereader(char *fname, char ***array);

int main(void){
    FILE *pre, *su;
    unsigned n, i;
    char **prefixes, **suffixes;    //  array of strings; prefixes and suffixes.

    puts("\nhere's the prefix list:");
    n = filereader("prefixes.txt", &prefixes);
//    for(i=0; i<n; i++) printf("\n%s", prefixes[i]);

    puts("\nhere's the suffix list:");
    n = filereader("suffixes.txt", &suffixes);
  //  for(i=0; i<n; i++) printf("\n%s", suffixes[i]);

    free(prefixes);
    free(suffixes);
}


unsigned filereader(char *fname, char ***array){
    unsigned n, i, len;
    char c, buffer[100];
    FILE *fp;

    if((fp=fopen(fname,"r")) == NULL){ printf("\ncan't open \'%s\'.", fname); exit(1); }//  open file in text mode.

    if(fgetc(fp)==EOF) { printf("\n\'%s\' is empty.", fname); exit(1); }                //  if empty file, exit.
    rewind(fp);

    n=i=0;                                                                              //  counts lines in file. file has to end with a newline!
    while((c=fgetc(fp))!=EOF) if(c=='\n') n++;                                          //  add something later to correct files if necessary.
    rewind(fp);

    if((*array = malloc(n*sizeof(char*))) == NULL){ puts("\nmalloc failed."); exit(1); }//  malloc an array of n char pointers.

    while(EOF != fscanf(fp, "%s\n", buffer)){                                           //  read str into buffer, malloc for it, copy it over, repeat.
        len = strlen(buffer);
        if((*(*(array)+i) = malloc(len)) == NULL){ puts("\nmalloc failed."); exit(1); } //  make space for this string.
        strcpy(*(*(array)+i++), buffer);                                                //  move the string I read into the space I just made.
    }

    fclose(fp);
    return n;
}
