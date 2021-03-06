/*
   Map generator for Blackmarket. Generates Streets and blocks.
   start:  2015.6.23.18:50
   end:    2015.6.23.23:11     (includes about an hour of just playing with it.)

    start:  2015.6.23~15:00     Adding a shopmaker function.
            2015.6.23.20:49     It works!  Took me about 5 hours.  The function identifies all the potential shop
                                locations by matching a thing I called a 'quadrant' to positions on the map.  A
                                quadrant describes the conditions necessary for a shop to be placed.  A quadrant
                                looks like this :

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

           2015.9.10.22:26      adding hoodmaker function.  Doesn't work yet.  Basically using most of streetmaker().

           2015.9.13.02:41      added hoodmaker function.
                                Next I need to add a function that reads my prefixes and suffixes for
                                hood names from a file, builds a name randomly, and then assigns it to
                                a neighbourhood index.  I can use an array of string pointers for this
                                and malloc strings of the right size, though that's ugly.  I'm not sure
                                yet.

            2015.9.15.14:12     I was going to add a function to give streets street names, but I figure it's
                                much easier to pass the street name map to streetmaker, and let it name streets
                                as it makes them.  Let's try that. Yeah that works.  Added a shitty while(1) to
                                walk around the city.  I need to import prefixes and suffixes now for street and
                                neighbourhood names.  Prob use my CodeEval code here somewhere.

            2015.9.16.13:32     Aw yeah! I can now read the prefix suffix files, with my badass filereader
                                function! I used triple pointers. I malloced everything so I only use the bare
                                minimum of memory to hold an array of strings. :D Now I have to do the same
                                thing for only the strings I need, then free the other stuff.

            2015.9.16.15:32     Street names are a litle trickier.  I can't just stick 'street' in front of a
                                hood-style name, because it gets monotonous.  I'll use either 1) prefix + street,
                                2) suffix + street, 3) prefix + suffix + street. So I need another array of street
                                suffixes.

            2015.9.17.16:02     Okay now streets have names!  The game can tell me what street I'm on and
                                what hood I'm in. Now I need to add street numbers.

            2015.9.17.16:55     Made street names and hood names proper, by capitalising the first letter accordingly.
                                Still need street numbers.

            2015.9.19.15:05     Something is causing occasional crashes.  No idea what.  Fix that before moving on.
                                define a DEBUG value, put in a bunch of if(DEBUG) puts() throughout the code to at least get an idea
                                where the bug is.
                                THE BUG IS IN STREETMAKER!!!  CHECK THAT!

            2015.9.19.23:21     Once I've fixed the bug, put the new hoodmaker function back in, from mapmaker-testingnewstuff.c.

            2015.9.19.23:21     Bug might still be in there, but it's not showing its face after about
                                20 runs, but if it's still there, it has something to do with filereader maybe,
                                or streetsuffs. I was getting malloc fails. No idea. I'm going to go ahead and
                                put the new hoodmaker function in.

            2015.9.20.02:25     I can't figure out this motherfucking bug, it keeps sliding away when I think
                                I have it. I'm going to have to comb through everything from scratch until I
                                find it. It's obviously some pointer fuck up but I don't know where.

            2015.9.20.02:43     Seems to have gone when I updated streetmaker. The bug was in streetmaker
                                but I corrected it... ...as far as I know. Fingers crossed. Everything's
                                probably fine now. I need to mess with how the blocks are distributed. I
                                changed the formula and now the cities look shitty.

            2015.9.20.03:20     shopmaker is fucked up and doesn't work.  look over that
                                again. actually it's fine.  Game needs a navigation map
                                and more types of tile: grass, trees, water, etc.

            2015.9.20.16:43     about to try making a navigation map, then I'll move it to main mapmaker.c file.

            2015.9.20.17:28     Ha ha.  I added it, it looks hideous but I can walk around a city quickly at least.  lol I suck.

            2015.9.21.01:45     I need to give shops and doors numbers on their respective
                                streets.  Also need to figure out a system for shops because
                                I have no idea right now.  Also the whole trading and rumor
                                system hasn't even been sketched out yet O_O.

            2015.9.29.21:05     Hello.  I think I'm going to try squashing all the maps together into an array of structs.
                                Nur weil habe ich keine Lust fuer mehr Kodierung und dies ist einfach.

            2015.9.29.22:23     Bug near line 356, when hoodmaker returns after making no neighbourhoods,
                                it returns 0. really, it has created 1 neighbourhood of the whole map. Not
                                sure if I should just add 1 to the count if zero, or add one always. need
                                to look into it, also this problem is causing my mapsquasher to die.

                                Nevermind, I fixed the mapsquash thing, but yeah the hood name number problem still remains.
            2015.10.2.01:25     It has been fixed, and I've cleaned up the debug stuff so it's prettier.  :)

            2015.10.4.20:52     Going to throw weatherreport in here, just because why not.
            2015.10.4.21:26     I put it in there, but I plan to take it out.  who gives a fuck about the weather.

            2015.10.6.17:54     Something I want to add at some point.  I want to put all text output into a buffer
                                and then later forward it to blocktexter, so the output looks good.  Maybe do it right now
                                for the street stuff.

            2015.10.11.13:56    Going to add a basic local description function: a look command basically.
                                I guess I'll try to add a sound and smell function at some point too.
                                Dude, I don't even care anymore.

            2015.10.31.23:05    Been a while. I'm here because of Vyvanse. Since last time, I've written an
                                algorithm that lets a shop buy the most profitable items from the supplier. I've
                                also come up with a stack of structs that will become a database of shops,
                                including inventory, and sales records. So now I need to build that. I really
                                need to learn how to break up my source code into multiple files because it's
                                about to become ridiculous.

            2015.11.01.02:27    3 hours later! The game now has shops with inventories, and sales histories.
                                There is a main supplier. All the shops can do their initial purchases there
                                whereby they use my SupplierPurchasing algorithm and buy the most profitable
                                stuff that they can afford and store. Things went without a problem at all so
                                that's fucking awesome. Can't believe it's all in there. About 1300 lines of
                                code so far. :)

            2015.11.2.22:54     A thought: if the supplier's list doesn't change, or more accurately,
                                if items always have the same item index, and items never get deleted, then shop
                                inventory lists could just be item indexes with quantity. I wouldn't need to
                                actually make copies of item info from the supplier. If I shop wanted to access
                                info on an item, it could just use its index number to access the supplier's db
                                and find out name, size, etc. it would only need to know how many it had of that
                                item, and the sales history of it, and its shop price, and its own mnb values.

            2015.11.3.01:50     I did what I said above, because there's no
                                point in having copies like that. Shops now actually have inventory linked
                                lists. When you enter a shop, that shop's details are displayed, and its
                                inventory. It's pretty awesome. Next step is to have the shops be able to sell
                                to the NPC masses, and then do their price calculations. This is where the
                                biggest slow down will happen.

            2015.11.4.13:59     I noticed that the clock moves forward even if you walk into a wall.
                                That shouldn't happen. Not important right now though. Next step is to have the
                                NPC masses buy shit from the shops. Iterate through all the stores, using the
                                stores price, figure out how many sales the masses would take, remove that
                                number from the inventory, and repeat for every item. I need to decide at what
                                rate the customers do this. I mean, I could do this every complete game day, or
                                every game week, etc. How often do customers do this, and shops restock, and so
                                on? Uh.

                                let's try updating the prices of all items after every turn.

            2015.11.6.23:16     To do that I'm going to have to set up a whole bunch
                                of new stuff. First I need a 2d array of 'dumpsters'
                                that tell me the rot rate and size of dumpsters that
                                contain every item for each hood. This is where items
                                go after they're sold to the NPC masses.

            2015.11.7.01:23     Ok. I have hood dumpsters now for every item. Their size is based on the
                                population of each hood. Their rot rate is random for now. This is the problem:
                                If I do the first shop, I calculate how many of an item it would sell based on
                                how full that item's dumpster is currently. For the second shop, the first shop
                                has changed the fullness of it so the second shop would have to sell the same
                                item for a different rate. However, this is all supposed to be happening
                                simultaneously. So to avoid that, it's like saving maps in Game Of Life, where I
                                don't make changes until everyone has moved. So...that means I need ANOTHER
                                fucking table to store these changes. The problem I have to avoid in doing this
                                is that the first shop might fill the dumpster, and the second shop wouldn't
                                know about it.  If I do it the first way, then shops that come first in the map
                                have an advantage.  One solution would be to work through the shops in a random
                                order but that means I'd have to deal with sorting lists of shops and fuck that.
                                The other issue I have is that this code has become unwieldly.

            2015.11.7.03:18     I think the best way to handle this is to just iterate through the shops and not
                                worry about shops having some advantage based on the order of iteration. Later,
                                I can change the order or something by starting at a different position in the
                                array or something.

                                I also need to set item desire stuff for hoods now since I'm switching to that now.
                                going to bed.

            2015.11.7.23:17     So *ahem*.  Here is what happens now:
                                The map is set up, all the shops stock themselves. you make a move
                                on the map, to enter a shop. then the shops sell their items to the masses. All
                                of them sell out. then you're shown the shop's inventory which has now emptied
                                itself as you walked in the door. :)

                                Solution: shops need to buy enough stock for more than one move.
                                shops need to restock at some threshhold point or something.
                                also, still need to add dumpster rot, but do the shop thing first:
                                if shops can only sell once, I'll never see the benefit of dumpster rot.
                                So turn the fist time stocking into a restock function, and call it whenever.
                                Also, I'm about to put in a bunch of stuff to watch shop[1] all the time to
                                see its behaviour.

            2015.11.8.03:11     Now I can see shopzero stuff.  it somehow needs to spread its sales throughout the day at least.

            2015.11.9.03:12     Almost exactly 24 hours since I last wrote :)
                                I'm thinking that NPCs buying from the shop might have to be
                                a probabilistic activity. The problem is this: Say an item is supposed to sell 6
                                items over the course of a week. Say I handle this daily, and so each day, the
                                shop sells 0.857 of this item. That's going to be rounded to zero. So the shop
                                will sell zero over the course of a week. Obviously, that's fucking stupid.
                                Instead, I could say that over the course of a week, the shop is likely to sell
                                6 on average. So that could happen in a bunch of different ways: all 6 on one
                                day, or 1 each day except one, etc. However, this means I'd have to rewrite some
                                stuff. It also doesn't guarantee that the item would sell this many times.  As
                                items are sold, I just change the probability as it goes.  Seems like a shitty
                                way to do it though.  I have to rethink a whole bunch of shit.

            2015.11.9.15:22     What is is about 12 hour cycles?
                                One thing that might be a problem that I've noticed: shops
                                buy items based on their profit density. However, there might be a favourable
                                profit density for selling less than 1 item, since it's a double calculation.
                                But that means I'd never sell that item. It has to be at least one sale, since
                                it becomes an unsigned value. Need to address that at some point.

            2015.11.9.16:11     I solved it sort of.  I checked to see if the shop would need to sell less than 1
                                item at the calculated price.  If so, it abandons buying it.  I'd rather it could
                                go back and pick the next most profitable item which might have been legit.
                                This program is a fuck up.

            2015.11.10.01:44    Suppliers fill with shit, shops stock themselves, then
                                sell to NPC masses, supplier restocks itself, dumpsters fill when sold to,
                                dumpsters rot themselves. inventory seems to be flowing around pretty well. The
                                problem is it takes too long, since it's done on every move, so every shop is
                                restocking and selling to the masses on every turn. so you can't really walk
                                around because of the intermittent delay. Somehow need to schedule this
                                happening.  EDIT: I took out some debug statements and it runs a little quicker.

            2015.11.10.01:44    If one move is 4 seconds, then 21600 moves is a real-time day. Obviously that's
                                too much.  Also, make 2 previous weather report records, so that way I can leave
                                the weather on all the time, and check that ti doesn't just keep repeating the
                                same two weathers all the time.
*/

