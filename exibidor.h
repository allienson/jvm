///
///		@file exibidor.h
///		@class exibidor.c "exibidor.c"
///		@headerfile leitor.h "leitor.h"
///
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
///		Arquivo que exibe na tela uma estrutura ClassFile
///		imprimindo todos os dados armazenados.
///
///		Recebe um ponteiro para uma estrutura ClassFile, imprime
///		cada parte de acordo com seu formato e suas subestruturas,
///		isso tudo com uma interface semelhante ao programa
///		"jclasslib bytecode viewer".
///

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
void imprimeMajorVersion(uint16_t);
void printTopo();
void printBlank();
void printBase();
void printSingleLine();

#endif