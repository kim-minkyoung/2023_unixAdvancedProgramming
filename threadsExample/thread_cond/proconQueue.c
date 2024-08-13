#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// define Queue size
#define QUEUESIZE 10

// define # of iterations
#define LOOP 20

int ret_count;

typedef struct {
    int buf[QUEUESIZE];
    int head;
    int tail;
    int full;
    int empty;
    pthread_mutex_t *mut;
    pthread_cond_t *notFull;
    pthread_cond_t *notEmpty;
} queue;

queue *queueInit(void);
void queueDelete(queue *q);         // Destroy queue
void queueuAdd(queue *q, int in);   // Add date
void queueuDel(queue *q, int *out); // fetch data

//Threads

void *producer(void *args);
void *consumer(void *args);

queue *queueInit(void)
{
    queue *q;
   
    // Create an initial queue 
    q = (queue *)malloc(sizeof(queue));
    if (q == NULL) 
        return NULL;

    q->empty = 1;   // 1 means Yes or True
    q->full = 0;    // 0 means No or False
    q->head = 0;
    q->tail = 0;
    q->mut = (pthread_mutex_t *) malloc(sizeof (pthread_mutex_t));
    ret_count = pthread_mutex_init(q->mut, NULL);
    if (ret_count) {
        fprintf(stderr, \
                "ERROR: Return code from pthread_mutex_init() is %d\n", \
                ret_count);
        exit(-1);
    }

    q->notFull = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(q->notFull, NULL);
    q->notEmpty = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(q->notEmpty, NULL);
    
    return q;
}

void queueDelete(queue * q)
{
    ret_count = pthread_mutex_destroy(q->mut);
    if (ret_count) {
        fprintf(stderr, \
                "ERROR: Return code from pthread_mutex_destroy() is %d\n", \
                ret_count);
        exit(-1);
    }
    free(q->mut);
    pthread_cond_destroy(q->notFull);
    free(q->notFull);
    pthread_cond_destroy(q->notEmpty);
    free(q->notEmpty);
    free(q);
}

void queueAdd(queue *q, int in)
{
    q->buf[q->tail] = in;
    q->tail++;
    if (q->tail == QUEUESIZE)
        q->tail = 0;
    if (q->tail == q->head)
        q->full = 1;
    q->empty = 0;
}

void queueDel(queue *q, int *out)
{
    *out = q->buf[q->head];
    q->head++;
    if (q->head == QUEUESIZE)
        q->head = 0;
    if (q->head == q->tail)
        q->empty = 1;
    q->full = 0;
}

void *producer(void *q)
{
    queue *fifo;
    int i;
    fifo = (queue *)q;

    for (i = 0; i < LOOP; i++) {

        pthread_mutex_lock(fifo->mut);
        while (fifo->full) {
            printf("producer: queue FULL. \n");
            pthread_cond_wait(fifo->notFull, fifo->mut);
        }
        queueAdd(fifo, i);
        pthread_mutex_unlock(fifo->mut);
        pthread_cond_signal(fifo->notEmpty);
        printf("producer: add %d\n", i);
        sleep(3);
    }

    return NULL;
}

void *consumer(void *q)
{
    queue *fifo;
    int i, d;

    fifo = (queue *)q;

    for (i = 0; i < LOOP; i++) {

        pthread_mutex_lock(fifo->mut);
        while (fifo->empty) {
            printf("producer: queue EMPTY. \n");
            pthread_cond_wait(fifo->notEmpty, fifo->mut);
        }
        queueDel(fifo, &d);
        pthread_mutex_unlock(fifo->mut);
        pthread_cond_signal(fifo->notFull);
        printf("producer: delete %d\n", d);
        sleep(5);
    }

    return NULL;
}

int main()
{

    queue *fifo;
    pthread_t pro, con;

    // Queue initialization
    fifo = queueInit();
    if (fifo == NULL) {
        fprintf(stderr, "main: Queue Initialization Failed.\n");
        exit(-1);
    }

    // Thread creation: pro & con
    ret_count = pthread_create(&pro, NULL, producer, fifo);
    if (ret_count) {
        fprintf(stderr, \
                "ERROR: Return code from pthread_create(&pro) is %d \n", \
                ret_count);
        exit(-1);
    }


    ret_count = pthread_create(&con, NULL, consumer, fifo);
    if (ret_count) {
        fprintf(stderr, \
                "ERROR: Return code from pthread_create(&con) is %d\n", \
                ret_count);
        exit(-1);
    }

    ret_count = pthread_join(pro, NULL);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_join(pro) is %d \
                \n", ret_count);
        exit(-1);
    }

    ret_count = pthread_join(con, NULL);
    if (ret_count) {
        fprintf(stderr, "ERROR: Return code from pthread_join(con) is %d \
                \n", ret_count);
        exit(-1);
    }

    queueDelete(fifo);

    return 0;
}
