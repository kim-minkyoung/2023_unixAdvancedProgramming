#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_message_function(void *ptr);

int main()
{

    pthread_t 	thread1, thread2; //stack에다가 local 변수로 선언 => 공간 할당 됨
    int 	thread1Return;
    int 	thread2Return;

    char	*message1 = "Thread1 is running!!";
    char	*message2 = "Thread2 is running!!";
 
    // create threads that will execute function */
    thread1Return = pthread_create(&thread1, NULL, print_message_function,
					(void*) message1); // compile 되는 순간에 자동으로 char* 됨

    thread2Return = pthread_create(&thread2, NULL, print_message_function,
					(void*) message2);
    
    // main이 먼저 죽지 않게 하기 위해 thread를 만나기 전까지 기다리기
    // fork()의 wait()와 유사한 기능
    // thread1과 thread2 둘 중 뭐가 먼저 종료될지 모름
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Thread1 returns: %d\n", thread1Return);
    printf("Thread2 returns: %d\n", thread2Return);
 
    printf("main() thread is finished!!\n"); 

    exit(0);
}

// Function used for threads
void *print_message_function(void *ptr)
{
    char *message;
    
    message = (char *)ptr;
    printf("%s\n", message);
}
