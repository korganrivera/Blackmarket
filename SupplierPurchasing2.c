/*
    Much simpler way to buy the most profit-dense items
    from the supplier.

    finished 2015.10.30.01:58 (after about 10 hours on Vyvanse. O_O)
    10 hours, 150 lines of code.  Insane.
*/

#include <stdio.h>
#include <time.h>       //  rand()
#include <stdlib.h>     //  rand()

#define N       50      //  number of supplier items.
#define CASH    1000.0  //  cash you have to start with.
#define SPACE   1000    //  space you have to start with.

#define SHOWLIST 1         //  set to non-zero to print table of items.

typedef struct _item {
    unsigned index;     //  number to identify the item.
    double m, b;        //  line equation constants that determine the price I could sell the product for.
    unsigned size;      //  size/volume/weight of the item.
    double cost;      //  what the item costs to buy.
    unsigned quantity;  //  number of this item that the store has.
    unsigned bought;    //  quantity bought so far.
}item;

item items[N];

int main(void){
    unsigned i, j;
    unsigned best_item;
    unsigned remaining_space;
    double remaining_cash;
    unsigned possible_flag;                     //  indicates if I traversed the list and found no options. 0 if no options.
    double profit_density;                      //  profit/space of item.
    double best_profit_density;
    double profit_made;
    double spent;

    srand(time(NULL));

    //  make N random items.
    if(SHOWLIST){
        puts("item\tquantity\tsize\tcost\tm\tb\tp_d");
        puts("--------------------------------------------------------------");
    }

    for (i=0; i<N; i++){
        items[i].index = i;
        items[i].size       = rand() % 100 + 1;
        items[i].cost       = (float)rand() / (float)(RAND_MAX) * 100.0;
        items[i].m          = ((float)rand() / (float)(RAND_MAX) * (-2.5)) - 0.5; //  generates random number between -0.5 and -3.
        items[i].b          = rand() % 100;
        items[i].quantity   = rand() % 100;
        items[i].bought     = 0;

        //  display items.
        if(SHOWLIST){
            profit_density = (((1.0 - items[i].b) / items[i].m) - items[i].cost) / (float)items[i].size;
            printf("%u\t%u\t\t%u\t%.2f\t%.2f\t%.2f\t", i, items[i].quantity, items[i].size, items[i].cost, items[i].m, items[i].b);
            if(profit_density < 0) puts("-");
            else printf("%.2f\n", profit_density);
        }
    }

    remaining_cash  = CASH;
    remaining_space = SPACE;

    //  for each item, calculate the profit density of buying one more item.
    //  if it's the highest value, buy it.  repeat while there are
    //  opportunities to buy (items, space, cash, profit - available.)

    do{
        possible_flag       = 0;
        best_item           = N;
        best_profit_density = 0.0;

        for (i = 0; i < N; i++){

                //  check that this item exists. if not, skip it.
                if(items[i].quantity == 0) continue;

                //  check if I can even store this item. if not, skip it.
                if(items[i].size > remaining_space) continue;

                //  check if I can even afford this item.  This fails when you have no money and the item is free, so...
                if(items[i].cost > remaining_cash) continue;

                //  ... this line takes care of it.
                if(remaining_cash < 0.01) continue;

                //  calculate the profit density of one more item.
                profit_density = ((((1.0 + (float)items[i].bought) - items[i].b) / items[i].m) - items[i].cost) / (float)items[i].size;

                //  if item has negative profit density, skip it.
                if(profit_density <= 0.0) continue;

                //  if there's at least one item that exists, that I can afford and store, and that is profitable, then keep going next time.
                possible_flag = 1;

                //  if this profit/space ratio is greatest found so far, save it.
                if(profit_density > best_profit_density){
                    best_profit_density = profit_density;
                    best_item           = i;
                }
        }

        //  if something was found, buy it.
        if(best_item != N){
            remaining_cash  -= items[best_item].cost;
            remaining_space -= items[best_item].size;
            items[best_item].quantity--;
            items[best_item].bought++;
        }

    }while(possible_flag);

    profit_made = spent = 0.0;
    puts("\n\nhere is what to buy:");
    for(i=0; i<N; i++){
        if(items[i].bought){
            printf("\n%u of item %u", items[i].bought, i);
            profit_made += ((float)items[i].bought)*((float)items[i].bought - items[i].b) / items[i].m - (float)items[i].bought*items[i].cost;
            spent += (float)items[i].bought * items[i].cost;
        }
    }
    printf("\n\ntotal profit:\t%.2f.", profit_made);
    printf("\ntotal spent:\t%.2f.\n", spent);
}
