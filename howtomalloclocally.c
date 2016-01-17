/*
    Trying to figure out how to do a pass a pointer by reference.
    Figured it out, keep this file for reference.  I need to do
    a triple pointer in my next problem.
    finished:  2015.9.16.12:02
*/

#include <stdio.h>
#include <stdlib.h>

void func(unsigned **n);

int main(void){
    unsigned *n;

    func(&n);
    printf("\nmain(): n[0] = %u", *(n));
}

void func(unsigned **n){
    *n = malloc(8*sizeof(unsigned));
    **(n) = 4;
    printf("\nfunc(): n[0] = %u", **(n));
}