/***************************************************************************************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <termios.h>

/*---------------------LINUX SHIT-------------------------------------*/
static struct termios old, noo;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  noo = old; /* make new settings same as old settings */
  noo.c_lflag &= ~ICANON; /* disable buffered i/o */
  noo.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &noo); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void)
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void)
{
  return getch_(1);
}

/*--------------------------------------------------------------------*/



#ifndef M_PI
    #define M_PI 3.14159265358979323
#endif
#define LEN         33                      //  must be 2^n + 1. This is the width of the graphs I generate for weather.
                                            //  This is currently being used for both length of graphs for item noise
                                            //  and for weather noise. That might be a problem later.

#define HEATMIN     30.0                    //  52.0.   These values will be calculated using yearly models later.
#define HEATMAX     75.0                    //  90.0.
#define TOTAL_ITEMS 500                     //  number of items the supplier holds.  replace this later when I read from a file.

//  some convenient aliases.
#define STREET      0
#define SHOP        1
#define DOOR        2
#define BLOCK       3

//  height and width of the entire map.
#define HEIGHT      30                      //  H50 x W80 is a good working size.
#define WIDTH       30

/*  let's say that each cell block is 8 metres square.  If a map has 50x80 cells, then it has an area of 400x640 metres = 0.256 km^2.
    if the population density of the city were 16,000/km^2, then the population would be 4096 people.  If the population were that of
    Aberdeen, 1169/km^2, then the population would be 299.  Let's set the density to 10,000/km^2.
    This'll be used to determine population of each hood, when I know the size of each hood.
    Needs to be denser than this.  A map of 50x80 has about 52 hoods, giving each shop a dumpster size of about 5.
    It needs to be more like 500.

    */
#define MAP_POPULATION  ((WIDTH*HEIGHT) * 7)

#define MINBLOCKSIZE    3                               //  A block is a group of BLOCK type cells.  This is the min. size they can be.
#define SHOPCHANCE      50                              //  Chance that a viable shop position actually becomes a shop.
#define HOODSIZE        5                               //  minimum height and width of a neighbourhood.

#define NPC_POP (HEIGHT*WIDTH)                          //  NPC population in game. using area of map as rough guide for now. approximates Aberdeen population.

#define DEBUG           0                               //  toggle for debug info. 1=on, 0=off.
#define SHOWLIST        1                               //  debug thing, shows supplierpurchasing working list.

#define CLOCK_ADVANCE   4                               //  number of seconds the clock advances every move. 4 seconds ~= 4mi/hour walking.
#define WEATHER_REPORT_INTERVAL (CLOCK_ADVANCE * 3)     //  how often the weather is reported.  (CLOCK_ADVANCE * 10) means after every 10 steps.
                                                        //  remember, if weather hasn't changed since last report, it isn't repeated. so
                                                        //  weather report every 10 steps is a maximum.
/***************************************************************************************************************/

//  This is used to record what the last weather report was, to avoid reporting the same thing each time.
typedef struct _weather {
        unsigned humidity;
        unsigned heat;
        unsigned cloud;
        unsigned temp;
        unsigned firsttime;
        unsigned tod;
        unsigned wind;
        unsigned sun;
}weather;

//  users character info.
typedef struct _user {
    unsigned position;
}user;

//  basic clock.
typedef struct _clocky {
    unsigned h;
    unsigned m;
    unsigned s;
}clocky;

//  A 'map unit'. an array of these makes a map.
//  This is only used for mapsquash.
typedef struct _cell {
    char type;          //  whether the cell is street, shop, door, or block.
    char *hood;         //  name of neighbourhood.
    char *street;       //  name of street.  if not a street block, this would be null.
    char *shopname;     //  if a shop, the name of the shop.
    unsigned index;     //  position on the map.
    struct _cell *u;    //  pointers  to adjacent cells.
    struct _cell *d;
    struct _cell *l;
    struct _cell *r;
}cell;

/*  a record of how many item sales a shop got at a given price
    at a given time. an array of these makes a sales history of
    a particular item. The shop will then use this to calculate
    a better price for the item. */
typedef struct sales_record {
    double price;
    unsigned sales;
    unsigned time;                          //  not sure what type yet.
    struct sales_record *next;              //  points to next node in list.
}sales_record;

/*  This is the struct that holds info about an item
    that a shop has. A linked list of these makes an
    inventory. If a shop requires more details about
    the item, it can lookup the supplier's db using
    the item's id number.   */
typedef struct item_record {
    unsigned id;                            //  number that identifies this item.
    unsigned quantity;
    unsigned size;
    double price;                           //  shop's current selling price for this item.
    double m,n,b;                           //  the shop's estimate of the demand function for this item.  sales = m(price) + n(time) + b.
    sales_record *sales_history;            //  This will be the head of a linked list probably. also the root of this linked list.
    sales_record *sales_history_end;        //  points to last node in the list.
    struct item_record *next;               //  points to next node in list.
}item_record;

/*  This is everything that makes a shop.
    An array of these makes all shops.  */
typedef struct shop {
    char *name;                             //  name of this shop.
    unsigned loc;                           //  position on the map.
    char *desc;                             //  shop's description.
    char open;                              //  whether the shop is open or not.
    int used_space;
    unsigned space;                         //  max warehouse space.
    double money;                           //  total money the shop has.
    item_record *inventory;                 //  the shop's inventory. the root of the linked list.
    item_record *inventory_end;             //  points to the last node in the list.

                                            //  also somehow have opening/closing hours for the store
                                            //  here. add that when I know what format time takes.
}shop;

//  A supplier's item.  An array of these
//  makes the inventory of the supplier.
//  I might just reuse item_record and scrap this.
typedef struct supplier_item {
    char *name;
    unsigned id;                            //  product ID.
    unsigned size;
    unsigned quantity;                      //  how many the supplier has.
    unsigned have;                          //  quantity shop already owns. Each shop uses this as scratch space.
    unsigned bought;                        //  number sold to the shop buying.  Shop uses this as scratch space to do its calculation.
    double cost;                            //  cost to purchase this item.
    double spent;                           //  how much the items cost in the shopping cart.
    double m,n,b;                           //  the actual demand function for this item. Only the supplier knows this.
    double *noise;                          //  each item has an array filled with noise to add to its demand line.  You can loop it.
    double rrp;                             //  price recommended to shops to sell at initially.
}supplier_item;

/*  where items go when they're sold to the NPC masses. each neighbourhood has
    one for each item. demand for an item is based on how full the dumpster is.
    The rot rate dictates how fast the items in the dumpster reduce.    */
typedef struct dumpster {
    unsigned size;          //  size of dumpster. max capacity of it.
    unsigned level;         //  current fullness of it.
    unsigned rot_amount;    //  amount by which content decays each turn.
}dumpster;

/***************************************************************************************************************/

unsigned streetmaker(unsigned x1,unsigned y1,unsigned x2,unsigned y2, char *map, unsigned blocksize, unsigned widthofmainmap, unsigned *stindexmap, unsigned sindex);
void show(unsigned r, unsigned c, char *map);                                   //  displays an array of chars, for *map (the main map.)
void ushow(unsigned *map, unsigned height, unsigned width);                     //  displays an array of unsigneds, for *hoodmap.
unsigned shopmaker(unsigned height, unsigned width, char *map, char chance);    //  places shops.
unsigned hoodmaker(unsigned *nhood, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned height, unsigned width, unsigned nsize, unsigned hindex);
unsigned filereader(char *fname, char ***array);                                //  reads text files with name parts for streets and hoods.
void proper(char *str);                                                         //  capitalises the first letter of given string.
void unproper(char *str);                                                       //  lowers the case of the first letter of given string.
void display(unsigned r, unsigned c, char *map, unsigned pos);                  //  shows map with user on it.
unsigned weatherreport(weather *last, double heat, double humidity, double wind, double cloud, clocky *c);
void noise(double *array, unsigned start, unsigned end, unsigned level, double min, double max);    //  fills an array with noise, using mid-placement algorithm.
void sinefiller(double *array, unsigned size, double min, double max);
void insideshop(shop *shopx);                                                   //  the dialog for the given shop.
void look(cell *map, unsigned loc);                                             //  describes stuff seen around you.
int stock_these(item_record **root, item_record **curr, item_record *n);        //  puts a set of items n into the shop's inventory. returns 1 if successful.
//void show_linkedlist(shop *shopx);                                              //  shows me what's in a shop's inventory.

/***************************************************************************************************************/

unsigned hcount;                            //  hcount and vcount are used to balance grid splitting in streetmaker function.
unsigned vcount;                            //  meh, might take these out.

/***************************************************************************************************************/



