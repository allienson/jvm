///
///  @file metodo.c
///
///  @headerfile frame.h "frame.h"
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
///  @date 28/06/2017
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

///
/// Retorna qual dos metodos da classe passada por parametro eh a main.
///
/// @param ClassFile* Ponteiro para uma estrutura ClassFile que contem a main
/// @return MethodInfo* Retorna o metodo main
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

///
/// Cria um frame para o metodo passado como parametro e empilha ele na pilha de frames.
///
/// @param MethodInfo* Ponteiro para uma estrutura MethodInfo
/// @param ClassFile** Ponteiro para uma estrutura ClassFile.
/// @return void
/// @see inicializaInstrucoes criaFrame
void empilhaMetodo(MethodInfo* metodo, ClassFile* classe) {
	inicializaInstrucoes();
	criaFrame(classe, metodo->cdAtrb);
}

///
/// Executa todas as intrucoes do frame corrente e retira ele da pilha de frames apos o termino.
///
/// @param Nao possui parametros
/// @return void
/// @see popFrame
void executaFrameCorrente() {
  while(frameCorrente->pc < frameCorrente->codeLength) {
    if(frameCorrente == NULL) {
			break;
    }
		instrucao[frameCorrente->code[frameCorrente->pc]]();
  }
	popFrame();
}

///
/// Funcao que instacia um objeto a partir da classe passada como parametro.
///
/// @param ClassFile* Ponteiro para uma estrutura ClassFile.
/// @return Objeto* Retorna o objeto criado.
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

///
/// Recebe uma classe e um indice do metodo que sera procurado. 
/// Se encontra, retorna o metodo. Se nao, retorna NULL.
///
/// @param ClassFile* Classe do frame corrente em execução
/// @param ClassFile* Classe do metodo que sera procurado
/// @param uint16_t Indice do metodo que sera procurado
/// @return @c MethodInfo* Um ponteiro para uma estrutura MethodInfo
/// @see retornaNome
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

///
/// Funcao que busca por um campo especifico dentro de uma classe.
///
/// @param char* Nome da classe em que se vai realizar a busca
/// @param char* Nome do campo a ser procurado
/// @param char* Descriptor do campo a ser procurado
/// @return int32_t Indice do campo procurado.
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

///
/// Funcao que percorre todo a area de metodos buscando a classe com o nome passado por parametro.
///
/// @param char* Nome da classe a ser procurada na area de metodos.
/// @return ClassFile* Retorna a classe procurada (se existe).
ClassFile* retornaClassePorNome(char* nomeClasse) {
	for (int i = 0; i < areaMetodos.numClasses; i++) {
		char* nomeClassAux = retornaNomeClass(areaMetodos.arrayClasses[i]);
		if (strcmp(nomeClasse, nomeClassAux) == 0){
			return areaMetodos.arrayClasses[i];
		}
	}
	return NULL;
}

///
/// Funcao que retorna o indice da classe
/// recebendo como entrada o nome
///
/// @param char* Nome da classe a ser procurada na area de metodos.
/// @return int Indice
int retornaIndiceDaClassePorNome(char* nomeClasse) {
	for (int i = 0; i < areaMetodos.numClasses; i++) {
		char* nomeClassAux = retornaNomeClass(areaMetodos.arrayClasses[i]);
		if (strcmp(nomeClasse, nomeClassAux) == 0){
			return i;
		}
	}
	return -1;
}

///
/// Funcao que retorna o numero de parametros de um determinado metodo de uma classe.
///
/// @param ClassFile* Ponteiro para ClassFile que contem a classe
/// @param MethodInfo* Ponteiro para MethodInfo que se deseja saber o numero de parametros.
/// @return int32_t Retirna o numero de parametros do metodo analisado.
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
