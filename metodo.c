///
///  @file carregador.c
///
///  @headerfile carregador.h "carregador.h"
///  @headerfile areaMetodos.h "areaMetodos.h"
///  @headerfile metodo.h "metodo.h"
///  @headerfile instrucao.h "instrucao.h"
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
///  @brief Arquivo que carrega a area de metodos na memoria
///  para inciar a execucao do programa Java.
///
///  Carrega as classes a partir do arquivo ".class" chamando
///  o leitor e em seguida carrega essa classe para a memoria.
///  Alem disso, confere se a classe ja esta carregada antes de
///  chamar o Leitor. Tambem retorna um ponteiro para uma classe
///  no array ou o seu nome, pelas funcoes implementadas.
///

#include "frame.h"
#include "metodo.h"
#include "instrucao.h"
#include "areaMetodos.h"

#include <stdlib.h>

extern struct Frame* frameCorrente;

uint32_t numObjetos = 0;

MethodInfo* buscaMetodoMain(ClassFile* classFile) {
	uint8_t* nome;
	uint8_t* desc;

	for(int i = 0; i < classFile->methodsCount; i++) {
		nome = classFile->constantPool[(classFile->methods[i].nameIndex -1)].info.Utf8.bytes;
		desc = classFile->constantPool[(classFile->methods[i].descriptorIndex - 1)].info.Utf8.bytes;
		if(strcmp("main",(char*)nome) == 0) {
			if(strcmp("([Ljava/lang/String;)V",(char*)desc) == 0) {
				return &(classFile->methods[i]);
			}
		}
	}

	printf("Método main não encontrado.");
	return NULL;
}

void empilhaMetodo(MethodInfo* metodo, ClassFile* classe) {
	inicializaInstrucoes();
	criaFrame(classe, metodo->cdAtrb);
}

void executaFrameCorrente() {
  while(frameCorrente->pc < frameCorrente->codeLength) {
    if(frameCorrente == NULL) {
			break;
    }
		instrucao[frameCorrente->code[frameCorrente->pc]]();
  }
	popFrame();
}

Objeto* criaObjeto(ClassFile* classe) {
	Objeto* objeto;

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
    searchName = (char*) searchClasse->constantPool[(searchClasse->methods[i].nameIndex-1)].info.Utf8.bytes;
		searchDesc = (char*) searchClasse->constantPool[(searchClasse->methods[i].descriptorIndex-1)].info.Utf8.bytes;
		if((strcmp(name,searchName) == 0) && (strcmp(desc,searchDesc) == 0)) {
			return(searchClasse->methods + i);
		}
	}
  return NULL;
}

int32_t buscaCampo(char* className, char* name, char* desc){
	ClassFile* classe = retornaClassePorNome(className);
	char* searchName;
	char* searchDesc;

	for(int i = 0; i < classe->fieldsCount; i++) {
		searchName = (char*) classe->constantPool[(classe->fields[i].nameIndex-1)].info.Utf8.bytes;
		searchDesc = (char*) classe->constantPool[(classe->fields[i].descriptorIndex-1)].info.Utf8.bytes;
		if ((strcmp(name, searchName) == 0) && (strcmp(desc, searchDesc) == 0)) {
			return i;
		}
	}

  exit(0);
}

ClassFile* retornaClassePorNome(char* nomeClasse) {
	for (int i = 0; i < areaMetodos.numClasses; i++) {
		char* nomeClassAux = retornaNomeClass(areaMetodos.arrayClasses[i]);
		if (strcmp(nomeClasse, nomeClassAux) == 0){
			return areaMetodos.arrayClasses[i];
		}
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
