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

extern struct frame* frameCorrente;

uint32_t numObjetos = 0;

MethodInfo* buscaMetodoMain() {
	ClassFile* main;
	uint16_t tamanhoNome;
  uint16_t tamanhoDesc;
	uint8_t* nome;
	uint8_t* desc;

	main = buscaClasseIndice(1);

	for(int i = 0; i < main->methodsCount; i++) {
		nome = main->constantPool[(main->methods[i].nameIndex -1)].info.Utf8.bytes;
		desc = main->constantPool[(main->methods[i].descriptorIndex - 1)].info.Utf8.bytes;

		if(strcmp("main",(char *)nome) == 0) {
			if(strcmp("([Ljava/lang/String;)V",(char *)desc) == 0) {
				return &(main->methods[i]);
			}
		}
	}

	printf("Método main não encontrado.");
	return NULL;
}

void empilhaMetodo(MethodInfo* metodo, ClassFile* classe) {
	newInstrucoes();
	criaFrame(classe->constantPool,classe,metodo->cdAtrb);
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

objeto* criaObjeto(classFile* classe) {
	objeto* objeto;

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

MethodInfo* buscaMetodo(classFile* indiceClasse, classFile* searchClasse, uint16_t indice){
	char* name = retornaNome(indiceClasse, indiceClasse->constant_pool[indice-1].info.NameAndType.name_index);
	char* desc = retornaNome(indiceClasse, indiceClasse->constant_pool[indice-1].info.NameAndType.descriptor_index);
	char* searchName;
	char* searchDesc;
	for(int i = 0; i < searchClasse->methods_count; i++) {
    searchName = searchClasse->constant_pool[(searchClasse->methods[i].name_index-1)].info.Utf8.bytes;
		searchDesc = searchClasse->constant_pool[(searchClasse->methods[i].descriptor_index-1)].info.Utf8.bytes;
		if((strcmp(name,searchName) == 0) && (strcmp(desc,searchDesc) == 0)) {
			return(searchClasse->methods + i);
		}
	}
}

int32_t buscaCampo(char* className, char* name, char* desc){
	classFile* classe = retornaClasseNome(className);
	uint8_t* searchName;
	uint8_t* searchDesc;

	for(int i = 0; i < classe->fields_count; i++) {
		searchName = classe->constant_pool[(classe->fields[i].name_index-1)].info.Utf8.bytes;
		searchDesc = classe->constant_pool[(classe->fields[i].descriptor_index-1)].info.Utf8.bytes;
		if ((strcmp(name, searchName) == 0) && (strcmp(desc, searchDesc) == 0)) {
			return i;
		}
	}
}

classFile* retornaClasseNome(char* nomeClasse) {
	for (int i = 0; i < area_met.num_classes; i++) {
		if (strcmp(nomeClasse, retornaNomeClasse(area_met.array_classes[i])) == 0)
			return area_met.array_classes[i];
	}
	return NULL;
}

int32_t retornaNumeroParametros(classFile* classe, method_info* metodo) {
	int32_t numeroParametros = 0;
	uint16_t length;
	uint8_t* bytes;

	bytes = classe->constant_pool[(metodo->descriptor_index-1)].info.Utf8.bytes;
	length = classe->constant_pool[(metodo->descriptor_index-1)].info.Utf8.length;

	int i = 0;
	while(i < length) {
        if(bytes[i] == ')')
			break;
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
