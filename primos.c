#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include  <sys/types.h>
#include <signal.h>
#include "funciones.h"
#include "archivos.h"

#define   BUF_SIZE   100


void *prueba(void *args){
  int i = *((int*)args);
  printf("Hola. Hilo %d\n",i );
}

int main(int argc, char const *argv[]) {
  if (validarEntrada(argv) == 0) return 0;
  FILE *archivoEntrada = fopen(argv[1], "r");
  int status;
  char buf[BUF_SIZE];
  int m = cantidadDeNumeros(archivoEntrada);
  int n = atoi(argv[4]);
  if (strcmp(argv[2],"-p") == 0){
    for (int i = 0; i<n; i++){
      int pid = fork();
      if (pid == 0) break;
      if (pid < 0) exit(1);
    //  printf("i: %i   Proceso: %d\n", i, pid);
    if (signal(SIGINT, catch_function) == SIG_ERR) {
      fputs("Error configurando manejador de señales.\n", stderr);
      return EXIT_FAILURE;
  }
      if (i != n-1) {leeArchivo(archivoEntrada, i+1, m/n);
        printf ("%i\n",m/n);
      }
      else {leeArchivo(archivoEntrada, i+1 , (m/n) + (m%n));
        printf("%i\n", (m/n) + (m%n));
      }
    }
  }
  else {
    parametros pHilos[n];
    pthread_t tid[n];
    for(int i= 0; i<n; i++){
        pHilos[i].m = m;
        pHilos[i].n = n;
        pHilos[i].entrada = archivoEntrada;
        pHilos[i].i = i;
        pthread_create(&tid[i],NULL,leeArchivoHilos,&pHilos[i]);
    }
    for(int i = 0; i<n; i++){
      pthread_join(tid[i], NULL);
    }
  }
  fclose(archivoEntrada);

  return 0;
}
