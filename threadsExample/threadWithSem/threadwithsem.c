#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREAD_NUM  2

int count = 0;
sem_t mysem;

void *thread_function(void *data)
{
    pthread_t   id;
    int         tmp;
    
    id = pthread_self(); 
    printf("Thread %lu Created.\n", (unsigned long int) id);

    while(1) {
        // From... each thread change values...
        //         race competition.....
        // That's why we need synchronization...
        // critical sections
        // Before jumping into critical section, we need a lock
        sem_wait(&mysem);
        tmp = count;
        tmp++;
        sleep(1); //insert code to make another thread run
        count = tmp;
        count++;
        printf("%lu: %d\n", (unsigned long int) id, count);
        // to ....  
        // We are done with the critical section
        // Need to release the lock
        sem_post(&mysem);
        sleep(2); //insert code to make another thread run
    }
}

int main()
{
    pthread_t p_thread[MAX_THREAD_NUM];
    int ret;
    int status;
    int i = 0;

    // Initialize smaphore
    if (sem_init(&mysem, 0, 1)) {
        perror("sem_init failed\n");
        exit(0);
    }

    for (i = 0; i < MAX_THREAD_NUM; i++) {
        ret = pthread_create(&p_thread[i], NULL, thread_function, (void*)&i);
        if (ret < 0) {
            perror("thread create error: ");
            exit(0);
        }
    }

    pthread_join(p_thread[0], NULL);
    pthread_join(p_thread[1], NULL);

    return 0;   
}
