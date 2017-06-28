///
///		@file frame.h
///		@class frame.c "frame.c"
///		@headerfile leitor.h "leitor.h"
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
///		Arquivo de definicao de das estruturas de um frame.

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
int checkaOverflowPilhaOp();
int checkaUnderflowPilhaOp();

#endif
