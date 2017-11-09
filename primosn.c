#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int esPrimo(int num){
  if ((num == 1) || (num == -1)) return 0;
  int i, primo = 0;
  for(i = 2; i<=num/2; i++)
    if (num % i == 0) return 0;
    return 1;
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

void leeArchivo(FILE *A, int proceso, int cantidadDeLineas , int *cont){
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
        //fprintf(salida,"%d  %d\n", entrada, esPrimo(entrada));
        *cont+=esPrimo(entrada);
        //printf("Contador: %d\n", *cont );
      }
  }
  fclose(salida);
}
typedef struct dato{
  int n,m, i, cont;
  FILE *entrada;
}parametros;

void *leeArchivoHilos(void *args ){
  parametros *par = (parametros *) args;
  int m = par->m, n = par-> n, i = par->i, cont = 0;
  FILE *archivoEntrada = par->entrada;
  //printf("i: %d\n",i);
    if (i != n-1) {leeArchivo(archivoEntrada, i+1, m/n,&cont);
      printf ("%i\n",m/n);
    }
    else {leeArchivo(archivoEntrada, i+1 , (m/n) + (m%n),&cont);
      printf("%i\n", (m/n) + (m%n));
    }
    printf("Numeros primos: %i\n", cont);
    par->cont = cont;
}

int main(int argc, char const *argv[]) {
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
      pthread_create(&tid[i],NULL,leeArchivoHilos,&pHilos[i]);
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
