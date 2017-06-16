//
//  main.c
//  Java Virtual Machine - Software Basico 2017-1
//
//  Created on 26/05/17.
//  Copyright © 2017 GrupoSB. All rights reserved.
//
//  Allisson Barros         12/0055619
//  Daniel Luz              13/0007714
//  Luiz Fernando Vieira    13/0013757
//  Mariana Pannunzio       12/0018276
//  Mateus Denucci          12/0053080

#include "carregador.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** 
 *Area de métodos que engloba o array de classes e seus respectivos metodos.
 */
AreaMetodos areaMetodos; 

/** 
 *booleano para verificar a primeira classe a ser carregada.
 */
bool primeira = false;

/**
 * É passado o path para o object.class a ser carregado.\n
 * 
 * Se ainda não foi carregado carrega no array de classes.
 * @param path do "object.class" a ser carregado no array de classes.
 * @return posição da classe alocada no array de classes.
 */
int32_t carregaObjectClass(char* ObjectClass) {

	int aux;

	// se eh a primeira classe a ser carregada
    if (primeira == false) {
        areaMetodos.numClasses = 0;
        primeira = true; 
    }

    //Se ja esta carregado retorna posição no array de classes.
	for (int32_t i = 0; i < areaMetodos.numClasses; i++) {
		if (strcmp(ObjectClass, retornaNomeClass(areaMetodos.arrayClasses[i])) == 0){
			return i;
		}
	}

	// Caso contrario, carrega a classe nova:

	//uma classe nova vai entrar na lista de classes carregadas.
	areaMetodos.numClasses++;
	aux = areaMetodos.numClasses;

	ClassFile** arrayClassesAux = NULL;

	//Realoca o tamanho do vetor para adicionar a nova classe carregada.
	//Classes previamente carregas não são perdidas com o realloc.
	arrayClassesAux = (ClassFile**) realloc(areaMetodos.arrayClasses, (aux*sizeof(ClassFile *)));

    areaMetodos.arrayClasses = (ClassFile**) calloc(1, sizeof(ClassFile*));
    areaMetodos.arrayClasses = arrayClassesAux;

    //Atualiza o nome do path para adicionar .class a um path
    //Aloca espaço do tamanho do nome da classe mais espaço para .class /0 e ./
    char* caminho = malloc(strlen(ObjectClass) + 10);
    if (strstr(ObjectClass,".class") != NULL) {
		sprintf(caminho, "%s", ObjectClass);
	} else {
		sprintf(caminho, "./%s.class",ObjectClass);
	}

    areaMetodos.arrayClasses[areaMetodos.numClasses - 1] = inicializaLeitor(caminho);  
    
	if(areaMetodos.arrayClasses[areaMetodos.numClasses -1] == NULL){
		printf("Erro ao carregar a nova classe!\n");
		exit(0);
	}

	return aux - 1;
}

/**
 * É passado o nome do arquivo .class a ser carregado.\n
 * 
 * Se ainda não foi carregado carrega no array de classes.
 * @param nome do arquivo ".class" a ser carregado no array de classes.
 * @return posição da classe alocada no array de classes.
 */
