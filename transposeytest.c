//  calculate the motherfucking transpose of a motherfucking matrix, AGAIN.

#include <stdio.h>
#include <stdlib.h>
#include <C:\Users\K\Desktop\code\Linear Algebra\MyLinAlg.c>
#define SIZE 3

//void inverse(unsigned size, float *array);
//void transpose(unsigned rows, unsigned cols, float *array, float *trans);

int main(void){
    float A[SIZE][SIZE]={
    {0,2,3},
    {0,4,6},
    {0,8,9}
    };
    float B[SIZE][SIZE], C[SIZE][SIZE];

    puts("\noriginal matrix A:");
    echoarray(SIZE, SIZE, &A[0][0]);

    copy(SIZE*SIZE, &A[0][0], &B[0][0]);

    puts("\nmatrix B is a copy of A:");
    echoarray(SIZE, SIZE, &B[0][0]);

    inverse(SIZE, &A[0][0]);
    puts("\ninverse of matrix A:");
    echoarray(SIZE, SIZE, &A[0][0]);

    multiply(SIZE,SIZE,SIZE,SIZE,&A[0][0],&B[0][0],&C[0][0]);

    puts("\nif that worked, I should get the identity matrix here:");
    echoarray(SIZE, SIZE, &C[0][0]);
}


