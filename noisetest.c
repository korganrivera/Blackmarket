/*
    Before I generate weather in my game, I need to write a few tools.
    I need to generate noise graphs.
    start:  2015.10.1.20:54
    end:    2015.10.1.22:19

    Should work well now.
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define LEN 33      //  must be 2^n + 1.
#define MIN 0.0
#define MAX 100.0

/*
    Takes an array, index of its bounds, level, and min and max values that determine the range of values,
    and puts values into the array using the mid-displacement routine.  On first call, level should be 1.
    e.g. noise(array1, 0, 16, 1, -5.5, 15.0)
    The size of the array MUST be 2^n+1.
*/
void noise(double *array, unsigned start, unsigned end, unsigned level, double min, double max){
    double avg, dis;
    unsigned mid;
    if(end == start+1) return;

    if(min>max) {printf("\nnoise(): min is greater than max. array not complete."); return; }

    avg = (*(array+start)+*(array+end))/2.0;
    mid = (start+end)/2;
    dis = (double)rand() / (double)RAND_MAX * (max-min) + min;
    dis /= (double)(1<<level);

    *(array+mid) = avg+dis;

    noise(array, start, mid, level+1, min, max);
    noise(array, mid, end, level+1, min, max);
}


int main(void){
    double array[LEN];
    unsigned i;

    srand(time(NULL));

    //  just in case.  this avoids a death.
    if(MIN>MAX) { puts("\nmain(): min is greater than max.  exiting. "); exit(1); }

    //  set the outer limits! if bounds are the same, I could loop the graph if I wanted.
    array[0] = array[LEN-1] = (double)rand() / (double)RAND_MAX * (MAX - MIN) + MIN;
    //array[0] = MIN;
    //array[LEN-1] = MAX;
    //array[0] = (double)rand() / (double)RAND_MAX * (MAX - MIN) + MIN;
    //array[LEN-1] = (double)rand() / (double)RAND_MAX * (MAX - MIN) + MIN;

    noise(array, 0, LEN-1, 1, MIN, MAX);

    //  display to check.
    for(i=0; i<LEN; i++) printf("%u,%.3f\n", i, array[i]);
}
