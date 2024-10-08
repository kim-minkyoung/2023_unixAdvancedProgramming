#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

void printids(const char *s)
{ 
	  pid_t pid;
	  pthread_t tid;
	  pid = getpid();
	  tid = pthread_self();
	  printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid, (unsigned int)tid, (unsigned int)tid);
}

void * thr_fn(void *arg)
{ 
	printids("new thread: ");
  	return((void *)0);
}

int main(void)
{ 

	int err;
	pthread_t ntid;

	err = pthread_create(&ntid, NULL, thr_fn, NULL);
	
	if (err != 0) { 
		fprintf(stderr, "can't create thread: %s\n", strerror(err));
		exit(-1);
	}
	
	printids("main thread:");
	sleep(1);
	
	exit(0);
} 
