#include <stdio.h>
#include <stdlib.h>

void func(unsigned **n);

int main(void){
    unsigned *n;

    func(&n);
    printf("\nmain(): n[0] = %u", *(n));        //  crash.
}

void func(unsigned **n){
    *n = malloc(8*sizeof(unsigned));
    **(n) = 4;
    printf("\nfunc(): n[0] = %u", **(n));
}
