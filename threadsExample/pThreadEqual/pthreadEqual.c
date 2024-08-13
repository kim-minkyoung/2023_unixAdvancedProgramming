// C program to demonstrate working of pthread_equal() 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <pthread.h> 

pthread_t tmp_thread; 
pthread_t thread_one; 

void* func_one(void* ptr) 
{ 
	// in this field we can compare two thread 
	// pthread_self gives a current thread id 
	printf("thread one: %ld\n", thread_one);
	printf("thread one itself: %ld\n", pthread_self());

	tmp_thread = pthread_self();
	if (pthread_equal(thread_one, tmp_thread) != 0) { 
		printf("equal\n"); 
	} else { 
		printf("not equal\n"); 
	} 
} 

// driver code 
int main() 
{ 
	// thread one 

	// assign the id of thread one in temporary 
	// thread which is global declared r 

	printf("main thread: %ld\n", pthread_self());
	// create a thread 
	pthread_create(&thread_one, NULL, func_one, NULL); 

	// wait for thread 
	pthread_join(thread_one, NULL); 
} 

