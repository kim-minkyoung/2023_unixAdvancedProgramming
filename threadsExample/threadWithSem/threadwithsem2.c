#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

sem_t *st;
  void *fun_t(void *arg) {
       printf("PNUCSE\n");
       sem_post(st);
       pthread_exit("Bye"); 
  }
   
int main() {
       pthread_t pt; 
void *res_t;


pthread_create(&pt,NULL,fun_t,NULL);
    sem_unlink("mysem");
    st=sem_open("mysem",O_CREAT,0777,0);
    sem_wait(st);
    printf("Manse\n");
    pthread_join(pt,&res_t);           
    sem_close(st);
    sem_unlink("mysem");
    return 0; 
}
