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

struct Frame* frameCorrente;

Vector* arrayVetores;

int32_t qtdArrays;
int32_t retorno;
int32_t retAlta;
int32_t retBaixa;
int8_t flagRet;

static struct StackFrame* topo = NULL;

void criaFrame(CpInfo* cp, ClassFile* classe, CodeAttribute* code){
	struct StackFrame* sf = NULL;
	sf = (struct StackFrame*) calloc(sizeof(struct StackFrame),1);

	if(sf == NULL) {
		printf("Problema na alocação do frame\n");
	}

	sf->refFrame = (struct Frame*) calloc(sizeof(struct Frame),1);

	pushFrame(cp, classe, code, sf);
}

void pushFrame(CpInfo* cp, ClassFile* classe, CodeAttribute* code, struct StackFrame* sf){
	sf->next = topo;
	topo = sf;

	topo->refFrame->pc = 0;
	topo->refFrame->classe = classe;
	topo->refFrame->constantPool = cp;
	topo->refFrame->maxStack = 2 * code->maxStack + 110;
	topo->refFrame->maxLocals = code->maxLocals;
	topo->refFrame->codeLength = code->codeLength;
	topo->refFrame->code = code->code;
	topo->refFrame->fields = calloc(sizeof(uint32_t), topo->refFrame->maxLocals);
  topo->refFrame->pilhaOp = calloc(1, sizeof(PilhaOp));
  topo->refFrame->pilhaOp->operandos = calloc(topo->refFrame->maxStack, sizeof(uint32_t));
  topo->refFrame->pilhaOp->depth = 0;

	frameCorrente = topo->refFrame;
}

void popFrame() {
	struct StackFrame *anterior;

	if (topo->next != NULL) {
		frameCorrente = topo->next->refFrame;

  	if(flagRet == 1) {
  		push(retorno);
  	} else if(flagRet == 2) {
  		push(retAlta);
  		push(retBaixa);
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

void desalocaFrame() {
	popFrame();
}

void push(int32_t valor) {
  mostraProfundidade(1);

	if(frameCorrente->pilhaOp->depth >= frameCorrente->maxStack){
		printf("Overflow na pilha de operandos!\n");
		exit(0);
	}

  frameCorrente->pilhaOp->depth += 1;
  frameCorrente->pilhaOp->operandos[frameCorrente->pilhaOp->depth - 1] = valor;
}

int32_t popOp() {
    mostraProfundidade(-1);

    frameCorrente->pilhaOp->depth -= 1;

    if (frameCorrente->pilhaOp->depth < 0) {
        printf("profundidade da pilha de operandos negativa: %d\n", frameCorrente->pilhaOp->depth);
        printf("pc: %d\n", frameCorrente->pc);
    }

    return frameCorrente->pilhaOp->operandos[frameCorrente->pilhaOp->depth];
}

void mostraProfundidade(int i) {
    int j = 0;

    if (j == 1) {
        inicializaDecodificador(dec);
        int numBytes = dec[frameCorrente->code[frameCorrente->pc]].bytes;
        printf("instrucao que modifica pilha: %s\n", dec[frameCorrente->code[frameCorrente->pc]].instrucao);
        printf("valor de pc: %d\n", frameCorrente->pc);
        printf("nova profundidade da pilha: %d\n", frameCorrente->pilhaOp->depth + i);
    }
}

void dumpStack(){
	int i;
    printf("\n");
	for(i = 0; i < frameCorrente->pilhaOp->depth; i++){
		printf("valor: %d\n",frameCorrente->pilhaOp->operandos[i]);
	}
}

void dumpFields(){
	int i;
	for(i = 0; i < frameCorrente->maxLocals; i++){
		printf("valor: %d\n",frameCorrente->fields[i]);
	}
}
