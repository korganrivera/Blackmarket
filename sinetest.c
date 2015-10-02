/*
    generate a sine wave to model heat over the course of a day, or whatever.
    give it the min and max value, and an array of some size, and
    it'll fill in the values.
    start:  2015.10.1.23:03
    end:    2015.10.2.00:47
*/
#include <stdio.h>
#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323
#endif
#define LEN 24
#define MAX 60.8
#define MIN 70.8

/*
given an array, fills array with sine wave starting at min, going to max, and back to min again.
I'm using it for daily temperatures. requires math.h and possible a redeclaration of M_PI.
e.g. sinefiller(array1, <size of array>, 40.8, 60.8);
*/
void sinefiller(double *array, unsigned size, double min, double max){
    unsigned i;
    //for(i=0; i<size; i++) array[i] = (double)(max-min)*sin((double)i/(size-1)*M_PI)+min;
    for(i=0; i<size; i++) array[i] = sin(M_PI*((double)i/(double)size)) * sin(M_PI*(double)i/((double)(size-1))) * ((double)max-(double)min) + (double)min;
}

int main(void){

    double array[LEN];
    unsigned i;

    sinefiller(array, LEN, MIN, MAX);
    for(i=0; i<LEN; i++) printf("\n%u,%.4f", i, array[i]);
}
