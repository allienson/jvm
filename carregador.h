///
///		@file carregador.h
///		@class carregador.c "carregador.c"
///		@headerfile leitor.h "leitor.h"
///		@headerfile areaMetodos.h "areaMetodos.h"
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
///		Carrega, inicializa e aloca um ClassFile e associa com a estrutura do AreaMetodos.
///

#ifndef CARREGADOR_H
#define CARREGADOR_H

#include "leitor.h"
#include "areaMetodos.h"

int32_t carregaClasseParaMemoria(char*);
void inicializaPrimeiraVez();
void carregaClasse(char*);
char* retornaNomeClass(ClassFile*);
ClassFile* buscaClassPorIndice(int);
char* retornaNome(ClassFile*, uint16_t);

#endif
