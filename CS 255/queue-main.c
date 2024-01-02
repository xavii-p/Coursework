#include<stdio.h>
#include<stdlib.h>

void enqueue(int item);
int dequeue();
void free_queue();


int main(char argc, char *argv[]){
    enqueue(12);
    enqueue(13);
    enqueue(14);
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
    printf("%d\n", dequeue());
}