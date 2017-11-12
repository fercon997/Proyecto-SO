#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <errno.h>          /* errno, ECHILD            */
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */
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
	printf ("  Child(%d) is in critical section.\n", i);
  sleep (1);
}

void up(Semaforo *sem)	{
	int n = sem_post(sem);
	if (n != 0) perror("sem_post failed");
}

int leeArchivo(FILE *A, int *tiempo, char *direccion){
  A = fopen("semaforo.txt", "r");
  int i = 0;
  if (!A) printf("\n\n          El archivo esta vacio!");
  else{
    while(!feof(A)){
      fscanf(A,"%i %c", &tiempo[i], &direccion[i]);
        //printf("i: %d, Request: %i %c\n", i, tiempo[i], direccion[i]);
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

void BisicRight(int tiempo, char direccion) {
	printf("  %i %c", tiempo, direccion);
}

int main (int argc, char **argv){
    int i;                        /*      loop variables          */
    key_t llave;                 /*      shared memory key       */
    int shmid;                    /*      shared memory id        */
    Semaforo *derecha;            /*      synch semaphore         *//*shared */
		Semaforo *izquierda;
		Semaforo *mutex;
    pid_t pid;                    /*      fork pid                */
    // int *p;                       /*      shared variable         *//*shared */
		FILE *entrada = NULL;
		int tiempo[100];
		char direccion[100];

		int lineas = leeArchivo(NULL, tiempo, direccion);
		printf("Cantidad de Lineas: %d\n", lineas);
		ordenar(tiempo, direccion, lineas);

    /* initialize a shared variable in shared memory */
    llave = ftok ("/dev/null", 5);       /* valid directory name and a number */
    printf ("Llave para p = %d\n", llave);
    shmid = shmget (llave, sizeof (int), 0644 | IPC_CREAT);
    if (shmid < 0){                           /* shared memory error check */
        perror ("shmget\n");
        exit (1);
    }		
		
    // p = (int *) shmat (shmid, NULL, 0);   /* attach p to shared memory */
    // *p = 0;
    // printf ("p=%d is allocated in shared memory.\n\n", *p);

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
        //shmdt (p);
        //shmctl (shmid, IPC_RMID, 0);

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
				char last = direccion[0];
				for (int j = 0; j < lineas; j++) {
					if ((direccion[j] == 'R') && (last == 'R')) {
        		down(derecha, i);           /* P operation */
						down(mutex, i);
						BisicRight(tiempo[j], direccion[j]);
        		up(mutex);
        		up(derecha);           /* V operation */
						last = 'R';
					} else {
						down(izquierda, i);           /* P operation */
						down(mutex, i);
						BisicRight(tiempo[j], direccion[j]);
        		up(mutex);
        		up(derecha);           /* V operation */
						last = 'L';
					}
				}
        exit (0);
    }
}
