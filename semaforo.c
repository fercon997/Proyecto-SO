#include <stdio.h>          
#include <stdlib.h>         
#include <sys/types.h>      
#include <sys/shm.h>        
#include <errno.h>          
#include <semaphore.h>      
#include <fcntl.h>          
#include <sys/wait.h>
#include <unistd.h>

typedef sem_t Semaforo;

Semaforo *crear_semaforo(char name[], int value) {
	Semaforo *sem = sem_open (name, O_CREAT | O_EXCL, 0644, value);
	return sem;
}

void down(Semaforo *sem, int i) {
	int n = sem_wait(sem);
	if (n != 0) perror("sem_wait failed");
	printf ("  Proceso#%d esperando.\n", i);
  sleep (1);
}

void downMutex(Semaforo *sem, int i) {
	int n = sem_wait(sem);
	if (n != 0) perror("sem_wait failed");
	printf ("  Proceso#%d entro a la region critica\n", i);
  sleep (1);
}

void up(Semaforo *sem)	{
	int n = sem_post(sem);
	if (n != 0) perror("sem_post failed");
}

void upMutex(Semaforo *sem, int i)	{
	int n = sem_post(sem);
	if (n != 0) perror("sem_post failed");
	printf("  Proceso#%d salio de la region critica\n", i);
}

void BicisRight(char direccion, int *tiempo1, int tiempo2, int proceso, char *last) {
	if ((*tiempo1 == tiempo2) && (*last == 'R') && (proceso == 0)) {
		*last = 'R';
		//*tiempo1 = *tiempo1 + 1;
		printf("\n  Proceso#%i  %i %c\n\n", proceso, *tiempo1, direccion);
	} else if ((direccion == 'R') && (proceso == 0)) {
		printf("\n  Proceso#%i  %i %c\n\n", proceso, *tiempo1, direccion);
		*last = 'R';
	}
	//printf("Proceso %i. last: %c\n", proceso, *last);
}

void BicisLeft(char direccion, int *tiempo1, int tiempo2, int proceso, char *last) {
	if ((*tiempo1 == tiempo2) && (*last == 'L') && (proceso == 1)) {
		*last = 'L';
		*tiempo1 = *tiempo1 + 1;
		printf("\n  Proceso#%i  %i %c\n\n", proceso, *tiempo1, direccion);
	} else if ((direccion == 'L') && (proceso == 1)) {
		printf("\n  Proceso#%i  %i %c\n\n", proceso, *tiempo1, direccion);
		*last = 'L';
	}
	//printf("Proceso: %i. last: %c\n", proceso, *last);
}

int leeArchivo(FILE *A, int *tiempo, char *direccion){
  A = fopen("semaforo.txt", "r");
  int i = 0;
  if (!A) printf("\n\n          El archivo esta vacio!");
  else{
    while(!feof(A)){
      fscanf(A,"%i %c", &tiempo[i], &direccion[i]);
      i++;
    }
  }
  return i-1;
}

void ordenar(int *tiempo, char *direccion, int cantidadDeLineas) {
	int aux1; char aux2;
	for (int i = 0; i < cantidadDeLineas-1; i++) {
		int j = i + 1;
		while (j < cantidadDeLineas) {
			if (tiempo[i] > tiempo[j]) {
				aux1 = tiempo[i];
				tiempo[i] = tiempo[j];
				tiempo[j] = aux1;
				aux2 = direccion[i];
				direccion[i] = direccion[j];
				direccion[j] = aux2;
			}
			j++;
		}
	}
}

int main (int argc, char **argv){
    int i;                        
    key_t llave;                 /*      shared memory key       */
    int shmid;                    /*      shared memory id        */
    Semaforo *derecha;            /*      synch semaphore         *//*shared */
		Semaforo *izquierda;
		Semaforo *mutex;
    pid_t pid;                    /*      fork pid                */
    char *last;                       /*      shared variable         *//*shared */
		FILE *entrada = NULL;
		int tiempo[100];
		char direccion[100];

		int lineas = leeArchivo(NULL, tiempo, direccion);
		printf("Cantidad de Lineas: %d\n", lineas);
		ordenar(tiempo, direccion, lineas);
		//char last = direccion[0];

    /* initialize a shared variable in shared memory */
    llave = ftok ("/dev/null", 5);       /* valid directory name and a number */
    printf ("Llave para p = %d\n", llave);
    shmid = shmget (llave, sizeof (int), 0644 | IPC_CREAT);
    if (shmid < 0){                           /* shared memory error check */
        perror ("shmget\n");
        exit (1);
    }		
		
    last = (char *) shmat (shmid, NULL, 0);   /* attach p to shared memory */
    *last = direccion[0];
    printf ("p=%d is allocated in shared memory.\n\n", *last);

    /********************************************************/


    /* initialize semaphores for shared processes */
    derecha = crear_semaforo("derecha", 10);
		izquierda = crear_semaforo("izquierda", 10);
		mutex = crear_semaforo("mutex", 1);
    /* name of semaphore is "pSem", semaphore is reached using this name */

    printf ("Se inicializaron los semaforos.\n\n");


    /* fork child processes */
    for (i = 0; i < 2; i++){
        pid = fork ();
        if (pid < 0) {
        /* check for error      */
            sem_unlink ("derecha");   
            sem_close(derecha); 
						sem_unlink ("izquierda");   
            sem_close(izquierda);
						sem_unlink ("mutex");   
            sem_close(mutex); 
            /* unlink prevents the semaphore existing forever */
            /* if a crash occurs during the execution         */
            printf ("Fork error.\n");
        }
        else if (pid == 0)
            break;                  /* child processes */
    }


    /******************************************************/
    /******************   PARENT PROCESS   ****************/
    /******************************************************/
    if (pid != 0){
        /* wait for all children to exit */
        while (pid = waitpid (-1, NULL, 0)){
            if (errno == ECHILD)
                break;
        }

        printf ("\nParent: All children have exited.\n");

        /* shared memory detach */
        shmdt (last);
        shmctl (shmid, IPC_RMID, 0);

        /* cleanup semaphores */
        sem_unlink ("derecha");   
        sem_close(derecha); 
				sem_unlink ("izquierda");   
        sem_close(izquierda);
				sem_unlink ("mutex");   
        sem_close(mutex);  
        /* unlink prevents the semaphore existing forever */
        /* if a crash occurs during the execution         */
        exit (0);
    }

    /******************************************************/
    /******************   CHILD PROCESS   *****************/
    /******************************************************/
    else{
    	for (int j = 0; j < lineas; j++) {
    		if (i == 0) {
    			//printf("Entrada Derecha 1\n");
    			down(derecha, i);
    			downMutex(mutex, i);
    			//printf("Derecha 1 last antes: %c\n", *last);
    			BicisRight(direccion[j], &tiempo[j+1], tiempo[j], i, last);
    			//printf("Derecha 1 last despues: %c\n", *last);
    			upMutex(mutex, i);
    			up(derecha);
    		} else {
    			//printf("Entrada Izquierda 1\n");
    			down(izquierda, i);
    			downMutex(mutex, i);
    			//printf("Izquierda 1 last antes: %c\n", *last);
    			BicisLeft(direccion[j], &tiempo[j+1], tiempo[j], i, last);
    			//printf("Izquierda 1 last despues: %c\n", *last);
    			upMutex(mutex, i);
    			up(izquierda);
    		}
    	}
        exit (0);
    }
}