int32_t carregaMemClass(char* nomeClass){
	
	int aux;

    // se eh a primeira classe a ser carregada
    if (primeira == false) {
        areaMetodos.numClasses = 0;
        primeira = true; 
    }

	//Se ja esta carregado retorna posição no array de classes.
	for (int32_t i = 0; i < areaMetodos.numClasses; i++) {
		if (strcmp(nomeClass, retornaNomeClass(areaMetodos.arrayClasses[i])) == 0){
			return i;
		}
	}

	//Se não está carregado carrega e salva no array de classes.

	//uma classe nova vai entrar na lista de classes carregadas.
	areaMetodos.numClasses++;
	aux = areaMetodos.numClasses;

	ClassFile** arrayClassesAux = NULL;

	//Realoca o tamanho do vetor para adicionar a nova classe carregada.
	//Classes previamente carregas não são perdidas com o realloc.
	arrayClassesAux = (ClassFile**) realloc(areaMetodos.arrayClasses, (aux*sizeof(ClassFile *)));
	//printf("ponteiro tempo: %d\n",*arrayClassesAux);

    areaMetodos.arrayClasses = (ClassFile**) calloc(1, sizeof(ClassFile*));
    areaMetodos.arrayClasses = arrayClassesAux;

    //Atualiza o nome do path para adicionar .class a um path
    //Aloca espaço do tamanho do nome da classe mais espaço para .class /0 e ./
    char* destino = malloc(strlen(nomeClass) + 10);
    if (strstr(nomeClass,".class") != NULL) {
		sprintf(destino, "%s", nomeClass);
	} else {
		sprintf(destino, "./%s.class",nomeClass);
	}

	//printf("destino: %s\n",destino);

    areaMetodos.arrayClasses[areaMetodos.numClasses - 1] = inicializaLeitor(destino);  
    
	if(areaMetodos.arrayClasses[areaMetodos.numClasses -1] == NULL){
		printf("Erro ao carregar classe!\n");
		exit(0);
	}

	return aux - 1;
}


/**
 *Acessa a constant pool no campo bytes a partir do index da classe e 
 *retorna o nome da classe.\n
 *Necessaria para saber se classe ja foi carregada ou não.
 * @param Estrutura ClassFile de que se deseja obter o nome.
 * @return nome da classe fornecida.
 */
char* retornaNomeClass(ClassFile* classe){
	int i;
	
	uint16_t thisClass = classe->thisClass;
	uint16_t nameIndex = (classe->constantPool[thisClass - 1]).info.Class.nameIndex;
	
	//char* retorno = (char*) malloc((classe->constantPool[nameIndex - 1]).info.Utf8.length + 1);
	//uint16_t indice = classe->constantPool[nameIndex - 1].info.Utf8.length;

	// SE TIVER DANDO TRETA AQUI, DESCOMENTA AS LINHAS DE CIMA E TIRA ESSAS DUAS ABAIXO
	uint16_t indice = classe->constantPool[nameIndex - 1].info.Utf8.length;
	char* retorno = (char*) malloc(indice + 1);			// +1 para o \0 ao final da string

	// Percorre o campo bytes na constant pool e salva no retorno.
	for (i = 0; i < indice; i++) {
		retorno[i] = (char) (classe->constantPool[nameIndex - 1]).info.Utf8.bytes[i];
	}

	// \0 no final do nome da classe a ser retornada
	retorno[i] = '\0';

	//Retorna nome da classe.
	return retorno;
}

/**
 * Função que retorna uma referencia a uma ClassFile de acordo com o indice.\n
 * Util para encontrar a classe da main no inicio do processamento.
 * @param Posicao a ser acessada no array de classes.
 * @return Referencia a um ClassFile ou NULL caso nao exista a referencia 
 */
ClassFile* buscaClassPorIndice(int indice){
	return indice >= areaMetodos.numClasses ? NULL : areaMetodos.arrayClasses[indice];
}


/**
 * Função que retorna um nome de um utf8 a partir de um indice.
 * Muito usada como função de auxílio para implementar as instruções.
 * @param Referência a um ClassFile para acesso a constant pool.
 * @param indice para acesso a constant pool.
 * @return retorna uma string com o nome.
 */
char * retornaNome(ClassFile* cf, uint16_t indiceNome) {
	int i;

	//Aloca espaço.
	char* retorno = malloc((cf->constantPool[indiceNome - 1]).info.Utf8.length + 1);
	uint16_t indice = cf->constantPool[indiceNome - 1].info.Utf8.length;
	//Percorre o campo bytes de um utf8 para um auxiliar.
	for (i = 0; i < indice; i++) {
		retorno[i] = (char) (cf->constantPool[indiceNome - 1]).info.Utf8.bytes[i];
	}

	//Adiciona \0 ao final do nome.
	retorno[i] = '\0';

	return retorno;
}
