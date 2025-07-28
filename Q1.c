#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

void* producer(void* args) {
  int n = *((int *) args);
  while (n-- > 0) {}
  return NULL;
}

void* consumer(void* args) {
  while(1) {};
  return NULL;
}

int main(void) {
  srand(time(NULL));
  pthread_t main_thr;
  pthread_t producer_thr;
  pthread_t consumer_thr;
  float sell_values[5];

  for (int i = 0; i < 3; i++) {
    pthread_create(&producer_thr, NULL, producer, NULL);
    pthread_join(producer_thr, NULL);
  }

  int value = rand() % 1000;

  return 0;
}
