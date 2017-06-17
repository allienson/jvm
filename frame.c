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

#include "frame.h"

Decodificador dec[NUM_INSTRUCAO];

Frame* frameCorrente;
Vector* arrayVetores;

int32_t qtdArrays;

static StackFrame* topo = NULL;

void criaFrame(ClassFile* classe, CodeAttribute* codeAttribute){
	StackFrame* stackFrame = NULL;
	stackFrame = (StackFrame*) calloc(sizeof(StackFrame),1);

	if(stackFrame == NULL) {
		printf("Problema na alocação do frame\n");
	}

	stackFrame->refFrame = (struct Frame*) calloc(sizeof(struct Frame),1);
	pushFrame(classe, codeAttribute, stackFrame);
}

void pushFrame(ClassFile* classe, CodeAttribute* codeAttribute, struct StackFrame* stackFrame){
	stackFrame->next = topo;
	topo = stackFrame;
	topo->refFrame->pc = 0;
	topo->refFrame->classe = classe;
	topo->refFrame->constantPool = classe->constantPool;
	topo->refFrame->maxStack = 2 * codeAttribute->maxStack + 110;
	topo->refFrame->maxLocals = codeAttribute->maxLocals;
	topo->refFrame->codeLength = codeAttribute->codeLength;
	topo->refFrame->code = codeAttribute->code;
	topo->refFrame->fields = calloc(sizeof(uint32_t), topo->refFrame->maxLocals);
  topo->refFrame->pilhaOp = calloc(1, sizeof(PilhaOp));
  topo->refFrame->pilhaOp->operandos = calloc(topo->refFrame->maxStack, sizeof(uint32_t));
  topo->refFrame->pilhaOp->depth = 0;
	frameCorrente = topo->refFrame;
}

void popFrame() {
	StackFrame *anterior;

	if (topo->next != NULL) {
		frameCorrente = topo->next->refFrame;

  	if(flagRet == 1) {
  		pushOp(retorno);
  	} else if(flagRet == 2) {
  		pushOp(retAlta);
  		pushOp(retBaixa);
  	}

  	flagRet = 0;
	} else {
		frameCorrente = NULL;
	}

	anterior = topo->next;

  free(topo->refFrame->pilhaOp->operandos);
  free(topo->refFrame->pilhaOp);
  free(topo->refFrame->fields);
	free(topo->refFrame);
	free(topo);

	topo = anterior;
}

void pushOp(int32_t valor) {
	if(frameCorrente->pilhaOp->depth >= frameCorrente->maxStack){
		printf("Overflow na pilha de operandos!\n");
		exit(0);
	}

  frameCorrente->pilhaOp->depth += 1;
  frameCorrente->pilhaOp->operandos[frameCorrente->pilhaOp->depth - 1] = valor;
}

int32_t popOp() {
	frameCorrente->pilhaOp->depth -= 1;

	if (frameCorrente->pilhaOp->depth < 0) {
		printf("profundidade da pilha de operandos negativa: %d\n", frameCorrente->pilhaOp->depth);
		printf("pc: %u\n", frameCorrente->pc);
	}

	return frameCorrente->pilhaOp->operandos[frameCorrente->pilhaOp->depth];
}

void dumpStack() {
  printf("\n");
	for(int i = 0; i < frameCorrente->pilhaOp->depth; i++) {
		printf("valor: %d\n", frameCorrente->pilhaOp->operandos[i]);
	}
}

void dumpFields() {
	for(int i = 0; i < frameCorrente->maxLocals; i++) {
		printf("valor: %d\n",frameCorrente->fields[i]);
	}
}
