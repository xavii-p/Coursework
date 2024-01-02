/*
THIS CODE WAS MY OWN WORK , IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR . Xavier_Pierce
*/

#include<stdio.h>
#include<stdlib.h>

typedef struct {
int nrows; // number of rows
int mcols; // number of cols
int **array;
} matrix;

matrix* matrix_sum(const matrix *m1, const matrix *m2){
    matrix *summatrix = (matrix*)malloc(sizeof(matrix)); 
    summatrix->nrows = m1->nrows;
    summatrix->mcols = m1->mcols;
    summatrix->array = (int**) malloc(summatrix->nrows * sizeof(int*)); // a 2D array must be initialized first with the rows, then all cols in a for loop
    for (int i = 0; i < summatrix->nrows; i++) {
        summatrix->array[i] = (int*) malloc(summatrix->mcols * sizeof(int));
    }

    if ((m1->nrows != m2->nrows) || (m1->mcols != m2->mcols)){ // if dimensions are not identical print message:
        printf("Addition dimension mismatch: m1 is %d by %d, and m2 is %d by %d.\n", m1->nrows, m1->mcols, m2->nrows, m2->mcols);
        return NULL;
    } else { // otherwise we will add the corresponding elements from m1 and m2 as our value for the sum
        for (int i = 0; i < (m1->nrows); i++){
            for (int j = 0; j < (m1->mcols); j++){
                summatrix->array[i][j] = m1->array[i][j] + m2->array[i][j];
            }
        }
        return summatrix;
    }
}

matrix* matrix_prod(const matrix *m1, const matrix *m2){
    matrix *prodmatrix = (matrix*)malloc(sizeof(matrix)); 
    prodmatrix->nrows = m1->nrows;
    prodmatrix->mcols = m2->mcols;
    prodmatrix->array = (int**) malloc(prodmatrix->nrows * sizeof(int*)); // a 2D array must be initialized first with the rows, then all cols in a for loop
    for (int i = 0; i < prodmatrix->nrows; i++) {
        prodmatrix->array[i] = (int*) malloc(prodmatrix->mcols * sizeof(int));
        for (int j = 0; j < prodmatrix->mcols; j++) {
            prodmatrix->array[i][j] = 0; // set all values in prodmatrix to 0
        }
    }

    if ((m1->mcols != m2->nrows)){ // if dimensions are incorrect print message:
        printf("Multiplication dimension mismatch: m1 is %d by %d, and m2 is %d by %d.\n", m1->nrows, m1->mcols, m2->nrows, m2->mcols);
        return NULL;
    } else { // otherwise compute the product
        for (int i = 0; i < prodmatrix->nrows; i++){
            for (int j = 0; j < prodmatrix->mcols;j++){
                for (int k = 0; k < m2->nrows;k++){
                    prodmatrix->array[i][j] += (m1->array[i][k]) * (m2->array[k][j]);
                }
            }
        }

    }
    return prodmatrix;
}