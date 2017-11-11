static void catch_function(int signo) {
    puts("I'm sorry Dave, I can't do that.");
}

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
  printf("Total de lineas: %i\n",cont-1);
  rewind(A);
  return cont -1 ;
}

typedef struct dato{
  int n, m, i, cont;
  FILE *entrada;
}parametros;


int validarEntrada(const char *argv[]) {
  if (!argv[1]){
    printf("Debe especificar el nombre del archivo de entrada despues del programa\n");
    return 0;
  }
  if ((!argv[2]) || ((strcmp(argv[2],"-p")) && (strcmp(argv[2],"-t")))) {
     printf("Debe especificar si quiere ejecura el programa con hilos o procesos.\n");
     printf("Utilice -t o -p\n");
     return 0;
  }
  char const *str = argv[3];
  if ((!str) || (strcmp(str,"-n") != 0)) {
    printf("Debe indicar el numero de procesos o hilos a ejecutar. Maximo 10.\n");
    printf("Ejemplo: -n 5\n");
    // printf("%s\n",str);
    // printf("%d\n", strcmp("-n",str));
    return 0;
  }
  if(!argv[4]) {
    printf("Debe indicar el numero de procesos o hilos a ejecutar. Maximo 10.\n");
    printf("Ejemplo: -n 5\n");
    return 0;
  }
  int n = atoi(argv[4]);
  printf("n: %i\n", n);
  if ((n<1) || (n>10)){
    printf("No puede haber menos de un proceso ni mas de 10\n");
    return 0;
  }
}

int lineasProcesar(int i, int m, int n, char str[10]){
  if (i != n-1) {
    printf ("Lineas %s #%i: %i\n",str, i, m/n);
    return m/n;
  }
  else {
    printf("Lineas %s #%i: %i\n", str, i, (m/n) + (m%n));
    return (m/n) + (m%n);
  }
}
