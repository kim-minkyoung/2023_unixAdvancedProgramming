#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREAD_NUM  2

int count = 0;
sem_t mysem;

void *t_function(void *data)
{
    pthread_t id;
    int tmp;
   
    id = pthread_self();
    printf("Thread %lu Created.\n", (unsigned long int) id);
   
    while(1)
    {
        //sem_wait(&mysem);
        
        // Original code
        // Do not control threads with semaphore
        tmp = count;
        tmp = tmp + 1;
        sleep(1);
        count = tmp;
        

        
        // Looks like this code run successfully.. but it's wrong
        count = count + 1;
        
        printf("%lu : %d\n", (unsigned long int) id, count);
        //sem_post(&mysem);
        sleep(2);
    }
}

int main(int argc, char **argv)
{
    pthread_t p_thread[MAX_THREAD_NUM];
    int thr_id;
    int status;
    int i = 0;

    if (sem_init(&mysem, 0, 1) )
    {
        printf("aaa\n");
        perror("Error");
        printf("bbb\n");
        //exit(0);
    }

    for( i = 0; i < MAX_THREAD_NUM; i++)
    {
        // 쓰레드 생성 아규먼트로 2 를 넘긴다.  
        thr_id = pthread_create(&p_thread[i], NULL, t_function, (void *)&i);
        if (thr_id < 0)
        {
            perror("thread create error : ");
            exit(0);
        }
    }
    
    pthread_join(p_thread[0], NULL);
    pthread_join(p_thread[1], NULL);
    
    return 0;
}
