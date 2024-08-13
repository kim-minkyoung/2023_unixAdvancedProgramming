#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Global is O.K
// int retval = 999;

void *thread_function(void *data)
{
    static int retval = 999;
    // Not static is not O.K
    //int retval = 999;
    int i = 0;
    int mycount = *((int *)data);

    while(1) {
	if (i == mycount) {
            pthread_exit((void*)&retval);
        }
        printf("Thread Running... %d: data = %d\n", i, mycount);
        i++;
        sleep(1);
    }
}

int main()
{
    pthread_t p_thread;
    void *threadret = NULL;
    int ret;
    int count = 3;

    ret = pthread_create(&p_thread, NULL, thread_function, (void *)&count);
    if (ret < 0) {
        perror("thread create error: ");
    }

    pthread_join(p_thread, &threadret); 
    printf("thread exit code: %d\n", *((int *)threadret));
    printf("main thread is finished...\n");
}
