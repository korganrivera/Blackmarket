/**
    void copy(unsigned size, float *array1, float *array2)                                                              copies array1 to array2.
    void echoarray(unsigned r, unsigned c, float *array)                                                                displays array to stdout.
    void scale(unsigned size, float scalar, float *array)                                                               multiplies array by scalar.
    void sum(unsigned size, float *array1, float *array2, float *array3)                                                sums two arrays together.
    void clear(unsigned size, float *array)                                                                             fills array with zeroes.
    void randomfill(unsigned size, float *array, unsigned max)                                                          fills array with random unsigned ints.
    void multiply(unsigned r1, unsigned c1, unsigned r2, unsigned c2, float *array1, float *array2, float *array3)      multiplies two arrays together
    float determinant(unsigned size, float *array)                                                                      calculates determinant of array.
    void inverse(unsigned size, float *array)                                                                           inverts a given array.
    void transpose(unsigned rows, unsigned cols, float *array, float *trans)                                            transposes a given array, puts result in trans.
    float dot(ROWS, &vector1[0][0], &vector2[0][0]);                                                                    returns dot product of two given vectors.

*/

#include <stdlib.h>
#include <stdio.h>

//call: copy(sizeof(from)/sizeof(from[0][0]),&from[0][0],&to[0][0]);
//call: copy(number of elements in array, pointer to array copying from, pointer to array copying to)
void copy(unsigned size, float *array1, float *array2){
    unsigned i;
    for(i=0; i<size; i++) *(array2+i) = *(array1+i);
}

//call: echoarray(3,4,&a[0][0]);
void echoarray(unsigned r, unsigned c, float *array){
    unsigned i;
    for(i=0; i<r*c; i++){  printf("%.2f\t",  *(array+i)); if(!((i+1)%c)) puts(""); }
    puts("");
}

//call: scale(sizeof(a)/sizeof(a[0][0]), 4.5, &a[0][0]);
void scale(unsigned size, float scalar, float *array){
    unsigned i;
    for(i=0; i<size; i++) *(array+i) *= scalar;
}

//call: sum(sizeof(a)/sizeof(a[0][0]), &a[0][0], &b[0][0], &c[0][0]);
void sum(unsigned size, float *array1, float *array2, float *array3){
    unsigned i;
    for(i=0; i<size; i++) *(array3+i) = *(array1+i) + *(array2+i);
}

//fills array with zeroes.
//call: clear(sizeof(a)/sizeof(a[0][0]), &a[0][0]);
void clear(unsigned size, float *array){
    unsigned i;
    for(i=0; i<size; i++) *(array+i) = 0;
}

//fills array with random values from 0 to (max-1).
//call: randomfill(sizeof(a)/sizeof(a[0][0]), &a[0][0], 100);
void randomfill(unsigned size, float *array, unsigned max){
    unsigned i;
    for(i=0; i<size; i++) *(array+i) = rand()%max;
}

//multiplies first two arrays, puts the result in array3.  Assumes the arrays are the correct size.  It's your fault if they're not.
//dimensions of the 3rd matrix should be [rows in 1st matrix][columns in 2nd matrix]
//uses pretty ugly pointer arithmetic, but only way I could make it work.
//call: multiply(3,4,4,5,&a[0][0],&b[0][0],&c[0][0]);
//where dimensions of a are 3x4, b are 4x5, and c are 3x5.
void multiply(unsigned r1, unsigned c1, unsigned r2, unsigned c2, float *array1, float *array2, float *array3){
    unsigned i,j;
    float sum;

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
float determinant(unsigned size, float *array){
    float *minor, sum=0.0;
    unsigned i,j,k, minorsize;

    if(size<2) return 0;
    if(size==2) return (*array * *(array+3) - *(array+1)* *(array+2));

    for(i=0; i<size; i++){
        //  determine size of minor matrix.
        minorsize = sizeof(float)*(size-1)*(size-1);

        //  malloc space for a size-1 array.
        if((minor = (float*) malloc(minorsize))==NULL){
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
void transpose(unsigned rows, unsigned cols, float *array, float *trans){
    unsigned i;
    for(i=0; i<rows*cols; i++) *(trans+i) = *(array+(i%rows)*cols+i/rows);
    //for(i=0; i<rows*cols; i++) *(trans+i) = array[i%rows][i/rows];

}


/*
    call: inverse(number of rows in matrix, pointer to first element of multidimensional array);
    purpose: inverts a given matrix.  original matrix is written over.
    2015.6.2
*/
void inverse(unsigned size, float *array){
    float arraydet;                                 //  determinant of given matrix.
    float *minor;                                   //  each minor matrix in given array.
    float *transposedmatrix;                        //  array transposed.
    float inversematrix[size][size];                //  this is where the transposed matrix will end up.
    float minordet;                                 //  determinant of each minor matrix.
    unsigned i,j,current, minorindex, minorsize;    //  loop variables, etc.

    minorsize = sizeof(float)*(size-1)*(size-1);    //  determine size of minor matrix.

    if((minor = (float*) malloc(minorsize))==NULL){ //  malloc space for a size-1 array. used for minor.
        puts("malloc asplode!");
        exit(1);
    }

    //  malloc space for transposed array.
    if((transposedmatrix = (float*) malloc(sizeof(float)*size*size))==NULL){
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


float dot(unsigned rows, float *vector1, float *vector2){
    float sum=0;
    unsigned i;

    for(i=0; i<rows; i++) sum += *(vector1+i) * *(vector2+i);
    return sum;
}

