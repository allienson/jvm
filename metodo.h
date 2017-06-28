///
///		@file metodo.h
///		@class metodo.c "metodo.c"
///		@headerfile carregador.h "carregador.h"
///
///		@date 28/06/17.
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
///		Define as funcoes que implementam a execucao de um 
///     metodo e instanciacao de objetos.

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
