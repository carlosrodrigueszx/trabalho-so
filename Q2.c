#include <stdio.h>
#define NUM_TERMS 2000000000
#define NUM_THREADS 16
#define PARTIAL_NUM_TERMS (( NUM_TERMS ) /( NUM_THREADS ) )

double partialFormula (int first_therm ) {
// A funcao ira processar PARTIAL_NUM_TERMS termos
  const int num_terms = first_therm + PARTIAL_NUM_TERMS ;

  // Aproxima o pi , de first_therm ate num_terms - 1
  double pi_approximation = 0;
  double signal = 1.0;
  for (int k = first_therm ; k < num_terms ; k ++) {
    pi_approximation += signal /(2* k + 1) ;
    signal *= -1.0;
  }

  return pi_approximation ;
}

void* partialProcessing ( void * args ) {
  int first_therm = *(( int *) args ) ;
  // obter tempo de inicio
  int sum = partialFormula ( first_therm ) ;
  // acessar buffer compartilhado
  // obter tempo de fim
  // mostrar TID e tempo empregado
  return NULL;
}

int main ( void ) {
  double pi = 0;

  // obter tempo de inicio
  for(int i = 0; i < NUM_THREADS ; i++) {
  // criar threads parciais
  }

  for (int i = 0; i < NUM_TERMS; ++i) {
    pi += partialFormula(i);
  }
  // esperar threads terminarem
  // obter tempo de fim
  // mostrar resultado e tempo emprego
}
