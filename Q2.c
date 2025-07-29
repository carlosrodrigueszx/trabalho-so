#include <stdio.h>
#include <time.h>
#define NUM_TERMS 2000000000
#define NUM_THREADS 16
#define PARTIAL_NUM_TERMS ((NUM_TERMS)/(NUM_THREADS))

double total_pi = 0.0;
pthread_mutex_t lock;

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
  clock_t start_time = clock();
  int sum = partialFormula (first_therm);
  pthread_t current_thread_id = pthread_self();

  // acessar buffer compartilhado

  // obter tempo de fim
  clock_t end_time = clock();
  clock_t total_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

  // mostrar TID e tempo empregado
  printf("TID: %lu: %.2fs\n", (unsigned long) current_thread_id, total_time);
  pthread_exit(NULL);
}

int main(void) {
  clock_t start_time = clock(); // Captura o tempo inicial
  double pi_approximation = 0; 
  double signal = 1.0;
  for (int k = 0; k < NUM_TERMS; k++) {
    pi_approximation += signal / (2 * k + 1);
    signal *= -1.0;
  }
  pi_approximation *= 4;

  clock_t end_time = clock(); // Captura o tempo final

  // Calcula o tempo de execução em segundos
  double total_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
  printf("Tempo Processo (Sequencial): %.2fs\n", total_time);

  pthread_t threads[NUM_THREADS];
  // obter tempo de inicio
  start_time = clock();
  for (int t = 0; t < NUM_THREADS; t++) {
    // criar threads parciais
    pthread_create(&threads[t], NULL, partialProcessing, arg)
  }

  for (int t = 0; t < NUM_THREADS; t++) {
    pthread_join(threads[t], NULL);
  }

  end_time = clock();
  total_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
  printf("Total Processo (Paralelo): %.2fs\n", total_time);

  pthread_mutex_destroy(&lock);
  return 0;
}

// >>>>>>>>>>>>>>>>>>>>>>> REFERÊNCIA DO GPT
/*
double total_sum = 0.0;
pthread_mutex_t mutex;

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

double partialFormula(int first_therm) {
    const int num_terms = first_therm + PARTIAL_NUM_TERMS;
    double pi_approximation = 0;
    double signal = (first_therm % 2 == 0) ? 1.0 : -1.0;

    for (int k = first_therm; k < num_terms; k++) {
        pi_approximation += signal / (2 * k + 1);
        signal *= -1.0;
    }

    return pi_approximation;
}

void *partialProcessing(void *args) {
    int first_therm = *((int *) args);
    double start_time = get_time();

    double partial = partialFormula(first_therm);

    pthread_mutex_lock(&mutex);
    total_sum += partial;
    pthread_mutex_unlock(&mutex);

    double end_time = get_time();

    printf("TID: %ld | Tempo: %.2fs\n", pthread_self(), end_time - start_time);

    free(args);
    pthread_exit(NULL);
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);

    double start_time = get_time();

    for (int i = 0; i < NUM_THREADS; i++) {
        int *first = malloc(sizeof(int));
        *first = i * PARTIAL_NUM_TERMS;
        pthread_create(&threads[i], NULL, partialProcessing, first);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    double end_time = get_time();
    pthread_mutex_destroy(&mutex);

    double pi = 4.0 * total_sum;

    printf("Valor de pi: %.9lf\n", pi);
    printf("Tempo total (paralelo): %.2fs\n", end_time - start_time);

    return 0;
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


// pthread_mutex_t lock;
// double total_pi = 0.0;

// typedef struct {
//     int tid;
//     int first_term;
// } ThreadArgs;

// double partialFormula(int first_term) {
//     int last_term = first_term + PARTIAL_NUM_TERMS;
//     double result = 0.0;
//     double signal = (first_term % 2 == 0) ? 1.0 : -1.0;

//     for (int k = first_term; k < last_term; k++) {
//         result += signal / (2 * k + 1);
//         signal *= -1.0;
//     }
//     return result;
// }

// void* partialProcessing(void* args) {
//     ThreadArgs* thread_data = (ThreadArgs*)args;

//     clock_t start = clock();

//     double local_sum = partialFormula(thread_data->first_term);

//     pthread_mutex_lock(&lock);
//     total_pi += local_sum;
//     pthread_mutex_unlock(&lock);

//     clock_t end = clock();
//     double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
//     printf("TID: %d: %.2f s\n", thread_data->tid, time_taken);

//     pthread_exit(NULL);
// }

// int main() {
//     pthread_t threads[NUM_THREADS];
//     ThreadArgs args[NUM_THREADS];

//     pthread_mutex_init(&lock, NULL);
//     clock_t start_time = clock();

//     for (int i = 0; i < NUM_THREADS; i++) {
//         args[i].tid = i;
//         args[i].first_term = i * PARTIAL_NUM_TERMS;
//         pthread_create(&threads[i], NULL, partialProcessing, &args[i]);
//     }

//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_join(threads[i], NULL);
//     }

//     clock_t end_time = clock();
//     double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

//     printf("Valor de pi (paralelo): %.9f\n", 4.0 * total_pi);
//     printf("Tempo total (paralelo): %.2f s\n", total_time);

//     pthread_mutex_destroy(&lock);
//     return 0;
// }

