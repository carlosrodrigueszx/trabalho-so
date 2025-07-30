#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5
#define NUM_PRODUTORAS 3

float buffer[BUFFER_SIZE];
int in = 0, count = 0;
int produtoras_finalizadas = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_cheio = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_vazio = PTHREAD_COND_INITIALIZER;

void *producer(void *args) {
  pthread_t tid = pthread_self();

  int iteracoes;
  do {
    iteracoes = (rand() % 11) + 20;
  } while (iteracoes % 5 != 0);

  for (int i = 0; i < iteracoes; i++) {
    float valor = (rand() % 1000) + 1;

    pthread_mutex_lock(&mutex);
    while (count == BUFFER_SIZE)
        pthread_cond_wait(&cond_vazio, &mutex);

    buffer[in] = valor;
    int pos = in;
    in = (in + 1) % BUFFER_SIZE;
    count++;

    pthread_cond_signal(&cond_cheio);

    printf("(P) TID: %ld | VALOR: R$ %.0f | ITERACAO: %d\n", tid, valor, i + 1);

    pthread_mutex_unlock(&mutex);

    sleep(rand() % 5 + 1);
  }

  pthread_mutex_lock(&mutex);
  produtoras_finalizadas++;
  if (count == BUFFER_SIZE) pthread_cond_signal(&cond_cheio);
  pthread_mutex_unlock(&mutex);

  printf("(P) TID: %ld finalizou\n", tid);

  return NULL;
}

void *consumer(void *args) {
  pthread_t tid = pthread_self();
  int iteracao = 1;

  while (1) {
    pthread_mutex_lock(&mutex);

    while (count < BUFFER_SIZE && produtoras_finalizadas < NUM_PRODUTORAS)
      pthread_cond_wait(&cond_cheio, &mutex);

    if (count < BUFFER_SIZE && produtoras_finalizadas == NUM_PRODUTORAS) {
      pthread_mutex_unlock(&mutex);
      break;
    }

    float valores[BUFFER_SIZE];
    float soma = 0;

    for (int i = 0; i < BUFFER_SIZE; i++) {
      int idx = (in - count + BUFFER_SIZE + i) % BUFFER_SIZE;
      valores[i] = buffer[idx];
      soma += valores[i];
    }

    count = 0;
    pthread_cond_broadcast(&cond_vazio);
    pthread_mutex_unlock(&mutex);

    printf("(C) TID: %ld | MEDIA: R$ %.0f | ITERACAO: %d\n", tid, soma / BUFFER_SIZE, iteracao++);
  }

  printf("(C) TID: %ld finalizou\n", tid);
  return NULL;
}

int main(void) {
  srand(time(NULL));

  pthread_t produtoras[NUM_PRODUTORAS];
  pthread_t consumidor;

  for (int i = 0; i < NUM_PRODUTORAS; i++) {
    pthread_create(&produtoras[i], NULL, producer, NULL);
  }

  pthread_create(&consumidor, NULL, consumer, NULL);

  for (int i = 0; i < NUM_PRODUTORAS; i++)
    pthread_join(produtoras[i], NULL);

  pthread_join(consumidor, NULL);

  return 0;
}