int main(void){
    unsigned len, a, b, c, i, j, r, *nhood, *stindexmap;
    unsigned hoodindex, streetindex, totalshops;
    char *map, input;
    user player;
    cell mapsquash[HEIGHT][WIDTH];
    clocky myclock;

    //  weather stuff.
    double heat[LEN];
    double humidity[LEN];
    double wind[LEN];
    double cloud[LEN];
    weather lastcheck;

    //  variables for reading names from files.
    unsigned totalprefixes, totalsuffixes, totalstreetsuffs;
    char **prefixes, **suffixes, **streetsuffs;     //  array of strings; prefixes, suffixes, and street types.
    char **hoodnames, **streetnames;                //  complete street and hood names.

    //  struct arrays for databases.
    shop *shopdb;                                   //  Shop database.
    supplier_item *supplies;                        //  Supplier's database.

    //  variables for supplier purchasing algorithm.
    unsigned best_item;
    unsigned possible_flag;                         //  indicates if I traversed the list and found no options. 0 if no options.
    unsigned found;                                 //  flags if I already have a record of this item in the shop inventory, even if quantity is zero.
    double profit_density;                          //  profit/space of item.
    double best_profit_density;
    double profit_made;
    double spent;

    //  for stocking up.
    item_record stock_this;                         //  when a shop buys a set of items from the supplier, I put that info into this struct.
    double required_price;                          //  I then add this struct to the shop's inventory list.  It's temp space for the copy.

    //  dumpster stuff.
    dumpster **dump;                                //  this'll be a 2d array, when I know the number of hoods and total items. dump[total items][# of hoods].
    unsigned dumpster_table_size;

    unsigned *hood_sizes;                           //  this'll become a lookup table the size of each hood.


    //  variables used when moving shit
    //  from shops to dumpsters.
    unsigned current_hood;                          //  when selling items to masses, shops use this to identify their hood.
    unsigned current_dumpster_size, current_dumpster_level;
    int current_sales;
    double current_price, current_revenue, current_profit;
    unsigned count_supplies;

    item_record *curr_item;                          //  used when I'm traversing each shop's inventory list.


    srand(time(NULL));


    if(DEBUG) printf("\nmapsize (HxW): %u x %u, population: %u", HEIGHT, WIDTH, MAP_POPULATION);


    if(DEBUG) printf("\nmallocing main map...");
    if((map = malloc(HEIGHT*WIDTH)) == NULL){ puts("\nmain(): malloc failed."); exit(1); }                          //  malloc space for main map.
    if(DEBUG) printf("done.");

    if(DEBUG) printf("\nmallocing nhood...");
    if((nhood = malloc(HEIGHT*WIDTH * sizeof(unsigned))) == NULL){ puts("\nmain(): malloc failed."); exit(1); }       //  malloc space for hood map.
    if(DEBUG) printf("done.");

    if(DEBUG) printf("\nmallocing stindexmap...");
    if((stindexmap = malloc(HEIGHT * WIDTH * sizeof(unsigned))) == NULL){ puts("\nmain(): malloc failed."); exit(1); }  //  malloc space for street name map.
    if(DEBUG) printf("done.");

    if(DEBUG) printf("\ninitialising maps...");
    for (i = 0; i < WIDTH*HEIGHT; i++) *(map + i) = BLOCK;                                             //  initialise main map to contain only blocks.
    for (i = 0; i < WIDTH*HEIGHT; i++) *(stindexmap + i) = 0;                                          //  initialise street index map to contain only zeroes.
    for (i = 0; i < WIDTH*HEIGHT; i++) *(nhood + i) = 0;                                               //  same for nhood map.
    if(DEBUG) printf("done.");


    if(DEBUG) printf("\ncalling streetmaker...");
    hcount=vcount=0;            //  streetmaker uses these variables.  they have to go somewhere.
    streetindex = streetmaker(0,0,HEIGHT-1, WIDTH-1, map, MINBLOCKSIZE, WIDTH, stindexmap, 1);                   //  build streets, index them. this function sets streetindex.
    streetindex--;      //  since streets aren't numbered zero, I have to reduce streetindex by one here.
    if(DEBUG) printf("done.");


    if(DEBUG) printf("\ncalling shopmaker...");
    totalshops = shopmaker(HEIGHT, WIDTH, map, SHOPCHANCE);                                                     //  find possible shop locations and place them.
    if(DEBUG) printf("...done. there are %u total shops.", totalshops);


    //  now I know how many shops there are, I need to make an array of that many shops.
    if(DEBUG) printf("\nmallocing shop database...");
    if((shopdb = malloc(totalshops*sizeof(shop))) == NULL){ puts("\nmain(): malloc failed."); exit(1); }        //  malloc space for hood map.
    if(DEBUG) printf("done.");


/*  walk through the map. Whenever I hit a shop, make the current
    shop's location equal to the map index. That way, when I enter a shop, I can
    look up which shop I'm in by searching the shopdb for whatever matches my
    position.   */
    if(DEBUG) printf("\nmaking shop address lookup table...");
    for (i = 0, j = 0; i < WIDTH*HEIGHT; i++){
         if(*(map + i) == SHOP){
            shopdb[j].loc = i;
            j++;
         }
    }
    if(DEBUG) printf("done.");

    //printf("\n\tshopzero's location is %u", shopdb[0].loc);

/*  I need to give shops some money and some space. I'm
    just going to give them random values for now. later
    base this on a height map over the game map, so that
    there are better areas to own a shop in.    */
    if(DEBUG) printf("\ndishing out cash and warehouse space to shops...");
    for (i = 0; i < totalshops; i++){
        shopdb[i].used_space = 0;
        shopdb[i].space = rand()%901 + 100;                                     //  100 to 1000 blocks of warehouse storage.
        shopdb[i].money = ((float)rand() / (float)(RAND_MAX)) * 900.0 + 100.0;  //  $100.00 to $1000.00 to each store initially.
        shopdb[i].inventory = NULL;
    }
    //show(HEIGHT, WIDTH, map);
    if(DEBUG) printf("done.");

    //printf("\n\tshopzero's initial space, money: %u, %.2f", shopdb[0].space, shopdb[0].money);

    if(DEBUG) printf("\ncalling hoodmaker...");
    hoodindex = hoodmaker(nhood, 0, 0, HEIGHT-1, WIDTH-1, HEIGHT, WIDTH, HOODSIZE, 0);          //  build the hood map, returns number of hoods made.
    if(DEBUG) {
        if(hoodindex == 1) printf("...done. there is 1 hood.");
        else printf("...done. there are %u hoods.", hoodindex);
    }
    if(DEBUG) printf("\n#streets: %u\t#hoods: %u\t#shops: %u", streetindex, hoodindex, totalshops);
    //puts("\nhood map:\n");
    //ushow(nhood, HEIGHT, WIDTH);
    //if(DEBUG) getche();

    //puts("\nstreet index map:\n");
    //ushow(stindexmap, HEIGHT, WIDTH);
    //if(DEBUG) getche();


    //  measure size of hoods.  Used to allocate population to each hood.
    if(DEBUG) printf("\nmeasuring hood sizes...");
    if((hood_sizes = malloc((hoodindex) * sizeof(unsigned))) == NULL){ puts("\nmain(): malloc failed."); exit(1); }
    //  initialise hood_sizes.
    for (i = 0; i < hoodindex; i++) *(hood_sizes + i) = 0;
    for (i = 0; i < HEIGHT*WIDTH; i++)
        (*(hood_sizes + *(nhood + i)))++;          //  if the current cell has hood number i, then increment hoodsizes[i].
/*  puts("\nhood sizes:");
    for (i = 0; i < hoodindex;i++){
        printf("\nhood %u: %u blocks, pop. est.=%.2f", i, *(hood_sizes + i), (float)*(hood_sizes + i)/(HEIGHT*WIDTH)*MAP_POPULATION);
    }   */
    if(DEBUG) printf("done.");

    //printf("\n\tshopzero's hood number is %u", nhood[shopdb[0].loc]);
    //printf("\n\tthe size of this hood is %u", *(hood_sizes + nhood[shopdb[0].loc]));

/*  malloc space for supplier. just hardcode # of supplies right now
    until things work, later on, put items into a separate file.    */
    if(DEBUG) printf("\nmallocing supplier database...");
    if((supplies = malloc( TOTAL_ITEMS * sizeof(supplier_item))) == NULL){ puts("\nmain(): malloc failed."); exit(1); }       //  malloc space for hood map.
    if(DEBUG) printf("done.");


    //  fill supplier's inventory with garbage to buy.
    if(DEBUG) printf("\nfilling supplier database with garbage to sell...");

    for (i = 0; i < TOTAL_ITEMS; i++){
        supplies[i].id          = i;
        supplies[i].quantity    = rand() % 100 + 1;
        supplies[i].size        = rand() % 100 + 1;
        supplies[i].bought      = 0;
        supplies[i].have        = 0;
        supplies[i].cost        = ((float)rand() / (float)(RAND_MAX)) * 100.0;      //  items cost $0 to $100.

    /*  initialising b for now, but it's set later when I know the shop's hood.
        this line is probably unnecessary.  */
        supplies[i].b           = 0;
        //  add something for 'n' later.
        supplies[i].m           = ((float)rand() / (float)(RAND_MAX)) * -3.5 - 3.5; //  gradient between -7 to -3.5.  Seems sensible.
    }
    if(DEBUG) printf("done.");

    //  display a few of the items to see if it's good.
    //puts("\nsample of supplier's items\n\nid\t#\tsize\tcost\tb\tm");
    //for(i=0; i<50; i++){
    //    printf("\n%u\t%u\t%u\t%.2f\t%.2f\t%.2f", supplies[i].id, supplies[i].quantity, supplies[i].size, supplies[i].cost, supplies[i].b, supplies[i].m);
   // }
    //puts("\npress key to continue");
    //getch();

    if(DEBUG) printf("\ngiving all items noise graphs...");
    for (i = 0; i < TOTAL_ITEMS; i++){

        //  malloc space for noise graph and then fill it.
        if((supplies[i].noise = malloc(LEN * sizeof(double))) == NULL){ puts("\nmain(): malloc failed."); exit(1); }       //  malloc space for hood map.
        noise(supplies[i].noise, 0, LEN-1, 1, 0.0, 100.0);       //  noise values set between 0 to 100.
    }
    if(DEBUG) printf("done.");


/*  make 2d array to hold dumpsters for each item for each hood. I do this by
    making an array of dumpster pointers, then mallocing a 1d array into each
    of those. kind of ugly but wanted to try it this way. It will let me use
    array notation I think, which has some benefits, but means I have to use
    two for loops. gross.   */
    if(DEBUG) printf("\nmallocing hood dumpsters...");
    if((dump = malloc(TOTAL_ITEMS * sizeof(dumpster*))) == NULL){ puts("\nmain(): malloc failed."); exit(1); }
    for (i = 0; i < TOTAL_ITEMS; i++) if((dump[i] = malloc(hoodindex * sizeof(dumpster))) == NULL) { puts("\nmain(): malloc failed."); exit(1); }
    if(DEBUG) printf("done.");


/*  give hoods and items random rot rates and dumpster sizes until I have real items to use.
    The size of the dumpster for item i in hood j is dump[i][j].size. */
    if(DEBUG) printf("\nsetting up dumpsters...");
    for (i = 0; i < TOTAL_ITEMS; i++){
        for (j = 0; j < hoodindex; j++){
            //  right now, the dumpster size for every item is the size of the hood's population.
            //  this isn't very good because it needs to adapt to the item also.
            //  so I'll multiply it by the size of the item for now. meh.
            dump[i][j].size     = (float)*(hood_sizes + j) / (HEIGHT*WIDTH) * MAP_POPULATION;
            dump[i][j].rot_amount = rand()%dump[i][j].size + 1;             //  dumpster rots by some number between 1 and its full size.
        }
    }


//  Read all text files containing words that I use to build street and hood names.
    if(DEBUG) printf("\nreading files...");
    totalprefixes = filereader("prefixes.txt", &prefixes);
    if(DEBUG) printf("\n\ttotalprefixes complete.");
    totalsuffixes = filereader("suffixes.txt", &suffixes);
    if(DEBUG) printf("\n\ttotalsuffixes complete.");
    totalstreetsuffs = filereader("streetsuffixes.txt", &streetsuffs);      //  something with streetsuffs is fucked. maybe.
    if(DEBUG) printf("\n\ttotalstreetsuffs complete.");
    if(DEBUG) printf("\n...done.");


    //  create hood names.
    //  first, create an array to hold hood names.
    if(DEBUG) printf("\nmallocing hoodnames...");
    if((hoodnames = malloc(hoodindex*sizeof(char*))) == NULL){ puts("hoodnames: malloc failed."); exit(1); } //  malloc space for 'hoodindex' character pointers.
    if(DEBUG) printf("done.");


    if(DEBUG) printf("\ngenerating hood names...");
//  generate a name for each hood, mallocing space for each name.  This keeps everything trim. :)
    for (i = 0; i < hoodindex; i++){

    //  get two randoms, to choose prefix and suffix.
        a = rand()%totalprefixes;
        b = rand()%totalsuffixes;

    //  figure how much space is needed for prefix, space, suffix, and null terminator.
        len = strlen(prefixes[a]) + strlen(suffixes[b]) + 2;

    //  make space for the hood name itself.
        if((*(hoodnames + i) = malloc(len)) == NULL) { puts("malloc failed."); exit(1); }

    //  make the name parts proper.
        proper(prefixes[a]);
        proper(suffixes[b]);

    //  copy the prefix, space, and suffix into it's new home. strcat automatically null terminates.
        strcpy(*(hoodnames + i), prefixes[a]);
        strcat(*(hoodnames + i), " ");
        strcat(*(hoodnames + i), suffixes[b]);
    }
    if(DEBUG) printf("done.");

    //printf("\n\tshopzero's hoodname is %s", *(hoodnames + (*(nhood + shopdb[0].loc))));



    //  Now create street names.  same process as above, probably turn this into a function later.
    //  create an array to hold street names.
    if(DEBUG) printf("\nmallocing streetnames...");
    if((streetnames = malloc(streetindex*sizeof(char*))) == NULL){ puts("malloc failed."); exit(1); } //  malloc space for 'streetindex' character pointers.
    if(DEBUG) printf("done.");

    if(DEBUG) printf("\ngenerating streetnames...");
    for (i = 0; i < streetindex; i++){

        //  for street names, I'm going to use prefix and/or suffix, and street type.
        a = rand()%totalprefixes;
        b = rand()%totalsuffixes;
        c = rand()%totalstreetsuffs;

        //  choose between only suffix, only prefix, or both.
        r = rand()%3;

        //  if r == 0, use all three parts in the name.  The prefix and suffix are concatenated to make one word.
        if(!r){

            //  figure how much space is needed for prefix, suffix, space, and null terminator.
            len = strlen(prefixes[a]) + strlen(suffixes[b]) + strlen(streetsuffs[c]) + 3;

            //  make space for the street name.
            if((*(streetnames + i) = malloc(len)) == NULL) { puts("malloc failed."); exit(1); } //  make space for this string.

            //  make the name parts proper.
            proper(prefixes[a]);
            unproper(suffixes[b]);
            proper(streetsuffs[c]);

            strcpy(*(streetnames + i), prefixes[a]);
            strcat(*(streetnames + i), suffixes[b]);
            strcat(*(streetnames + i), " ");
            strcat(*(streetnames + i), streetsuffs[c]);
        }

        //  if r == 1, only use a prefix and a street type.
        else if(r == 1){

            //  figure how much space is needed for prefix, space, and null terminator.
            len = strlen(prefixes[a]) + strlen(streetsuffs[c]) + 2;

            //  make space for the street name.
            if((*(streetnames + i) = malloc(len)) == NULL) { puts("(r=1): malloc failed."); exit(1); } //  make space for this string.

            //  make the name parts proper.
            proper(prefixes[a]);
            proper(streetsuffs[c]);

            strcpy(*(streetnames + i), prefixes[a]);
            strcat(*(streetnames + i), " ");
            strcat(*(streetnames + i), streetsuffs[c]);
        }

        //  if r == 2, only use a suffix and a street type.
        else if(r == 2){
            //  figure how much space is needed for prefix, suffix, space, and null terminator.
            len = strlen(suffixes[b]) + strlen(streetsuffs[c]) + 2;

            //  make space for the street name.
            if((*(streetnames + i) = malloc(len)) == NULL) { puts("(r=2): malloc failed."); exit(1); } //  make space for this string.

            //  make the name parts proper.
            proper(suffixes[b]);
            proper(streetsuffs[c]);

            strcpy(*(streetnames +i), suffixes[b]);
            strcat(*(streetnames +i), " ");
            strcat(*(streetnames +i), streetsuffs[c]);
        }

        else puts("\nfuuuuuck");
    }

    //  Since street names and hood names have been made, I can free the file copies.
    free(prefixes);
    free(suffixes);
    free(streetsuffs);

    if(DEBUG) printf("done.");


/*  if(DEBUG){
        puts("\nhood names for this map:");
        for (i = 0; i < hoodindex; i++){
            printf("\n\t%s", *(hoodnames + i));
        }

        puts("\n\nstreet names for this map:");
        for (i = 0; i < streetindex; i++){
            printf("\n\t%s", *(streetnames + i));
        }
    }
*/


    /*  Here is where I squash all the maps into a single 2d array of structs.  I haven't decided if I'll use this yet. */
    if(DEBUG) printf("\nsquashing maps...");
    //  time to squash all this into mapsquash.
    for (i = 0; i < HEIGHT; i++){
        for (j = 0; j < WIDTH; j++){
            mapsquash[i][j].type = *(map + i*WIDTH + j);    //  set the cell type.
            //  make space for hood name.
            if((mapsquash[i][j].hood = malloc(1 + strlen(hoodnames[nhood[i*WIDTH + j]]))) == NULL) { puts("malloc1 failed."); exit(1); }
            //  then copy it over.
            strcpy(mapsquash[i][j].hood, hoodnames[nhood[i*WIDTH + j]]);

            //  next, do the same for the streetname.
            //  copy street name over if it's a street.
            if(*(map + i*WIDTH + j) == STREET){
                if((mapsquash[i][j].street = malloc(1 + strlen(streetnames[stindexmap[i*WIDTH + j]-1]))) == NULL) { puts("malloc2 failed."); exit(1); }
                //  then copy it over.
                strcpy(mapsquash[i][j].street, streetnames[stindexmap[i*WIDTH + j]-1]);
            }
            //else *(map + i*WIDTH + j) = NULL;

            //  set index.  use later, probably.
            mapsquash[i][j].index = i*WIDTH + j;
            //  now, connect all these shits together.  change routine if on edge and/or corner.    hassle.
            //  if in center, connect to everything.  There must be a better way than this.
            if(i && j && i != (HEIGHT-1) && j != WIDTH-1){
                mapsquash[i][j].u = &mapsquash[i - 1][j];
                mapsquash[i][j].d = &mapsquash[i + 1][j];
                mapsquash[i][j].l = &mapsquash[i][j - 1];
                mapsquash[i][j].r = &mapsquash[i][j + 1];
            }
            //  else, if on left edge and not a corner...
            else if(!j && i>0 && i < (HEIGHT-1)){
                mapsquash[i][j].u = &mapsquash[i - 1][j];
                mapsquash[i][j].d = &mapsquash[i + 1][j];
                mapsquash[i][j].r = &mapsquash[i][j + 1];
            }
            //  else, if on right edge and not a corner...
            else if(j == (WIDTH-1) && i > 0 && i < (HEIGHT-1)){
                mapsquash[i][j].u = &mapsquash[i - 1][j];
                mapsquash[i][j].d = &mapsquash[i + 1][j];
                mapsquash[i][j].l = &mapsquash[i][j - 1];
            }
            //  else, if on top edge and not a corner...
            else if(!i && j>0 && j < (WIDTH-1)){
                mapsquash[i][j].d = &mapsquash[i + 1][j];
                mapsquash[i][j].r = &mapsquash[i][j + 1];
                mapsquash[i][j].l = &mapsquash[i][j - 1];
            }
            //  else, if on bottom edge and not a corner...
            else if(i == (HEIGHT-1) && j>0 && j < (WIDTH - 1)){
                mapsquash[i][j].u = &mapsquash[i - 1][j];
                mapsquash[i][j].r = &mapsquash[i][j + 1];
                mapsquash[i][j].l = &mapsquash[i][j - 1];
            }
            //  else, if on top left corner...
            else if(!i && !j){
                mapsquash[i][j].r = &mapsquash[i][j + 1];
                mapsquash[i][j].d = &mapsquash[i + 1][j];
            }
            //  else, if on top right corner...
            else if(!i && j == (WIDTH - 1)){
                mapsquash[i][j].l = &mapsquash[i][j - 1];
                mapsquash[i][j].d = &mapsquash[i + 1][j];
            }
            //  else, if on bottom left corner...
            else if(i == (HEIGHT - 1) && !j){
                mapsquash[i][j].r = &mapsquash[i][j + 1];
                mapsquash[i][j].u = &mapsquash[i - 1][j];
            }
            //  else, if on bottom right corner...
            else if(i == (HEIGHT - 1) && j == (WIDTH - 1)){
                mapsquash[i][j].u = &mapsquash[i - 1][j];
                mapsquash[i][j].l = &mapsquash[i][j - 1];
            }
        }
    }
    if(DEBUG) printf("done.");


    //  setup weather stuff.
    lastcheck.firsttime=1;                          //  flag used to check if weatherreport() has been called before or not.
    sinefiller(heat, LEN, HEATMIN, HEATMAX);        //  set heat values between HEATMIN and HEATMAX.  later, min and max will be set by the current season.

    humidity[0] = humidity[LEN-1] = 50.0;           //  set outer limits of arrays to be noised.
    wind[0]     = wind[LEN-1]     = 0.0;
    cloud[0]    = cloud[LEN-1]    = 30.0;
    //wind[0] = wind[LEN-1] = (double)rand() / (double)RAND_MAX * 20.0;

    noise(humidity, 0, LEN-1, 1, 0.0, 100.0);       //  set humidity values between 0-100%.
    noise(wind,     0, LEN-1, 1, 0.0, 88.1);        //  set wind values between 0-88.1, using the Beufort scale.
    noise(cloud,    0, LEN-1, 1, 0.0, 81.0);        //  set cloud values between 0-100%.

    myclock.h = 0;
    myclock.m = 0;
    myclock.s = 0;



//  -----------v-------big ugly game loop, just for testing-------------v-----------------

/*  put the player on a random map tile. If that tile is
    not a STREET tile, move the player forward on the map
    until he is on a STREET tile. If I've moved hom past
    the end of the map, move him back to the first tile.    */
    i = HEIGHT * WIDTH;
    player.position = rand() % i;
    while(*(map + player.position) != STREET){
        player.position++;
        if(player.position == i) player.position = 0;
    }

    if(DEBUG) printf("\nentering infinite loop...\n\n");
    while(1){

      system("cls");

    //  show map with user on it.
        display(HEIGHT, WIDTH, map, player.position);

    //  display current game time.
        printf("\n%02d:%02d:%02d\n", myclock.h, myclock.m, myclock.s);

    //  report weather after every (WEATHER_REPORT_INTERVAL) seconds.
        if(!((myclock.s + myclock.m + myclock.h) % WEATHER_REPORT_INTERVAL)) weatherreport(&lastcheck, heat[i], humidity[i], wind[i], cloud[i], &myclock);


    //  advance the clock by CLOCK_ADVANCE seconds.
        for (i = 0; i < CLOCK_ADVANCE; i++){
            myclock.s++;

            if(myclock.s == 60) {
                myclock.s = 0;
                myclock.m++;
            }
            if(myclock.m == 60){
                myclock.m = 0;
                myclock.h++;
            }
            if(myclock.h == 24){
                myclock.h = 0;
            }
        }



    /*  tell the user which street and hood
        he's in, and whether or not he's in
        a shop. */
        if(*(map + player.position) == STREET) printf("\n\nyou are on %s, ", streetnames[stindexmap[player.position]-1]);
        else if(*(map + player.position) == SHOP) printf("\nyou are in a shop, ");

    //  use this code when I decide to switch to using mapsquash. It's more readable.
        //if(map[player.position].type==STREET) printf("\nyou are on %s, ", map[player.position].street);

        printf("in the neighbourhood of %s.", hoodnames[nhood[player.position]]);

    //   dsplay the user's movement options, based on his current position.
        printf("\noptions: x ");
        if(player.position/WIDTH){
            if(*(map + player.position-WIDTH) == STREET || *(map + player.position-WIDTH) == DOOR) printf("n ");
        }
        if(player.position/WIDTH != HEIGHT-1){
            if(*(map + player.position + WIDTH) == STREET || *(map + player.position + WIDTH) == DOOR) printf("s ");
        }
        if(player.position%WIDTH){
            if(*(map + player.position-1) == STREET || *(map + player.position-1) == DOOR) printf("w ");
        }
        if(player.position%WIDTH != WIDTH-1){
            if(*(map + player.position + 1) == STREET || *(map + player.position + 1) == DOOR) printf("e");
        }


    /*  if player is inside a shop, call insideshop() to give the shop's dialog.
        The shop's dialog shows inventory content, etc. */
        if(*(map + player.position) == SHOP) {
            //  search the shopdb for a .loc that matches this current map position.
            for (i = 0; i < totalshops; i++) if(shopdb[i].loc == player.position) break;
            if(i != totalshops) insideshop(&shopdb[i]);
            else puts("\nI can't find the shop. :/");   //  probably unnecessary.
        }

        else if(*(map + player.position) == STREET) puts("\nYou are on the street.");



    //  Make all shops stock up using my SupplierPurchasing algorithm.
    if(DEBUG) printf("\nshops are stocking up...");
    for (j = 0; j < totalshops; j++){

    //  if shop has no space at all, skip it: no item has
    //  anything less than size of 1.
        if(shopdb[j].used_space == shopdb[j].space) continue;

    //  get shop's hood number. Needed to calculate rrp and b.
        current_hood = *(nhood + shopdb[j].loc);

    //  move the shop's inventory quantities into the have column,
    //  so that it includes shit that it already owns in the price calculations.
    //  grab the head of the inventory linked list, and run through it.
        curr_item = shopdb[j].inventory;
        while(curr_item != NULL){
            supplies[curr_item->id].have = curr_item->quantity;
            curr_item = curr_item->next;
        }


   /*   This is what's about to happen. For each item, I calculate the profit density
        of buying one more item. If that density is the highest in all the list, the
        shop buys that product at that quantity. Checks are made that the quantity is
        available to buy, and the shop has money and space. */

        do{
            possible_flag       = 0;
            best_item           = TOTAL_ITEMS;
            best_profit_density = 0.0;

            for (i = 0; i < TOTAL_ITEMS; i++){

                    //  check that this item exists. if not, skip it.
                    if(supplies[i].quantity == 0) continue;

                    //  check if I can even store this item. if not, skip it.
                    if(supplies[i].size > (shopdb[j].space - shopdb[j].used_space)) continue;

                    //  if shop has less than 0.01, then it effectively has no money.
                    if(shopdb[j].money < 0.01) continue;

                    //  check if I can even afford one more item.
                    if(supplies[i].cost > shopdb[j].money) continue;

                /*  calculate the item's b value, based on shop's location. b
                    is equal to the population of that hood, which happens to
                    be the same size as that hood's dumpster for that item. */
                    supplies[i].b = dump[i][current_hood].size;

                    //  number of total items upon which the profit is calculated.
                    current_sales = supplies[i].bought + supplies[i].have + 1;

                    current_price   = (current_sales - supplies[i].b) / supplies[i].m;      //  price you'd charge for it.
                    if(current_price <= supplies[i].cost) continue;                         //  if it costs >= amount you'd charge, skip it.

                    current_profit  = current_price - supplies[i].cost;                     //  profit per item sold.

                //  calculate the profit density of one more item.
                //  this is how much profit I get out of the item per size unit, per cost unit.
                //  I add 1 to cost unit here to avoid division by zero when the item is free.
                //  doesn't hurt too much.
                    profit_density = current_profit / (float)supplies[i].size;

                //  if another item isn't profitable, skip it.
                    if(profit_density <= 0.0) continue;

                //  if there's at least one item that exists, that I can afford and store, and that is profitable, then keep going next time.
                    possible_flag = 1;

                //  if this profit/space ratio is greatest found so far, save it.
                    if(profit_density > best_profit_density){
                        best_profit_density = profit_density;
                        best_item           = i;
                    }
            }

        //  if something was found, add it to bought list (like a shopping cart).
            if(best_item != TOTAL_ITEMS){
                supplies[best_item].quantity--;
                supplies[best_item].bought++;
                shopdb[j].money         -= supplies[best_item].cost;
                shopdb[j].used_space    += supplies[best_item].size;
                if(shopdb[j].used_space < 0) { puts("\nbonerrrrs!!!!!!!!!!!!##!@$@#$%$%^&%^*"); shopdb[j].used_space = 0; }
            }

        }while(possible_flag);

    /*  Here is where I actually put the contents of the bought list
        into the shop.  */
        //printf("\n\nhere is what shop %u bought:", j);
        for (i = 0; i < TOTAL_ITEMS; i++){
            if(supplies[i].bought) {
                stock_this.id       = supplies[i].id;
                stock_this.quantity = supplies[i].bought;
                stock_this.m        = supplies[i].m;
                stock_this.n        = supplies[i].n;
                stock_this.b        = supplies[i].b;
                stock_this.price    = ((float)stock_this.quantity + (float)supplies[i].have - stock_this.b) / stock_this.m;
                stock_this.size     = supplies[i].size;

            //  if shop already has this item, then search for it and update it.
            //  otherwise just use the stock_these function to put in a new item.
                curr_item = shopdb[j].inventory;
                found = 0;
                while(curr_item != NULL){
                    if(curr_item->id == stock_this.id){
                        curr_item->quantity += stock_this.quantity;
                        curr_item->price = stock_this.price;
                        found = 1;
                        break;
                    }
                    curr_item = curr_item->next;
                }

                if(!found) stock_these(&shopdb[j].inventory, &shopdb[j].inventory_end, &stock_this);

            //  clear item for next shop.
                supplies[i].bought  = 0;
                supplies[i].have    = 0;

                //printf("\nshop %u bought {item %u}", j, stock_this.id);

            }
        }
    }
    if(DEBUG) printf("done.");
    //puts("\n\there is shopzero's inventory:");
    //insideshop(&shopdb[0]);


    if(DEBUG) printf("\nselling to NPC masses...");
/*  Here, all shops sell their content to the NPC masses. for
    every item in every shop, figure out how many will sell of
    it will sell in that hood. if the shop has that quantity,
    sell it. If it doesn't, sell all of what it has.    */
    //puts("\nshops are about to sell to the masses.");
    for (i = 0; i < totalshops; i++){
    //  get shop's hood number. more readable to use this.
        current_hood = *(nhood + shopdb[i].loc);

    //  grab the head of the inventory linked list.
        curr_item = shopdb[i].inventory;

        //printf("\nshop %u, hood=%u.", i, current_hood);
        //if(curr_item == NULL) puts("\nshop is empty.  next shop~!");

        //if(i == 0 && curr_item == NULL) printf("\n\tshopzero has no stuff.");
        while(curr_item != NULL){
        //  if this item hasn't sold out, then process it.
            if(curr_item->quantity != 0) {

              //  if(i == 0) printf("\n\tshopzero has item %u: price=%.2f quant=%u m=%.2f b=%.2f", curr_item->id, curr_item->price, curr_item->quantity, curr_item->m, curr_item->b);

            /*  use the current item's id to
                look at the dumpster table in
                this hood's column to get the
                fullness for this item. */
                current_dumpster_level  = dump[curr_item->id][current_hood].level;
                current_dumpster_size   = dump[curr_item->id][current_hood].size;

                //if(i==0) printf("\nthis item's dumpster level and size: %u, %u", current_dumpster_level, current_dumpster_size);

            /*  Use that to figure out how many sales the shop gets using its current
                price for the item. The b in the line equation gets multiplied by the
                ratio (space left in dumpster)/(dumpster size).
                Also, to avoid a rounding error preventing the shop from selling an item,
                I've added 0.5 to the sales.  That way, selling 0.5 items becomes 1 item,
                10.6 items becomes 11 items, etc.   */
                current_sales = 0.5 + curr_item->m * curr_item->price + (1.0 - (double)current_dumpster_level / (double)current_dumpster_size) * curr_item->b;
                if(current_sales < 0) current_sales = 0;    //  if negative however, that's not a sale.


                //printf("\nitem %u: m=%.2f, price=%.2f, b=%.2f", curr_item->id, curr_item->m, curr_item->price, curr_item->b);
                //printf("\n\titem %u x %u, dlevel=%u, dsize=%u, could sell %d", curr_item->id, curr_item->quantity, current_dumpster_level, current_dumpster_size, current_sales);

            /*  if shop has this many items to sell then move
                this number of items from shop to dumpster. */
                if(current_sales != 0 && current_sales <= curr_item->quantity){
                    //  edit the item's quantity, update dumpster value.
                    curr_item->quantity -= current_sales;
                    dump[curr_item->id][current_hood].level += current_sales;

                    //  update shop's cash.
                    shopdb[i].money += current_sales * curr_item->price;

                    //  update shop's available space.
                    shopdb[i].used_space -= current_sales * curr_item->size;

                    //if(i==0) printf("\n\tshopzero just made %.2f", current_sales * curr_item->price);
                }

                //  otherwise, just sell all that it has.
                else if(current_sales != 0){
                    //printf("\nI don't have this many, so I'll sell all I have for this price.");
                    shopdb[i].money += curr_item->quantity * curr_item->price;
                    dump[curr_item->id][current_hood].level += curr_item->quantity;
                    //if(i==0) printf("\n\tshopzero just made %.2f selling everything", curr_item->quantity * curr_item->price);
                    curr_item->quantity = 0;
                }
            //  if dumpster is overflowing, force it to be equal to max size (full).
                if(dump[curr_item->id][current_hood].level > dump[curr_item->id][current_hood].size) dump[curr_item->id][current_hood].level = dump[curr_item->id][current_hood].size;
            }

            //else printf("\nI have sold out of this item.");

            curr_item = curr_item->next;
        } //end of while
    //puts("\npress key to continue");
    //getche();
    }//end of for.
    if(DEBUG) puts("done.");



//  if the quantity of a thing is zero at the suppliers, put more there.
    if(DEBUG) printf("supplier is restocking...");
    for (i = 0; i < TOTAL_ITEMS; i++){
        if(supplies[i].bought == 0){
            supplies[i].quantity    = rand() % 100 + 1;
            supplies[i].bought      = 0;
           // printf("\nsupplier's restocked {item %u}: %u items.", i, supplies[i].quantity);
        }
    }
    if(DEBUG) puts("done.");


    //  tell how may supplies are left at the suppliers.
    if(DEBUG) {
        count_supplies = 0;
        for (i = 0; i < TOTAL_ITEMS; i++) count_supplies += supplies[i].quantity;
        printf("there are %u items in the suppliers", count_supplies);
    }

    //  do some dumpster rotting.
    if(DEBUG) printf("\nrotting some dumpsters...");
    for (i = 0; i < TOTAL_ITEMS; i++){
        for (j = 0; j < hoodindex; j++){
            if(dump[i][j].level == 0) continue;
            if(dump[i][j].rot_amount >= dump[i][j].level) dump[i][j].level = 0;
            else dump[i][j].level -= dump[i][j].rot_amount;
        }
    }
    if(DEBUG) puts("done.");

        //  get user's input.
        printf("\n: ");
        //input=getchar();
        input = getche();
        putchar('\n');

        if(input == 'x') exit(0);

    /*  if user uses arrows on keyboard, then deal
        with the escape character, and convert to
        a direction.    */
        if(input == -32){
            input=getche();
            if(input == 'H') input = 'n';
            else if(input == 'P') input = 's';
            else if(input == 'K') input = 'w';
            else if(input == 'M') input = 'e';
        }

        if(input == 'n'){
            if(*(map + player.position-WIDTH) == DOOR) player.position -= 2*WIDTH;
            else if(player.position/WIDTH == 0) puts("\nyou can't go any further north.");
            else if(*(map + player.position-WIDTH) != STREET) puts("\nthere's a building there.");
            else player.position -= WIDTH;
        }

        if(input == 's'){
            if(*(map + player.position + WIDTH) == DOOR) player.position += 2*WIDTH;
            else if(player.position/WIDTH == HEIGHT-1) puts("\nyou can't go any further south.");
            else if(*(map + player.position + WIDTH) != STREET) puts("\nthere's a building there.");
            else player.position += WIDTH;
        }

        if(input == 'w'){
            if(*(map + player.position-1) == DOOR) player.position -= 2;
            else if(player.position%WIDTH == 0) puts("\nyou can't go any further west.");
            else if(*(map + player.position-1) != STREET) puts("\nthere's a building there.");
            else player.position -= 1;
        }

        if(input == 'e'){
            if(*(map + player.position + 1) == DOOR) player.position += 2;
            else if(player.position%WIDTH == WIDTH-1) puts("\nyou can't go any further east.");
            else if(*(map + player.position + 1) != STREET) puts("\nthere's a building there.");
            else player.position += 1;
        }


   }//end of infinite loop.
//  ----------^--------big ugly game, just for testing------------^------------------

    free(map);
    free(nhood);
    free(stindexmap);
}



