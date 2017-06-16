//
//  leitor.h
//  LeitorExibidor - Software Basico 2017-1
//
//  Created on 26/05/17.
//  Copyright © 2017 GrupoSB. All rights reserved.
//
//  Allisson Barros         12/0055619
//  Daniel Luz              13/0007714
//  Luiz Fernando Vieira    13/0013757
//  Mariana Pannunzio       12/0018276
//  Mateus Denucci          12/0053080

#ifndef METODO_H
#define METODO_H

#include "carregador.h"

MethodInfo* buscaMetodoMain();
void empilhaMetodo(MethodInfo* , ClassFile*);
void executaFrameCorrente();
Objeto* criaObjeto(ClassFile*);
MethodInfo* buscaMetodo(ClassFile*, ClassFile*, uint16_t);
int32_t retornaNumeroParametros(ClassFile*, MethodInfo*);
ClassFile* retornaClasseNome(char*);
int32_t buscaCampo(char*, char*, char*);

#endif
