#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include  <sys/types.h>


#define   BUF_SIZE   100

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
typedef struct dato{
  int n,m, i;
  FILE *entrada;
}parametros;

void *leeArchivoHilos(void *args ){
  parametros *par = (parametros *) args;
  int m = par->m, n = par-> n, i = par->i;
  FILE *archivoEntrada = par->entrada;
  //printf("i: %d\n",i);
    if (i != n-1) {leeArchivo(archivoEntrada, i+1, m/n);
      printf ("%i\n",m/n);
    }
    else {leeArchivo(archivoEntrada, i+1 , (m/n) + (m%n));
      printf("%i\n", (m/n) + (m%n));
    }
}

void *prueba(void *args){
  int i = *((int*)args);
  printf("Hola. Hilo %d\n",i );
}

int main(int argc, char const *argv[]) {
  if (!argv[1]){
    printf("Debe especificar el nombre del archivo despues del programa\n");
    return 0;
  }
  if ((!argv[2]) || ( (strcmp(argv[2],"-p") != 0) && (strcmp(argv[2],"-h")) )){
     printf("Debe especificar si quiere ejecura el programa con hilos o procesos.\n");
     printf("Utilice -p o -h\n");
     return 0;
  }
  char const *str = argv[3];
  if ( (!str) || (strcmp(str,"-n") != 0) ){
    printf("Debe indicar el numero de procesos o hilos a ejecutar. Maximo 10.\n");
    printf("Ejemplo: -n 5\n");
    printf("%s\n",str);
    printf("%d\n", strcmp("-n",str));
    return 0;
  }
  if(!argv[4]){
    printf("Debe indicar la cantidad de procesos. Maximo 10\n");
    return 0;
  }
  int n = atoi(argv[4]);
  printf("n: %i\n", n);
  if ((n<1) || (n>10)){
    printf("No puede haber menos de un proceso ni mas de 10\n");
    return 0;
  }
  FILE *archivoEntrada = fopen(argv[1], "r");
  int status;
  char buf[BUF_SIZE];
  int m = cantidadDeNumeros(archivoEntrada);
  if (strcmp(argv[2],"-p") == 0){
    for (int i = 0; i<n; i++){
      int pid = fork();
      if (pid == 0) break;
      if (pid < 0) exit(1);
    //  printf("i: %i   Proceso: %d\n", i, pid);
      if (i != n-1) {leeArchivo(archivoEntrada, i+1, m/n); printf ("%i\n",m/n);}
      else {leeArchivo(archivoEntrada, i+1 , (m/n) + (m%n)); printf("%i\n", (m/n) + (m%n));}
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
