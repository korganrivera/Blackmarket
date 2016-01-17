//testing parameter variables to see how they behave.

unsigned funca(unsigned a);
unsigned funcb(unsigned b);

int main(void){
    unsigned c=8;
    funca(c);
    funcb(c);
    printf("\nmain has vale %u", c);
}


unsigned funca(unsigned a){

    printf("\nfunca has value %u", a);
    a++;
    printf("\nafter ++, funca has value %u", a);
    funcb(a);
    printf("\nback at funca, I have the value %u", a);
}

unsigned funcb(unsigned b){

    printf("\nfuncb has value %u", b);
    b++;
    printf("\nafter ++, funcb has value %u", b);
}

