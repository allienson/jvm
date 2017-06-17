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

#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>
#include <stdlib.h>
#include "leitor.h"

typedef struct PilhaOp {
    int depth;
    int32_t* operandos;
} PilhaOp;

typedef struct Frame {
	int32_t* fields;
	CpInfo* constantPool;
	ClassFile* classe;
	uint16_t maxStack;
	uint16_t maxLocals;
	uint32_t codeLength;
	uint8_t* code;
	uint32_t pc;
  PilhaOp* pilhaOp;
} Frame;

typedef struct StackFrame {
	Frame* refFrame;
	struct StackFrame* next;
} StackFrame;

typedef struct Vector {
	int32_t referencia;
	int32_t tamanho;
	int8_t tipo;
} Vector ;

int32_t retorno;
int32_t retAlta;
int32_t retBaixa;
int8_t flagRet;

int32_t popOp();

void criaFrame(ClassFile*, CodeAttribute*);
void desalocaFrame();
void pushOp(int32_t);
void dumpStack();
void dumpFields();
void pushFrame(ClassFile*, CodeAttribute*, StackFrame*);
void popFrame();

#endif
