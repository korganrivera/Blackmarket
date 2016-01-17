/*
    street index giver.
    My map has streets, but I can't name streets until I give each contiguous
    length of street is given an index number.  Then I can somehow import names and
    apply them.  Then, I can get rid of the list of imported names, to save memory.
    I'll be doing this naming thing recursively as usual.  I'll start on any random
    piece of street and spread through the streets like bacteria.

    start:  2015.9.15.12:49
    end:
*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define HEIGHT 60
#define WIDTH 60

void ushow(unsigned * map, unsigned height, unsigned width);    //  displays an array of unsigneds.
void streetindexgiver(unsigned *stnames, unsigned x1, unsigned y1, unsigned x2, unsigned y2);

unsigned streetindex=0;

int main(void){
    unsigned *stindexmap, i, mapsize = HEIGHT*WIDTH;

    srand(time(NULL));

    if((stindexmap = malloc(HEIGHT*WIDTH*sizeof(unsigned))) == NULL){ puts("\nmain(): malloc failed."); exit(1); }  //  malloc space for neighbourhood map.
    streetindexgiver(stindexmap, 0, 0, HEIGHT-1, WIDTH-1, HOODSIZE);                                                //  build the hood map.
    puts("Here is the complete street name map:");
    ushow(stindexmap, HEIGHT, WIDTH);
    free(stindexmap);
}


void ushow(unsigned * map, unsigned height, unsigned width){
    unsigned i, j;

    for(i=0; i<height; i++){
        for(j=0; j<width; j++){
            if(*(map+i*width+j)<10) printf("  %u", *(map+i*width+j));   //  this is pretty ugly formatting stuff :/
            else printf(" %u", *(map+i*width+j));
        }
        putchar('\n');
    }
}


void streetindexgiver(unsigned *stnames, unsigned x1, unsigned y1, unsigned x2, unsigned y2){
    //  first find any piece of street in the map.  I'll start at 0,0 and scroll.





}
