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
///  @date 28/06/2017
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
#include <string.h>

#define FALSE 0
#define TRUE 1

AreaMetodos areaMetodos;

int primeira = FALSE;

///
/// Carrega o arquivo .class na memoria e adiciona a classe na area de metodos.
///
/// @param char* Contem o caminho para a classe a ser lida.
/// @return @c int32_t Retorna o numero de classes em area de metodos - 1
/// @see inicializaPrimeiraVez inicializaLeitor
int32_t carregaClasseParaMemoria(char* caminhoClasse) {
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

///
/// Incializa o valor de areaMetodos.numClasses com 0 caso seja a primeira classe a ser lida.
/// Seta uma variavel global para indicar que a primeira classe ja foi lida.
///
/// @param Nao possui parametros
/// @return @c void
void inicializaPrimeiraVez() {
  if (primeira == FALSE) {
    areaMetodos.numClasses = 0;
    primeira = TRUE;
  }
}

///
/// Retorna o nome da classe desejada.
///
/// @param ClassFile* Classe que se deseja saber o nome.
/// @return @c char* Nome da classe desejada
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

///
/// Caso o indice passado seja um numero menor que o numero de classes salvas na
/// area de metodos.
///
/// @param int Indice da classe no array de area de metodos que se deseja saber.
/// @return @c ClassFile* retorna um ponteiro para a classe apontada por esse indice no array da area de metodos.
ClassFile* buscaClassPorIndice(int indice) {
	return indice >= areaMetodos.numClasses ? NULL : areaMetodos.arrayClasses[indice];
}

///
/// Retorna o nome da classe desejada com base no classfile e no indice do nome.
///
/// @param ClassFile* Classe que se deseja saber o nome.
/// @param uint16_t Indce da classe.
/// @return @c char* Retorna o nome da classe desejada com base no classfile e no indice do nome.
char* retornaNome(ClassFile* classe, uint16_t indiceNome) {
	int i;

	char* retorno = malloc((classe->constantPool[indiceNome - 1]).info.Utf8.length + 1);
	uint16_t indice = classe->constantPool[indiceNome - 1].info.Utf8.length;

	for (i = 0; i < indice; i++) {
		retorno[i] = (char) (classe->constantPool[indiceNome - 1]).info.Utf8.bytes[i];
	}

	retorno[i] = '\0';

	return retorno;
}
