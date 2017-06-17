///
///  @file carregador.c
///  @headerfile carregador.h "carregador.h"
///
///  @authors
///  Allisson Barros         12/0055619\n
///  Daniel Luz              13/0007714\n
///  Luiz Fernando Vieira    13/0013757\n
///  Mariana Pannunzio       12/0018276\n
///  Mateus Denucci          12/0053080\n
///
///  @date 26/06/2017
///
///  @copyright Copyright © 2017 GrupoSB. All rights reserved.
///
///  @brief
///  Carrega, inicializa e aloca um ClassFile e associa com a estrutura do AreaMetodos.
///

#include "carregador.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

AreaMetodos areaMetodos;

int primeira = FALSE;

int32_t carregaMemClass(char* caminhoClasse) {
  inicializaPrimeiraVez();

	for (int i = 0; i < areaMetodos.numClasses; i++) {
		char* nomeClassAux = retornaNomeClass(areaMetodos.arrayClasses[i]);
		if (strcmp(caminhoClasse, nomeClassAux) == 0) {
			return i;
		}
	}

  areaMetodos.numClasses++;
  ClassFile** arrayClassesAux = NULL;
  arrayClassesAux = (ClassFile**) realloc(areaMetodos.arrayClasses, (areaMetodos.numClasses*sizeof(ClassFile*)));
  areaMetodos.arrayClasses = (ClassFile**) calloc(1, sizeof(ClassFile*));
  areaMetodos.arrayClasses = arrayClassesAux;
  char* caminhoDestino = malloc(strlen(caminhoClasse) + 10);

  if (strstr(caminhoClasse, ".class") != NULL) {
	   sprintf(caminhoDestino, "%s", caminhoClasse);
  } else {
    sprintf(caminhoDestino, "./%s.class",caminhoClasse);
  }

  areaMetodos.arrayClasses[areaMetodos.numClasses - 1] = inicializaLeitor(caminhoDestino);

  if(areaMetodos.arrayClasses[areaMetodos.numClasses -1] == NULL){
    printf("Erro ao carregar classe!\n");
    exit(0);
  }
  
  return areaMetodos.numClasses - 1;
}

void inicializaPrimeiraVez() {
  if (primeira == FALSE) {
    areaMetodos.numClasses = 0;
    primeira = TRUE;
  }
}

char* retornaNomeClass(ClassFile* classe){
	uint16_t thisClass = classe->thisClass;
	uint16_t nameIndex = (classe->constantPool[thisClass - 1]).info.Class.nameIndex;
	uint16_t indice = classe->constantPool[nameIndex - 1].info.Utf8.length;
	char* retorno = (char*) malloc(indice + 1);

  int i;
	for (i = 0; i < indice; i++) {
		retorno[i] = (char) (classe->constantPool[nameIndex - 1]).info.Utf8.bytes[i];
	}

	retorno[i] = '\0';
	return retorno;
}

ClassFile* buscaClassPorIndice(int indice) {
	return indice >= areaMetodos.numClasses ? NULL : areaMetodos.arrayClasses[indice];
}

char* retornaNome(ClassFile* cf, uint16_t indiceNome) {
	int i;

	char* retorno = malloc((cf->constantPool[indiceNome - 1]).info.Utf8.length + 1);
	uint16_t indice = cf->constantPool[indiceNome - 1].info.Utf8.length;

	for (i = 0; i < indice; i++) {
		retorno[i] = (char) (cf->constantPool[indiceNome - 1]).info.Utf8.bytes[i];
	}

	retorno[i] = '\0';

	return retorno;
}
