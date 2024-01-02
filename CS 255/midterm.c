/*
THIS CODE WAS MY OWN WORK , IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. Xavier_Pierce
*/



#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int upper(double num){
    double input = num; 
    double remainder = 0.0;
    int result; //answer to be returned
    if(input > 0){
    double remainder = (int) (input + 1) - input  ; // a calculation that allows me to see if I should round up or down
    } else {
    double remainder = input - ((int)input) ; // another calculation but for negative input
   // printf("%f", remainder);
    }
    if (remainder < .5 && input > 0){ // round up (if positive)
        result = (int) (input + 1);
      //  printf("%d", result);
        return result;
        
    } else if(remainder >= .5 && input > 0){ // round down (if positive)
        result = (int) input;
      //  printf("%d", result);
        return result;
    } else if(remainder > .5 && input < 0){ // round up (if negative)
        result = (int) (input + (-1));
     //   printf("%d", result);
        return result;
    } else if (remainder <= .5 && input < 0){ // round down (if negative)
        result = (int) input;
     //   printf("%d", result);
        return result;
    }

}

char* first(const char *str, char x){
    for (int i = 0; str[i] != '\0'; i++){ // iterate through string characters to find a matching one
        if (str[i] == x){
            return (char*)(str + (sizeof(char) * i)); //calculate the address of the current char in str
        } else {
            continue; // next iteration
        }
        return NULL;
    }
}

   char** input_array(void){
        int running = 1;
        int numStrings = 0;
        char str;
        while (running == 1){
            printf("Please enter a String: ");
            
            str = getchar();
            if(str == '\0'){
                break;
            }
            input[numStrings] = (char)str;
            numStrings++;


        }
        return input;

    }


    char *convert(unsigned int num){
        int quotient = num;
        int position = 0;

        if (num == 0){ // handle the case where input may be 0
            char *str = (char*)malloc(sizeof(char) * 2);
            str[0] = '0'; 
            str[1] = '\0'; //null
            return str;
        }

        char hex_digits[] = "0123456789abcdef";
        char *str = (char*)malloc(sizeof(char) * 9); // range of unsigned int stops at 16^8 so max amount of space needed would be for 9 hex values
        while (num != 0){ // this loop performs repeated division, using the remainder to decide the hex digit
            int remainder = quotient % 16;
            quotient = (int) num / 16;
            str[position] = hex_digits[remainder]; // string is backwards, need to flip
            position++;
            num = num / 16;

        }

        for (int i = 0; i < position / 2; i++) { // reverse the string
            char temp = str[i];
            str[i] = str[position - i - 1];
            str[position - i - 1] = temp;
        }
        
        return str;

    }

    char* binary(const char *num, int len){
        if (len == 0){ // return nothing if len is 0
        char *empty = (char*)malloc(1);
        return empty;

        } else if (len == 16){ // return an exact copy of the input if len is 16
        char *copyArr = (char*)malloc(17);
        strcpy(copyArr, num);
        return copyArr;

        } else if (len < 16){ // truncate the string
        char *copyArr = (char*)malloc(17);
        strcpy(copyArr, num);
        char *returnArr = (char*)malloc(len + 1);
        int emptyPosition = 0; // keep track of where remaining elements will be put
        for (int i = (16-len); i < 17; i++){ // start at element 16-len and iterate through rest of string to copy
            returnArr[emptyPosition] = copyArr[i];
            emptyPosition++;
        }
        

        }  else if (len > 16 && num[0] == 1){ //padden/extend the string with 1's
        char *copyArr = (char*)malloc(17);
        strcpy(copyArr, num);
        char *copyArr2 = (char*)malloc(len);
        int x = 16 - len;
        int marker = 0;
        for (int i = x; i < 17; i++){
                copyArr[marker] = copyArr2[i];
                marker++;
            }

        for (int i = 0; i < x; i++){
                copyArr2[i] = '1';
            }

        return copyArr2;

         }  else if (len > 16 && num[0] == 0){ //padden the string with 0's
        char *copyArr = (char*)malloc(17);
        strcpy(copyArr, num);
        char *copyArr2 = (char*)malloc(len);
        int x = 16 - len;
        int marker = 0;
        for (int i = x; i < 17; i++){
             copyArr[marker] = copyArr2[i];
             marker++;
        }

        for (int i = 0; i < x; i++){
                copyArr2[i] = '0';
        }
        return copyArr2;
        }  
    }
