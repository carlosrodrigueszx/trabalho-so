#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>

#define NUM_THREADS 2

#define SIZE 100000000

int array[SIZE];

unsigned int resultado = 0;

pthread_mutex_t mutex;

sem_t empty;
sem_t full;

int item[10] = {0};
int idx = 0; 

void *producer(void *args) {
    int i = 20;

    while(i-- > 0) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        item[idx++] = rand() % 10;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }

    pthread_exit(NULL);
}

void *consumer(void *args) {    
    int i = 20;

    while(i-- > 0) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        printf("%d\n", item[idx--]);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    /* Criar mutex */
    pthread_mutex_init(&mutex, NULL);

    sem_init(&empty, 0, 10);
    sem_init(&full, 0, 0);

    pthread_t producer_thread;
    pthread_create(&producer_thread, NULL, &producer, NULL);

    pthread_t consumer_thread;
    pthread_create(&consumer_thread, NULL, &consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    pthread_mutex_destroy(&mutex);
    sem_close(&empty);
    sem_close(&full);

    return EXIT_SUCCESS;
}
