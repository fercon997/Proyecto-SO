

void leeArchivo(FILE *A, int proceso, int cantidadDeLineas){
  char str[12];
  sprintf(str, "%d.txt", proceso);
  printf("%s\n", str);
  FILE *salida = fopen(str, "w");
  int entrada = 0;
  if (!A) printf("\n\n          El archivo esta vacio!");
  else {
    for(int i = 0; i < cantidadDeLineas; i++){
      fscanf(A, "%d", &entrada);
      //printf("%d", entrada);
      fprintf(salida,"%d  %d\n", entrada, esPrimo(entrada));
    }
  }
  fclose(salida);
}

void *leeArchivoHilos(void *args ){
  parametros *par = (parametros *) args;
  int m = par->m, n = par->n, i = par->i;
  FILE *archivoEntrada = par->entrada;
  //printf("i: %d\n",i);
  leeArchivo(archivoEntrada, i+1, lineasProcesar(i, m, n, "hilo"));
}

void leeArchivoN(FILE *A, int proceso, int cantidadDeLineas , int *cont){
  int entrada = 0;
  if (!A)
    printf("\n\n          El archivo esta vacio!");
  else {
    for(int i = 0; i < cantidadDeLineas; i++){
      fscanf(A, "%d", &entrada);
      //printf("%d", entrada);
      //fprintf(salida,"%d  %d\n", entrada, esPrimo(entrada));
      *cont += esPrimo(entrada);
      //printf("Contador: %d\n", *cont );
    }
  }
}

void *leeArchivoHilosN(void *args ) {
  parametros *par = (parametros *) args;
  int m = par->m, n = par->n, i = par->i, cont = 0;
  FILE *archivoEntrada = par->entrada;
  //printf("i: %d\n",i);
  leeArchivoN(archivoEntrada, i+1, lineasProcesar(i, m, n, "hilo"), &cont);
  printf("Cantidad de numeros primos en hilo #%i: %i\n", i, cont);
  par->cont = cont;
}
