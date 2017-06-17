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

#ifndef CARREGADOR_H
#define CARREGADOR_H

#include "leitor.h"
#include "areaMetodos.h"

int32_t carregaMemClass(char*);
void inicializaPrimeiraVez();
void carregaClasse(char*);
char* retornaNomeClass(ClassFile*);
ClassFile* buscaClassPorIndice(int);
char * retornaNome(ClassFile*, uint16_t);

#endif
