#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include "funciones.h"
#include "archivos.h"

#define N 10

struct lista_de_procesos {
	int pid;
	struct lista_de_procesos *prox;
};

typedef struct {
	int val;
	struct lista_de_procesos procesos;
} sem;

void insertarProceso(struct lista_de_procesos **p, int pid) {
	struct lista_de_procesos *t;
	if (!(*p)) {
		t = malloc(sizeof(*t));
		t->prox = NULL;
		*p = t;
	} else {
		t = *p;
		while (t->prox)
			t = t->prox;
		struct lista_de_procesos *s = malloc(sizeof(*s));
		s->prox = t->prox;
		t->prox = s;
		t = s;
	}
	t->pid = pid;
}

void eliminar(struct lista_de_procesos **p) {
	struct lista_de_procesos *t = *p;
	*p = (*p)->prox;
	free(t);
	t = (*p);
}

void eliminarProceso(struct lista_de_procesos **p, int pid) {
	struct lista_de_procesos *t = *p;
	if (*p) {
		if (t->pid == pid) {
			eliminar(p);
		} else {
			while ((t->prox) && (t->prox->pid != pid))
				t = t->prox;
			if (t->prox) {
				struct lista_de_procesos *aux = t->prox;
				t->prox = aux->prox;
				free(aux);
			}
		}
	}
}

void down(sem *s, int pid, struct lista_de_procesos *p) {
	s->val = s->val - 1;
	if (s->val < 0) {
		insertarProceso(p, pid);
		sleep(1);
	}	
}

void up(sem *s, int pid, struct lista_de_procesos *p) {
	s->val = s->val + 1;
	if (s->val <= 0) {
		eliminarProceso(p, pid);
		wake(/*algo*/);
}

void leerArchivo(int lineas, FILE *A) {
	int t;
	char direccion;
	for (int i = 0; i < lineas; i++) {
		fscanf(A, "%d", &t);
		fscanf(A, "%s", &direccion);
		printf("Linea %d: %d %c", i+1, t, direccion);
	}
}

int main(int argc, char const *argv[]) {
	FILE *archivoEntrada = fopen(argv[1], "r");
	int lineas = cantidadDeNumeros(archivoEntrada);
	leerArchivo(lineas, archivoEntrada);
	return 0;
}
