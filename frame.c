///
///  @file frame.c
///  @headerfile frame.h "frame.h"
///
///  @authors
///  Allisson Barros         12/0055619\n
///  Daniel Luz              13/0007714\n
///  Luiz Fernando Vieira    13/0013757\n
///  Mariana Pannunzio       12/0018276\n
///  Mateus Denucci          12/0053080\n
///
///  @date 28/06/2017
///
///  @copyright Copyright © 2017 GrupoSB. All rights reserved.
///
///  @brief
///  Gerenciamento e alocação de memória das frames do programa executado.
///

#include "frame.h"

Decodificador dec[NUM_INSTRUCAO];

Frame* frameCorrente;

int32_t qtdArrays;

static StackFrame* topo = NULL;

///
/// Cria um frame e coloca ele na pilha de frames.
///
/// @param ClassFile* Ponteiro para a estrutura ClassFile.
/// @param CodeAttribute* Ponteiro para a estrutura CodeAttribute.
/// @return @c void
/// @see pushFrame
void criaFrame(ClassFile* classe, CodeAttribute* codeAttribute){
	StackFrame* stackFrame = NULL;
	stackFrame = (StackFrame*) calloc(sizeof(StackFrame),1);

	if(stackFrame == NULL) {
		printf("Problema na alocação do frame\n");
	}

	stackFrame->refFrame = (Frame*) calloc(sizeof(Frame),1);
	pushFrame(classe, codeAttribute, stackFrame);
}

///
/// Coloca o frame passado por parametro na pilha de frames e atualiza o topo da pilha.
///
/// @param ClassFile* Ponteiro para a estrutura ClassFile.
/// @param CodeAttribute* Ponteiro para a estrutura CodeAttribute.
/// @param StackFrame* Ponteiro para a estrutura StackFrame
/// @return @c void
void pushFrame(ClassFile* classe, CodeAttribute* codeAttribute, StackFrame* stackFrame){
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

///
/// Retira um frame da pilha de frames e atualiza o topo da pilha.
///
/// @param Nao possui parametros
/// @return @c void
/// @see pushOp
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

///
/// Empilha um valor na pilha de operandos.
///
/// @param int32_t Valor a ser inserido na pilha de operandos.
/// @return @c void
void pushOp(int32_t valor) {
	
	if(checkaOverflowPilhaOp()){
		printf("Overflow na pilha de operandos!\n");
		exit(0);
	}
	frameCorrente->pilhaOp->operandos[frameCorrente->pilhaOp->depth] = valor;
  	frameCorrente->pilhaOp->depth += 1;
}

///
/// Desempilha um valor na pilha de operandos.
///
/// @param Nao possui parametros.
/// @return @c int32_t Retorna o valor desempilhado da pilha de operandos.
int32_t popOp() {
	
	frameCorrente->pilhaOp->depth -= 1;

	if (checkaUnderflowPilhaOp()) {
		printf("profundidade da pilha de operandos negativa: %d\n", frameCorrente->pilhaOp->depth);
		printf("pc: %u\n", frameCorrente->pc);
	}

	return frameCorrente->pilhaOp->operandos[frameCorrente->pilhaOp->depth];
}

///
/// Confere se o tamanho da pilha excedeu o
/// tamanho maximo (max stack)
///
/// @param Nao possui parametros.
/// @return @c int Resultado de uma comparacao (0 ou 1)
int checkaOverflowPilhaOp(){
	return frameCorrente->pilhaOp->depth >= frameCorrente->maxStack;
}

///
/// Confere se a profundidade da pilha esta negativa.
///
/// @param Nao possui parametros.
/// @return @c int Resultado de uma comparacao (0 ou 1)
int checkaUnderflowPilhaOp(){
	return frameCorrente->pilhaOp->depth < 0;
}

