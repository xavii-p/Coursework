// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING A TUTOR,
// CODE WRITTEN BY OTHER STUDENTS, OR CODE DERIVED FROM AN AI TOOL - XAVIER PIERCE

// reads shared memory segment and reports on perfect numbers found and for each process the number of perfect numbers found, the total number of numbers tested, and the total number of numbers skipped
// -k option: kill all compute processes and terminate manage

#include "defs.h"
#include <getopt.h>



int main(int argc, char *argv[]){

    int sid;	/* segment id of shared memory segment */
    shmem* shm_array;	/* pointer to shared array, no storage yet*/
    int semid;	/* semaphore id */
    int qid;
    struct computeProcess* processArray;

    //get shared memory segment
    if ((sid=shmget(SHM_KEY,sizeof(shmem),IPC_CREAT | 0666))== -1) {
        perror("Error getting shared memory segment (shmget)");
        exit(3);
    }

    //map shared memory segment into address space
    if ((shm_array=((shmem *) shmat(sid,0,0)))== (shmem *) -1) {
        perror("Error attaching shared memory segment (shmat)");
        exit(3);
    }



    int option;
    int k_flag = 0;
    while ((option = getopt(argc, argv, "k")) != -1) {
        switch (option) {
            case 'k':
                //set flag and handle
                k_flag = 1;
                break;
                    
            default:
                printf("Usage: report [-k]\n");
                exit(3);
            }

        }

    if (k_flag) {
        
        //print report
        printf("Perfect Numbers Found: ");
        for (int i = 0; i < 20; i++) {
            if (shm_array->perfectNumbers[i] != 0) {
                printf("%d ", shm_array->perfectNumbers[i]);
            }
        }
        printf("\n");
        int sumFound = 0;
        int sumTested = 0;
        int sumSkipped = 0;

        for (int i =0; i < 20; i++){
            if (shm_array->processArray[i].pid != 0) {
                sumFound += shm_array->processArray[i].numFound;
                sumTested += shm_array->processArray[i].numTested;
                sumSkipped += shm_array->processArray[i].numSkipped;
                printf("pid(%d): found: %d, tested: %d, skipped: %d\n", shm_array->processArray[i].pid, shm_array->processArray[i].numFound, shm_array->processArray[i].numTested, shm_array->processArray[i].numSkipped);
            }
        }
        printf("Statistics:\n");
        printf("Total found: %d ", sumFound);
        printf("Total tested: %d ", sumTested);
        printf("Total skipped: %d\n", sumSkipped);

        //send SIGINT to manage
        kill(shm_array->managePID, SIGINT);

        //terminate
        exit(0);
    } else {
        //print report
        printf("Perfect Numbers Found: ");
        for (int i = 0; i < 20; i++) {
            if (shm_array->perfectNumbers[i] != 0) {
                printf("%d ", shm_array->perfectNumbers[i]);
            }
        }
        printf("\n");
        int sumFound = 0;
        int sumTested = 0;
        int sumSkipped = 0;

        for (int i =0; i < 20; i++){
            if (shm_array->processArray[i].pid != 0) {
                sumFound += shm_array->processArray[i].numFound;
                sumTested += shm_array->processArray[i].numTested;
                sumSkipped += shm_array->processArray[i].numSkipped;
                printf("pid(%d): found: %d, tested: %d, skipped: %d\n", shm_array->processArray[i].pid, shm_array->processArray[i].numFound, shm_array->processArray[i].numTested, shm_array->processArray[i].numSkipped);
            }
        }
        printf("Statistics:\n");
        printf("Total found: %d ", sumFound);
        printf("Total tested: %d ", sumTested);
        printf("Total skipped: %d \n", sumSkipped);

        //terminate
        exit(0);
    }
}
