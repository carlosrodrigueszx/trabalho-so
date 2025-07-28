#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>

#define NUM_THREADS 2

#define SIZE 100000000

int array[SIZE];

unsigned int resultado = 0;

pthread_mutex_t mutex;
pthread_cond_t cond;

int item[20] = {0};
int idx = 0; 

void *producer(void *args) {
    for(int i = 0; i < 20; i++) {
        pthread_mutex_lock(&mutex);
        item[idx++] = rand() % 10;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }

    pthread_exit(NULL);
}

void *consumer(void *args) {  
    for(int i = 0; i < 20; i++) {
        pthread_mutex_lock(&mutex);
        while(idx == 0) pthread_cond_wait(&cond, &mutex);
        printf("%d\n", item[--idx]);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    /* Criar mutex */
    pthread_mutex_init(&mutex, NULL);

    pthread_cond_init(&cond, NULL);

    pthread_t producer_thread;
    pthread_create(&producer_thread, NULL, &producer, NULL);

    pthread_t consumer_thread;
    pthread_create(&consumer_thread, NULL, &consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return EXIT_SUCCESS;
}
