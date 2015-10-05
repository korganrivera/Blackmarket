/*
    weather simulation using sine waves and noise
    to feed a function that reports the weather.

    start:  2015.10.3.10:41 I need to have some kind of normal distribution of weather things, so that it doesn't
                            casually become monsoon season as easily as a normal day. But meh, it works okay for now.

    end:    2015.10.4.20:33 Been tweaking this all day.  I don't really like it but it works.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323
#endif
#define LEN 33          //  must be 2^n+1. This is the width of the graphs I generate.
#define HEATMIN 30.0    //  52.0.
#define HEATMAX 75.0    //  90.0.
#define DEBUG 0

//  this is used to record what the last weather report was, to avoid reporting the same thing each time.
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


unsigned weatherreport(weather *last, double heat, double humidity, double wind, double cloud, double time);
void noise(double *array, unsigned start, unsigned end, unsigned level, double min, double max);
void sinefiller(double *array, unsigned size, double min, double max);

int main(void){
    double heat[LEN];
    double humidity[LEN];
    double wind[LEN];
    double cloud[LEN];
    unsigned i;
    weather lastcheck;

    srand(time(NULL));
    lastcheck.firsttime=1;                          //  used to check if weatherreport() has been called before or not.
    sinefiller(heat, LEN, HEATMIN, HEATMAX);        //  set heat values between HEATMIN and HEATMAX.

    humidity[0] = humidity[LEN-1] = 50.0;           //  set outer limits of arrays to be noised.
    wind[0]     = wind[LEN-1]     = 0.0;
    cloud[0]    = cloud[LEN-1]    = 30.0;
    //wind[0] = wind[LEN-1] = (double)rand() / (double)RAND_MAX * 20.0;

    noise(humidity, 0, LEN-1, 1, 0.0, 100.0);       //  set humidity values between 0-100%.
    noise(wind,     0, LEN-1, 1, 0.0, 88.1);        //  set wind values between 0-88.1, using the Beufort scale.
    noise(cloud,    0, LEN-1, 1, 0.0, 81.0);       //  set cloud values between 0-100%.

if(DEBUG){
    //  display to check.
    puts("\nheat check:");
    for(i=0; i<LEN; i++) printf("%u,%.3f\n", i, heat[i]);
    puts("\npress key...");
    getche();

    //  display to check.
    puts("\nhumidity check:");
    for(i=0; i<LEN; i++) printf("%u,%.3f\n", i, humidity[i]);
    puts("\npress key...");
    getche();

    //  display to check.
    puts("\nwind check:");
    for(i=0; i<LEN; i++) printf("%u,%.3f\n", i, wind[i]);
    puts("\npress key...");
    getche();
}

    for(i=0; i<LEN; i++){
        weatherreport(&lastcheck, heat[i], humidity[i], wind[i], cloud[i], (double)i/(double)LEN*24.0);
    }
}


unsigned weatherreport(weather *last, double heat, double humidity, double wind, double cloud, double time){

    //printf("\n\nheat: %.3fF\thumidity: %.3f%%\twind: %.3f\tcloud: %.3f\ttime: %f", heat, humidity, wind, cloud, time);

    printf("\nit is about %.0f:00. ", time);

    //  time of day.
    if(last->firsttime){
        last->firsttime=0;

        //  time of day.
        if(time<12.0){      printf("it is morning. ");   last->tod = 0; }
        else if(time<17.0){ printf("it is afternoon. "); last->tod = 1; }
        else if(time<20.0){ printf("it is evening. ");   last->tod = 2; }
        else{               printf("it is night. ");     last->tod = 3; }

        //  temp.
        if(heat<32.0){      printf("freezing. ");   last->temp = 0; }
        else if(heat<44.0){ printf("cold. ");       last->temp = 1; }
        else if(heat<57.0){ printf("chilly. ");     last->temp = 2; }
        else if(heat<74.0){ printf("mild. ");       last->temp = 3; }
        else if(heat<83.0){ printf("warm. ");       last->temp = 4; }
        else if(heat<94.0){ printf("very warm. ");  last->temp = 5; }
        else if(heat<99.0){ printf("hot. ");        last->temp = 6; }
        else{               printf("very hot. ");   last->temp = 7; }

        //  humidity.
        if(humidity<40.0){       printf("dry. ");       last->humidity = 0; }
        else if(humidity<60.0){  printf("");            last->humidity = 1; }
        else if(humidity<75.0){  printf("humid. ");     last->humidity = 2; }
        else if(humidity<90.0){  printf("very humid. ");last->humidity = 3; }
        else{                    printf("rain. ");      last->humidity = 4; }

        //  sunniness.
        if(cloud<49.0 && time>7.0 && time<16.0){ printf("sunny. "); last->sun = 0; }
        else last->sun = 1;

        //  cloudiness.
        if(cloud<16.0){      printf("clear skies. ");       last->cloud = 0; }
        else if(cloud<33.0){ printf("a little cloudy. ");   last->cloud = 1; }
        else if(cloud<49.0){ printf("partly cloudy. ");     last->cloud = 2; }
        else if(cloud<65.0){ /*printf("cloudy. ");*/        last->cloud = 3; }
        else if(cloud<81.0){ printf("very cloudy. ");       last->cloud = 4; }
        else{                printf("overcast. ");          last->cloud = 5; }

        if(wind<1.1){       printf("calm.");                last->wind = 0; }
        else if(wind<5.5){  printf("light air.");           last->wind = 1; }
        else if(wind<11.9){ printf("light breeze.");        last->wind = 2; }
        else if(wind<19.7){ printf("gentle breeze.");       last->wind = 3; }
        else if(wind<28.7){ printf("moderate breeze.");     last->wind = 4; }
        else if(wind<38.8){ printf("fresh breeze.");        last->wind = 5; }
        else if(wind<49.9){ printf("strong breeze.");       last->wind = 6; }
        else if(wind<61.8){ printf("high wind.");           last->wind = 7; }
        else if(wind<74.6){ printf("gale.");                last->wind = 8; }
        else if(wind<88.1){ printf("strong/severe gale.");  last->wind = 9; }
    }

    else{
        if(time<12.0 && last->tod != 0){                      printf("it is morning. ");    last->tod = 0; }
        else if(time >= 12.0 && time<17.0 && last->tod != 1){ printf("it is afternoon. ");  last->tod = 1; }
        else if(time >= 17.0 && time<20.0 && last->tod != 2){ printf("it is evening. ");    last->tod = 2; }
        else if(time >= 20.0 && last->tod != 3){              printf("it is night. ");      last->tod = 3; }

        //  temp.
        if(heat<32.0 && last->temp != 0){                    printf("freezing. ");   last->temp = 0; }
        else if(heat>=32.0 && heat<44.0 && last->temp != 1){ printf("cold. ");       last->temp = 1; }
        else if(heat>=44.0 && heat<57.0 && last->temp != 2){ printf("chilly. ");     last->temp = 2; }
        else if(heat>=57.0 && heat<74.0 && last->temp != 3){ printf("mild. ");       last->temp = 3; }
        else if(heat>=74.0 && heat<83.0 && last->temp != 4){ printf("warm. ");       last->temp = 4; }
        else if(heat>=83.0 && heat<94.0 && last->temp != 5){ printf("very warm. ");  last->temp = 5; }
        else if(heat>=94.0 && heat<99.0 && last->temp != 6){ printf("hot. ");        last->temp = 6; }
        else if(heat>=99.0 && last->temp != 7){              printf("very hot. ");   last->temp = 7; }

        //  humidity.
        if(humidity<40.0 && last->humidity != 0){                         printf("dry. ");        last->humidity = 0; }
        else if(humidity>=40.0 && humidity<60.0 && last->humidity != 1){  printf("");             last->humidity = 1; }
        else if(humidity>=60.0 && humidity<75.0 && last->humidity != 2){  printf("humid. ");      last->humidity = 2; }
        else if(humidity>=75.0 && humidity<90.0 && last->humidity != 3){  printf("very humid. "); last->humidity = 3; }
        else if(humidity>=90.0 && last->humidity != 4){                   printf("rain. ");       last->humidity = 4; }

        //  sunniness.
        if(cloud<49.0 && time>7.0 && time<16.0 && last->sun != 0){ printf("sunny. "); last->sun = 0; }
        else if(last->sun != 1) last->sun = 1;

        //  cloudiness.
        if(cloud<16.0 && last->cloud != 0){      printf("clear skies. ");       last->cloud = 0; }
        else if(cloud<33.0 && last->cloud != 1){ printf("a little cloudy. ");   last->cloud = 1; }
        else if(cloud<49.0 && last->cloud != 2){ printf("partly cloudy. ");     last->cloud = 2; }
        else if(cloud<65.0 && last->cloud != 3){ printf("cloudy. ");            last->cloud = 3; }
        else if(cloud<81.0 && last->cloud != 4){ printf("very cloudy. ");       last->cloud = 4; }
        else if(last->cloud != 5){               printf("overcast. ");          last->cloud = 5; }

        //  wind.
        if(wind<1.1 && last->wind != 0){       printf("calm.");                last->wind = 0; }
        else if(wind<5.5 && last->wind != 1){  /*printf("light air.");*/           last->wind = 1; }
        else if(wind<11.9 && last->wind != 2){ /*printf("light breeze.");*/        last->wind = 2; }
        else if(wind<19.7 && last->wind != 3){ printf("gentle breeze.");       last->wind = 3; }
        else if(wind<28.7 && last->wind != 4){ /*printf("moderate breeze.");*/     last->wind = 4; }
        else if(wind<38.8 && last->wind != 5){ /*printf("fresh breeze.");*/        last->wind = 5; }
        else if(wind<49.9 && last->wind != 6){ printf("strong breeze.");       last->wind = 6; }
        else if(wind<61.8 && last->wind != 7){ printf("high wind.");           last->wind = 7; }
        else if(wind<74.6 && last->wind != 8){ printf("gale.");                last->wind = 8; }
        else if(wind<88.1 && last->wind != 9){ printf("strong/severe gale.");  last->wind = 9; }
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