/*  takes a 2d array and draws a horizontal or a vertical street across it.
    This makes two rectangles. The function is called recursively with each
    rectangle, until no rectangle side is larger than blocksize. x1,y1 are top-left
    cell of rectangle, x2,y2 are bottom-right cell. map is pointer to the array.
    blocksize is maximum size that a rectangle's side can be. widthofmainmap is same
    as x2 on the first call. This function also gives streets an index number so I
    can name it later.  */
unsigned streetmaker(unsigned x1,unsigned y1,unsigned x2,unsigned y2, char *map, unsigned blocksize, unsigned widthofmainmap, unsigned *stindexmap, unsigned sindex){
    unsigned vside, hside, p, i;
    char vsideflag=0;

//  calculate length of sides of current rectangle.
    vside = x2-x1 + 1;
    hside = y2-y1 + 1;

//  if both sides are small enough, we're done.
    if(vside < (2*blocksize + 1) && hside < (2*blocksize + 1)) return sindex;

/*  Otherwise, if both sides are larger enough to be split, choose randomly between
    them, but do it proportionally to the ratio of horizontal to vertical cuts made
    so far. This way, if vertical cuts are dominating, then horizontal ones will
    become more likely, and vice versa. This should help to stop wacky ratios from
    making cities have too much of one and not the other.   */
    else if(vside>2*blocksize && hside>2*blocksize) vsideflag=(hcount < (rand()%(hcount + vcount + 2)));

//  else if the vertical side is the only one that can be cut, do it.
    else if(vside>2*blocksize) vsideflag=1;

    if(vsideflag){
        vcount++;
        p = rand()%(1 + x2-blocksize-(x1 + blocksize)) + x1 + blocksize;
        for (i = y1; i<=y2; i++) {
                *(map + (p)*widthofmainmap + i) = STREET;               //  mark that cell as a street.
                *(stindexmap + (p)*widthofmainmap + i) = sindex;        //  give that cell a street index number on the street index map.
        }
        sindex = streetmaker(x1, y1, p-1, y2, map, blocksize, widthofmainmap, stindexmap, sindex + 1);
        sindex = streetmaker(p + 1, y1, x2, y2, map, blocksize, widthofmainmap, stindexmap, sindex);
    }
    else{
        hcount++;
        p = rand()%(1 + y2-blocksize-(y1 + blocksize)) + y1 + blocksize;
        for (i = x1; i<=x2; i++){
                *(map + i*widthofmainmap + p) = STREET;
                *(stindexmap + i*widthofmainmap + p) = sindex;
        }
        sindex = streetmaker(x1, y1, x2, p-1, map, blocksize, widthofmainmap, stindexmap, sindex + 1);
        sindex = streetmaker(x1, p + 1, x2, y2, map, blocksize, widthofmainmap, stindexmap, sindex);
    }
    return sindex;
}



