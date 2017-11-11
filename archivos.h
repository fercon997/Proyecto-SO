// #include "funciones.h"

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

void leeArchivoN(FILE *A, int proceso, int cantidadDeLineas , int *cont){
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

void *leeArchivoHilosN(void *args ){
  parametros *par = (parametros *) args;
  int m = par->m, n = par-> n, i = par->i, cont = 0;
  FILE *archivoEntrada = par->entrada;
  //printf("i: %d\n",i);
    if (i != n-1) {leeArchivoN(archivoEntrada, i+1, m/n,&cont);
      printf ("%i\n",m/n);
    }
    else {leeArchivoN(archivoEntrada, i+1 , (m/n) + (m%n),&cont);
      printf("%i\n", (m/n) + (m%n));
    }
    printf("Numeros primos: %i\n", cont);
    par->cont = cont;
}
