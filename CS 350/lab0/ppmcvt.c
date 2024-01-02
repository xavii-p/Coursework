// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING A TUTOR,
// CODE WRITTEN BY OTHER STUDENTS, OR CODE DERIVED FROM AN AI TOOL - XAVIER PIERCE
#include "pbm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

typedef struct { // options structure to hold all command line information
    char* mode[];
    char* arg[];
    char* infile[];
    char* outfile[];
} Options;

Options processcmdline(){ //function to process the options and arguments in the command line
    Options input;
    int opt;
    int transformation_count = 0;
    while ((opt = getopt(argc, argv,"bg:i:r:smt:n:o:" )) != -1) { // use getopt and switch to find all options(there can only be 1 in a given call)
        if (transformation_count > 1){
            fprintf("Error: Multiple transformations specified\n");
            exit(1);
        }
        switch(opt)
        {
            case 'b': 
                input->mode = "b";
                input->arg = NULL;
                transformation_count++;
                break;

            case 'g':
                if (strtol(optarg) > 65535){
                    fprintf("Error: Invalid max grayscale pixel value: %s; must be less than 65,536\n");
                    exit(1);
                }
                input->mode = "g";
                input->arg = optarg;
                transformation_count++;
                break;

            case 'i':
                if ((strcmp(optarg, "red")) != 0 || (strcmp(optarg, "green")) != 0 || (strcmp(optarg, "blue")) != 0){
                        fprintf("Error: Invalid channel specification: (%s); should be 'red', 'green', or 'blue'", optarg);
                        exit(1);
                }
                input->mode = "i";
                input->arg = optarg;
                transformation_count++;
                break;

            case 'r':
                if ((strcmp(optarg, "red")) != 0 || (strcmp(optarg, "green")) != 0 || (strcmp(optarg, "blue")) != 0){
                        fprintf("Error: Invalid channel specification: (%s); should be 'red', 'green', or 'blue'", optarg);
                        exit(1);
                }
                input->mode = "r";
                input->arg = optarg;
                transformation_count++;
                break;

            case 's':
                input->mode = "s";
                transformation_count++;
                break;

            case 'm':
                input->mode = "m";
                transformation_count++;
                break;
                
            case 't':
                if (optarg < 1 || optarg > 8){
                    fprintf("Error: Invalid scale factor: %d; must be 1-8\n");
                    exit(1);
                }
                input->mode = "t";
                input->arg = optarg;
                transformation_count++;
                break;

            case 'n':
                if (optarg < 1 || optarg > 8){
                    fprintf("Error: Invalid scale factor: %d; must be 1-8\n");
                    exit(1);
                }
                input->mode = "n";
                input->arg = optarg;
                transformation_count++;
                break;

            case 'o': //not considered a transformation, so transformation count doesn't change
                if (optarg == NULL){
                    fprintf("Error: No output file specified\n");
                    exit(1);
                }
                input->outfile = optarg;
                break;
            
            case '?': //unknown option was given
                fprintf("Usage: ppmcvt [-bgirsmtno] [FILE]\n");
                exit(1);

        }
    }
    if (argv[optind] == NULL){
        fprintf("Error: No input file specified\n");
        exit(1);
    }
    return input;
}

void bitmap(Options input){
    PPMImage* input_image = read_ppmfile(input->infile);
    PBMImage* output_image = new_pbmimage(input_image->width, input_image->height);

    for (int i = 0; i < input_image->height; i++){
        for (int j = 0; j < input_image->width; j++){
                float average = (input_image->pixmap[0][i][j] + input_image->pixmap[1][i][j] + input_image->pixmap[2][i][j]) / 3;
                output_image->pixmap[i][j] = (unsigned int)(average < (float)(input_image->max / 2));
        }
    }

    write_pbmfile(output_image, input->outfile);
    del_ppmimage(input_image);
    del_pbmimage(output_image);
}

void grayscale(Options input){
    PPMImage* input_image = read_ppmfile(input->infile);
    PGMImage* output_image = new_pgmimage(input_image->width, input_image->height, input->arg);

    for (int i = 0; i < input_image->height; i++){
        for (int j = 0; j < input_image->width; j++){
                float average = (input_image->pixmap[0][i][j] + input_image->pixmap[1][i][j] + input_image->pixmap[2][i][j]) / 3;
                output_image->pixmap[i][j] = (unsigned int)((float)(average / input_image->max) * (float)(input->arg));
        }
    }

    write_pgmfile(output_image, input->outfile);
    del_ppmimage(input_image);
    del_pgmimage(output_image);
}

