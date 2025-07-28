#include <stdio.h>
#include <pthread.h>

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

  return 0;
}
