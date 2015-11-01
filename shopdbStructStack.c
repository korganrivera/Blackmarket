/*
    I thought of a way to stack some struct arrays, to make a population
    of shops. make a struct that holds a record of a sale, at a price, at a specific
    time. an array of these is a sales history. then, make a struct that holds info
    about an item: name, quantity, price, etc. put the first struct array inside
    this one, and now an item contains its own sales history. make an array of this
    second struct, and now I have an inventory database. make a third struct that
    describes the attributes of a shop: name, location, open/closed, opening hours,
    etc. Put the 2nd struct array inside this and now a shop contains its own
    inventory. make an array of those, and now I have a database of shops. :) So
    let's try doing that.
*/

struct sales_record {
    double price;
    unsigned sales;
    unsigned time;                          //  not sure what type yet.
};

struct item_record {
    char *name;
    unsigned index;
    unsigned quantity;
    double price;
    double m,n,b;                           //  the shop's estimate of the demand function for this item.
    struct sales_record *sales_history;     //  This will be the head of a linked list probably.
};

struct _shop {
    char *name;
    unsigned index;
    char *desc;
    char open;
    struct item_record *inventory;          //  the shop's inventory.
                                            //  also somehow have opening/closing hours for the store here.
};

int main(void){

    struct _shop shop[100];



}
