#include<stdio.h>
#include<stdlib.h>
int upper(double num);
char* first(const char *str, char x);
char** input_array(void);
char *convert(unsigned int num);
char* binary(const char *num, int len);


int main(){
   // printf("The ceiling is %d.\n", upper(3.6));
    // printf("The ceiling is %d.\n", upper(-6.5));
    // printf("The ceiling is %d.\n", upper(-3.2));
    // printf("The ceiling is %d.\n", upper(7.8));
   // const char array[] = "Hello";
   // printf("%p\n", first(array, 'e'));

   // input_array();
   // convert(200);
   // convert(123456);

   // binary("0010110111111010", 8);
  //  binary("0010110111111010", 0);
   binary("0010110111111010", 32);

   binary("1010110111111010", 20);

}