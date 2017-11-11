#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "funciones.h"
#include "archivos.h"

int main(int argc, char const *argv[]) {
  if (validarEntradaN(argv) == 0) return 0;
  int n = atoi(argv[3]);
  FILE *archivoEntrada = fopen(argv[1], "r");
  int m = cantidadDeNumeros(archivoEntrada);
  parametros pHilos[n];
  for (int i = 0; i<n; i++) pHilos[i].cont=0;
  pthread_t tid[n];
  int cont = 0;
  for(int i= 0; i<n; i++){
      pHilos[i].m = m;
      pHilos[i].n = n;
      pHilos[i].entrada = archivoEntrada;
      pHilos[i].i = i;
      pthread_create(&tid[i],NULL,leeArchivoHilosN,&pHilos[i]);
  }
  for(int i = 0; i<n; i++){
    pthread_join(tid[i], NULL);
  }
  for (int i=0; i<n ; i++){
    cont+=pHilos[i].cont;
  }
  printf("Hay %i numeros primos\n", cont);
fclose(archivoEntrada);

  return 0;
}
