/*
THIS CODE WAS MY OWN WORK , IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. Xavier_Pierce
*/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct u{
    int* set;
    int len;
};

struct op{
    char opcode;
    int input1;
    int input2;
};

char* cat(const char* s1, const char* s2){
    int s1Count = 0;
    int s2Count = 0;
    int i = 0;

    // get the number of chars in each string not including the null character
    while (s1[i] != '\0'){
        s1Count++;
        i++;
    }

    i = 0;
    
    while (s2[i] != '\0'){
        s2Count++;
        i++;
    }

    // create array for result string
    char* resultArr = (char*) malloc(sizeof(char) * (s1Count + s2Count));

    int j = 0;
    if (s1[j] != '\0'){ //skip the null character at the end of each string
        while (j < s1Count){
            resultArr[j] = s1[j]; // add the characters from s1 to result
            j++;
        }
    }
   
    j = s1Count; // start where s1 left off
    for (int i = 0; i < s2Count; i++){
       if (s2[i] != '\0') { 
        resultArr[j] = s2[i]; // add the characters from s2 to result
        j++;
        }
    }

    return resultArr;
} 

struct u* set_union(const struct u* set1, const struct u* set2){
        struct u* result = (struct u*) malloc(sizeof(struct u*));
        int duplicates = 0;

    // get the number of elements that appear in both sets; the final length of the union will have a length of all the elements in 
    // both sets minus the duplicates
        for (int i = 0; i < set1 -> len; i++){
            for (int j = 0; j < set2 -> len; j++){
                if (set1 -> set[i] == set2 -> set[j]){
                    duplicates++;
                    
                }
            }
        }

        
        if (duplicates == 0){ // if there are no duplicates add all elements from set1 then all elements from set2
            result -> len = (set1 -> len + set2 -> len);
            result -> set = (int*) malloc(sizeof(int) * result -> len);

            //add all set1 elements
            for (int i = 0; i < set1 -> len; i++){
                result -> set[i] = set1 -> set[i];
            }

            //add all set2 elements 
            int s2Index = 0;
            for (int i = set1 -> len; i < result -> len; i++){ // start where set1 leaves off in the union
                result -> set[i] = set2 -> set[s2Index];
                s2Index++;
            }
        } else { // otherwise calculate length of union then add all elements from set1; compare elements in set2 to elements that already exist in the union
            result -> len = (set1 -> len + set2 -> len) - duplicates;
            result -> set = (int*) malloc(sizeof(int) * result -> len);

            // add all elements from set1
            for (int i = 0; i < set1 -> len; i++){
                result -> set[i] = set1 -> set[i];
            }
            
            int s1Index = set1 -> len; // start where set1 leaves off in the union 
            for (int i = 0; i < set2 -> len; i++){
                bool contained = false;
                for (int j = 0; j < result -> len; j++){ // compare element from set2 to all existing elements in the union
                    if (set2 -> set[i] != result -> set[j]){

                        continue;
                            
                    } else { // element from set2 already exists in the union
                        contained = true;
                    }
                }
        
                if (!contained) result -> set[s1Index++] = set2 -> set[i]; // add the element from set2 to the union only if it is not contained
            }
        }

        // sort using BubbleSort
        for (int i = 0; i < result -> len; i++){
            for (int j = i + 1; j < result -> len; j++){
                if (result -> set[i] > result -> set[j]){
                    int temp = result -> set[i];
                    result -> set[i] = result -> set[j];
                    result -> set[j] = temp;
                }
            }
        }
        return result;

}

struct op* decode(unsigned short bits){
    struct op* result = (struct op*) malloc(sizeof(struct op*));
    unsigned short* temp = malloc(sizeof(unsigned short) * 16);
    printf("%u ", bits);


    for (int i = 0; i < 16; i++){ // elements from 0 to 6 are input2 and 7 to 13 are input1
        temp[i] = bits % 10;
        bits /= 10;
        printf("%d ", temp[i]);
    } // binary string becomes reversed (input2 -> input1 -> opcode)
    
    int base = 1;
    for (int i = 0; i < 14; i++){ // calculate decimal value
        int value2 = (int) temp[i] * base;
        result -> input2 += value2;
        base *= 2;
    }

    for (int i = 7; i < 14; i++){
        int value1 = (int) temp[i] * base;
        result -> input1 += value1;
        base *= 2;
    }
    // determine opcode based on last two bits (they're reversed in the temp array)
        if (temp[15] == 0 && temp[14] == 0){
            result -> opcode = '+';
        } else if (temp[15] == 0 && temp[14] == 1){
            result -> opcode = '-';
        } else if (temp[15] == 1 && temp[14] == 0){
            result -> opcode = '*';
        } else if (temp[15] == 1 && temp[14] == 1){
            result -> opcode = '/';
        } 

        


        // display appropriate message
        
        if (result -> opcode == '+'){
            int sum = result -> input1 + result -> input2;
            printf("The sum of %d and %d is %d", result -> input1, result -> input2, sum);
        } else if (result -> opcode == '-'){
            int difference = result -> input1 - result -> input2;
            printf("The difference of %d and %d is %d", result -> input1, result -> input2, difference);
        } else if (result -> opcode == '*'){
            int product = result -> input1 * result -> input2;
            printf("The product of %d and %d is %d", result -> input1, result -> input2, product);
        } else if (result -> opcode == '/'){
            int quotient = result -> input1 / result -> input2;
            printf("The quotient of %d and %d is %d", result -> input1, result -> input2, quotient);
        }

        return result;
}



