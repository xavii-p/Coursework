#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "matrix.h"

matrix* matrix_sum(const matrix *m1, const matrix *m2);

matrix* matrix_prod(const matrix *m1, const matrix *m2);




int main(int argc, char *argv[]){
    matrix *m1 = (matrix*) malloc(sizeof(matrix));
    matrix *m2 = (matrix*) malloc(sizeof(matrix));
    m1->nrows = 2;
    m2->nrows = 7;
    m1->mcols = 7;
    m2->mcols = 8;
    m1->array = (int**) malloc(m1->nrows * sizeof(int*)); // this is how you initialize a 2D array (must initialize pointer for each row then in for loop initialize for each column)
    for (int i = 0; i < m1->nrows; i++) {
        m1->array[i] = (int*) malloc(m1->mcols * sizeof(int));
    }
    m2->array = (int**) malloc(m2->nrows * sizeof(int*));
    for (int i = 0; i < m2->nrows; i++) {
        m2->array[i] = (int*) malloc(m2->mcols * sizeof(int));
    }
   
    srand(time(NULL));   // Initialization, should only be called once.

    // fill in random values
    for (int i = 0; i < (m1->nrows); i++){ 
            for (int j = 0; j < (m1->mcols); j++){
                m1->array[i][j] = rand() % 5;
                
            }
    }
    for (int i = 0; i < (m2->nrows); i++){ 
            for (int j = 0; j < (m2->mcols); j++){
                m2->array[i][j] = rand() % 5;
            }
    }

      matrix_prod(m1,m2);
    
}