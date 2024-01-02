// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING A TUTOR,
// CODE WRITTEN BY OTHER STUDENTS, OR CODE DERIVED FROM AN AI TOOL - XAVIER PIERCE

// manage creates and maintains shared memory segment, a message queue, and a semaphore; 
// also keeps track of active compute processes accessing the shared memory segment
// cleans up all IPC elements when terminated by a SIGINT, SIGQUIT, SIGTERM signal


#include "defs.h"

//global variables used by shutdown signal handler
int sid;	/* segment id of shared memory segment */
shmem* shm_array;	/* pointer to shared array, no storage yet*/
int semid;	/* semaphore id */
int qid;
struct computeProcess* processArray;

void shutdown(int sig) {
    //kill all compute processes
    for (int i =0; i < 20; i++){
        if (shm_array->processArray[i].pid != 0) {
            kill(shm_array->processArray[i].pid, SIGINT);
        }
    }
    sleep(5);
    /* Unmap and deallocate the shared segment */

	if (shmdt((char  *) shm_array) == -1) {
		perror("Error detaching shared memory segment (shmdt)");
		exit(1);
	}
	if (shmctl(sid,IPC_RMID,0) == -1) {
		perror("Error deallocating shared memory segment (shmctl)");
		exit(1);
	}

    //delete semaphore
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("Error deleting semaphore (semctl)");
        exit(1);
    }

    //delete message queue
    if (msgctl(qid, IPC_RMID, NULL) == -1) {
        perror("Error deleting message queue (msgctl)");
        exit(1);
    }
    //terminate manage
    exit(0);
}

int main(int argc, char *argv[]) {

    printf("Storing one bit per byte..\n");
    

    struct sigaction action;
    action.sa_handler = shutdown;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGQUIT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);

	

    
    /* create shared segment */
    if ((sid=shmget(SHM_KEY,sizeof(shmem), IPC_CREAT | 0666))== -1) {
        perror("Error creating shared memory segment (shmget))");
        exit(1);
    }

    if ((shm_array=((shmem *) shmat(sid,0,0)))== (shmem *) -1) {
		perror("Error attaching shared memory segment (shmat))");
		exit(1);
		}
    
    //initialize shared memory segment
   


     for (int i = 0; i < (sizeof(shm_array -> bitmap)); i++){
            shm_array -> bitmap[i] = 0;
    }

    for (int i = 0; i < 20; i++){
            shm_array -> perfectNumbers[i] = 0;
    }

    for (int i = 0; i < 20; i++){
            shm_array -> processArray[i].pid = 0;
            shm_array -> processArray[i].numFound = 0;
            shm_array -> processArray[i].numTested = 0;
            shm_array -> processArray[i].numSkipped = 0; 
    }

    shm_array->managePID = getpid();
    
    //create semaphore
    struct sembuf sb;	/* semaphore buffer */
    
    if ((semid=semget(SEM_KEY,1 ,IPC_CREAT | 0666))== -1) {
        perror("Error creating semaphore (semget)");
        exit(1);
    }


    //create message queue

    my_msg* msg;    
        if ((qid=msgget(MSG_KEY, IPC_CREAT | 0666)) == -1){
            perror("Error creating message queue (msgget)");
            exit(1);
        }
    while(1) { //wait for messages

        msg = malloc(sizeof(my_msg));
        
        if (msgrcv(qid, msg, sizeof(my_msg), -2, 0) == -1){
            perror("Error receiving message (msgrcv)");
            exit(1);
        }

        if (msg->type == 1) {
            //compute process registration
            //add process to process array
            for (int i = 0; i < 20; i++) {
                if (shm_array->processArray[i].pid == 0) {
                    shm_array->processArray[i].pid = msg->data;

                    //unlock semaphore
                    sb.sem_op = 1;
                    sb.sem_num = 0;
                    sb.sem_flg = 0;
                    if (semop(semid, &sb, 1) == -1) {
                        perror("Unlock (V) failed");
                        exit(1);
                    }

                    break;
                }
            }

        } else if (msg->type == 2) {
           // printf("Perfection\n");
            // printf("Number: %d\n", msg->data);
            //number found
            //add number to perfect numbers array
            for (int i = 0; i < 20; i++) {
                if (shm_array->perfectNumbers[i] == 0) {
                    shm_array->perfectNumbers[i] = msg->data;
                    break;
                }
            }

            
        }

        

    }
    return 0;
}