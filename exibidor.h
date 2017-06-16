//
//  exibidor.h
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

#ifndef EXIBIDOR_h
#define EXIBIDOR_h

#include <stdio.h>

#include "leitor.h"

void printaClassFile(ClassFile*);
void printaCpInfo(ClassFile*);
void printaInterfaces(ClassFile*);
void printaFieldInfo(ClassFile*);
void printaMethodInfo(ClassFile*);
void printaAttributeInfo(ClassFile*);
void imprimeStringPool(CpInfo*, int);
void imprimeCode(ClassFile*, CodeAttribute*);
void imprimeExc(ClassFile*, ExceptionsAttribute*);
double hexToDouble(uint32_t, uint32_t);
long hexToLong(uint32_t, uint32_t);
void printAccessFlag(uint16_t);
void printTopo();
void printBlank();
void printBase();
void printSingleLine();

#endif