void isolate(Options input){
    PPMImage* input_image = read_ppmfile(input->infile);
    PPMImage* output_image = new_ppmimage(input_image->width, input_image->height, input_image->max);
    if ((strcmp(input->arg, "red")) = 0){
        for (int i = 0; i < input_image->height; i++){
            for (int j = 0; j < input_image->width; j++){
                output_image->pixmap[1][i][j] = 0;
                output_image->pixmap[2][i][j] = 0;
            }
        }
    } else if ((strcmp(input->arg, "green")) = 0){
        for (int i = 0; i < input_image->height; i++){
            for (int j = 0; j < input_image->width; j++){
                output_image->pixmap[0][i][j] = 0;
                output_image->pixmap[2][i][j] = 0;
            }
        }
    } else {
        for (int i = 0; i < input_image->height; i++){
            for (int j = 0; j < input_image->width; j++){
                output_image->pixmap[0][i][j] = 0;
                output_image->pixmap[1][i][j] = 0;
            }
        }
    }
    write_ppmfile(output_image, input->outfile);
    del_ppmimage(input_image);
    del_ppmimage(output_image);
}

void remove(Options input){
    PPMImage* input_image = read_ppmfile(input->infile);
    PPMImage* output_image = new_ppmimage(input_image->width, input_image->height, input_image->max);

    if ((strcmp(input->arg, "red")) = 0){
        for (int i = 0; i < input_image->height; i++){
            for (int j = 0; j < input_image->width; j++){
                output_image->pixmap[0][i][j] = 0;
            }
        }
    } else if ((strcmp(input->arg, "green")) = 0){
        for (int i = 0; i < input_image->height; i++){
            for (int j = 0; j < input_image->width; j++){
                output_image->pixmap[1][i][j] = 0;
            }
        }
    } else {
        for (int i = 0; i < input_image->height; i++){
            for (int j = 0; j < input_image->width; j++){
                output_image->pixmap[2][i][j] = 0;
            }
        }
    }

    write_ppmfile(output_image, input->outfile);
    del_ppmimage(input_image);
    del_ppmimage(output_image);
}

void sepia(Options input){
    PPMImage* input_image = read_ppmfile(input->infile);
    PPMImage* output_image = new_ppmimage(input_image->width, input_image->height, input_image->max);

    for (int i = 0; i < input_image->height; i++){
            for (int j = 0; j < input_image->width; j++){
                output_image->pixmap[0][i][j] = (unsigned int)((float)(.393 * input_image->pixmap[0][i][j]) + (float) (0.769 * input_image->pixmap[1][i][j]) + (float) (0.189 * input_image->pixmap[2][i][j]));
                output_image->pixmap[1][i][j] = (unsigned int)((float)(.349 * input_image->pixmap[0][i][j]) + (float) (0.686 * input_image->pixmap[1][i][j]) + (float) (0.168 * input_image->pixmap[2][i][j]));;
                output_image->pixmap[2][i][j] = (unsigned int)((float)(.272 * input_image->pixmap[0][i][j]) + (float) (0.534 * input_image->pixmap[1][i][j]) + (float) (0.131 * input_image->pixmap[2][i][j]));;
            }
        }

    write_ppmfile(output_image, input->outfile);
    del_ppmimage(input_image);
    del_ppmimage(output_image);
}

void mirror(Options input){
    PPMImage* input_image = read_ppmfile(input->infile);
    PPMImage* output_image = new_ppmimage(input_image->width, input_image->height, input_image->max);

    write_ppmfile(output_image, input->outfile);
    del_ppmimage(input_image);
    del_ppmimage(output_image);
}

void thumbnail(Options input){
    PPMImage* input_image = read_ppmfile(input->infile);
    PPMImage* output_image = new_ppmimage((input_image->width) / input->arg, (input_image->height) / input->arg, (input_image->max) / input->arg);

    int h = 0;
    int w = 0;
    for (int i = 0; i < output_image->height; i++){
            for (int j = 0; j < output_image->width; j++){
                output_image->pixmap[0][i][j] = input_image->pixmap[0][h][w];
                output_image->pixmap[1][i][j] = input_image->pixmap[1][h][w];;
                output_image->pixmap[2][i][j] = input_image->pixmap[2][h][w];;
                w += input->arg;
            }
            h+=input->arg;
        }

    write_ppmfile(output_image, input->outfile);
    del_ppmimage(input_image);
    del_ppmimage(output_image);
}

void nup(Options input){
    PPMImage* input_image = read_ppmfile(input->infile);
    PPMImage* output_image = new_ppmimage(input_image->width, input_image->height, input_image->max);

    write_ppmfile(output_image, input->outfile);
    del_ppmimage(input_image);
    del_ppmimage(output_image);
}

int main( int argc, char *argv[] )
{
   Options input = processcmdline();
   if (strcmp(input->mode, "b") == 0){
        bitmap(input);

   } else if (strcmp(input->mode, "g") == 0){
        grayscale(input);

   } else if (strcmp(input->mode, "i") == 0){
        isolate(input);

   } else if (strcmp(input->mode, "r") == 0){
        remove(input);
   } else if (strcmp(input->mode, "s") == 0){
        sepia(input);

   } else if (strcmp(input->mode, "m") == 0){
        mirror(input);
   } else if (strcmp(input->mode, "t") == 0){
        thumbnail(input);
   } else if (strcmp(input->mode, "n") == 0){
        nup(input);
   } 

    return 0;
}
