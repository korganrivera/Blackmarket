/*
    weather simulation using sine waves and noise
    to feed a function that reports the weather.

    start: 2015.10.3.10:41

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323
#endif
#define LEN 33      //  must be 2^n+1.
#define HEATMAX 60.8
#define HEATMIN 70.8

unsigned weatherreport(double heat, double humidity, double wind, double time);
void noise(double *array, unsigned start, unsigned end, unsigned level, double min, double max);
void sinefiller(double *array, unsigned size, double min, double max);

int main(void){
    double heat[LEN];
    double humidity[LEN];
    double wind[LEN];
    unsigned i;

    sinefiller(heat, LEN, HEATMIN, HEATMAX);        //  heat values between MIN and MAX.
    noise(humidity, 0, LEN-1, 1, 0.0, 100.0);   //  humidity values between 0-100%
    noise(wind, 0, LEN-1, 1, 0.0, 88.1);      //  wind values between 0-88.1, using the Beufort scale.

    for(i=0; i<LEN; i++){
        weatherreport(heat[i], humidity[i], wind[i], (double)i/(double)LEN*24.0);
    }
}


unsigned weatherreport(double heat, double humidity, double wind, double time){

    printf("\n\nreceived: %f, %f, %f, %f");


    printf("\nit is %f", time);




    if(time<12.0) printf("\nit is morning.");
    else if(time<17.0) printf("\nit is afternoon.");
    else if(time<20.0) printf("\nit is evening.");
    else printf("\nit is night.");

    if(humidity<(100.0/3.0) && heat<52.0) printf("\ncold and dry.");
    else if(humidity<(200.0/3.0) && heat<52.0) printf("\ncold.");
    else if(heat<52) printf("\ncold and wet. rain.");

    if(humidity<(100.0/3.0) && heat<78.0) printf("\nwarm and dry.");
    else if(humidity<(200.0/3.0) && heat<52.0) printf("\nnice warm day.");
    else if(heat<52) printf("\nwarm and wet. rain.");

    if(humidity<(100.0/3.0)) printf("\nhot and dry.");
    else if(humidity<(200.0/3.0)) printf("\nhot.");
    else if(heat<52) printf("\nhot and wet. very humid. gross rain.");

    if(wind<1.1) printf("\ncalm.");
    else if(wind<5.5) printf("\nlight air.");
    else if(wind<11.9) printf("\nlight breeze.");
    else if(wind<19.7) printf("\ngentle breeze.");
    else if(wind<28.7) printf("\nmoderate breeze.");
    else if(wind<38.8) printf("\nfresh breeze.");
    else if(wind<49.9) printf("\nstrong breeze.");
    else if(wind<61.8) printf("\nhigh wind.");
    else if(wind<74.6) printf("\ngale.");
    else if(wind<88.1) printf("\nstrong/severe gale.");

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

    if(min>max) {printf("\nnoise(): min is greater than max. array not complete."); return; }

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
    //for(i=0; i<size; i++) array[i] = (double)(max-min)*sin((double)i/(size-1)*M_PI)+min;
    for(i=0; i<size; i++) array[i] = sin(M_PI*((double)i/(double)size)) * sin(M_PI*(double)i/((double)(size-1))) * ((double)max-(double)min) + (double)min;
}
