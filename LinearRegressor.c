/**
    I'm going to try to write a general purpose linear regression thing.
    You give the program some array of coordinates, and the program will use
    multiple regression to give you the best fitting line or plane or whatever. If
    you have 2-tuple coordinates, you need to give the program at least 2 pairs.
    3-tuples, you need at least 3 of them, and so on. If you have 3 3-tuples, you'll
    get a perfect fit. If you have more, you'll get a regression. In every case, the
    program will report the r^2. User's array is read from a csv file.

    pseudo: open file, check file formatting, check for correct
    number of rows and columns, count rows and columns, malloc
    space for arrays accordingly. read file into arrays. Do the
    operation. report results in correct form according to the
    n in n-tuple. report the r^2. done.

    2015.5.23.12:09 Got it working.  Need to add file reading functions, but the core of it works.  Pretty cool!
    2015.6.2.20:06  I lost the MyLinAlg.c file, and have to rewrite it.  So far there's something wrong with it. -_-
    2015.6.2.20:39  Okay fixed.  Actually ended up with a shorter function.  Not sure why the other one died.

    2015.6.6.12:16  Needs more testing.  Didn't give me correct answers on my last use.  Had a problem with a 10x2 matrix, transpose gave wrong answer and
                    r-squared came out negative @_@

    2015.10.6.11:46 Messing with this.  It's filled with shit.  Doesn't work for a bunch of given problems.  Do i want to get into this again?
                    Doesn't work with more than 3 columns.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MyLinAlg.c"

#define ROWS 9  //  make sure rows is >= COLS.
#define COLS 3

int main(int argc, char *argv[]){

    FILE *in;
    unsigned i;
    char c;
    double tempmatrix1[ROWS][1], tempmatrix2[ROWS][1];   //  used for calculating rsquared. 1d defined as 2d.  I know it's pointless. :)
    double d, f=0, average=0, rsquared;
    double b[COLS][1];                                   //  this is matrix b.  It will hold the solution in the end.
    double s[ROWS][1];                                   //  this is matrix s.  It will hold the number of sales.  it comes from the last vector in the user's matrix.
                                                        //  later on these arrays will be malloced.
    double xtrans[COLS][ROWS];                           //  transpose of matrix.
    double transmult[COLS][COLS];                        //  matrix times its transpose.
    double transmult2[COLS][ROWS];                       //  transmult times xtrans.

    double matrix[ROWS][COLS]={       //  this is the shit that the user gives me.  I'll reuse the space once I move some stuff around.  It will be matrix x after that.

        {6, 22.2, 12.54},
        {7, 23.2, 19.24},
        {7, 21.6, 14.27},
        {8, 22.9, 17.9},
        {9, 23.3, 21.09},
        {10, 25.6, 18.1},
        {10, 25.5, 19.03},
        {10, 24.7, 19.5},
        {14, 22.9, 27.3}
    };

    //  calculate average of actual y values for r-squared later.
    for(i=0; i<ROWS; i++) average += matrix[i][COLS-1];
    average /= (double)ROWS;

    puts("\nuser's matrix:");
    echoarray(ROWS,COLS,&matrix[0][0]);

    //  move right-most vector of user's matrix into the sales vector.
    for(i=0; i<ROWS; i++) s[i][0] = matrix[i][COLS-1];

    // make right-most vector of user's matrix all ones.
    for(i=0; i<ROWS; i++) matrix[i][COLS-1] = 1;

//    puts("\nmake right vector all ones:");
//    echoarray(ROWS,COLS,&matrix[0][0]);

    //  transpose x into xtrans.
    transpose(ROWS, COLS, &matrix[0][0],&xtrans[0][0]);

    puts("\ntranspose x:");
    echoarray(COLS,ROWS,&xtrans[0][0]);

    //  multiply the transpose 'xtrans' with x, put into 'transmult'.
    multiply(COLS,ROWS,ROWS,COLS,&xtrans[0][0],&matrix[0][0],&transmult[0][0]);

    puts("\nmultiply transpose with x:");
    echoarray(COLS,COLS,&transmult[0][0]);-

    //  invert 'transmult'.
    inverse(COLS, &transmult[0][0]);

    puts("\ninvert that:");
    echoarray(COLS,COLS,&transmult[0][0]);
    //  multiply 'transmult' with 'xtrans'. put into 'transmult2'.
    multiply(COLS,COLS,COLS,ROWS,&transmult[0][0],&xtrans[0][0],&transmult2[0][0]);

    puts("\nmultiply that with xtrans:");
    echoarray(COLS, ROWS, &transmult2[0][0]);

    //  then multiply that with 's'. put into 'B'.
    multiply(COLS,ROWS,ROWS,1,&transmult2[0][0],&s[0][0],&b[0][0]);

    //  output the solution vector.
    puts("\nsolution:");
    echoarray(COLS, 1, &b[0][0]);

    printf("\nThese are the coefficients for the linear model z=mx+ny+...+b");
    printf("\nThe numbers above are the coefficients m,n,...,b");
    printf("\nx is the 1st column of your matrix, y is the 2nd, etc.");

    //  time to calculate rsquared. using linear algebra for convenience.
    multiply(ROWS, COLS, COLS, 1, &matrix[0][0], &b[0][0],&tempmatrix1[0][0]);      //  multiply x with b to get predicted values.

    puts("\n\nHere are predicted values using the regression.\nThese values should be close to the values in your last column:");
    echoarray(ROWS, 1, &tempmatrix1[0][0]);

    scale(ROWS, -1.0, &tempmatrix1[0][0]);                                          //  multiply all values in tempmatrix1 by -1.
    sum(ROWS, &s[0][0], &tempmatrix1[0][0], &tempmatrix2[0][0]);                    //  subtract actual values from predicted values.
    d = dot(ROWS, &tempmatrix2[0][0], &tempmatrix2[0][0]);                          //  sum squares of elements in tempmatrix2. This is numerator for rsquared.

   printf("\ndot = %f", d);

    for(i=0; i<ROWS; i++) f += (s[i][0]-average) * (s[i][0]-average);               //  sum square of difference between actual s values and average s value.
                                                                                    //  this is denominator for rsquared.
    rsquared = d/f;
    rsquared = 1.0-rsquared;

    printf("\nr-squared: %.2f\n", rsquared);
}


/** This code is temporarily on hold until I get the more important stuff working.  This will be the stuff I need for when I'm reading matrices from files.

    //  confirm that correct number of arguments are given on command line.
    if(argc!=2){
        printf("\nUSAGE: %s <csv file>", argv[0]);
        printf("\nType \'%s help\' for more information.", argv[0]);
        exit(1);
    }

    //  give user more info if requested.
    if(!strcmp(argv[1],"help")){
        puts("\nGeneral purpose multiple regression calculator.");
        puts("Takes a csv file that contains an array of coords");
        puts("of any length and attempts to perform a multiple");
        puts("regression on the data. Make sure the last column");
        puts("of your array is the dependent variable, and all");
        puts("the rest are the dependent variables. Also make");
        puts("sure you have at least as many rows as you have");
        puts("columns. You can't fit a line to a single point,");
        puts("you can't fit a plane to only 2 points, etc.");
        exit(0);
    }

    //  open given file name.
    if((in = fopen(argv[1], "r")) == NULL){
        printf("\nCan't open file \'%s\'", argv[1]);
        exit(1);
    }

    //  check file only contains numbers, commas, periods, newlines, and whitespace exist in file. (can't deal with scientific notation as of yet, because I'm lazy.)
    //  do this part later.

    //  count the commas and newlines to get size of matrix.
    while((c=fgetc(in))!=EOF){
        if(c=='\n') newlines++;
        else if(c==',') commas++;
    }

    if(!newlines){ puts("\nYou have new newlines bro."); exit(1); }
    else{
        rows = newlines;
        cols = commas/newlines+1;
        printf("\nThere are %u commas and %u newlines.  I'm guessing this is a %u x %u matrix", commas, newlines, rows, cols);
    }

    //  malloc space for the matrix.
    //  determine size of minor matrix.
    arraysize = sizeof(double)*rows*cols;

    //  malloc space for array.
    if((array = (double*) malloc(arraysize))==NULL){
        puts("malloc asplode!");
        exit(1);
    }

    rewind(in);

    //read numbers into the array.
    //postponing for now, this is tedious.

    //puts("");
    //for(i=0; i<rows*cols; i++) printf("%.2f ", *(array+i));

    fclose(in);
*/
