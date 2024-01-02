/*
THIS CODE WAS MY OWN WORK , IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR . Xavier_Pierce
*/

#include<stdio.h>
#include<stdlib.h>

struct Node{ //each node will contain a key and a pointer to the next node
    int key;
    struct Node *next;
};

    struct Node *first; //global pointers to first and last element in queue
    struct Node *last;
    

void enqueue(int item){
    struct Node *newnode = (struct Node*)malloc(sizeof(struct Node)); // create new node and store key
    newnode -> key = item;
    newnode -> next = NULL;

    if (first == NULL){ // if it's the first node in the queue place at the front otherwise place behind last element
        first = newnode;
    } else {
        last -> next = newnode;
    }
    last = newnode;
}

int dequeue(){
    int value = first -> key;
    if (first == NULL){ //empty queue (underflow)
        return -1;
    } else {    // remove first element by making it's next element first
        struct Node *temp = first;
        first = first -> next;
        return value;
    }
}

void free_queue(){
    struct Node *currentnode = first;
    struct Node *temp = NULL;

    while(currentnode != NULL){ //free all pointers to nodes
        temp = currentnode;
        currentnode = currentnode -> next;
        free(temp);
    }

}
