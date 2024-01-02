// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING A TUTOR,
// CODE WRITTEN BY OTHER STUDENTS, OR CODE DERIVED FROM AN AI TOOL - XAVIER PIERCE

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc < 6) {
        printf("Usage: %s [n:s:l:] \n", argv[0]);
        exit(1);
    }

    int option;
    int n = 0;
    int s = 0;
    int l = 0;
    while ((option = getopt(argc, argv, "n:s:l:")) != -1) {
        switch (option) {
            case 'n':
                n = atoi(optarg);
                if (n > 1){
                    printf("This program only allows 1 sorter.");
                    exit(1);
                }
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'l':
                l = atoi(optarg);
                break;
            case '?':
                printf("Usage: %s [n:s:l:] \n", argv[0]);
                exit(1);
        }
    }




    int fd_parseToSort[2];
    int fd_sortToSuppress[2];
    if (pipe(fd_parseToSort)){//create pipe fd_parseToSort[o] for reading and fd_parseToSort[1] for writing
        perror("Pipe failed.");
        exit(1);
    } 
    if (pipe(fd_sortToSuppress)){//create pipe fd_sortToSupress[o] for reading and fd_sortToSupress[1] for writing
        perror("Pipe failed.");
        exit(1);
    
    } 


    if (!fork()){ //child for sorting, must be able to write to supress child and read from parse
        dup2(fd_parseToSort[0], 0); //replace stdin with read end of parseToSort
        close(fd_parseToSort[0]); 
        close(fd_parseToSort[1]); //cannot write to parse/parent

        dup2(fd_sortToSuppress[1], 1); //replace stdout with write end of sortToSuppress
        close(fd_sortToSuppress[1]); 
        close(fd_sortToSuppress[0]); //cannot read from suppress

        FILE* parseToSort = fdopen(0, "r"); // we can use fgetc
        FILE* sortToSuppress = fdopen(1, "w"); // we can use fputs

        if (parseToSort == NULL){
            perror("Error attaching to pipe for reading from parse process.");
            exit(1);
        }
        if (sortToSuppress == NULL){
            perror("Error attaching to pipe for writing to suppress process.");
            exit(1);
        }

        execl("/usr/bin/sort", "sort", NULL);
        perror("Error executing sort.");
        exit(1);

    } else { //parent for parsing, must be able to write to sort and read from stdin

        //code for parsing
        

        FILE* parseToSort = fdopen(fd_parseToSort[1], "w"); // we can use fputc

        
        if (parseToSort == NULL){
            perror("Error attaching to pipe for writing to sort process.");
            exit(1);
        }

        char c;
        int wordLength = 0;
        char nextChar;
        char buffer[256];

        while ((c = fgetc(stdin)) != EOF){
             if (isalpha(c)) { // Check if character is part of a word
                buffer[wordLength] = tolower(c);
                wordLength++;

             } else { 
                if (wordLength > 0 && wordLength <= s){ //word is too short & buffer should be emptied
                    wordLength = 0;
                    memset(buffer, 0, sizeof(buffer)); //reset buffer

                } else if (wordLength >= s && wordLength <= l){ //word is within range
                    buffer[wordLength] = '\0'; //add null terminator
                    fputs(buffer, parseToSort); //send word to sort
                    fputc('\n', parseToSort); //send new line to sort
                    wordLength = 0;
                } else { //word is too long and we only want to use the first l characters
                    buffer[l] = '\0'; //add null terminator
                    fputs(buffer, parseToSort); //send word to sort
                    fputc('\n', parseToSort); //send new line to sort
                    wordLength = 0;
                    

                }
                
             }
            
        }
        
        fclose(parseToSort); //indicating end of file to sort
        

        if(!fork()){ //child for suppressing, must be able to read from sort and write to stdout
            dup2(fd_sortToSuppress[0], 0); //replace stdin with read end of sortToSuppress
            close(fd_sortToSuppress[0]);
            close(fd_sortToSuppress[1]); //cannot write to sort

            FILE* sortToSuppress = fdopen(0, "r"); // now we can use fgets
            if (sortToSuppress == NULL){
                perror("Error attaching to pipe for reading from sort process.");
                exit(1);
            }

            
            int wordCount = 0;
            char word[256];
            char buffer[256];
            //remove new line from buffer


            while ((fgets(buffer, sizeof(buffer), sortToSuppress))){
                buffer[strcspn(buffer, "\n")] = 0; //remove new line from buffer
                if (wordCount == 0){ //first or new word is copied into word
                    strcpy(word, buffer);
                    wordCount = 1;
                } else  if (strcmp(buffer, word) != 0){ //words are not the same, send previous word to stdout
                    printf("%-10d%s\n", wordCount, word);
                    strcpy(word, buffer); //reset word
                    wordCount = 1; //reset wordCount
            
                } else if (strcmp(buffer, word) == 0) {
                    wordCount++;
                } 
        
            }

            printf("%-10d%s\n", wordCount, word); //print last word
            fclose(sortToSuppress);
            exit(0);

        } else {
            close(fd_parseToSort[0]); //cannot read from sort
            close(fd_sortToSuppress[1]); //cannot write to suppress
            close(fd_sortToSuppress[0]); //cannot read from suppress

            wait(NULL);
            wait(NULL);

            return 0;
        } // end of suppress child
    } //end of parse parent
}