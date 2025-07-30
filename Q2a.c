#include <time.h>
#include <stdio.h>

#define NUM_TERMS 2000000000

int main(void) {
  double f_pi = 0; 

  clock_t start_time = clock(); // Captura o tempo inicial
  double signal = 1.0;
  for (int k = 0; k < NUM_TERMS; k++) {
    f_pi += signal / (2 * k + 1);
    signal *= -1.0;
  }
  f_pi *= 4;
  clock_t end_time = clock(); // Captura o tempo final

  // Calcula o tempo de execução em segundos
  double total_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
  printf("Tempo Processo (Sequencial): %.2fs\n", total_time);
  
  return 0;
}

