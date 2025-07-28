#include <stdio.h>
#include <time.h>
#define NUM_TERMS 2000000000
#define NUM_THREADS 16
#define PARTIAL_NUM_TERMS (( NUM_TERMS )/( NUM_THREADS ))

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
  int first_therm = *((int*) args) ;
  // obter tempo de inicio
  int sum = partialFormula ( first_therm ) ;
  // acessar buffer compartilhado
  // obter tempo de fim
  // mostrar TID e tempo empregado
  return NULL;
}

int main(void) {
  clock_t start_time, end_time;
  double cpu_time_used;

  start_time = clock(); // Captura o tempo inicial

  double pi_approximation = 0; 
  double signal = 1.0;
  for (int k = 0; k < NUM_TERMS; k++) {
    pi_approximation += signal / (2 * k + 1);
    signal *= -1.0;
  }
  pi_approximation *= 4;

  // Captura o tempo final
  end_time = clock();

  // Calcula o tempo de execução em segundos
  cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
  printf("Tempo Processo (Sequencial): %.2fs\n", cpu_time_used);

  // obter tempo de inicio
  for(int i = 0; i < NUM_THREADS ; i++) {
    // criar threads parciais
    start_time = clock();
    pthread_t thread;
    pthread_create(&buffer_threads[i], NULL, local_thr, arg)
    pthread_join(local_thr, NULL);
    end_time = clock();
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    // Imprime o ID da thread e o tempo empregado
    printf("TID: %lu: %.2fs\n", local_thr);
  }

  /*
void* minha_thread(void* arg) {
    int id = *(int*)arg;
    printf("Thread %d está rodando\n", id);
    free(arg);  // liberar memória alocada para o argumento
    return NULL;
}

int main() {
    pthread_t buffer_threads[NUM_THREADS];  // buffer de threads

    for (int i = 0; i < NUM_THREADS; i++) {
        int* arg = malloc(sizeof(int)); // argumento exclusivo para cada thread
        *arg = i;
        if ( != 0) {
            perror("Erro ao criar thread");
            return 1;
        }
    }

    // Esperar todas as threads terminarem
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(buffer_threads[i], NULL);
    }

    printf("Todas as threads terminaram.\n");
    return 0;
}
  */  
}
