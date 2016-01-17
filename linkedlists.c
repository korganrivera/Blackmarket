/*
    Some linked-list stuff for shop inventories.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    char c;
    struct node *next;
} node;

int add(node **root, node **curr, node *n);


int main(void){
    node *root, *curr, *scout, n;
    root = curr = NULL;

    n.next   = NULL;

    n.c      = 'x';
    add(&root, &curr, &n);
    n.c      = 'o';
    add(&root, &curr, &n);
    n.c      = 'r';
    add(&root, &curr, &n);

    scout = root;
        while(scout){
            printf("%c ", scout->c);
            scout = scout->next;
        }


}


int add(node **root, node **curr, node *n){
    node *poop;
    if((poop = malloc(sizeof(node))) == NULL) return 0; //  make space for new data. return 0 on fail.
    if(*root == NULL) *curr = *root = poop;             //  if root NULL, data will go there.
    else *curr = (*curr)->next = poop;                  //  otherwise, data will go in next spot.
    **curr = *n;                                        //  copy data into new spot.
    (*curr)->next = NULL;                               //  NULL the list.
    return 1;
}
