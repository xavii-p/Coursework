#include<stdio.h>
#include "final.h"
#include<stdlib.h>

char* cat(const char* s1, const char* s2);
struct u* set_union(const struct u* set1, const struct u* set2);
struct op* decode(unsigned short bits);

int main(){
    // char* s1 = "Hello";
   //  char* s2 = "World";
   // printf("%s\n", cat(s1, s2));

   /* struct u* set1 = malloc(sizeof(struct u*));
   struct u* set2 = malloc(sizeof(struct u*));
   set1 -> len = 3;
   set2 -> len = 4;

   set1 -> set = (int*) malloc(sizeof(int) * set1 -> len);
   set2 -> set  = (int*) malloc(sizeof(int) * set2 -> len);

   
   set1 -> set[0] = 7;
   set1 -> set[1] = 8;
   set1 -> set[2] = 9;
  // set1 -> set[3] = 1
  // set1 -> set[4] = 1

   set2 -> set[0] = 1;
   set2 -> set[1] = 3;
   set2 -> set[2] = 5;
   set2 -> set[3] = 7;
  //  set2 -> set[4] = 1 



   struct u* set3 = set_union(set1, set2);
    for (int i = 0; i < set3 -> len; i++){
        printf("%d ", set3 -> set[i]);
    }
   */
    unsigned short bits =  1101101000000101;
    decode(1101101000000101);

}
