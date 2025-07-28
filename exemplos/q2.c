#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_THREADS 2

#define SIZE 100000000

int array[SIZE];

unsigned int resultado = 0;

pthread_mutex_t mutex;

void *task(void *args) {
    int first = *((int *) args);

    unsigned int acc = 0;
    for(int i = first; i < (first + (SIZE/NUM_THREADS)); i++) {
        acc += array[i];
    }

    pthread_mutex_lock(&mutex);
    resultado += acc;
    pthread_mutex_unlock(&mutex);
}

int main() {
    /* Preenchi o array */
    for(int i = 0; i < SIZE; i++) {
        array[i] = i;
    }

    /* Criar mutex */
    pthread_mutex_init(&mutex, NULL);

    /* Criar as threads */
    pthread_t thread[NUM_THREADS];

    int first[NUM_THREADS];

    struct timeval start;
    gettimeofday(&start, NULL);

    for(int i = 0; i < NUM_THREADS; i++) {
        first[i] = i * (SIZE/NUM_THREADS);
        pthread_create(&thread[i], NULL, &task, &first[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread[i], NULL);
    }

    struct timeval end;
    gettimeofday(&end, NULL);

    /* Liberar o mutex */
    pthread_mutex_destroy(&mutex);

    double time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0;

    printf("Resultado = %u | Tempo %lf segundos\n", resultado, time);

    return EXIT_SUCCESS;
}
