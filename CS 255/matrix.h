
typedef struct {
int nrows; // number of rows
int mcols; // number of cols
int **array;
} matrix;


matrix* matrix_sum(const matrix *m1, const matrix *m2);

matrix* matrix_prod(const matrix *m1, const matrix *m2);

