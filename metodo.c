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

#include "frame.h"
#include "metodo.h"
#include "instrucao.h"
#include "areaMetodos.h"

#include <stdlib.h>

extern struct Frame* frameCorrente;

AreaMetodos areaMet;

uint32_t numObjetos = 0;

MethodInfo* buscaMetodoMain() {
	ClassFile* main;
	uint16_t tamanhoNome;
  uint16_t tamanhoDesc;
	uint8_t* nome;
	uint8_t* desc;

	main = buscaClassPorIndice(1);

	for(int i = 0; i < main->methodsCount; i++) {
		nome = main->constantPool[(main->methods[i].nameIndex -1)].info.Utf8.bytes;
		desc = main->constantPool[(main->methods[i].descriptorIndex - 1)].info.Utf8.bytes;

		if(strcmp("main",(char*)nome) == 0) {
			if(strcmp("([Ljava/lang/String;)V",(char*)desc) == 0) {
				return &(main->methods[i]);
			}
		}
	}

	printf("Método main não encontrado.");
	return NULL;
}

void empilhaMetodo(MethodInfo* metodo, ClassFile* classe) {
	newInstrucoes();
	criaFrame(classe->constantPool, classe, metodo->cdAtrb);
}

void executaFrameCorrente() {
	for(;(frameCorrente->pc) < frameCorrente->codeLength;) {
		if(frameCorrente == NULL) {
			break;
    }
		instrucao[frameCorrente->code[frameCorrente->pc]]();
	}

	desalocaFrame();
}

Objeto* criaObjeto(ClassFile* classe) {
	Objeto* objeto;

	if(numObjetos == 0) {
		heap = calloc(128,sizeof(struct objeto*));
	}

	objeto = calloc(sizeof(objeto),1);
	objeto->classe = classe;

	objeto->campos = calloc(sizeof(uint32_t), classe->fieldsCount);
	objeto->indiceCampos = calloc(sizeof(uint32_t), classe->fieldsCount);

	for(int i = 0; i < classe->fieldsCount; i++) {
		objeto->indiceCampos[i] = classe->fields[i].nameIndex;
	}

	return objeto;
}

MethodInfo* buscaMetodo(ClassFile* indiceClasse, ClassFile* searchClasse, uint16_t indice){
	char* name = retornaNome(indiceClasse, indiceClasse->constantPool[indice-1].info.NameAndType.nameIndex);
	char* desc = retornaNome(indiceClasse, indiceClasse->constantPool[indice-1].info.NameAndType.descriptorIndex);
	char* searchName;
	char* searchDesc;
	for(int i = 0; i < searchClasse->methodsCount; i++) {
    searchName = searchClasse->constantPool[(searchClasse->methods[i].nameIndex-1)].info.Utf8.bytes;
		searchDesc = searchClasse->constantPool[(searchClasse->methods[i].descriptorIndex-1)].info.Utf8.bytes;
		if((strcmp(name,searchName) == 0) && (strcmp(desc,searchDesc) == 0)) {
			return(searchClasse->methods + i);
		}
	}
  return NULL;
}

int32_t buscaCampo(char* className, char* name, char* desc){
	ClassFile* classe = retornaNomeClass(className);
	uint8_t* searchName;
	uint8_t* searchDesc;

	for(int i = 0; i < classe->fieldsCount; i++) {
		searchName = classe->constantPool[(classe->fields[i].nameIndex-1)].info.Utf8.bytes;
		searchDesc = classe->constantPool[(classe->fields[i].descriptorIndex-1)].info.Utf8.bytes;
		if ((strcmp(name, searchName) == 0) && (strcmp(desc, searchDesc) == 0)) {
			return i;
		}
	}
}

ClassFile* retornaClasseNome(char* nomeClasse) {
	for (int i = 0; i < areaMet.numClasses; i++) {
		if (strcmp(nomeClasse, retornaNomeClass(areaMet.arrayClasses[i])) == 0)
			return areaMet.arrayClasses[i];
	}
	return NULL;
}

int32_t retornaNumeroParametros(ClassFile* classe, MethodInfo* metodo) {
	int32_t numeroParametros = 0;
	uint16_t length;
	uint8_t* bytes;

	bytes = classe->constantPool[(metodo->descriptorIndex-1)].info.Utf8.bytes;
	length = classe->constantPool[(metodo->descriptorIndex-1)].info.Utf8.length;

	int i = 0;
	while(i < length) {
    if(bytes[i] == ')') {
			break;
		}

    if(bytes[i] == 'L') {
			while(bytes[i] != ';') {
				i++;
			}
			numeroParametros++;
		} else if((bytes[i] == 'Z')||(bytes[i] == 'S')||(bytes[i] == 'F')|| (bytes[i] == 'I')||(bytes[i] == 'C')||(bytes[i] == 'B')) {
			numeroParametros++;
		} else if((bytes[i] == 'D')||(bytes[i] == 'J')) {
			numeroParametros+=2;
		}

		i++;
	}
	return numeroParametros;
}
