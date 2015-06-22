/**
    int copy(unsigned size, double *array1, double *array2)                                                             copies array1 to array2. returns amount copied.
    int echoarray(unsigned r, unsigned c, double *array)                                                                displays array to stdout.
    int scale(unsigned size, double scalar, double *array)                                                              multiplies array by scalar.
    int sum(unsigned size, double *array1, double *array2, double *array3)                                              sums two arrays together.
    int clear(unsigned size, double *array)                                                                             fills array with zeroes.
    int randomfill(unsigned size, double *array, unsigned max)                                                          fills array with random unsigned ints.
    int multiply(unsigned r1, unsigned c1, unsigned r2, unsigned c2, double *array1, double *array2, double *array3)    multiplies two arrays together
    double determinant(unsigned size, double *array)                                                                    calculates determinant of array.
    int inverse(unsigned size, double *array)                                                                           inverts a given array.
    int transpose(unsigned rows, unsigned cols, double *array, double *trans)                                           transposes a given array, puts result in trans.
    double dot(ROWS, &vector1[0][0], &vector2[0][0]);                                                                   returns dot product of two given vectors.
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

//call: copy(sizeof(from)/sizeof(from[0][0]),&from[0][0],&to[0][0]);
//call: copy(number of elements in array, pointer to array copying from, pointer to array copying to)
int copy(unsigned size, double *array1, double *array2){
    unsigned i;

    if(!size) return -1;                                                //  if size is 0, return -1 to flag problem.  Still runnable though.
    assert(array1!=NULL && array2!=NULL && "copy(): array pointers can't be NULL.");
    for(i=0; i<size; i++) *(array2+i) = *(array1+i);
    return i;
}

//call: echoarray(3,4,&a[0][0]);
int echoarray(unsigned r, unsigned c, double *array){
    unsigned i, x;
    if(!r || !c ) return -1;                                            //  if r or c is 0, then return -1 to flag problem.  Code still runnable though.
    assert(array!=NULL && "echoarray(): array pointer can't be NULL."); //  make sure pointers aren't null.  abort if so.
    assert(c < (UINT_MAX/r) && "echoarray(): unsigned wrap happened."); //  detect an unsigned wraparound before it happens.

    for(i=0; i<r*c; i++){  printf("%.4f\t",  *(array+i)); if(!((i+1)%c)) puts(""); }
    puts("");
    return i;
}

//call: scale(sizeof(a)/sizeof(a[0][0]), 4.5, &a[0][0]);
int scale(unsigned size, double scalar, double *array){
    unsigned i;
    if(!size) return -1;

    assert(array!=NULL && "scale(): array pointer can't be NULL.");     //  make sure pointer isn't null.  abort if so.
    for(i=0; i<size; i++) *(array+i) *= scalar;
    return i;
}

//call: sum(sizeof(a)/sizeof(a[0][0]), &a[0][0], &b[0][0], &c[0][0]);
int sum(unsigned size, double *array1, double *array2, double *array3){
    unsigned i;

    if(!size) return -1;
    assert(array1!=NULL && array2!=NULL && array3!=NULL && "sum(): array pointers can't be NULL.");
    for(i=0; i<size; i++) *(array3+i) = *(array1+i) + *(array2+i);
    return i;
}

//fills array with zeroes.
//call: clear(sizeof(a)/sizeof(a[0][0]), &a[0][0]);
int clear(unsigned size, double *array){
    unsigned i;

    if(!size) return -1;
    assert(array!=NULL && "clear(): array pointer can't be NULL.");
    for(i=0; i<size; i++) *(array+i) = 0;
    return i;
}

//fills array with random values from 0 to (max-1).
//call: randomfill(sizeof(a)/sizeof(a[0][0]), &a[0][0], 100);
int randomfill(unsigned size, double *array, unsigned max){
    unsigned i;
    if(!max) return -1;
    assert(array!=NULL && "randomfill(): array pointer can't be NULL.");
    for(i=0; i<size; i++) *(array+i) = rand()%max;
    return i;
}

/*  multiplies first two arrays, puts the result in array3.
    dimensions of the 3rd matrix should be [rows in 1st matrix][columns in 2nd matrix]
    uses pretty ugly pointer arithmetic, but only way I could make it work.
    call: multiply(3,4,4,5,&a[0][0],&b[0][0],&c[0][0]);
    where dimensions of a are 3x4, b are 4x5, and c are 3x5.
    Not sure how to detect if wrap happens with sum.
*/
int multiply(unsigned r1, unsigned c1, unsigned r2, unsigned c2, double *array1, double *array2, double *array3){
    unsigned i,j;
    double sum;

    assert(array1!=NULL && array2!=NULL && array3!=NULL && "multiply(): array pointers can't be NULL.");
    assert(c1==r2 && "multiply(): array1's columns must equal array2's rows.");
    if(!r1 || !c1 || !r2 || !c2) return -1;
    assert((c2 < UINT_MAX/r1) && "multiply(): unsigned wrap happened.");

    for(i=0; i<r1*c2; i++){
        sum=0;
        for(j=0; j<r2; j++) sum += *(array1+i/c2*c1+j) * *(array2+j*c2+i%c2);
        *(array3+i) = sum;
    }
}


