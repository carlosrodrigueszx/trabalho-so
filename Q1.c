#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5
#define NUM_PRODUTORAS 3
#define VENDAS_POR_PRODUTOR 5

float sell_values[BUFFER_SIZE];
int in = 0, out = 0, count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_cheio = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_vazio = PTHREAD_COND_INITIALIZER;

void* producer(void* arg) {
  int id = *((int*)arg);
  for (int i = 0; i < VENDAS_POR_PRODUTOR; i++) {
    float valor = (rand() % 1000) / 10.0;  // valor aleatório entre 0 e 100.0

    pthread_mutex_lock(&mutex);

    while (count == BUFFER_SIZE) {
      printf("Produtor %d: buffer cheio, esperando...\n", id);
      pthread_cond_wait(&buffer_vazio, &mutex);
    }

    sell_values[in] = valor;
    printf("Produtor %d: registrou venda R$ %.2f na posição %d\n", id, valor, in);
    in = (in + 1) % BUFFER_SIZE;
    count++;

    pthread_cond_signal(&buffer_cheio);
    pthread_mutex_unlock(&mutex);

    sleep(1);  // simula tempo entre vendas
  }

  free(arg);
  return NULL;
}

void* consumer(void* arg) {
  int total_vendas = NUM_PRODUTORAS * VENDAS_POR_PRODUTOR;
  for (int i = 0; i < total_vendas; i++) {
    pthread_mutex_lock(&mutex);

    while (count == 0) {
      printf("Consumidor: buffer vazio, esperando...\n");
      pthread_cond_wait(&buffer_cheio, &mutex);
    }

    float valor = sell_values[out];
    printf("Consumidor: processou venda de R$ %.2f na posição %d\n", valor, out);
    out = (out + 1) % BUFFER_SIZE;
    count--;

    pthread_cond_signal(&buffer_vazio);
    pthread_mutex_unlock(&mutex);

    sleep(2);  // simula tempo de processamento
  }

  return NULL;
}

int main(void) {
  srand(time(NULL));

  pthread_t produtores[NUM_PRODUTORAS];
  pthread_t consumidor;

  // Criar thread consumidora
  pthread_create(&consumidor, NULL, consumer, NULL);

  // Criar threads produtoras
  for (int i = 0; i < NUM_PRODUTORAS; i++) {
    int* id = malloc(sizeof(int));
    *id = i + 1;
    pthread_create(&produtores[i], NULL, producer, id);
  }

  // Esperar produtores terminarem
  for (int i = 0; i < NUM_PRODUTORAS; i++) {
    pthread_join(produtores[i], NULL);
  }

  // Esperar consumidor terminar
  pthread_join(consumidor, NULL);

  return 0;
}
