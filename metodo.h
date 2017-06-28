///
///		@file metodo.h
///		@class metodo.c "metodo.c"
///		@headerfile carregador.h "carregador.h"
///
///		@date 26/06/17.
///		@copyright Copyright © 2017 GrupoSB. All rights reserved.
///
///		@authors
///		Allisson Barros         12/0055619\n
///		Daniel Luz              13/0007714\n
///		Luiz Fernando Vieira    13/0013757\n
///		Mariana Pannunzio       12/0018276\n
///		Mateus Denucci          12/0053080
///
///		@brief
///		Arquivo que carrega a area de metodos na memoria
///		para inciar a execucao do programa Java.
///
///		Carrega as classes a partir do arquivo ".class" chamando
///		o Leitor e em seguida carrega essa classe para a memoria.
///		Alem disso, confere se a classe ja esta carregada antes de
///		chamar o Leitor. Tambem retorna um ponteiro para uma classe
///		no array ou o seu nome, pelas funcoes implementadas.
///

#ifndef METODO_H
#define METODO_H

#include "carregador.h"

AreaMetodos areaMetodos;

MethodInfo* buscaMetodoMain();
void empilhaMetodo(MethodInfo*, ClassFile*);
void executaFrameCorrente();
Objeto* criaObjeto(ClassFile*);
MethodInfo* buscaMetodo(ClassFile*, ClassFile*, uint16_t);
int32_t retornaNumeroParametros(ClassFile*, MethodInfo*);
ClassFile* retornaClassePorNome(char*);
int retornaIndiceDaClassePorNome(char*);
int32_t buscaCampo(char*, char*, char*);

#endif
