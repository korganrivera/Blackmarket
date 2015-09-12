/*
    Neighbourhood maker.
    Takes an array, splits it into rectangular neighbourhoods, similar to the
    street-making algorithm. Returns the array filled with numbers indicating
    separate neighbourhoods.  Each number should be used to assign names to
    neighbourhoods.

    start:  2015.9.10.20:33
    end:    2015.9.11.15:08
*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define HEIGHT 60
#define WIDTH 60
#define HOODSIZE 5      //  minimum height and width of a neighbourhood.

void ushow(unsigned * map, unsigned height, unsigned width);    //  displays an array of unsigneds.
void hoodmaker(unsigned *nhood, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned height, unsigned width, unsigned nsize);

unsigned hoodindex=1;   //  holds the hood number being used as neighbourhoods are being generated.

int main(void){
    unsigned *nhood, i, mapsize = HEIGHT*WIDTH;

    srand(time(NULL));

    if((nhood = malloc(HEIGHT*WIDTH*sizeof(unsigned))) == NULL){ puts("\nmain(): malloc failed."); exit(1); }   //  malloc space for neighbourhood map.
    for(i=0; i<mapsize; i++) *(nhood+i) = 0;                                                                    //  initialise the map with zeroes.
    hoodmaker(nhood, 0, 0, HEIGHT-1, WIDTH-1, HEIGHT, WIDTH, HOODSIZE);                                         //  build the hood map.
    puts("Here is the complete nhood map:");
    ushow(nhood, HEIGHT, WIDTH);
    free(nhood);
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


void hoodmaker(unsigned *nhood, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned height, unsigned width, unsigned nsize){
    unsigned rectheight, rectwidth;     //  dimensions of the current rectangle.
    unsigned p;                         //  random position on whichever side.
    char vsplitflag=0;                  //  choice of side to split.  if 1, map will be split from left to right.
    unsigned i,j,a,b;

    rectheight = x2-x1+1;
    rectwidth = y2-y1+1;

    if(rectheight<2*nsize && rectwidth<2*nsize) return;                     //  if both sides are too small to be split, return.
    if(rectheight>=2*nsize && rectwidth>=2*nsize) vsplitflag = rand()%2;    //  if both sides are big enough to split, choose between them randomly.
    else if(rectheight>=2*nsize) vsplitflag = 1;                            //  if only rectheight is big enough to be split, choose that.

    if(vsplitflag){

        //  first I need to find the bounds.
        a = x1+nsize;
        b = x2-nsize+1;

        //  choose a random number between a and b.
        p = rand()%(b-a+1)+a;

        //  fill the chosen rectangle with the current hoodindex.
        for(i=x1; i<p; i++)
            for(j=y1; j<=y2; j++)
                *(nhood+i*width+j) = hoodindex;

        hoodindex++;

        //  call hoodmaker with each new separate rectangle.
        hoodmaker(nhood, x1, y1, p-1, y2, HEIGHT, WIDTH, HOODSIZE);
        hoodmaker(nhood, p, y1, x2, y2, HEIGHT, WIDTH, HOODSIZE);
    }

    else{

        //  calculate the bounds.
        a = y1+nsize;
        b = y2-nsize+1;

        //  choose a random number between a and b.
        p = rand()%(b-a+1)+a;

        //  fill the chosen rectangle with the current hoodindex.
        for(i=x1; i<=x2; i++)
            for(j=y1; j<p; j++)
                *(nhood+i*width+j) = hoodindex;

        hoodindex++;

        //  call hoodmaker with each new separate rectangle.
        hoodmaker(nhood, x1, y1, x2, p-1, HEIGHT, WIDTH, HOODSIZE);
        hoodmaker(nhood, x1, p, x2, y2, HEIGHT, WIDTH, HOODSIZE);
    }
}