/**     for each top element, build its minor, get determinant of minor, multiply by its element. add or subtract this value from the sum.
        return this sum.  Convert this to double when I can be bothered.
        call: determinant(#OfRowsInMatrix, &a[0][0]);
*/
double determinant(unsigned size, double *array){
    double *minor, sum=0.0;
    unsigned i,j,k, minorsize;

    assert(array!=NULL && "determinant(): array pointer can't be NULL.");

    if(!size) return 0;
    if(size==1) return (*array);
    if(size==2) return (*array * *(array+3) - *(array+1)* *(array+2));

    for(i=0; i<size; i++){
        //  determine size of minor matrix.
        minorsize = sizeof(double)*(size-1)*(size-1);

        //  malloc space for a size-1 array.
        if((minor = (double*) malloc(minorsize))==NULL){
            puts("malloc asplode!");
            exit(1);
        }

        //  build the minor.
        for(j=size,k=0; j<size*size; j++){
            if(i==j%size) continue;          // if you're on the same column as current element, skip it. top row automatically skipped.
            else *(minor+k++) = *(array+j);
        }

        //If on an odd-numbered column, add the result to the sum, otherwise subtract it.
        if(i%2) sum -= *(array+i)*determinant(size-1,minor);
        else sum += *(array+i)*determinant(size-1,minor);

        free(minor);
    }
    return sum;
}


/*
    call: transpose(rows, cols, pointer to first element of n-dimensional array, pointer to first element of array that has vice versa row col numbers)
    purpose: transposes a given matrix, puts result in trans.
*/
int transpose(unsigned rows, unsigned cols, double *array, double *trans){
    unsigned i;

    assert(array!=NULL && trans!=NULL && "transpose(): array pointers can't be NULL.");
    if(!rows || !cols) return -1;
    assert((rows < UINT_MAX/cols) && "transpose(): unsigned wrap happened.");

    for(i=0; i<rows*cols; i++) *(trans+i) = *(array+(i%rows)*cols+i/rows);
    //for(i=0; i<rows*cols; i++) *(trans+i) = array[i%rows][i/rows];
    return i;
}


/*
    call: inverse(number of rows in matrix, pointer to first element of multidimensional array);
    purpose: inverts a given matrix.  original matrix is written over.
    2015.6.2
    2015.6.6    Might not be working correctly.

*/
int inverse(unsigned size, double *array){
    double arraydet;                                 //  determinant of given matrix.
    double *minor;                                   //  each minor matrix in given array.
    double *transposedmatrix;                        //  array transposed.
    double inversematrix[size][size];                //  this is where the transposed matrix will end up.
    double minordet;                                 //  determinant of each minor matrix.
    unsigned i,j,current, minorindex, minorsize;    //  loop variables, etc.

    assert(array!=NULL && "inverse(): array pointers can't be NULL.");
    if(size<2) return -1;
    minorsize = sizeof(double)*(size-1)*(size-1);    //  determine size of minor matrix.
    assert((size < UINT_MAX/size) && "inverse(): unsigned wrap.");

    if((minor = (double*) malloc(minorsize))==NULL){ //  malloc space for a size-1 array. used for minor.
        puts("malloc asplode!");
        exit(1);
    }

    //  malloc space for transposed array.
    if((transposedmatrix = (double*) malloc(sizeof(double)*size*size))==NULL){
        puts("malloc asplode!");
        exit(1);
    }

    transpose(size, size, array, transposedmatrix);                     //  transpose matrix.

    for(current=0; current<size*size; current++){                       //  for each element in matrix
        minorindex=0;
        for(j=0; j<size*size; j++){
            if(j/size==current/size || j%size==current%size) continue;  //  skip element if it's on the same row or col as current.
            *(minor+minorindex++) = *(transposedmatrix+j);              //  otherwise put that element in the minor matrix.
        }
        minordet = determinant(size-1, minor);                          //  calculate determinant of minor.
        inversematrix[current/size][current%size] = minordet;           //  put that value in the inverse matrix.
    }

    //  checkerboard the inverse matrix here. if else here could be put into one line.
    for(j=0; j<size*size; j++){
        if(((j/size)%2) && (!((j%size)%2))) inversematrix[j/size][j%size] *= -1.0;      //  if on odd-numbered row and even-numbered column, multiply element by -1.0.
        else if(!(((j/size)%2)) && ((j%size)%2)) inversematrix[j/size][j%size] *= -1.0; //  else if on even-numbered row and odd-numbered column, multiply element by -1.0.
    }

    arraydet = determinant(size, array);                                //  calculate determinant of the original array.
    if(arraydet) arraydet = 1.0/arraydet;                               //  invert that determinant if it's not zero.
    scale(size*size, arraydet, &inversematrix[0][0]);                   //  multiply elements in inversematrix by that determinant.
    copy(size*size, &inversematrix[0][0], array);                       //  copy inversematrix over original matrix.
}

//  dot function of two vectors.  Can't detect if sum wraps, figure out later.
double dot(unsigned rows, double *vector1, double *vector2){
    double sum=0;
    unsigned i;

    assert(vector1!=NULL && vector2!=NULL && "dot(): array pointers can't be NULL.");

    for(i=0; i<rows; i++) sum += *(vector1+i) * *(vector2+i);
    return sum;
}
