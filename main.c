#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int esPrimo(int num){
  if ((num == 1) || (num == -1)) return 0;
  int i, primo = 0;
  for(i = 2; i<=num; i++)
    if (num % i == 0) primo++;
  if(primo == 1) return 1;
  return 0;
}

int cantidadDeNumeros(FILE *A){
  int cont = 0, aux;
  if (!A) printf("El archivo esta vacio\n");
  else{
    while(!feof(A)){
      fscanf(A,"%d",&aux);
      cont++;
    }
  }
  printf("%i\n",cont-1);
  rewind(A);
  return cont -1 ;
}

void leeArchivo(FILE *A, int proceso, int cantidadDeLineas){
  char str[12];
  sprintf(str,"%d.txt", proceso);
  printf("%s\n", str);
  FILE *salida = fopen(str, "w");
  int entrada = 12;
  if (!A)
		printf("\n\n          El archivo esta vacio!");
	else{
      for(int i = 0; i<cantidadDeLineas; i++){
        fscanf(A, "%d", &entrada);
        //printf("%d", entrada);
        fprintf(salida,"%d  %d\n", entrada, esPrimo(entrada));
      }
  }
  fclose(salida);
}

int main(int argc, char const *argv[] ,char **env) {
  FILE *archivoEntrada = fopen(argv[1], "r");
  int m = cantidadDeNumeros(archivoEntrada);
  for (int i = 0; i<4; i++){
    int pid = fork();
    if (pid == 0) break;
    //  printf("i: %i   Proceso: %d\n", i, pid);
    if (i != 3) {leeArchivo(archivoEntrada, i+1, m/4); printf ("%i\n",m/4);}
    else {leeArchivo(archivoEntrada, i+1 , (m/4) + (m%4)); printf("%i\n", (m/4) + (m%4));}
  }
  fclose(archivoEntrada);

  return 0;
}
