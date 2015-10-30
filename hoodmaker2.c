/*
    Neighbourhood maker 2.
    Takes an array, splits it into rectangular parts. Gives
    each rectangle a number and fills it with that number.

                                This should be a better version. I was using a global to store hood index
                                before, which I want to avoid. So I'm rewriting the recursive function a
                                little. This time, I'll send the next hood value with the function call,
                                but also return the highest number acquired, so the next call knows where
                                to start. The added benefit of this is that the initial instance of the
                                function will return the total number of hoods, which is nice.
    end:    2015.9.19.14:34     Yep it works.  hoodmaker() no longer relies on any globals.

*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define HEIGHT 55
#define WIDTH 70

void ushow(unsigned * map, unsigned height, unsigned width);    //  displays an unsigned array.
unsigned hoodmaker(unsigned *nhood, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned height, unsigned width, unsigned nsize, unsigned hindex);


int main(void){
    unsigned *nhood, i, mapsize = HEIGHT*WIDTH;
    unsigned count;
    unsigned nsize = 10;                                        //  minimum height and width of an inner rectangle.

    srand(time(NULL));

    if((nhood = malloc(HEIGHT*WIDTH*sizeof(unsigned))) == NULL){ puts("\nmain(): malloc failed."); exit(1); }   //  malloc space for neighbourhood map.
    for(i=0; i<mapsize; i++) *(nhood+i) = 0;                                                                    //  initialise the map with zeroes.
    count = hoodmaker(nhood, 0, 0, HEIGHT-1, WIDTH-1, HEIGHT, WIDTH, nsize, 0);                                 //  build the hood map.

    ushow(nhood, HEIGHT, WIDTH);
    free(nhood);
    printf("\nthere are %u hoods.", count);
    printf("\nmin hood size is %u", nsize);
}


void ushow(unsigned * map, unsigned height, unsigned width){
    unsigned i, j;

    for(i=0; i<height; i++){
        for(j=0; j<width; j++) printf("%3u", *(map+i*width+j));
        putchar('\n');
    }
}


//  on the first call, hindex has to be zero.
unsigned hoodmaker(unsigned *nhood, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned height, unsigned width, unsigned nsize, unsigned hindex){
    unsigned rectheight, rectwidth;     //  dimensions of the current rectangle.
    unsigned p;                         //  random position on whichever side is chosen.
    char vsplitflag=0;                  //  choice of side to split.  if 1, map will be split from left to right.
    unsigned i,j,a,b;
    unsigned hoodindex;

    if(!hindex) hoodindex=1;
    else hoodindex = hindex;

    rectheight = x2-x1+1;
    rectwidth = y2-y1+1;

    if(rectheight<2*nsize && rectwidth<2*nsize) return hindex;              //  if both sides are too small to be split, return.
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

        //  call hoodmaker with each new separate rectangle.
        hoodindex = hoodmaker(nhood, x1, y1, p-1, y2, height, width, nsize, hoodindex+1);
        hoodindex = hoodmaker(nhood, p, y1, x2, y2, height, width, nsize, hoodindex);
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

        //  call hoodmaker with each new separate rectangle.
        hoodindex = hoodmaker(nhood, x1, y1, x2, p-1, height, width, nsize, hoodindex+1);
        hoodindex = hoodmaker(nhood, x1, p, x2, y2, height, width, nsize, hoodindex);
    }

    return hoodindex;
}
