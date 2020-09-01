#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbolBB.h"

/*****************************************************************************************
* Funciones GENERALES adaptadas a los dos ejercicios mediante DIRECTIVAS DE COMPILACIÓN  *
/*****************************************************************************************/
tipoNodo *creaNodo(tipoClave clave[19])
{ tipoNodo * nuevo;
  
  //   nuevo =(tipoNodo *)calloc(1, sizeof(tipoNodo));
  if ((nuevo =(tipoNodo *)malloc(sizeof(tipoNodo)))==NULL)
	return NULL;
  else 
  {	strcpy(nuevo->clave, clave);
  	nuevo->izq=NULL;
  	nuevo->der=NULL;
	nuevo->n_repetido = 1;
  return nuevo;
  }
}

void preOrden(tipoArbolBB a)
{ if (a) { 
  	printf("%s \n", a->clave);
  	preOrden(a->izq);
  	preOrden(a->der);
  }
} 

void enOrden(tipoArbolBB a)
{ if (a) { 
	enOrden(a->izq);
   	printf("%s \n", a->clave);
    	enOrden(a->der);
  }
} 

void posOrden(tipoArbolBB a)
{ if (a) {
  	enOrden(a->izq);
  	enOrden(a->der);
   	printf("%s \n", a->clave);
  }
} 

/* PARA PERMITIR LOS DATOS DUPLICADOS: CADA VEZ QUE SE INTENTA INSERTAR UN DATO DUPLICADO SE AUMENTA EL VALOR DEL CAMPO N_REPETIDO, DE ESTA FORMA
   SABEMOS EL NUMERO DE DUPLICADOS QUE HAY EN EL DOCUMENTO SIN LLEGAR A INSERTARLOS.								*/
int genABB(char *ficheroDatos, tipoArbolBB *arbol) {
	int contadorNORepetidas = 0;
	int contadorRepetidas = 0;
	FILE *f; 
	tipoAlumno alumno;

	f = fopen(ficheroDatos, "rb");
	if (f == NULL) {
		printf("Error no se ha podido abrir el fichero\n");
		return -1;
	}
	
	while (1){
		fread(&alumno, sizeof(tipoAlumno), 1, f);
		//printf("Los apellidos son %s -- %s\n", alumno.ape1, alumno.ape2);
		if (insertar(arbol, alumno.ape1, 1) == 0) {
			contadorNORepetidas++;
		} else {
			contadorRepetidas++;
		}
		if (insertar(arbol, alumno.ape2, 1) == 0) {
			contadorNORepetidas++;
		} else {
			contadorRepetidas++;
		}
		if (feof(f)) {
			break;
		}
	}

	return contadorNORepetidas;
}

int insertar(tipoArbolBB *raiz,tipoClave *clave, tipoInfo n_repetido)
{ 
	tipoNodo *nuevo;
	int resultado = 0;
	if (*raiz == NULL) {
		nuevo = creaNodo(clave);
		*raiz = nuevo;
		return 0;
	} else {
		resultado = strcmp(clave, (*raiz)->clave);
		if (resultado == 0) { // Elemento existente
			(*raiz)->n_repetido = (*raiz)->n_repetido + 1;
		} else if (resultado < 0) { // Nuevo es menor que raiz clave.
			insertar(&((*raiz)->izq), clave, n_repetido);
		} else if (resultado > 0) {
			insertar(&((*raiz)->der), clave, n_repetido);
		}
	}
	
  
}

void enOrdenRepetidos(tipoArbolBB a)
{ if (a) { 
	enOrdenRepetidos(a->izq);
   	if (a->n_repetido > 1) printf("%s -> %d\n", a->clave, a->n_repetido);
    enOrdenRepetidos(a->der);
  }
} 

int main() {
    int numeroApellidos = 0;
    tipoArbolBB arbol = NULL;

    numeroApellidos = genABB("alumnos.dat", &arbol);
    printf("El numero de apellidos diferentes en el documento es %d\n", numeroApellidos);

	enOrdenRepetidos(arbol);
    return 0;
}

