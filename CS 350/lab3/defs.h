// Description: This file contains all the header files and global variables
/* 
The shared memory segment contains the following data:
A bit map large enough to contain 2^25 bits. If a bit is off it 
indicates the corresponding integer has not been tested. The bits
are stored as 2^22 bytes.
•An array of integers of length 20 to contain the perfect numbers 
found.
•An array of "process" structures of length 20, to summarize data 
on the currently active compute processes. This structure
contains the pid, the number of perfect numbers found, the 
number of candidates tested, and the number of candidates 
skipped, since they had already been tested. "Compute" should 
never test a number already marked in the bitmap.
•The pid of the manager process.

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h> // shared memory
#include <sys/msg.h> // message queue
#include <sys/sem.h> // semaphore
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#define SHM_KEY (key_t)44710
#define SEM_KEY (key_t)44711
#define MSG_KEY (key_t)44712
#define SHM_SIZE (size_t)(sizeof(shmem))
#define BITMAP_SIZE ((1<<22) / 8)



typedef struct computeProcess{
    pid_t pid;
    int numTested;
    int numSkipped;
    int numFound;
};

typedef struct  {
		long type;
		int data;
} my_msg;

typedef struct{
    pid_t managePID;
    char bitmap[BITMAP_SIZE];
    int perfectNumbers[20];
    struct computeProcess processArray[20];


}shmem;

