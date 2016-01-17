/*
    Neighbourhood name generator.
    Takes prefixes and suffixes from two textfiles and randomly pairs them.

    start: 2015.6.23.23:49
*/


#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void){

    FILE *prefixes, *suffixes;
    char pre[100], suf[100];

    //  open files in text mode.
    if((prefixes=fopen("prefixes.txt","r")) == NULL){ puts("\ncan't open file."); exit(1);}
    if((suffixes=fopen("suffixes.txt","r")) == NULL){ puts("\ncan't open file."); exit(1);}

    //  if empty file, exit.
    if(fgetc(prefixes)==EOF) { puts("\nempty file."); exit(1); }
    if(fgetc(suffixes)==EOF) { puts("\nempty file."); exit(1); }
    rewind(prefixes);
    rewind(suffixes);

    srand (time(NULL));

    //  figure out number of words in the file. malloc space to hold that many strings.
    //  put them in an array of strings of prefixes.
    //  do the same with the suffixes file.
    //  choose a word from each array randomly.
    //  if prefix doesn't end with 's', decide randomly to add one or not.
    //  decide randomly whether to put a space between prefix and suffix or not.
    //  if together, capitalise the first letter of prefix.
    //  else, capitalise both words. done.
    //  make the function do all the file opens, so it's not in main.





}
