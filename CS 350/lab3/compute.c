// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING A TUTOR,
// CODE WRITTEN BY OTHER STUDENTS, OR CODE DERIVED FROM AN AI TOOL - XAVIER PIERCE

// compute takes one command line parameter, START, and computes the perfect numbers beginning at START
// and continuing until the program is terminated by a signal. there may be multiple instances of compute

#include "defs.h"

//global variables used by shutdown signal handler
int sid;	/* segment id of shared memory segment */
shmem* shm_array;	/* pointer to shared array, no storage yet*/
int semid;	/* semaphore id */
int qid;
struct computeProcess* processArray;

void shutdown(int sig) {
    //delete own process entry from process array
    for (int i =0; i < 20; i++){
        if (shm_array->processArray[i].pid == getpid()) {
            shm_array->processArray[i].pid = 0;
            shm_array->processArray[i].numFound = 0;
            shm_array->processArray[i].numTested = 0;
            shm_array->processArray[i].numSkipped = 0;
            break;
        }
    }
    //terminate
    exit(0);
}

int main(int argc, char *argv[]){
    
    struct sigaction action;
    action.sa_handler = shutdown;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGQUIT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
    
    
    //get shared memory segment
    if ((sid=shmget(SHM_KEY,sizeof(shmem),0))== -1) {
        perror("Error getting shared memory segment (shmget)");
        exit(2);
    }

    //map shared memory segment into address space
    if ((shm_array=((shmem*) shmat(sid,0,0)))== (shmem*) -1) {
        perror("Error attaching shared memory segment (shmat)");
        exit(2);
    }

    //get semaphore
    struct sembuf sb;	/* semaphore buffer */

    if ((semid=semget(SEM_KEY,1 ,0))== -1) {
        perror("Error getting semaphore (semget)");
        exit(2);
    }

    //get message queue
    
    if ((qid=msgget(MSG_KEY,0))== -1) {
        perror("Error getting message queue (msgget)");
        exit(2);
    }
    my_msg* msg = malloc(sizeof(my_msg));

    
    
    //register with manage
    
    msg->type = 1;
    msg->data = (int) getpid();
    if (msgsnd(qid, msg, sizeof(my_msg), 0) == -1) {
        perror("Error sending message (msgsnd)");
        exit(2);
    }

    sb.sem_op =-1;  /* set up for a lock operation*/
    sb.sem_num =0;
    sb.sem_flg =0;

    if (semop(semid, &sb, 1) == -1) { /* will block if locked */
        perror("Lock (P) failed");
        exit(2);
    }

    
    //get start value from command line
    if (argc != 2) {
        printf("Usage: compute START\n");
        exit(2);
    }
    int start = atoi(argv[1]);
    if (start < 1) {
        printf("Invalid start value; must be greater than zero\n");
        exit(2);
    }

    //perfect number calculation
    int n,i,sum,startPos, firstIteration;

    n=start;
    startPos = start;
    firstIteration = 1;


        while (1) {
            if (n >= (int) (sizeof(shm_array->bitmap))) {
                printf("n is out of bounds, looping around..\n");
                n = 0;
            } else if (n == startPos && firstIteration == 0) {
                break;
            }
           // printf("calculating %d\n", n);
            // check bitmap
            if (shm_array->bitmap[n] == 1) {
                //update process array
                for (int i =0; i < 20; i++){
                    if (shm_array->processArray[i].pid == getpid()) {
                        shm_array->processArray[i].numSkipped++;
                        break;
                    }
                }
                n++;
                continue;
            } else {
            sum=1;
            for (i=2;i<n;i++)
                if (!(n%i)) sum+=i;
            
            if (sum==n){
                
                // printf("Perfect number found: %d\n", n);
                //update process array
                for (int i =0; i < 20; i++){
                    if (shm_array->processArray[i].pid == getpid()) {
                        shm_array->processArray[i].numFound++;
                        break;
                    }

                }

                //update bitmap
                shm_array->bitmap[n] = 1;

                //send message to manage
                
                msg->type = 2;
                msg->data = n;
                if (msgsnd(qid, msg, sizeof(my_msg), 0) == -1) {
                    perror("Error sending message (msgsnd)");
                    exit(2);
                }
               // printf("Message sent\n");
            } else {
                //update process array
                for (int i = 0; i < 20; i++){
                    if (shm_array->processArray[i].pid == getpid()) {
                        shm_array->processArray[i].numTested++;
                        break;
                    }
                }

                //update bitmap
                shm_array->bitmap[n] = 1;
            }
            n++;
            firstIteration = 0;
        }
        }
    return 0;
}
