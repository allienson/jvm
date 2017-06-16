#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>
#include "leitor.h"

void criaFrame(CpInfo* cp, ClassFile* classe, CodeAttribute* code);
void desalocaFrame();

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

struct StackFrame {
	Frame* refFrame;
	struct StackFrame* next;
};

typedef struct Array{
	int32_t referencia;
	int32_t tamanho;
	int8_t tipo;
} vector;

struct Frame* frameCorrente;

Vector* arrayVetores;

int32_t qtdArrays;
int32_t retorno;
int32_t retAlta,retBaixa;
int8_t flagRet;

void push(int32_t valor);
int32_t popOp();

void dumpStack();
void dumpFields();
void pushFrame(CpInfo*, ClassFile*, CodeAttribute*, struct StackFrame*);
void popFrame();
void mostraProfundidade(int i);

static struct StackFrame* topo = NULL;

#endif
