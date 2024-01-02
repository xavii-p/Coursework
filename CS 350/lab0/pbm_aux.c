//THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING A TUTOR,
//CODE WRITTEN BY OTHER STUDENTS, OR CODE DERIVED FROM AN AI TOOL - XAVIER PIERCE
#include "pbm.h"

PPMImage * new_ppmimage( unsigned int w, unsigned int h, unsigned int m )
{
    PPMImage* new_ppmimage; // pointer for the image to be returned
    new_ppmimage = (PPMImage*)malloc(sizeof(PPMImage)); // allocate memory and assign struct attributes appropriate values
    new_ppmimage->width = w;
    new_ppmimage->height = h;
    new_ppmimage->max = m;
    
    for (int i = 0; i < 3; i++){ // PPM images have RGB (3) 2D arrays; initialize all rows for R,G,B (0,1,2)
        new_ppmimage->pixmap[i] = (unsigned int**)malloc(new_ppmimage->height * sizeof(unsigned int*));
    }
    
    for (int i = 0; i < new_ppmimage->height; i++){ // initialize all columns for each row
        new_ppmimage->pixmap[0][i] = (unsigned int*)malloc(new_ppmimage->width * sizeof(unsigned int));
        new_ppmimage->pixmap[1][i] = (unsigned int*)malloc(new_ppmimage->width * sizeof(unsigned int));
        new_ppmimage->pixmap[2][i] = (unsigned int*)malloc(new_ppmimage->width * sizeof(unsigned int));
    }
    return new_ppmimage;

}

PBMImage * new_pbmimage( unsigned int w, unsigned int h )
{
    PBMImage* new_pbmimage; //image to be returned
    new_pbmimage = (PBMImage*)malloc(sizeof(PBMImage));
    new_pbmimage->width = w;
    new_pbmimage->height = h;
    new_pbmimage->pixmap = (unsigned int**)malloc(new_pbmimage->height * sizeof(unsigned int*)); // initialize all rows

    for (int i = 0 ; i < new_pbmimage->height; i++){ //initialize all columns for each row
        new_pbmimage->pixmap[i] = (unsigned int*)malloc(new_pbmimage->width * sizeof(unsigned int));
    }
    return new_pbmimage;
}

PGMImage * new_pgmimage( unsigned int w, unsigned int h, unsigned int m )
{
    PGMImage* new_pgmimage; //image to be returned
    new_pgmimage = (PGMImage*)malloc(sizeof(PGMImage));
    new_pgmimage->width = w;
    new_pgmimage->height = h;
    new_pgmimage->max = m;
    new_pgmimage->pixmap = (unsigned int**)malloc(new_pgmimage->height * sizeof(unsigned int*)); // initialize all rows

    for (int i = 0 ; i < new_pgmimage->height; i++){ //initialize all columns for each row
        new_pgmimage->pixmap[i] = (unsigned int*)malloc(new_pgmimage->width * sizeof(unsigned int));
    }
    return new_pgmimage;
}

void del_ppmimage( PPMImage * p )
{
    for (int i = 0; i < p->height; i++){ // deallocation is allocation in reverse; start with all rows
       free(p->pixmap[0][i]);
       free(p->pixmap[1][i]);
       free(p->pixmap[2][i]);
    }
    for (int i = 0; i < 3; i++){ //deallocate R,G,B
        free(p->pixmap[i]);
    }
    free(p->pixmap);
    free(p); //release pointer for the image

}

void del_pgmimage( PGMImage * p )
{
    for (int i = 0 ; i < p->height; i++){ // deallocate all columns
        free(p->pixmap[i]);
    }
    free(p->pixmap); //deallocate all rows
    free(p); //free the image ptr
}

void del_pbmimage( PBMImage * p )
{
    for (int i = 0 ; i < p->height; i++){
        free(p->pixmap[i]);
    }
    free(p->pixmap);
    free(p);
}

