#include <bits/time.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_TERMS 2000000000
#define NUM_THREADS 8
#define PARTIAL_NUM_TERMS ((NUM_TERMS)/(NUM_THREADS))

double thrs_sum = 0;
double total_thr_time = 0;
pthread_mutex_t lock, time_lock; 

double partialFormula(int first_therm) {
  // A funcao ira processar PARTIAL_NUM_TERMS termos
  const int num_terms = first_therm + PARTIAL_NUM_TERMS;

  // Aproxima o pi, de first_therm ate num_terms - 1
  double pi_approximation = 0; 
  double signal = 1.0;

  for (int k = first_therm; k < num_terms; k++) {
    pi_approximation += signal / (2 * k + 1);
    signal *= -1.0;
  }

  return pi_approximation;
}

void* partialProcessing(void* args) {
  int first_therm = *((int*) args);
  free(args);

  // obter tempo de inicio
  struct timespec t0, t1;
  clock_gettime(CLOCK_MONOTONIC, &t0);
  // clock_t start_time = clock();
  double sum = partialFormula(first_therm);

  // acessar buffer compartilhado
  pthread_mutex_lock(&lock);
  thrs_sum += sum;
  pthread_mutex_unlock(&lock);

  // obter tempo de fim
  clock_gettime(CLOCK_MONOTONIC, &t1);
  // clock_t end_time = clock();
  double total_time = (t1.tv_sec  - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec)*1e-9;
  // double total_time2 = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

  // acessar buffer compartilhado de tempo
  pthread_mutex_lock(&time_lock);
  total_thr_time += total_time;
  pthread_mutex_unlock(&time_lock);

  // mostrar TID e tempo empregado
  printf("TID: %lu: %.2fs\n", (unsigned long) pthread_self(), total_time);
  // printf("TID: %lu: clock(): %.2fs\n", (unsigned long) pthread_self(), total_time2);
  pthread_exit(NULL);
}

int main(void) {
  double f_pi = 0, s_pi = 0; 
  struct timespec m0, m1;

  clock_t start_time = clock(); // Captura o tempo inicial
  // clock_gettime(CLOCK_MONOTONIC, &m0); // wall-clock
  double signal = 1.0;
  for (int k = 0; k < NUM_TERMS; k++) {
    f_pi += signal / (2 * k + 1);
    signal *= -1.0;
  }
  f_pi *= 4;
  // clock_gettime(CLOCK_MONOTONIC, &m1);
  clock_t end_time = clock(); // Captura o tempo final

  // Calcula o tempo de execução em segundos
  double total_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
  // double gettime = (m1.tv_sec  - m0.tv_sec) + (m1.tv_nsec - m0.tv_nsec)*1e-9;
  printf("Tempo Processo (Sequencial): %.2fs\n", total_time);
  // printf("Tempo Processo Gettime(Sequencial): %.2fs\n", gettime);

  pthread_t threads[NUM_THREADS];
  pthread_mutex_init(&lock, NULL);      // inicializa o mutex que protege o thrs_sum
  pthread_mutex_init(&time_lock, NULL); // inicializa o mutex que protege o total_thr_time

  // obter tempo de inicio
  start_time = clock();
  // clock_gettime(CLOCK_MONOTONIC, &m0);
  for (int t = 0; t < NUM_THREADS; t++) {
    int* arg = malloc(sizeof(int));
    *arg = t * PARTIAL_NUM_TERMS;
    // criar threads parciais
    pthread_create(&threads[t], NULL, &partialProcessing, arg);
  }

  // esperar threads terminarem
  for (int t = 0; t < NUM_THREADS; t++) {
    pthread_join(threads[t], NULL);
  }
  s_pi = thrs_sum * 4;

  // obter tempo de fim
  end_time = clock();
  total_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
  // clock_gettime(CLOCK_MONOTONIC, &m1);
  // gettime = (m1.tv_sec  - m0.tv_sec) + (m1.tv_nsec - m0.tv_nsec)*1e-9;
  // printf("Tempo Processo Gettime(paralelo): %.2fs\n", gettime);

  // mostrar resultado e tempo empregado
  printf("Total Processo (Paralelo): %.2fs\n", total_time);
  printf("Total Threads: %.2fs\n", total_thr_time);
  printf("pi (Sequencial) = %.9f, pi (Paralelo) = %.9f\n", f_pi, s_pi);

  pthread_mutex_destroy(&lock);
  pthread_mutex_destroy(&time_lock);
  return 0;
}
