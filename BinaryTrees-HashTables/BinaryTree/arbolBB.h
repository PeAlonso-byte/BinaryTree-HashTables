#ifndef __ARBOLBB_H
#define __ARBOLBB_H 


	typedef char tipoClave;
	typedef int tipoInfo;

	typedef struct tipoNodo {
	        tipoClave clave[19];
			tipoInfo n_repetido;
	        struct tipoNodo *izq,*der;
	} tipoNodo;

	typedef tipoNodo *tipoArbolBB;
	
	typedef struct tipoAlumno{
		char dni[9];
		char nombre[19];
		char ape1[19];
		char ape2[19];
		char provincia[11];
	} tipoAlumno;

	// Funciones proporcionadas en arbolBB.c

	tipoNodo *creaNodo(tipoClave clave[19]);
	void enOrden(tipoArbolBB raiz);
	void preOrden(tipoArbolBB raiz);
	void posOrden(tipoArbolBB raiz);
	int genABB(char *ficheroDatos, tipoArbolBB *arbol);
	int insertar(tipoArbolBB *raiz,tipoClave *clave, tipoInfo repetido);

#endif

