/*
    I'm going to try to use my linearregressor stuff with the noisy curve thing, that
    represents customer demand moving around.  I wish I could easily graph both to see
    how well it's fitting.

    start: 2015.9.10.11:42

*/

#include <stdio.h>
#include <stdlib.h>

#define NUM_POINTS 10
#define NUM_COMMANDS 2
#define ROWS 10  //  make sure rows is >= COLS.
#define COLS 3

int main(void){
    unsigned i;
    char * commandsForGnuplot[] = {"set title \"PLOT TEST\"", "plot 'data.temp'"};
    double matrix[ROWS][COLS]={
        {8.0, 0.0, 100.4},
        {12.0, 1.0, 104.01},
        {18.0, 2.0, 97.49},
        {20.0, 3.0, 87.41},
        {4.0, 4, 97.63},
        {10.0, 5, 88.41},
        {9.0, 6, 96.41},
        {12.0, 7, 102.17},
        {3.0, 8, 116.29},
        {14.0, 9, 97.32}
    };

    FILE *temp = fopen("data.temp", "w");
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");

    for (i=0; i < NUM_POINTS; i++){
        //fprintf(temp, "%lf %lf %lf\n", matrix[i][0], matrix[i][1], matrix[i][2]); //Write the data to a temporary file
        fprintf(temp, "%lf %lf\n", matrix[i][1], matrix[i][2]); //Write the data to a temporary file
    }




    fprintf(gnuplotPipe, "set title \"PLOT TEST\""); //Send commands to gnuplot one by one.

    fprintf(gnuplotPipe, "plot 'data.temp' \n"); //Send commands to gnuplot one by one.
    fflush(gnuplotPipe);

    puts("\npress key to continue.");
    getch();

    for (i=0; i < NUM_POINTS; i++){
        matrix[i][0] += 10;
        matrix[i][1] += 10;
        matrix[i][2] += 10;
    }

    fprintf(gnuplotPipe, "plot 'data.temp' \n"); //Send commands to gnuplot one by one.
    fflush(gnuplotPipe);
}
