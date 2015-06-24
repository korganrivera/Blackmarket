/*
    Map generator for Blackmarket. Generates Streets and blocks.
    start:  2015.6.23.18:50
    end:    2015.6.23.23:14 (includes about an hour of just playing with it.)
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define STREET 0
#define SHOP 1
#define DOOR 2
#define BLOCK 3

#define HEIGHT 60
#define WIDTH  80

void streetmaker(unsigned x1,unsigned y1,unsigned x2,unsigned y2, char *map, unsigned blocksize, unsigned widthofmainmap);
void show(unsigned r, unsigned c, char *map);

unsigned hcount;
unsigned vcount;


int main(void){
    unsigned i, j;
    char *map;

    srand (time(NULL));

    if((map = malloc(HEIGHT*WIDTH)) == NULL){ puts("\nmain(): malloc failed."); exit(1); }      //  malloc space for map.
    for(i=0; i<WIDTH*HEIGHT; i++) *(map+i) = BLOCK;                                             //  initialise map with all blocks.
    hcount=vcount=0;                                                                            //  initialise horizontal and vertical cut counters.
    streetmaker(0,0,HEIGHT-1, WIDTH-1, map, 4, WIDTH);                                          //  build streets.
    printf("\nSTREETMAKER %u x %u map\n\n", HEIGHT, WIDTH);                                     //  display  city.
    show(HEIGHT, WIDTH, map);
    printf("\n\nhcount=%u, vcount=%u", hcount, vcount);                                         //  display number of horizontal and vertical cuts, just to check.
}


//  takes a 2d array and draws a horizontal or a vertical street across it.  This makes two rectangles.
//  The function is called recursively with each rectangle, until no rectangle side is larger than blocksize.
//  x1,y1 are top-left cell of rectangle, x2,y2 are bottom-right cell.  map is pointer to the array. blocksize is maximum size
//  that a rectangle's side can be.  widthofmainmap is same as x2 on the first call.
void streetmaker(unsigned x1,unsigned y1,unsigned x2,unsigned y2, char *map, unsigned blocksize, unsigned widthofmainmap){
    unsigned vside, hside, p, i;
    char vsideflag=0;

    //  calculate length of sides.
    vside = x2-x1+1;
    hside = y2-y1+1;

    //  if both sides are small enough, we're done.
    if(vside<=blocksize && hside<=blocksize) return;

    //Otherwise, if both sides are larger enough to be split, choose randomly between
    //them, but do it proportionally to the ratio of horizontal to vertical cuts made
    //so far. This way, if vertical cuts are dominating, then horizontal ones will
    //become more likely, and vice versa. This should help to stop wacky ratios from
    //making cities have too much of one and not the other.
    else if(vside>blocksize && hside>blocksize) vsideflag=(hcount<(rand()%(hcount+vcount+2)));

    //  else if the vertical side is the only one that can be cut, do it.
    else if(vside>blocksize) vsideflag=1;

    if(vsideflag){
        vcount++;
        p = rand()%(vside-2)+1;
        for(i=y1; i<=y2; i++) *(map+(p+x1)*widthofmainmap+i) = STREET;
        streetmaker(x1, y1, p-1+x1, y2, map, blocksize, widthofmainmap);
        streetmaker(p+1+x1, y1, x2, y2, map, blocksize, widthofmainmap);
    }
    else{
        hcount++;
        p = rand()%(hside-2)+1;
        for(i=x1; i<=x2; i++) *(map+i*widthofmainmap+y1+p) = STREET;
        streetmaker(x1, y1, x2, p-1+y1, map, blocksize, widthofmainmap);
        streetmaker(x1, p+1+y1, x2, y2, map, blocksize, widthofmainmap);
    }
}

//  display the map.
void show(unsigned r, unsigned c, char *map){
    unsigned i, size=r*c;

    for(i=0; i<size; i++){
        if(*(map+i)==BLOCK) putchar(178);
        else if(*(map+i)==STREET) putchar(' ');
        else if(*(map+i)==SHOP) putchar('$');
        else if(*(map+i)==DOOR) putchar('D');
        if(i%c+1==c) putchar('\n');
    }
}
