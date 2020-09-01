#include "dispersion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Crea un fichero hash inicialmente vacio seg�n los criterios especificados en la pr�ctica
// Primera tarea a realizar para  crear un fichero organizado mediante DISPERSI�N
void creaHvacio(char *fichHash)
{ FILE *fHash;
  tipoCubo cubo;
  int j;
  int numCubos =CUBOS+CUBOSDESBORDE;

  memset(&cubo,0,sizeof(cubo));

  fHash = fopen(fichHash,"wb");
  for (j=0;j<numCubos;j++) fwrite(&cubo,sizeof(cubo),1,fHash);
  fclose(fHash);
}
// Lee el contenido del fichero hash organizado mediante el m�todo de DISPERSI�N seg�n los criterios
// especificados en la pr�ctica. Se leen todos los cubos completos tengan registros asignados o no. La
// salida que produce esta funci�n permite visualizar el m�todo de DISPERSI�N

int leeHash(char *fichHash)
{ FILE *f;
  tipoCubo cubo;
  int j,i=0;
  size_t numLee;

   f = fopen(fichHash,"rb");
   rewind(f);
   fread(&cubo,sizeof(cubo),1,f);
   while (!feof(f)){
	for (j=0;j<C;j++) {
        if (j==0)    	printf("Cubo %2d (%2d reg. ASIGNADOS)",i,cubo.numRegAsignados);
        else  	printf("\t\t\t");
	if (j < cubo.numRegAsignados) 
		    printf("\t%s %s %s %s %s\n",
	    		cubo.reg[j].dni,
			cubo.reg[j].nombre,
			cubo.reg[j].ape1,
		  	cubo.reg[j].ape2,
  	                cubo.reg[j].provincia);
	else printf ("\n");
        }
       i++;
       fread(&cubo,sizeof(cubo),1,f);
   }
   fclose(f);
   return i;
}

int creaHash(char *fichEntrada,char *fichHash) {
  int reg_desbordados = 0;
  int bloq_desbordado = 0;
  FILE *f;
  FILE *h;
  int hash = 0;
  tipoAlumno alumno;
  tipoCubo cubo;
  creaHvacio(fichHash);

  f = fopen(fichEntrada, "rb");
  if (f == NULL) {
    printf("Error, el fichero no se ha podido abrir.\n");
    return -1;
  }
  h = fopen(fichHash, "r+");
  if (h == NULL) {
    printf("Error, el fichero hash no se ha podido abrir.\n");
    return -1;
  }

  while (1) {

    fread(&alumno, sizeof(tipoAlumno), 1, f);
    hash = atoi(alumno.dni)%CUBOS;

    fseek(h, hash*sizeof(tipoCubo), SEEK_SET);
    fread(&cubo, sizeof(tipoCubo), 1, h);
    
    if (cubo.numRegAsignados >= 5) {
      cubo.numRegAsignados = cubo.numRegAsignados + 1;
      fseek(h, hash*sizeof(tipoCubo), SEEK_SET);
      fwrite(&cubo, sizeof(tipoCubo), 1, h);
      if ((reg_desbordados != 0) && (reg_desbordados % 5 == 0)) {
        bloq_desbordado++;
      }
      fseek(h, (bloq_desbordado-4)*sizeof(tipoCubo), SEEK_END);
      fread(&cubo, sizeof(tipoCubo), 1, h);
      cubo.reg[cubo.numRegAsignados] = alumno;
      cubo.numRegAsignados = cubo.numRegAsignados + 1;
      reg_desbordados++;
      fseek(h, (bloq_desbordado-4)*sizeof(tipoCubo), SEEK_END);
      fwrite(&cubo, sizeof(tipoCubo), 1, h);

    } else {
      cubo.reg[cubo.numRegAsignados] = alumno;
      cubo.numRegAsignados = cubo.numRegAsignados + 1;
      fseek(h, hash*sizeof(tipoCubo), SEEK_SET);
      fwrite(&cubo, sizeof(tipoCubo), 1, h);
    }
    
    if (feof(f)) {
      break;
    }
  }
  fclose(f);
  fclose(h);
  return reg_desbordados;
}

int buscaReg(FILE *fHash, tipoAlumno *reg,char *dni) {
  int numCubo = 0;
  tipoCubo cubo;
  int hash = 0;
  if (fHash == NULL) {
    printf("El fichero hash no se ha podido abrir.\n");
    return -1;
  }

  hash = atoi(dni)%CUBOS;

  fseek(fHash, hash*sizeof(tipoCubo), SEEK_SET);
  fread(&cubo, sizeof(tipoCubo), 1, fHash);

  for (int i = 0; i < 5; i++) {
    if (strcmp(cubo.reg[i].dni, dni) == 0) { // Registro encontrado.
      numCubo = hash;
      *reg = cubo.reg[i];
      return numCubo;
    } 
  }

  
  if (cubo.numRegAsignados > 5) { // Tiene registros desbordados
    for (int i = 0; i <= 4; i++) {
      fseek(fHash, (i-4)*sizeof(tipoCubo), SEEK_END);
      fread(&cubo, sizeof(tipoCubo), 1, fHash);
      if (cubo.numRegAsignados == 0) {
        break;
      }
      for (int j = 0; j < cubo.numRegAsignados; j++) {
        if (strcmp(cubo.reg[j].dni, dni) == 0) { // Registro encontrado.
          numCubo = (CUBOS+CUBOSDESBORDE)+(i-4);
          *reg = cubo.reg[j];
          return numCubo;
        } 
      }
    }
  }
  return -1;
}

int modificarReg(char *fichero, char *dni, char *provincia) {
  int numCubo = 0;
  int k = 0;
  tipoAlumno reg;
  tipoCubo cubo;
  FILE *f;
  f = fopen(fichero, "r+");
  if (f == NULL) {
    printf("Error, no se ha podido abrir el fichero\n");
    return -1;
  }
  k = buscaReg(f,&reg,dni);

  
  if (k<0) {
    //printf("El dni no existe\n");
    fclose(f);
    return -1;
  }
  strcpy(reg.provincia, provincia);
  fseek(f, k*sizeof(tipoCubo), SEEK_SET);
  fread(&cubo, sizeof(tipoCubo), 1, f);
  for (int i = 0; i < cubo.numRegAsignados; i++) {
    if (strcmp(cubo.reg[i].dni, reg.dni) == 0){
      cubo.reg[i] = reg;
      break;
    }
  }
  fseek(f, k*sizeof(tipoCubo), SEEK_SET);
  fwrite(&cubo, sizeof(tipoCubo), 1, f);
  
  fclose(f);
  return k;
}

