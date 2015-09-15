/*
    Map generator for Blackmarket. Generates Streets and blocks.
    start:  2015.6.23.18:50
    end:    2015.6.23.23:11     (includes about an hour of just playing with it.)

    start:  2015.6.23~15:00     Adding a shopmaker function.
            2015.6.23.20:49     It works!  Took me about 5 hours.  The function identifies all the potential shop
                                locations by matching a thing I called a 'quadrant' to positions on the map.  A
                                quadrant describes the conditions necessary for a shop to be placed.  A quadrant
                                looks like this:

                                  _  S  _
                                 !S  B !S
                                  _  B  _

                                S means street. !S means 'not street'. B means block. I match this shape
                                all over the map, because the bottom B is where a shop can go. The B above it
                                would become a door, and you can't have street tiles on either side of a door. I
                                match this everywhere, then I rotate the quadrant three times for the other
                                matches. Wherever a door can be placed, I write SHOP on a map copy. After the
                                quadrant stuff, I go through the map one cell at a time, visiting each potential
                                shop location. I decide if that location will be a shop or not randomly and, if
                                it will, I count how many potential doors it has, and choose between those
                                randomly too. Then I write my map copy of doors and shops on top of the original
                                map, and it's done!

                                note: hcount and vcount are globals here that are used in streetmaker.
                                They need to be taken out, they're just for debug right now.

            2015.9.10.22:26     adding hoodmaker function.  Doesn't work yet.  Basically using most of streetmaker().
            2015.9.13.02:41     added hoodmaker function.
                                Next I need to add a function that reads my prefixes and suffixes for hood names from a file,
                                builds a name randomly, and then assigns it to a neighbourhood index.  I can use an array of string
                                pointers for this and malloc strings of the right size, though that's ugly.  I'm not sure yet.
            2015.9.15.14:12     I was going to add a function to give streets street names, but I figure it's much easier to pass
                                the street name map to streetmaker, and let it name streets as it makes them.  Let's try that.
                                Yeah that works.  Added a shitty while(1) to walk around the city.  I need to import prefixes and
                                suffixes now for street and neighbourhood names.  Prob use my CodeEval code here somewhere.
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define STREET 0
#define SHOP 1
#define DOOR 2
#define BLOCK 3

#define HEIGHT 30
#define WIDTH  40

#define MAXBLOCKSIZE 10                     //  blocks are the BLOCKS that aren't streets.  This is the maximum length or width a block can be although it doesn't really work that way.
#define SHOPCHANCE 40                       //  chance that a viable shop position actually becomes a shop.
#define HOODSIZE 10                         //  minimum height and width of a neighbourhood.

void streetmaker(unsigned x1,unsigned y1,unsigned x2,unsigned y2, char *map, unsigned blocksize, unsigned widthofmainmap, unsigned *stindexmap);
void show(unsigned r, unsigned c, char *map);
void ushow(unsigned *map, unsigned height, unsigned width);                //  displays an array of unsigneds, for hoodmap.
void shopmaker(unsigned height, unsigned width, char *map, char chance);
void hoodmaker(unsigned *nhood, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned nsize);

unsigned hcount;                            //  used to balance grid splitting in streetmaker function.
unsigned vcount;
unsigned hoodindex=1;                       //  holds the hood number being used as neighbourhoods are being generated.
unsigned streetindex=1;

typedef struct _user {
    unsigned position;       //  index of map where user character is.
}user;

int main(void){
    unsigned i, j, *nhood, *stindexmap;
    char *map;
    user player;
    char input;

    srand (time(NULL));

    if((map = malloc(HEIGHT*WIDTH)) == NULL){ puts("\nmain(): malloc failed."); exit(1); }                          //  malloc space for map.
    if((nhood = malloc(HEIGHT*WIDTH*sizeof(unsigned))) == NULL){ puts("\nmain(): malloc failed."); exit(1); }       //  malloc space for neighbourhood map.
    if((stindexmap = malloc(HEIGHT*WIDTH*sizeof(unsigned))) == NULL){ puts("\nmain(): malloc failed."); exit(1); }  //  malloc space for street name map.

    for(i=0; i<WIDTH*HEIGHT; i++) *(map+i) = BLOCK;                                             //  initialise map to contain only blocks.
    for(i=0; i<WIDTH*HEIGHT; i++) *(stindexmap+i) = 0;                                          //  initialise street index map to contain only zeroes.
                                                                                                //  have to do that since it's filled with garbage.
                                                                                                //  but why don't I do this with nhood?
    for(i=0; i<WIDTH*HEIGHT; i++) *(nhood+i) = 0;

    hcount=vcount=0;                                                                            //  initialising.

    streetmaker(0,0,HEIGHT-1, WIDTH-1, map, MAXBLOCKSIZE, WIDTH, stindexmap);                   //  build streets, and index them.

    shopmaker(HEIGHT, WIDTH, map, SHOPCHANCE);                                                  //  find possible shop locations and place them.
    puts("\n\nafter returning from shopmaker:\n");
    show(HEIGHT, WIDTH, map);

    hoodmaker(nhood, 0, 0, HEIGHT-1, WIDTH-1, HOODSIZE);                                        //  build the hood map.
    puts("Here is the complete nhood map:");
    ushow(nhood, HEIGHT, WIDTH);

    puts("Here is the complete street index map:");
    ushow(stindexmap, HEIGHT, WIDTH);

    //  DO THIS NEXT.
    //  Here is where I get the prefix list for naming things.
    //  open prefix file, count # of lines (x), malloc space for x char pointers, rewind,
    //  measure length of first string, malloc space for that, read it in, go to next string,
    //  repeat until end of file. close file.  Do similar procedure for suffixes.
    //  once streets and neighbourhoods have been given their names, free the arrays.
    //  saves a ton of space :)


//  -----------v-------big ugly game loop, just for testing-------------v-----------------

    //  put user in random place.  actually, just put him on the first piece of street from top left.  do this better later.
    for(i=0; i<HEIGHT*WIDTH; i++) if(*(map+i)==STREET) break;
    player.position = i;

    while(1){

        puts("\ncell\tstreet\thood\toptions");
        printf("%u\t%u\t%u\tx ", player.position, *(stindexmap+player.position), *(nhood+player.position));

        if(player.position/WIDTH){
            if(*(map+player.position-WIDTH)==STREET) printf("n ");
        }

        if(player.position/WIDTH!=HEIGHT-1){
            if(*(map+player.position+WIDTH)==STREET) printf("s ");
        }

        if(player.position%WIDTH){
            if(*(map+player.position-1)==STREET) printf("w ");
        }

        if(player.position%WIDTH!=WIDTH-1){
            if(*(map+player.position+1)==STREET) printf("e");
        }

        if(*(map+player.position)==SHOP) puts("\nYou are in a shop.");
        else if(*(map+player.position)==STREET) puts("\nYou are on the street.");


        printf("\n: ");
        input=getchar();
        fflush(stdin);

        if(input=='x') exit(0);

        if(input=='n'){
            if(*(map+player.position-WIDTH)==DOOR) player.position -= 2*WIDTH;
            else if(player.position/WIDTH==0) puts("\nyou can't go any further north.");
            else if(*(map+player.position-WIDTH)!=STREET) puts("\nthere's a building there.");
            else player.position -= WIDTH;
        }

        if(input=='s'){
            if(*(map+player.position+WIDTH)==DOOR) player.position += 2*WIDTH;
            else if(player.position/WIDTH==HEIGHT-1) puts("\nyou can't go any further south.");
            else if(*(map+player.position+WIDTH)!=STREET) puts("\nthere's a building there.");
            else player.position += WIDTH;
        }

        if(input=='w'){
            if(*(map+player.position-1)==DOOR) player.position -= 2;
            else if(player.position%WIDTH==0) puts("\nyou can't go any further west.");
            else if(*(map+player.position-1)!=STREET) puts("\nthere's a building there.");
            else player.position -= 1;
        }

        if(input=='e'){
            if(*(map+player.position+1)==DOOR) player.position += 2;
            else if(player.position%WIDTH==WIDTH-1) puts("\nyou can't go any further east.");
            else if(*(map+player.position+1)!=STREET) puts("\nthere's a building there.");
            else player.position += 1;
        }
    }

//  ----------^--------big ugly game loop, just for testing------------^------------------

    free(map);
    free(nhood);
    free(stindexmap);
}


//  takes a 2d array and draws a horizontal or a vertical street across it.  This makes two rectangles.
//  The function is called recursively with each rectangle, until no rectangle side is larger than blocksize.
//  x1,y1 are top-left cell of rectangle, x2,y2 are bottom-right cell.  map is pointer to the array. blocksize is maximum size
//  that a rectangle's side can be.  widthofmainmap is same as x2 on the first call.
//  This function also gives streets an index number so I can name it later.
void streetmaker(unsigned x1,unsigned y1,unsigned x2,unsigned y2, char *map, unsigned blocksize, unsigned widthofmainmap, unsigned *stindexmap){
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
        for(i=y1; i<=y2; i++) {
                *(map+(p+x1)*widthofmainmap+i) = STREET;                //  mark that cell as a street.
                *(stindexmap+(p+x1)*widthofmainmap+i) = streetindex;    //  give that cell a street index number on the street index map.
        }
        streetindex++;                                                  //  increment index to give the next street a new name/index.
        streetmaker(x1, y1, p-1+x1, y2, map, blocksize, widthofmainmap, stindexmap);
        streetmaker(p+1+x1, y1, x2, y2, map, blocksize, widthofmainmap, stindexmap);
    }
    else{
        hcount++;
        p = rand()%(hside-2)+1;
        for(i=x1; i<=x2; i++){
                *(map+i*widthofmainmap+y1+p) = STREET;
                *(stindexmap+i*widthofmainmap+y1+p) = streetindex;
        }
        streetindex++;
        streetmaker(x1, y1, x2, p-1+y1, map, blocksize, widthofmainmap, stindexmap);
        streetmaker(x1, p+1+y1, x2, y2, map, blocksize, widthofmainmap, stindexmap);
    }
}

//  display the map.
void show(unsigned r, unsigned c, char *map){
    unsigned i, size=r*c;

    for(i=0; i<size; i++){
            if(!(i%c)) putchar(' ');
            putchar(' ');
        if(*(map+i)==BLOCK) putchar(178);
        else if(*(map+i)==STREET) putchar(' ');
        else if(*(map+i)==SHOP) putchar('$');
        else if(*(map+i)==DOOR) putchar('D');
        if(i%c+1==c) putchar('\n');
    }
}


//  finds potential shop locations and marks them.  Then randomly decides which will become shops.
void shopmaker(unsigned height, unsigned width, char *map, char chance){
    char quadrant[5]={STREET, STREET, BLOCK, STREET, BLOCK};        //  this is a shape I place on the map to see if it matches. It gets rotated too.
    unsigned i, j, myheight, mywidth, mysize, mapsize;
    char *mymap;
    char *potentialshops;           //  1 if a shop could go here, 0 if otherwise.
    char quadflag=0;                //  indicates which quadrant is being used. 0=first, 1=second, etc.
    unsigned start, limit;          //  where quadrant starts and ends.
    char tmp;                       //  used to rotate quadrant.
    char doorcount;                 //  number of doors a potential shop has to choose from.
    unsigned doorindex[4];          //  use this to note down where potential doors are to a shop.

    myheight=height+2;
    mywidth=width+2;
    mapsize = height*width;

    //  I need to make a copy of the grid, because I need an edge around it for the next step.
    if((mymap = malloc(mysize = myheight*mywidth)) == NULL){ puts("\nshopmaker(): malloc failed."); exit(1); }

    //  copy map into centre of mymap.
    for(i=0, j=width+1; i<mapsize; i++, j++){
        if(j%(mywidth)==width+1) j+=2;
        *(mymap+j) = *(map+i);
    }

    //  initialise the edge of mymap with blocks.
    for(i=0; i<mysize; i++) if(!(i%mywidth) || (i%mywidth==mywidth-1) || !(i/mywidth) || (i/mywidth==myheight-1)) *(mymap+i) = BLOCK;

    //  another map copy, 1 if shop could go here, 0 otherwise.
    if((potentialshops = malloc(mysize)) == NULL){ puts("\nshopmaker(): malloc failed."); exit(1); }

    //  initialise shop potentials to STREET. I just needed any value that wasn't SHOP or DOOR.
    for(i=0; i<mysize; i++) *(potentialshops+i) = STREET;

    //  calculate start and end points based on current quadrant.
    for(quadflag=0; quadflag<4; quadflag++){

        if(quadflag==0){                        //  maybe combine pairs of these later.
            start = mywidth;
            limit = mysize-1-3*mywidth-2;
        }
        else if(quadflag==1){
            start = 1;
            limit = mysize-1-2*mywidth-3;
        }
        else if(quadflag==2){
            start = mywidth;
            limit = mysize-1-3*mywidth-2;
        }
        else if(quadflag==3){
            start = 1;
            limit = mysize-1-2*mywidth-3;
        }

        for(i=start; i<=limit; i++){

            //  if quadrant is off the map, skip it forward.  these could combine too.
            if(quadflag==0){        if(i%mywidth==mywidth-2) i+=2; }
            else if(quadflag==1){   if(i%mywidth==mywidth-3) i+=4; }
            else if(quadflag==2){   if(i%mywidth==mywidth-2) i+=2; }
            else if(quadflag==3){   if(i%mywidth==mywidth-3) i+=4; }

            //  if first quadrant, and it matches its current position, save potential shop location indicated by the first quadrant.
            if(quadflag==0){
                if(*(mymap+i+1) == quadrant[0] && *(mymap+i+mywidth)!=quadrant[1] && *(mymap+i+mywidth+1)==quadrant[2] && \
                *(mymap+i+mywidth+2)!=quadrant[3] && *(mymap+i+2*mywidth+1)==quadrant[4]){
                    *(potentialshops+i+2*mywidth+1) = SHOP;     //  mark potential shop location.
                    *(potentialshops+i+mywidth+1) = DOOR;       //  mark potential door location.
                }
            }

            //  if 2nd quadrant, and it matches its current location, save potential shop location indicated by the 2nd quadrant.
            else if(quadflag==1){
                if(*(mymap+i+1) != quadrant[0] && *(mymap+i+mywidth)==quadrant[1] && *(mymap+i+mywidth+1)==quadrant[2] && \
                *(mymap+i+mywidth+2)==quadrant[3] && *(mymap+i+2*mywidth+1)!=quadrant[4]) {
                    *(potentialshops+i+mywidth) = SHOP;     //  mark potential shop location.
                    *(potentialshops+i+mywidth+1) = DOOR;   //  mark potential door location.
                }
            }

            //  repeat for 3rd quadrant.
            else if(quadflag==2){
                if(*(mymap+i+1) == quadrant[0] && *(mymap+i+mywidth)!=quadrant[1] && *(mymap+i+mywidth+1)==quadrant[2] && \
                *(mymap+i+mywidth+2)!=quadrant[3] && *(mymap+i+2*mywidth+1)==quadrant[4]) {
                    *(potentialshops+i+1) = SHOP;
                    *(potentialshops+i+mywidth+1) = DOOR;
                }
            }

            //  repeat for 4th quadrant.
            else if(quadflag==3){
                if(*(mymap+i+1) != quadrant[0] && *(mymap+i+mywidth)==quadrant[1] && *(mymap+i+mywidth+1)==quadrant[2] && \
                *(mymap+i+mywidth+2)==quadrant[3] && *(mymap+i+2*mywidth+1)!=quadrant[4]) {
                    *(potentialshops+i+mywidth+2) = SHOP;
                    *(potentialshops+i+mywidth+1) = DOOR;
                }
            }
        }   //  end of from start to end loop.

        //  rotate the quadrant.
        tmp = quadrant[0];
        quadrant[0] = quadrant[1];
        quadrant[1] = quadrant[4];
        quadrant[4] = quadrant[3];
        quadrant[3] = tmp;

     }//end of for(quadflag...)

    //  at this point, potentialshops should contain SHOP where shops can go. so print it and see.
    //  show(myheight, mywidth, potentialshops);

    //  now, go back through the map, to visit each potential location. randomly decide if it will be a shop or not.
    //  if so, choose where door should be if more than 1 choice.  place shop, and door. move to next potential shop location.
    for(i=mywidth+1, doorcount=0; i<mysize-1-mywidth; i++){

        if(*(potentialshops+i)!=SHOP) continue;     //  if not a potential shop, continue.
        if(i%mywidth==mywidth-1) i+=2;              //  if on outer edge, skip forward two place back onto map.

        if((rand()%100+1)>chance){                                                          //  shop has chance percent of being built.
            *(potentialshops+i) = BLOCK;                                                    //  Otherwise it's turned into a block.
            if(*(potentialshops+i-mywidth)==DOOR)   *(potentialshops+i-mywidth) = BLOCK;    //  And so are all its potential doors.
            if(*(potentialshops+i+mywidth)==DOOR)   *(potentialshops+i+mywidth) = BLOCK;
            if(*(potentialshops+i-1)==DOOR)         *(potentialshops+i-1) = BLOCK;
            if(*(potentialshops+i+1)==DOOR)         *(potentialshops+i+1) = BLOCK;
        }

        //  count and note indexes of surrounding doors.
        if(*(potentialshops+i-mywidth) == DOOR){ doorindex[doorcount]=i-mywidth; doorcount++; }
        if(*(potentialshops+i+mywidth) == DOOR){ doorindex[doorcount]=i+mywidth; doorcount++; }
        if(*(potentialshops+i+1) == DOOR){ doorindex[doorcount]=i+1; doorcount++; }
        if(*(potentialshops+i-1) == DOOR){ doorindex[doorcount]=i-1; doorcount++; }

        //  if only one door, leave it there.  reset doorcount and skip further tests, go to next shop.
        if(doorcount==1) { doorcount=0; continue; }

        //  if two doors, choose which one gets deleted.
        else if(doorcount==2){ if(rand()%2)  *(potentialshops+doorindex[0]) = BLOCK; else *(potentialshops+doorindex[1]) = BLOCK;}

        //  else if 3 doors available, choose which 2 get destroyed.
        else if(doorcount==3){
            j = rand()%3;   //  choose one of the three doors.
            if(j==0)        *(potentialshops+doorindex[1]) = *(potentialshops+doorindex[2]) = BLOCK;    //  if door 0 chosen, destroy doors 1 and 2.
            else if(j==1)   *(potentialshops+doorindex[0]) = *(potentialshops+doorindex[2]) = BLOCK;    //  if door 1, destroy doors 0 and 2.
            else if(j==2)   *(potentialshops+doorindex[0]) = *(potentialshops+doorindex[1]) = BLOCK;    //  and so on.
        }

        //  else if 4 doors available, choose which 3 get expunged.
        else if(doorcount==4){
            j = rand()%4;   //  choose one of the three doors.
            if(j==0)        *(potentialshops+doorindex[1]) = *(potentialshops+doorindex[2]) = *(potentialshops+doorindex[3]) = BLOCK;    //  if door 0, destroy doors 1, 2, and 3.
            else if(j==1)   *(potentialshops+doorindex[0]) = *(potentialshops+doorindex[2]) = *(potentialshops+doorindex[3]) = BLOCK;    //  if door 1, destroy doors 0, 2, and 3.
            else if(j==2)   *(potentialshops+doorindex[0]) = *(potentialshops+doorindex[1]) = *(potentialshops+doorindex[3]) = BLOCK;    //  if door 2, destroy doors 0, 1, and 3.
            else if(j==3)   *(potentialshops+doorindex[0]) = *(potentialshops+doorindex[1]) = *(potentialshops+doorindex[2]) = BLOCK;    //  if door 3, destroy doors 0, 1, and 2.
        }

        //  reset doorcount for next shop.
        doorcount=0;

    }   //  now all doors have been chosen.

    //  write doors and shops to main map.
    for(i=0, j=width+1; i<mapsize; i++, j++){
        if(j%(mywidth)==width+1) j+=2;
            if(*(potentialshops+j)!=STREET) *(map+i) = *(potentialshops+j);
    }

    free(mymap);
    free(potentialshops);
}

void hoodmaker(unsigned *nhood, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned nsize){
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
                *(nhood+i*WIDTH+j) = hoodindex;

        hoodindex++;

        //  call hoodmaker with each new separate rectangle.
        hoodmaker(nhood, x1, y1, p-1, y2, HOODSIZE);
        hoodmaker(nhood, p, y1, x2, y2, HOODSIZE);
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
                *(nhood+i*WIDTH+j) = hoodindex;

        hoodindex++;

        //  call hoodmaker with each new separate rectangle.
        hoodmaker(nhood, x1, y1, x2, p-1, HOODSIZE);
        hoodmaker(nhood, x1, p, x2, y2, HOODSIZE);
    }
}


void ushow(unsigned * map, unsigned height, unsigned width){
    unsigned i, j;

    for(i=0; i<height; i++){
        putchar(' ');
        for(j=0; j<width; j++){
            if(*(map+i*width+j)<10) printf(" %u", *(map+i*width+j));   //  this is pretty ugly formatting stuff :/
            else printf("%u", *(map+i*width+j));
        }

        putchar('\n');
    }
}


