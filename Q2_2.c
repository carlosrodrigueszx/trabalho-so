#include <bits/time.h>
#include <pthread.h>
#include <sys/time.h>
// #include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_TERMS 2000000000
#define NUM_THREADS 4
#define PARTIAL_NUM_TERMS ((NUM_TERMS)/(NUM_THREADS))

double thrs_sum = 0; // Soma das threads
double total_thr_time = 0; // Tempo total das threads
pthread_mutex_t sum_mutex, time_mutex; 

double partialFormula(int first_term) {
  // A funcao ira processar PARTIAL_NUM_TERMS termos
  const int num_terms = first_term + PARTIAL_NUM_TERMS;

  // Aproxima o pi, de first_therm ate num_terms - 1
  double pi_approximation = 0; 
  double signal = 1.0;

  for (int k = first_term; k < num_terms; k++) {
    pi_approximation += signal / (2 * k + 1);
    signal *= -1.0;
  }

  return pi_approximation;
}

void* partialProcessing(void* args) {
  int first_term = *((int*) args);
  struct timespec start, end;

  // obter tempo de inicio
  clock_gettime(CLOCK_MONOTONIC, &start);
  double sum = partialFormula(first_term);

  // acessar buffer compartilhado
  pthread_mutex_lock(&sum_mutex);
  thrs_sum += sum;
  pthread_mutex_unlock(&sum_mutex);

  // obter tempo de fim
  clock_gettime(CLOCK_MONOTONIC, &end);
  double total_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)*1e-9;

  // acessar buffer compartilhado de tempo
  pthread_mutex_lock(&time_mutex);
  total_thr_time += total_time;
  pthread_mutex_unlock(&time_mutex);

  // mostrar TID e tempo empregado
  printf("TID: %lu: %.2fs\n", (unsigned long) pthread_self(), total_time);
  pthread_exit(NULL);
}

int main(void) {
  double f_pi = 0, s_pi = 0; 
  struct timespec start, end;

  clock_gettime(CLOCK_MONOTONIC, &start);
  double signal = 1.0;
  for (int k = 0; k < NUM_TERMS; k++) {
    f_pi += signal / (2 * k + 1);
    signal *= -1.0;
  }
  f_pi *= 4;
  clock_gettime(CLOCK_MONOTONIC, &end);

  // Calcula o tempo de execução em segundos
  double total_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;
  printf("Tempo Processo (Sequencial): %.2fs\n", total_time);

  pthread_mutex_init(&sum_mutex, NULL);      // inicializa o mutex que protege o thrs_sum
  pthread_mutex_init(&time_mutex, NULL); // inicializa o mutex que protege o total_thr_time
  pthread_t threads[NUM_THREADS];
  int first[NUM_THREADS];

  // obter tempo de inicio
  clock_gettime(CLOCK_MONOTONIC, &start);
  for (int t = 0; t < NUM_THREADS; t++) {
    first[t] = t * PARTIAL_NUM_TERMS;
    // criar threads parciais
    pthread_create(&threads[t], NULL, &partialProcessing, &first[t]);
  }

  // esperar threads terminarem
  for (int t = 0; t < NUM_THREADS; t++) {
    pthread_join(threads[t], NULL);
  }
  s_pi = thrs_sum * 4;

  // obter tempo de fim
  clock_gettime(CLOCK_MONOTONIC, &end);
  total_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;

  // mostrar resultado e tempo empregado
  printf("Total Processo (Paralelo): %.2fs\n", total_time);
  printf("Total Threads: %.2fs\n", total_thr_time);
  printf("pi (Sequencial) = %.9f, pi (Paralelo) = %.9f\n", f_pi, s_pi);

  pthread_mutex_destroy(&sum_mutex);
  pthread_mutex_destroy(&time_mutex);
  return 0;
}