//  display the map.
void show(unsigned r, unsigned c, char *map){
    unsigned i, size=r*c;

    for (i = 0; i < size; i++){
        if(!(i%c)) putchar(' ');
        if(*(map + i) == BLOCK) putchar(178);
        else if(*(map + i) == STREET) putchar(' ');
        else if(*(map + i) == SHOP) putchar('$');
        else if(*(map + i) == DOOR) putchar('D');
        if(i%c + 1 == c) putchar('\n');
    }
}



//  finds potential shop locations and marks them.  Then randomly decides which will become shops.
unsigned shopmaker(unsigned height, unsigned width, char *map, char chance){
    char quadrant[5]={STREET, STREET, BLOCK, STREET, BLOCK};        //  this is a shape I place on the map to see if it matches. It gets rotated too.
    unsigned i, j, myheight, mywidth, mysize, mapsize;
    char *mymap;
    char *potentialshops;           //  1 if a shop could go here, 0 if otherwise.
    char quadflag = 0;              //  indicates which quadrant is being used. 0=first, 1=second, etc.
    unsigned start, limit;          //  where quadrant starts and ends.
    char tmp;                       //  used to rotate quadrant.
    char doorcount;                 //  number of doors a potential shop has to choose from.
    unsigned doorindex[4];          //  use this to note down where potential doors are to a shop.
    unsigned totalshops = 0;
    unsigned totalpotentials = 0;

    myheight = height + 2;
    mywidth  = width + 2;
    mapsize  = height * width;

    //  I need to make a copy of the grid, because I need an edge around it for the next step.
    if((mymap = malloc(mysize = myheight * mywidth)) == NULL){ puts("\nshopmaker(): malloc failed."); exit(1); }

    //  copy map into centre of mymap.
    for (i = 0, j=width + 1; i < mapsize; i++, j++){
        if(j%(mywidth) == width + 1) j += 2;
        *(mymap + j) = *(map + i);
    }

    //  initialise the edge of mymap with blocks.
    for (i = 0; i < mysize; i++) if(!(i % mywidth) || (i % mywidth == mywidth-1) || !(i / mywidth) || (i / mywidth == myheight - 1)) *(mymap + i) = BLOCK;

    //  another map copy, 1 if shop could go here, 0 otherwise.
    if((potentialshops = malloc(mysize)) == NULL){ puts("\nshopmaker(): malloc failed."); exit(1); }

    //  initialise shop potentials to STREET. I just needed any value that wasn't SHOP or DOOR.
    for (i = 0; i < mysize; i++) *(potentialshops + i) = STREET;

    //  calculate start and end points based on current quadrant.
    for (quadflag = 0; quadflag < 4; quadflag++){

        if(quadflag == 0){                        //  maybe combine pairs of these later.
            start = mywidth;
            limit = mysize - 1 - 3 * mywidth - 2;
        }
        else if(quadflag == 1){
            start = 1;
            limit = mysize - 1 - 2 * mywidth - 3;
        }
        else if(quadflag == 2){
            start = mywidth;
            limit = mysize - 1 - 3 * mywidth - 2;
        }
        else if(quadflag == 3){
            start = 1;
            limit = mysize - 1 - 2 * mywidth - 3;
        }

        for (i = start; i <= limit; i++){

            //  if quadrant is off the map, skip it forward.  these could combine too.
            if(quadflag == 0){        if(i%mywidth == mywidth-2) i+=2; }
            else if(quadflag == 1){   if(i%mywidth == mywidth-3) i+=4; }
            else if(quadflag == 2){   if(i%mywidth == mywidth-2) i+=2; }
            else if(quadflag == 3){   if(i%mywidth == mywidth-3) i+=4; }

            //  if first quadrant, and it matches its current position, save potential shop location indicated by the first quadrant.
            if(quadflag == 0){
                if(*(mymap+i+1) == quadrant[0] && *(mymap+i+mywidth) != quadrant[1] && *(mymap+i+mywidth+1) == quadrant[2] && \
                *(mymap+i+mywidth+2) != quadrant[3] && *(mymap+i+2*mywidth+1) == quadrant[4]){
                    *(potentialshops+i+2*mywidth+1) = SHOP;     //  mark potential shop location.
                    *(potentialshops+i+mywidth+1) = DOOR;       //  mark potential door location.
                }
            }

            //  if 2nd quadrant, and it matches its current location, save potential shop location indicated by the 2nd quadrant.
            else if(quadflag == 1){
                if(*(mymap+i+1) != quadrant[0] && *(mymap+i+mywidth) == quadrant[1] && *(mymap+i+mywidth+1) == quadrant[2] && \
                *(mymap+i+mywidth+2) == quadrant[3] && *(mymap+i+2*mywidth+1) != quadrant[4]) {
                    *(potentialshops+i+mywidth) = SHOP;     //  mark potential shop location.
                    *(potentialshops+i+mywidth+1) = DOOR;   //  mark potential door location.
                }
            }

            //  repeat for 3rd quadrant.
            else if(quadflag == 2){
                if(*(mymap+i+1)  ==  quadrant[0] && *(mymap+i+mywidth) != quadrant[1] && *(mymap+i+mywidth+1) == quadrant[2] && \
                *(mymap+i+mywidth+2) != quadrant[3] && *(mymap+i+2*mywidth+1) == quadrant[4]) {
                    *(potentialshops+i+1) = SHOP;
                    *(potentialshops+i+mywidth+1) = DOOR;
                }
            }

            //  repeat for 4th quadrant.
            else if(quadflag == 3){
                if(*(mymap+i+1) != quadrant[0] && *(mymap+i+mywidth) == quadrant[1] && *(mymap+i+mywidth+1) == quadrant[2] && \
                *(mymap+i+mywidth+2) == quadrant[3] && *(mymap+i+2*mywidth+1) != quadrant[4]) {
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

     }//end of for (quadflag...)

    //  at this point, potentialshops should contain SHOP where shops can go. so print it and see.
    //  show(myheight, mywidth, potentialshops);

    //  now, go back through the map, to visit each potential location. randomly decide if it will be a shop or not.
    //  if so, choose where door should be if more than 1 choice.  place shop, and door. move to next potential shop location.
    for (i = mywidth+1, doorcount=0; i < mysize-1-mywidth; i++){

        if(*(potentialshops+i) != SHOP) continue;     //  if not a potential shop, continue.
        else totalpotentials++;
        if(i%mywidth == mywidth-1) i+=2;              //  if on outer edge, skip forward two place back onto map.


        if((rand()%101)>chance){                                                            //  shop has chance percent of being built.
            *(potentialshops+i) = BLOCK;                                                    //  Otherwise it's turned into a block.
            if(*(potentialshops+i-mywidth) == DOOR)   *(potentialshops+i-mywidth) = BLOCK;    //  And so are all its potential doors.
            if(*(potentialshops+i+mywidth) == DOOR)   *(potentialshops+i+mywidth) = BLOCK;
            if(*(potentialshops+i-1) == DOOR)         *(potentialshops+i-1) = BLOCK;
            if(*(potentialshops+i+1) == DOOR)         *(potentialshops+i+1) = BLOCK;
        }

        //  count and note indexes of surrounding doors.
        if(*(potentialshops+i-mywidth) == DOOR){ doorindex[doorcount]=i-mywidth; doorcount++; }
        if(*(potentialshops+i+mywidth) == DOOR){ doorindex[doorcount]=i+mywidth; doorcount++; }
        if(*(potentialshops+i+1) == DOOR){ doorindex[doorcount]=i+1; doorcount++; }
        if(*(potentialshops+i-1) == DOOR){ doorindex[doorcount]=i-1; doorcount++; }

        //  if only one door, leave it there.  reset doorcount and skip further tests, go to next shop.
        if(doorcount == 1) { doorcount=0; continue; }

        //  if two doors, choose which one gets deleted.
        else if(doorcount == 2){ if(rand()%2)  *(potentialshops+doorindex[0]) = BLOCK; else *(potentialshops+doorindex[1]) = BLOCK;}

        //  else if 3 doors available, choose which 2 get destroyed.
        else if(doorcount == 3){
            j = rand()%3;   //  choose one of the three doors.
            if(j == 0)        *(potentialshops+doorindex[1]) = *(potentialshops+doorindex[2]) = BLOCK;    //  if door 0 chosen, destroy doors 1 and 2.
            else if(j == 1)   *(potentialshops+doorindex[0]) = *(potentialshops+doorindex[2]) = BLOCK;    //  if door 1, destroy doors 0 and 2.
            else if(j == 2)   *(potentialshops+doorindex[0]) = *(potentialshops+doorindex[1]) = BLOCK;    //  and so on.
        }

        //  else if 4 doors available, choose which 3 get expunged.
        else if(doorcount == 4){
            j = rand()%4;   //  choose one of the three doors.
            if(j == 0)        *(potentialshops+doorindex[1]) = *(potentialshops+doorindex[2]) = *(potentialshops+doorindex[3]) = BLOCK;    //  if door 0, destroy doors 1, 2, and 3.
            else if(j == 1)   *(potentialshops+doorindex[0]) = *(potentialshops+doorindex[2]) = *(potentialshops+doorindex[3]) = BLOCK;    //  if door 1, destroy doors 0, 2, and 3.
            else if(j == 2)   *(potentialshops+doorindex[0]) = *(potentialshops+doorindex[1]) = *(potentialshops+doorindex[3]) = BLOCK;    //  if door 2, destroy doors 0, 1, and 3.
            else if(j == 3)   *(potentialshops+doorindex[0]) = *(potentialshops+doorindex[1]) = *(potentialshops+doorindex[2]) = BLOCK;    //  if door 3, destroy doors 0, 1, and 2.
        }

        //  reset doorcount for next shop.
        doorcount=0;

    }   //  now all doors have been chosen.

    //  write doors and shops to main map.
    for (i = 0, j=width+1; i < mapsize; i++, j++){
        if(j%(mywidth) == width+1) j+=2;
            if(*(potentialshops+j) != STREET){
                *(map+i) = *(potentialshops+j);
                if(*(potentialshops+j) == SHOP) totalshops++;
            }

    }

    free(mymap);
    free(potentialshops);
    if(DEBUG) printf("\n\ttotal potential shops: %u\n", totalpotentials);
    return totalshops;
}


//  makes the hood maps, rectangles on the map that are each assigned a number each.
//  on the first call, hindex has to be zero.
unsigned hoodmaker(unsigned *nhood, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned height, unsigned width, unsigned nsize, unsigned hindex){
    unsigned rectheight, rectwidth;     //  dimensions of the current rectangle.
    unsigned p;                         //  random position on whichever side is chosen.
    char vsplitflag=0;                  //  choice of side to split.  if 1, map will be split from left to right.
    unsigned i,j,a,b;
    unsigned hoodindex;                 //  hoodindex is unnecessary here, I can just use hindex, fix this later.

    if(!hindex) hoodindex=1;            //  this happens on first call.
    else hoodindex = hindex;

    rectheight = x2-x1+1;
    rectwidth = y2-y1+1;

    if(rectheight < 2*nsize && rectwidth < 2*nsize) return hoodindex;           //  if both sides are too small to be split, return.
    if(rectheight>=2*nsize && rectwidth>=2*nsize) vsplitflag = rand()%2;    //  if both sides are big enough to split, choose between them randomly.
    else if(rectheight>=2*nsize) vsplitflag = 1;                            //  if only rectheight is big enough to be split, choose that.

    if(vsplitflag){

        //  first I need to find the bounds.
        a = x1+nsize;
        b = x2-nsize+1;

        //  choose a random number between a and b.
        p = rand()%(b-a+1)+a;

        //  fill the chosen rectangle with the current hoodindex.
        for (i = x1; i < p; i++)
            for (j = y1; j<=y2; j++)
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
        for (i = x1; i<=x2; i++)
            for (j = y1; j < p; j++)
                *(nhood+i*width+j) = hoodindex;

        //  call hoodmaker with each new separate rectangle.
        hoodindex = hoodmaker(nhood, x1, y1, x2, p-1, height, width, nsize, hoodindex+1);
        hoodindex = hoodmaker(nhood, x1, p, x2, y2, height, width, nsize, hoodindex);
    }

    return hoodindex;
}



void ushow(unsigned * map, unsigned height, unsigned width){
    unsigned i, j;

    for (i = 0; i < height; i++){
        for (j = 0; j < width; j++) printf("%3u", *(map+i*width+j));
        putchar('\n');
    }
}



unsigned filereader(char *fname, char ***array){
    unsigned n, i, len;
    char c, buffer[50];
    FILE *fp;

    if((fp=fopen(fname,"r")) == NULL){ printf("\ncan't open \'%s\'.", fname); exit(1); }        //  open file in text mode.

    n=i=0;                                                                                      //  counts lines in file. file has to end with a newline!

    while((c=fgetc(fp)) != EOF) if(c == '\n') n++;                                                  //  add something later to correct files if necessary.
    if(!n) { printf("\n\'%s\' is empty.", fname); exit(1); }                                    //  if empty file, exit.
    rewind(fp);

    if((*array = malloc(n*sizeof(char*))) == NULL){ puts("\nbutt: malloc failed."); exit(1); }  //  malloc an array of n char pointers.

    while(EOF != fscanf(fp, "%s\n", buffer)){                                                   //  read str into buffer, malloc for it, copy it over, repeat.
        len = strlen(buffer)+1;
        if((*(*(array)+i) = malloc(len)) == NULL){ puts("\npoop: malloc failed."); exit(1); }   //  make space for this string.
        strcpy(*(*(array)+i), buffer);                                                          //  move the string I read into the space I just made.
        strcat(*(*(array)+i++), "");                                                            //  this should null terminate the string.
    }

    fclose(fp);
    return n;
}


void proper(char *str){
    if(str == NULL) return;
    *str = toupper(*str);
}


void unproper(char *str){
    if(str == NULL) return;
    *str = tolower(*str);
}


void display(unsigned r, unsigned c, char *map, unsigned pos){
    unsigned i, size=r*c;

    putchar('\n');
    for (i = 0; i < size; i++){
        if(!(i%c)) putchar(' ');                    //  puts a space on the left edge, easier to read map.
        if(i == pos) putchar('@');                    //  player's character.
        else if(*(map+i) == BLOCK) putchar(178);      //  filled block.
        else if(*(map+i) == STREET) putchar(' ');     //  street.
        else if(*(map+i) == SHOP) putchar('$');
        else if(*(map+i) == DOOR) putchar('D');
        if(i%c+1 == c) putchar('\n');
    }
}


unsigned weatherreport(weather *last, double heat, double humidity, double wind, double cloud, clocky *c){

    //printf("\n\nheat: %.3fF\thumidity: %.3f%%\twind: %.3f\tcloud: %.3f\ttime: %f", heat, humidity, wind, cloud, time);
    unsigned time;      //  just the hour from the clock.

    time = c->h;

    //  time of day.
    if(last->firsttime){
        last->firsttime=0;

        //  time of day.
        if(time < 12.0){        printf("it is morning. ");  last->tod = 0; }
        else if(time < 17.0){   printf("it is afternoon. ");last->tod = 1; }
        else if(time < 20.0){   printf("it is evening. ");  last->tod = 2; }
        else{                   printf("it is night. ");    last->tod = 3; }

        //  temp.
        if(heat < 32.0){        printf("freezing. ");       last->temp = 0; }
        else if(heat < 44.0){   printf("cold. ");           last->temp = 1; }
        else if(heat < 57.0){   printf("chilly. ");         last->temp = 2; }
        else if(heat < 74.0){   printf("mild. ");           last->temp = 3; }
        else if(heat < 83.0){   printf("warm. ");           last->temp = 4; }
        else if(heat < 94.0){   printf("very warm. ");      last->temp = 5; }
        else if(heat < 99.0){   printf("hot. ");            last->temp = 6; }
        else{                   printf("very hot. ");       last->temp = 7; }

        //  humidity.
        if(humidity < 40.0){        printf("dry. ");        last->humidity = 0; }
        else if(humidity < 60.0){   printf("");             last->humidity = 1; }
        else if(humidity < 75.0){   printf("humid. ");      last->humidity = 2; }
        else if(humidity < 90.0){   printf("very humid. "); last->humidity = 3; }
        else{                       printf("rain. ");       last->humidity = 4; }

        //  sunniness.
        if(cloud < 49.0 && time>7.0 && time < 16.0){ printf("sunny. "); last->sun = 0; }
        else last->sun = 1;

        //  cloudiness.
        if(cloud < 16.0){       printf("clear skies. ");       last->cloud = 0; }
        else if(cloud < 33.0){  printf("a little cloudy. ");   last->cloud = 1; }
        else if(cloud < 49.0){  printf("partly cloudy. ");     last->cloud = 2; }
        else if(cloud < 65.0){  /*printf("cloudy. ");*/        last->cloud = 3; }
        else if(cloud < 81.0){  printf("very cloudy. ");       last->cloud = 4; }
        else{                   printf("overcast. ");          last->cloud = 5; }

        //  wind.
        if(wind < 1.1){       printf("calm.");                last->wind = 0; }
        else if(wind < 5.5){  printf("light air.");           last->wind = 1; }
        else if(wind < 11.9){ printf("light breeze.");        last->wind = 2; }
        else if(wind < 19.7){ printf("gentle breeze.");       last->wind = 3; }
        else if(wind < 28.7){ printf("moderate breeze.");     last->wind = 4; }
        else if(wind < 38.8){ printf("fresh breeze.");        last->wind = 5; }
        else if(wind < 49.9){ printf("strong breeze.");       last->wind = 6; }
        else if(wind < 61.8){ printf("high wind.");           last->wind = 7; }
        else if(wind < 74.6){ printf("gale.");                last->wind = 8; }
        else if(wind < 88.1){ printf("strong/severe gale.");  last->wind = 9; }
    }

    else{
        if(time < 12.0 && last->tod != 0){                      printf("it is morning. ");    last->tod = 0; }
        else if(time >= 12.0 && time < 17.0 && last->tod != 1){ printf("it is afternoon. ");  last->tod = 1; }
        else if(time >= 17.0 && time < 20.0 && last->tod != 2){ printf("it is evening. ");    last->tod = 2; }
        else if(time >= 20.0 && last->tod != 3){                printf("it is night. ");      last->tod = 3; }

        //  temp.
        if(heat < 32.0 && last->temp != 0){                    printf("freezing. ");   last->temp = 0; }
        else if(heat>=32.0 && heat < 44.0 && last->temp != 1){ printf("cold. ");       last->temp = 1; }
        else if(heat>=44.0 && heat < 57.0 && last->temp != 2){ printf("chilly. ");     last->temp = 2; }
        else if(heat>=57.0 && heat < 74.0 && last->temp != 3){ printf("mild. ");       last->temp = 3; }
        else if(heat>=74.0 && heat < 83.0 && last->temp != 4){ printf("warm. ");       last->temp = 4; }
        else if(heat>=83.0 && heat < 94.0 && last->temp != 5){ printf("very warm. ");  last->temp = 5; }
        else if(heat>=94.0 && heat < 99.0 && last->temp != 6){ printf("hot. ");        last->temp = 6; }
        else if(heat>=99.0 && last->temp != 7){                printf("very hot. ");   last->temp = 7; }

        //  humidity.
        if(humidity < 40.0 && last->humidity != 0){                         printf("dry. ");        last->humidity = 0; }
        else if(humidity>=40.0 && humidity < 60.0 && last->humidity != 1){  printf("");             last->humidity = 1; }
        else if(humidity>=60.0 && humidity < 75.0 && last->humidity != 2){  printf("humid. ");      last->humidity = 2; }
        else if(humidity>=75.0 && humidity < 90.0 && last->humidity != 3){  printf("very humid. "); last->humidity = 3; }
        else if(humidity>=90.0 && last->humidity != 4){                     printf("rain. ");       last->humidity = 4; }

        //  sunniness.
        if(cloud < 49.0 && time>7.0 && time < 16.0 && last->sun != 0){ printf("sunny. "); last->sun = 0; }
        else if(last->sun != 1) last->sun = 1;

        //  cloudiness.
        if(cloud < 16.0 && last->cloud != 0){       printf("clear skies. ");        last->cloud = 0; }
        else if(cloud < 33.0 && last->cloud != 1){  printf("a little cloudy. ");    last->cloud = 1; }
        else if(cloud < 49.0 && last->cloud != 2){  printf("partly cloudy. ");      last->cloud = 2; }
        else if(cloud < 65.0 && last->cloud != 3){  printf("cloudy. ");             last->cloud = 3; }
        else if(cloud < 81.0 && last->cloud != 4){  printf("very cloudy. ");        last->cloud = 4; }
        else if(last->cloud != 5){                  printf("overcast. ");           last->cloud = 5; }

        //  wind.
        if(wind < 1.1 && last->wind != 0){          printf("calm.");                last->wind = 0; }
        else if(wind < 5.5 && last->wind != 1){     /*printf("light air.");*/       last->wind = 1; }
        else if(wind < 11.9 && last->wind != 2){    /*printf("light breeze.");*/    last->wind = 2; }
        else if(wind < 19.7 && last->wind != 3){    printf("gentle breeze.");       last->wind = 3; }
        else if(wind < 28.7 && last->wind != 4){    /*printf("moderate breeze.");*/ last->wind = 4; }
        else if(wind < 38.8 && last->wind != 5){    /*printf("fresh breeze.");*/    last->wind = 5; }
        else if(wind < 49.9 && last->wind != 6){    printf("strong breeze.");       last->wind = 6; }
        else if(wind < 61.8 && last->wind != 7){    printf("high wind.");           last->wind = 7; }
        else if(wind < 74.6 && last->wind != 8){    printf("gale.");                last->wind = 8; }
        else if(wind < 88.1 && last->wind != 9){    printf("strong/severe gale.");  last->wind = 9; }
    }
}


/*
    Takes an array, indexes of its bounds, level, and min and max values that determine the range of values,
    and puts values into the array using the mid-displacement routine.  On first call, level should be 1.
    e.g. noise(array1, 0, 16, 1, -5.5, 15.0)
    The size of the array MUST be 2^n+1.
*/
void noise(double *array, unsigned start, unsigned end, unsigned level, double min, double max){
    double avg, dis;
    unsigned mid;
    if(end == start+1) return;

    if(min>max) { printf("\nnoise(): min is greater than max. array not complete."); return; }

    avg = (*(array+start)+*(array+end))/2.0;
    mid = (start+end)/2;
    dis = (double)rand() / (double)RAND_MAX * (max-min) + min;
    dis /= (double)(1<<level);

    *(array+mid) = avg+dis;

    noise(array, start, mid, level+1, min, max);
    noise(array, mid, end, level+1, min, max);
}


/*
given an array, fills array with sine wave starting at min, going to max, and back to min again.
I'm using it for daily temperatures. requires math.h and possible a redeclaration of M_PI.
e.g. sinefiller(array1, <size of array>, 40.8, 60.8);
*/
void sinefiller(double *array, unsigned size, double min, double max){
    unsigned i;
    //for (i = 0; i < size; i++) array[i] = (double)(max-min)*sin((double)i/(size-1)*M_PI)+min;
    for (i = 0; i < size; i++) array[i] = sin(M_PI*((double)i/(double)size)) * sin(M_PI*(double)i/((double)(size-1))) * ((double)max-(double)min) + (double)min;
}


/*
The dialog for the given shop.  Shows the shop's introduction,
and inventory for sale.  Maybe other menu to sell to the shop.
probably also deals with the transactions by user.
*/
void insideshop(shop *shopx){
    item_record *scout;
    unsigned total_items;   //  if this is zero after traversing the list, say 'sold out'

    if(shopx == NULL) {
        //  display null shop testing dialog.
        puts("\nYou are in a NULL shop.  You're probably testing. :)\n");
        puts("BILL THE BUTCHER");
        puts("   fine meats   ");
        puts("----------------");
        puts("BUY     |      $");
        puts("beef    |   4.56");
        puts("tails   |   1.23");
        puts("livers  |  14.30");
        puts("bone    | 100.06");
        puts("blood   |   0.07");

        puts("\nwhat'll it be, sir?");
        puts("don't respond. not ready yet. get out, sir.");
    }

    else{
        printf("\n\nSHOP at %u", shopx->loc);
        //printf("\nspace: %u", shopx->space);
        //printf("\nmoney: %.2f", shopx->money);

        puts("\n\nINVENTORY");
        puts("---------");
        puts("item\t\tquantity\tprice\tsize");
        //  display this shop's inventory list.
        scout = shopx->inventory;
        total_items = 0;
        while(scout){
            if(scout->quantity != 0) {
                printf("\n{item %u}\t%u\t\t%.2f\t%u", scout->id, scout->quantity, scout->price, scout->size);
                total_items++;
            }
            scout = scout->next;
        }
        if(total_items == 0) printf("\n{empty: sold out}");
        else printf("\n\n%u items total.", total_items);
        printf("\nspace: %u|%u\tmoney:%.2f\n", shopx->used_space, shopx->space, shopx->money);
    }
}

/*  This is a function that uses mapsquash,
    which I haven't committed to yet.   */
void look(cell *map, unsigned loc){
    //if(map[loc]->l == SHOP) printf("\nThere is a shop to the west.");
    //if(map[loc]->r == SHOP) printf("\nThere is a shop to the east.");
    //if(map[loc]->u == SHOP) printf("\nThere is a shop to the north.");
    //if(map[loc]->d == SHOP) printf("\nThere is a shop to the south.");
}

/*  Puts the item n into the shop's inventory linked list. this doesn't
    only add one item: item n has a quantity number to go with it. This
    is currently used when the shop buys stuff from the supplier, but it
    could be used for transactions with players I suppose.  */
int stock_these(item_record **root, item_record **curr, item_record *n){
    item_record *poop;

    if((poop = malloc(sizeof(item_record))) == NULL) return 0;  //  make space for new data. return 0 on fail.
    if(*root == NULL) *curr = *root = poop;                     //  if root NULL, data will go there.
    else *curr = (*curr)->next = poop;                          //  otherwise, data will go in next spot.
    **curr = *n;                                                //  copy data into new spot.
    (*curr)->next = NULL;                                       //  NULL the list.
    return 1;
}






