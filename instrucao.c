///
///  @file instrucao.c
///  @headerfile instrucao.h "instrucao.h"
///	 @headerfile frame.h "frame.h"
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
///  Define, interpreta e executa as instruções do programa sendo executado.
///

#include "instrucao.h"
#include "frame.h"
#include "carregador.h"
#include "areaMetodos.h"
#include "metodo.h"

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

Vector* arrayVetores = NULL;

int32_t qtdArrays = 0;

extern struct Frame* frameCorrente;

int naoEmpilhaFlag = 0;

Decodificador dec[NUM_INSTRUCAO];

///
/// Atualiza o valor de PC, considerando a quantidade
/// de bytes que a instrucao ocupa e avancando ate a
/// a posica da proxima instrucao no bytecode.
///
/// @param Nenhum
/// @return @c void
/// @see inicializaDecodificador
void atualizaPc() {
	inicializaDecodificador(dec);
	int numBytes = dec[frameCorrente->code[frameCorrente->pc]].bytes;

	for(int8_t i = 0; i < numBytes + 1; i++) {
		frameCorrente->pc++;
	}
}

///
/// Recupera o UTF8 final de uma constante do Constant Pool,
/// recursivamente a partir uma estrutura CpInfo e da tag de
/// cada constante lida, ate se chegar numa constante UTF8.
///
/// @param CpInfo* Ponteiro para uma estrutura Constant Pool
/// @param int Posicao a ser buscada na Constant Pool
/// @return @c int Posicao da constnte final UTF8
/// @see inicializaDecodificador
int resolveUTF(CpInfo* cp, int posPool) {
    int tag;
    tag = cp[posPool].tag;

    if (tag == CONSTANT_Utf8) {
        return posPool;
    }

    switch(tag) {
        case CONSTANT_Class:
            return resolveUTF(cp, cp[posPool].info.Class.nameIndex - 1);
        case CONSTANT_String:
            return resolveUTF(cp, cp[posPool].info.String.stringIndex - 1);
        case CONSTANT_Integer:
            return resolveUTF(cp, cp[posPool].info.String.stringIndex - 1);
        case CONSTANT_Float:
            return resolveUTF(cp, cp[posPool].info.String.stringIndex - 1);
    }

    exit(0);

    
}

///
/// Prepara o vetor de ponteiros de funcao e preenche cada
/// posicao com a funcao que implementa cada instrucao do
/// bytecode (mnemonicos).
///
/// @param Nenhum
/// @return @c void
void inicializaInstrucoes() {
	instrucao[0] = nop;
	instrucao[1] = aconst_null;
	instrucao[2] = iconst_m1;
	instrucao[3] = iconst_0;
	instrucao[4] = iconst_1;
	instrucao[5] = iconst_2;
	instrucao[6] = iconst_3;
	instrucao[7] = iconst_4;
	instrucao[8] = iconst_5;
	instrucao[9] = lconst_0;
	instrucao[10] = lconst_1;
	instrucao[11] = fconst_0;
	instrucao[12] = fconst_1;
	instrucao[13] = fconst_2;
	instrucao[14] = dconst_0;
	instrucao[15] = dconst_1;
	instrucao[16] = bipush;
	instrucao[17] = sipush;
	instrucao[18] = ldc;
	instrucao[19] = ldc_w;
	instrucao[20] = ldc2_w;
	instrucao[21] = iload;
	instrucao[22] = lload;
	instrucao[23] = fload;
	instrucao[24] = dload;
	instrucao[25] = aload;
	instrucao[26] = iload_0;
	instrucao[27] = iload_1;
	instrucao[28] = iload_2;
	instrucao[29] = iload_3;
	instrucao[30] = lload_0;
	instrucao[31] = lload_1;
	instrucao[32] = lload_2;
	instrucao[33] = lload_3;
	instrucao[34] = fload_0;
	instrucao[35] = fload_1;
	instrucao[36] = fload_2;
	instrucao[37] = fload_3;
	instrucao[38] = dload_0;
	instrucao[39] = dload_1;
	instrucao[40] = dload_2;
	instrucao[41] = dload_3;
	instrucao[42] = aload_0;
	instrucao[43] = aload_1;
	instrucao[44] = aload_2;
	instrucao[45] = aload_3;
	instrucao[46] = iaload;
	instrucao[47] = laload;
	instrucao[48] = faload;
	instrucao[49] = daload;
	instrucao[50] = aaload;
	instrucao[51] = baload;
	instrucao[52] = caload;
	instrucao[53] = saload;
	instrucao[54] = istore;
	instrucao[55] = lstore;
	instrucao[56] = fstore;
	instrucao[57] = dstore;
	instrucao[58] = astore;
	instrucao[59] = istore_0;
	instrucao[60] = istore_1;
	instrucao[61] = istore_2;
	instrucao[62] = istore_3;
	instrucao[63] = lstore_0;
	instrucao[64] = lstore_1;
	instrucao[65] = lstore_2;
	instrucao[66] = lstore_3;
	instrucao[67] = fstore_0;
	instrucao[68] = fstore_1;
	instrucao[69] = fstore_2;
	instrucao[70] = fstore_3;
	instrucao[71] = dstore_0;
	instrucao[72] = dstore_1;
	instrucao[73] = dstore_2;
	instrucao[74] = dstore_3;
	instrucao[75] = astore_0;
	instrucao[76] = astore_1;
	instrucao[77] = astore_2;
	instrucao[78] = astore_3;
	instrucao[79] = iastore;
	instrucao[80] = lastore;
	instrucao[81] = fastore;
	instrucao[82] = dastore;
	instrucao[83] = aastore;
	instrucao[84] = bastore;
	instrucao[85] = castore;
	instrucao[86] = sastore;
	instrucao[87] = pop;
	instrucao[88] = pop2;
	instrucao[89] = dup;
	instrucao[90] = dup2_x1;
	instrucao[91] = dup2_x2;
	instrucao[92] = dup2;
	instrucao[93] = dup2_x1;
	instrucao[94] = dup2_x2;
	instrucao[95] = swap;
	instrucao[96] = iadd;
	instrucao[97] = ladd;
	instrucao[98] = fadd;
	instrucao[99] = dadd;
	instrucao[100] = isub;
	instrucao[101] = lsub;
	instrucao[102] = fsub;
	instrucao[103] = dsub;
	instrucao[104] = imul;
	instrucao[105] = lmul;
	instrucao[106] = fmul;
	instrucao[107] = dmul;
	instrucao[108] = idiv;
	instrucao[109] = _ldiv;
	instrucao[110] = fdiv;
	instrucao[111] = ddiv;
	instrucao[112] = irem;
	instrucao[113] = lrem;
	instrucao[114] = frem;
	instrucao[115] = _drem;
	instrucao[116] = ineg;
	instrucao[117] = lneg;
	instrucao[118] = fneg;
	instrucao[119] = dneg;
	instrucao[120] = ishl;
	instrucao[121] = lshl;
	instrucao[122] = ishr;
	instrucao[123] = lshr;
	instrucao[124] = iushr;
	instrucao[125] = lushr;
	instrucao[126] = iand;
	instrucao[127] = land;
	instrucao[128] = ior;
	instrucao[129] = lor;
	instrucao[130] = ixor;
	instrucao[131] = lxor;
	instrucao[132] = iinc;
	instrucao[133] = i2l;
	instrucao[134] = i2f;
	instrucao[135] = i2d;
	instrucao[136] = l2i;
	instrucao[137] = l2f;
	instrucao[138] = l2d;
	instrucao[139] = f2i;
	instrucao[140] = f2l;
	instrucao[141] = f2d;
	instrucao[142] = d2i;
	instrucao[143] = d2l;
	instrucao[144] = d2f;
	instrucao[145] = i2b;
	instrucao[146] = i2c;
	instrucao[147] = i2s;
	instrucao[148] = lcmp;
	instrucao[149] = fcmpl;
	instrucao[150] = fcmpg;
	instrucao[151] = dcmpl;
	instrucao[152] = dcmpg;
	instrucao[153] = ifeq;
	instrucao[154] = ifne;
	instrucao[155] = iflt;
	instrucao[156] = ifge;
	instrucao[157] = ifgt;
	instrucao[158] = ifle;
	instrucao[159] = if_icmpeq;
	instrucao[160] = if_icmpne;
	instrucao[161] = if_icmplt;
	instrucao[162] = if_icmpge;
	instrucao[163] = if_icmpgt;
	instrucao[164] = if_icmple;
	instrucao[165] = if_acmpeq;
	instrucao[166] = if_acmpne;
	instrucao[167] = _goto;
	instrucao[168] = jsr;
	instrucao[169] = ret;
	instrucao[170] = tableswitch;
	instrucao[171] = lookupswitch;
	instrucao[172] = ireturn;
	instrucao[173] = lreturn;
	instrucao[174] = freturn;
	instrucao[175] = dreturn;
	instrucao[176] = areturn;
	instrucao[177] = _return;
	instrucao[178] = getstatic;
	instrucao[179] = putstatic;
	instrucao[180] = getfield;
	instrucao[181] = putfield;
	instrucao[182] = invokevirtual;
	instrucao[183] = invokespecial;
	instrucao[184] = invokestatic;
	instrucao[185] = invokeinterface;
	instrucao[187] = _new;
	instrucao[188] = newarray;
	instrucao[189] = anewarray;
	instrucao[190] = arraylength;
	instrucao[192] = checkcast;
	instrucao[193] = instanceof;
	instrucao[196] = wide;
	instrucao[197] = multianewarray;
	instrucao[198] = ifnull;
	instrucao[199] = ifnonnull;
	instrucao[200] = goto_w;
	instrucao[201] = jsr_w;
}

///
/// Implementacao da instrucao <i>nop</i>, que nao executa nada,
/// apenas avanca o PC.
///
/// @param Nenhum
/// @return @c void
void nop() {
	frameCorrente->pc++;
}

///
/// Implementacao da instrucao <i>aconst_null</i> que empilha
/// uma referencia NULL na pilha de operandos e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp
void aconst_null() {
  pushOp((int32_t)NULL);
	frameCorrente->pc++;
}

///
/// Implementacao da instrucao <i>iconst_m1</i>
/// que empilha o valor -1 na pilha de operandos
/// e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp
void iconst_m1() {
  char* tipo = "I";
  tipoGlobal = tipo;
  pushOp(-1);
  frameCorrente->pc++;
}

///
/// Implementacao da instrucao <i>iconst_0</i>
/// que empilha o valor 0 na pilha de operandos
/// e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void iconst_0() {
  char* tipo = "I";
  tipoGlobal = tipo;
	pushOp((int32_t) 0);
	atualizaPc();
}

///
/// Implementacao da instrucao <i>iconst_1</i>
/// que empilha o valor 1 na pilha de operandos
/// e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp
void iconst_1() {
  char* tipo = "I";
  tipoGlobal = tipo;
  pushOp(1);
  frameCorrente->pc++;
}

///
/// Implementacao da instrucao <i>iconst_2</i>
/// que empilha o valor 2 na pilha de operandos
/// e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp
void iconst_2() {
  char* tipo = "I";
  tipoGlobal = tipo;
  pushOp(2);
  frameCorrente->pc++;
}

///
/// Implementacao da instrucao <i>iconst_3</i>
/// que empilha o valor 3 na pilha de operandos
/// e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp
void iconst_3() {
  char* tipo = "I";
  tipoGlobal = tipo;
  pushOp(3);
  frameCorrente->pc++;
}

///
/// Implementacao da instrucao <i>iconst_4</i>
/// que empilha o valor 4 na pilha de operandos
/// e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp
void iconst_4() {
  char* tipo = "I";
  tipoGlobal = tipo;
  pushOp(4);
  frameCorrente->pc++;
}

///
/// Implementacao da instrucao <i>iconst_5</i>
/// que empilha o valor 0 na pilha de operandos
/// e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp
void iconst_5() {
  char* tipo = "I";
  tipoGlobal = tipo;
  pushOp(5);
  frameCorrente->pc++;
}

///
/// Implementacao da instrucao <i>lconst_0</i>
/// que empilha o valor 0 na pilha de operandos
/// e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp
void lconst_0() {
  char* tipo = "L";
  tipoGlobal = tipo;
  int32_t parteAlta;
  int32_t parteBaixa;
  parteAlta = 0x0000;
  parteBaixa = 0x0000;
  pushOp(parteAlta);
  pushOp(parteBaixa);
  frameCorrente->pc++;
}

///
/// Implementacao da instrucao <lconst_1</i>
/// que empilha o valor 1 na pilha de operandos
/// e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp
void lconst_1() {
  char* tipo = "L";
  tipoGlobal = tipo;
  int32_t parteAlta;
  int32_t parteBaixa;
  parteAlta = 0x0000;
  parteBaixa = 0x0001;
  pushOp(parteAlta);
  pushOp(parteBaixa);
  frameCorrente->pc++;
}

///
/// Implementacao da instrucao <i>fconst_0</i>
/// que empilha o valor 0.0 na pilha de operandos
/// e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void fconst_0() {
  char* tipo = "F";
  tipoGlobal = tipo;
  int32_t* valPilha;
  float valF = 0.0;
  valPilha = (int32_t*) malloc(sizeof(int32_t));
  memcpy(valPilha, &valF, sizeof(int32_t));
  pushOp(*valPilha);
	atualizaPc();
}

///
/// Implementacao da instrucao <i>fconst_1</i>
/// que empilha o valor 1.0 na pilha de operandos
/// e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void fconst_1() {
  char* tipo = "F";
  tipoGlobal = tipo;
	int32_t* valPilha;
	float valF = 1.0;
	valPilha = (int32_t*) malloc(sizeof(int32_t));
	memcpy(valPilha, &valF, sizeof(int32_t));
	pushOp(*valPilha);
	atualizaPc();
}

///
/// Implementacao da instrucao <i>fconst_2</i>
/// que empilha o valor 2.0 na pilha de operandos
/// e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void fconst_2() {
  char* tipo = "F";
  tipoGlobal = tipo;
	int32_t* valPilha;
	float valF = 2.0;
	valPilha = (int32_t*) malloc(sizeof(int32_t));
	memcpy(valPilha, &valF, sizeof(int32_t));
	pushOp(*valPilha);
	atualizaPc();
}

///
/// Implementacao da instrucao <i>dconst_0</i>
/// que empilha o valor 0.0 na pilha de operandos
/// e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp
void dconst_0() {
  char* tipo = "D";
  tipoGlobal = tipo;
  double double0 = 0.0;
  int64_t temp;
  int32_t parteAlta;
  int32_t parteBaixa;
	memcpy(&temp, &double0, sizeof(int64_t));
	parteAlta = temp >> 32;
	parteBaixa = temp & 0xffffffff;
  pushOp(parteAlta);
  pushOp(parteBaixa);
  frameCorrente->pc++;
}

///
/// Implementacao da instrucao <i>dconst_1</i>
/// que empilha o valor 1.0 na pilha de operandos
/// e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp
void dconst_1() {
  char* tipo = "D";
  tipoGlobal = tipo;
  double double1 = 1.0;
  int64_t temp;
  int32_t parteAlta;
  int32_t parteBaixa;
	memcpy(&temp, &double1, sizeof(int64_t));
	parteAlta = temp >> 32;
	parteBaixa = temp & 0xffffffff;
  pushOp(parteAlta);
  pushOp(parteBaixa);
  frameCorrente->pc++;
}

///
/// Empilha um byte de argumento na pilha extendendo
/// o Imediato como se fosse um valor inteiro e
/// avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void bipush() {
	int8_t argumento = (int8_t) frameCorrente->code[frameCorrente->pc + 1];
	pushOp((int32_t)argumento);
	atualizaPc();
}

///
/// Empilha os dois bytes de argumento como um short
/// que eh extendido para um inteiro e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void sipush() {
  uint8_t byte1, byte2;
  int16_t shortTemp;
	byte1 = frameCorrente->code[(frameCorrente->pc + 1)];
	byte2 = frameCorrente->code[(frameCorrente->pc + 2)];
	shortTemp = byte1;
  shortTemp = (shortTemp << 8) + byte2;
  pushOp((int32_t) shortTemp);
  atualizaPc();
}

///
/// Empilha um valor inteiro / float / string
/// recuperado da Constant Pool na pilha de operandos,
/// a partir do argumento indice.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc resolveUTF
void ldc() {
  uint32_t indice;
  tipoGlobal = NULL;
  indice = frameCorrente->code[frameCorrente->pc + 1];
  if (frameCorrente->constantPool[indice - 1].tag == CONSTANT_Float) {
    pushOp(frameCorrente->constantPool[indice - 1].info.Float.bytes);
  } else if (frameCorrente->constantPool[indice - 1].tag == CONSTANT_Integer) {
  	pushOp(frameCorrente->constantPool[indice - 1].info.Integer.bytes);
  } else if (frameCorrente->constantPool[indice - 1].tag == CONSTANT_String) {
    uint32_t indice_utf;
    indice_utf = resolveUTF(frameCorrente->constantPool, indice-1);
    pushOp(indice_utf);
  } else {
    printf("Erro na instrução ldc \n");
    exit(1);
  }
  atualizaPc();
}

///
/// Empilha um valor inteiro / float / string
/// recuperado da Constant Pool na pilha de operandos,
/// a partir do argumento indice que agora possui
/// dois bytes.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc resolveUTF
void ldc_w() {
  uint32_t indice;
  indice = ((frameCorrente->code[frameCorrente->pc + 1] << 8) + (frameCorrente->code[frameCorrente->pc + 2]));
  if (frameCorrente->constantPool[indice - 1].tag == CONSTANT_Float) {
    pushOp(frameCorrente->constantPool[indice - 1].info.Float.bytes);
  } else if (frameCorrente->constantPool[indice - 1].tag == CONSTANT_Integer) {
    pushOp(frameCorrente->constantPool[indice - 1].info.Integer.bytes);
  } else if (frameCorrente->constantPool[indice - 1].tag == CONSTANT_String) {
    uint32_t indice_utf;
    indice_utf = resolveUTF(frameCorrente->constantPool, indice-1);
    pushOp(indice_utf);
  } else {
    printf("Erro na instrução ldc_w \n");
    exit(1);
  }
	atualizaPc();
}

///
/// Empilha um valor long / double
/// recuperado da Constant Pool na pilha de operandos,
/// a partir do argumento indice que possui
/// dois bytes.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void ldc2_w() {
	uint16_t indice = ((frameCorrente->code[frameCorrente->pc + 1] << 8) + (frameCorrente->code[frameCorrente->pc + 2]));
	uint8_t tag = (frameCorrente->constantPool[indice-1]).tag;

	if(tag == 5) {
		uint32_t alta = frameCorrente->constantPool[indice-1].info.Long.highBytes;
		uint32_t baixa = frameCorrente->constantPool[indice-1].info.Long.lowBytes;
		pushOp(alta);
		pushOp(baixa);
	}

	if(tag == 6) {
		uint32_t alta = frameCorrente->constantPool[indice-1].info.Double.highBytes;
		uint32_t baixa = frameCorrente->constantPool[indice-1].info.Double.lowBytes;
		pushOp(alta);
		pushOp(baixa);
	}

	atualizaPc();
  flagLNEG = FALSE;
}

///
/// Coloca um inteiro na pilha de operandos
/// a partir de um indice para o array de
/// variaveis locais.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void iload() {
  char* tipo = "I";
  tipoGlobal = tipo;
	int32_t indice = (int32_t) frameCorrente->code[frameCorrente->pc + 1];
	int32_t argumento = frameCorrente->fields[indice];
	pushOp(argumento);
	atualizaPc();
}

///
/// Coloca um long na pilha de operandos
/// a partir de um indice para o array de
/// variaveis locais.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void lload() {
	char* tipo = "L";
  tipoGlobal = tipo;
  int32_t indice;
  int32_t parteAlta;
  int32_t parteBaixa;
  indice = frameCorrente->code[frameCorrente->pc + 1];
  parteAlta = frameCorrente->fields[indice + 0];
  parteBaixa = frameCorrente->fields[indice + 1];
  pushOp(parteAlta);
  pushOp(parteBaixa);
  atualizaPc();
}

///
/// Coloca um float na pilha de operandos
/// a partir de um indice para o array de
/// variaveis locais.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void fload() {
	char* tipo = "F";
  tipoGlobal = tipo;
  int32_t indice;
  int32_t valor;
  indice = frameCorrente->code[frameCorrente->pc + 1];
  valor = frameCorrente->fields[indice];
  pushOp(valor);
  atualizaPc();
}

///
/// Coloca um double na pilha de operandos
/// a partir de um indice para o array de
/// variaveis locais.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void dload() {
  int32_t indice;
  int32_t parteAlta;
  int32_t parteBaixa;
  char* tipo = "D";
  tipoGlobal = tipo;
  indice = frameCorrente->code[frameCorrente->pc + 1];
  parteAlta = frameCorrente->fields[indice + 0];
  parteBaixa = frameCorrente->fields[indice + 1];
  pushOp(parteAlta);
  pushOp(parteBaixa);
  atualizaPc();
}

///
/// Coloca uma referencia a partir
/// do array de variaveis locais
/// na pilha
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void aload() {
    int32_t indice;
    int32_t valor;
    indice = frameCorrente->code[frameCorrente->pc + 1];
    valor = frameCorrente->fields[indice];
    pushOp(valor);
    atualizaPc();
}

///
/// Coloca a primeira variavel inteira
/// do array de variaveis locais
/// na pilha
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void iload_0() {
	char* tipo = "I";
  tipoGlobal = tipo;
  int32_t valor;
  valor = frameCorrente->fields[0];
  pushOp(valor);
	atualizaPc();
}

///
/// Coloca a segunda variavel inteira
/// do array de variaveis locais
/// na pilha
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void iload_1() {
	char* tipo = "I";
  tipoGlobal = tipo;
  int32_t valor;
  valor = frameCorrente->fields[1];
  pushOp(valor);
  atualizaPc();
}

///
/// Coloca a terceira variavel inteira
/// do array de variaveis locais
/// na pilha
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void iload_2() {
	char* tipo = "I";
  tipoGlobal = tipo;
  int32_t valor;
  valor = frameCorrente->fields[2];
  pushOp(valor);
  atualizaPc();
}

///
/// Coloca a quarta variavel inteira
/// do array de variaveis locais
/// na pilha
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void iload_3() {
  int32_t valor;
  char* tipo = "I";
  tipoGlobal = tipo;
  valor = frameCorrente->fields[3];
  pushOp(valor);
  atualizaPc();
}

///
/// Coloca a primeira variavel
/// do array de variaveis locais
/// na pilha (long)
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void lload_0() {
	char* tipo = "L";
  tipoGlobal = tipo;
  int32_t indice;
  int32_t parteAlta;
  int32_t parteBaixa;
  indice = 0;
  parteAlta = frameCorrente->fields[indice + 0];
  parteBaixa = frameCorrente->fields[indice + 1];
  pushOp(parteAlta);
  pushOp(parteBaixa);
	atualizaPc();
}

///
/// Coloca a segunda variavel
/// do array de variaveis locais
/// na pilha (long)
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void lload_1() {
  int32_t indice;
  int32_t parteAlta;
  int32_t parteBaixa;
  char* tipo = "L";
  tipoGlobal = tipo;
  indice = 1;
  parteAlta = frameCorrente->fields[indice + 0];
  parteBaixa = frameCorrente->fields[indice + 1];
  pushOp(parteAlta);
  pushOp(parteBaixa);
  atualizaPc();
}

///
/// Coloca a terceira variavel
/// do array de variaveis locais
/// na pilha (long)
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void lload_2() {
	char* tipo = "L";
  tipoGlobal = tipo;
  int32_t indice;
  int32_t parteAlta;
  int32_t parteBaixa;
  indice = 2;
  parteAlta = frameCorrente->fields[indice + 0];
  parteBaixa = frameCorrente->fields[indice + 1];
  pushOp(parteAlta);
  pushOp(parteBaixa);
  atualizaPc();
}

///
/// Coloca a quarta variavel
/// do array de variaveis locais
/// na pilha (long)
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void lload_3() {
	char* tipo = "L";
  tipoGlobal = tipo;
  int32_t indice;
  int32_t parteAlta;
  int32_t parteBaixa;
  indice = 3;
  parteAlta = frameCorrente->fields[indice + 0];
  parteBaixa = frameCorrente->fields[indice + 1];
  pushOp(parteAlta);
  pushOp(parteBaixa);
  atualizaPc();
}

///
/// Coloca a primeira variavel
/// do array de variaveis locais
/// na pilha (float)
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void fload_0() {
	char* tipo = "F";
  tipoGlobal = tipo;
  int32_t indice;
  int32_t valor;
  indice = 0;
  valor = frameCorrente->fields[indice];
  pushOp(valor);
  atualizaPc();
}

///
/// Coloca a segunda variavel
/// do array de variaveis locais
/// na pilha (float)
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void fload_1() {
	char* tipo = "F";
  tipoGlobal = tipo;
  int32_t indice;
  int32_t valor;
  indice = 1;
  valor = frameCorrente->fields[indice];
  pushOp(valor);
  atualizaPc();
}

///
/// Coloca a terceira variavel
/// do array de variaveis locais
/// na pilha (float)
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void fload_2() {
	char* tipo = "F";
  tipoGlobal = tipo;
  int32_t indice;
  int32_t valor;
  indice = 2;
  valor = frameCorrente->fields[indice];
  pushOp(valor);
  atualizaPc();
}

///
/// Coloca a quarta variavel
/// do array de variaveis locais
/// na pilha (float)
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void fload_3() {
	char* tipo = "F";
  tipoGlobal = tipo;
  int32_t indice;
  int32_t valor;
  indice = 3;
  valor = frameCorrente->fields[indice];
  pushOp(valor);
  atualizaPc();
}

///
/// Coloca a primeira variavel
/// do array de variaveis locais
/// na pilha (double)
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void dload_0() {
	char* tipo = "D";
  tipoGlobal = tipo;
  int32_t indice;
  int32_t parteAlta;
  int32_t parteBaixa;
  indice = 0;
  parteAlta = frameCorrente->fields[indice + 0];
  parteBaixa = frameCorrente->fields[indice + 1];
  pushOp(parteAlta);
  pushOp(parteBaixa);
  atualizaPc();
}

///
/// Coloca a segunda variavel
/// do array de variaveis locais
/// na pilha (double)
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void dload_1() {
  int32_t indice;
  int32_t parteAlta;
  int32_t parteBaixa;
  char* tipo = "D";
  tipoGlobal = tipo;
  indice = 1;
  parteAlta = frameCorrente->fields[indice + 0];
  parteBaixa = frameCorrente->fields[indice + 1];
  pushOp(parteAlta);
  pushOp(parteBaixa);
  atualizaPc();
}

///
/// Coloca a terceira variavel
/// do array de variaveis locais
/// na pilha (double)
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void dload_2() {
  int32_t indice;
  int32_t parteAlta;
  int32_t parteBaixa;
  char* tipo = "D";
  tipoGlobal = tipo;
  indice = 2;
  parteAlta = frameCorrente->fields[indice + 0];
  parteBaixa = frameCorrente->fields[indice + 1];
  pushOp(parteAlta);
  pushOp(parteBaixa);
  atualizaPc();
}

///
/// Coloca a quarta variavel
/// do array de variaveis locais
/// na pilha (double)
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void dload_3() {
  int32_t indice;
  int32_t parteAlta;
  int32_t parteBaixa;
  char* tipo = "D";
  tipoGlobal = tipo;
  indice = 3;
  parteAlta = frameCorrente->fields[indice + 0];
  parteBaixa = frameCorrente->fields[indice + 1];
  pushOp(parteAlta);
  pushOp(parteBaixa);
  atualizaPc();
}

///
/// Coloca uma referencia a partir
/// da primeira posicao do
/// array de variaveis locais
/// na pilha
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void aload_0() {
	pushOp(frameCorrente->fields[0]);
	atualizaPc();
}

///
/// Coloca uma referencia a partir
/// da segunda posicao do
/// array de variaveis locais
/// na pilha
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void aload_1() {
  int32_t indice, valor;
  indice = 1;
  valor = frameCorrente->fields[indice];
  pushOp(valor);
  atualizaPc();
}

///
/// Coloca uma referencia a partir
/// da terceira posicao do
/// array de variaveis locais
/// na pilha
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void aload_2() {
  int32_t indice, valor;
  indice = 2;
  valor = frameCorrente->fields[indice];
  pushOp(valor);
  atualizaPc();
}

///
/// Coloca uma referencia a partir
/// da quarta posicao do
/// array de variaveis locais
/// na pilha
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc
void aload_3() {
  int32_t indice;
  int32_t valor;
  indice = 3;
  valor = frameCorrente->fields[indice];
  pushOp(valor);
  atualizaPc();
}

///
/// Coloca na pilha de operandos
/// um valor inteiro de um array
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc popOp
void iaload() {
	int32_t* referencia;
	int32_t indice = popOp();
	referencia = (int32_t*)popOp();
	pushOp(referencia[indice]);
	atualizaPc();
}

///
/// Coloca na pilha de operandos
/// um valor long de um array
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc popOp
void laload() {
	char* tipo = "L";
  tipoGlobal = tipo;
	int32_t indice = popOp();
	int32_t* referencia;
	referencia = (int32_t*)popOp();
	pushOp(referencia[(2*indice)+1]);
	pushOp(referencia[(2*indice)]);
	atualizaPc();
}

///
/// Coloca na pilha de operandos
/// um valor float de um array
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc popOp
void faload() {
	char* tipo = "F";
  tipoGlobal = tipo;
	int32_t* referencia;
	int32_t indice = popOp();
	referencia = (int32_t*)popOp();
	int32_t valPilha;
	memcpy(&valPilha, &((float *)referencia)[indice], sizeof(int32_t));
	pushOp(valPilha);
	atualizaPc();
}

///
/// Coloca na pilha de operandos
/// um valor double de um array
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc popOp
void daload() {
	char* tipo = "D";
  tipoGlobal = tipo;
	int32_t indice = popOp();
	int32_t* referencia;
	referencia = (int32_t*)popOp();
	pushOp(referencia[(2*indice)+1]);
	pushOp(referencia[(2*indice)]);
	atualizaPc();
}

///
/// Coloca na pilha de operandos
/// um valor de referencia
/// extraido de um array
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc popOp
void aaload() {
	int32_t* referencia;
	int32_t indice = popOp();
	referencia = (int32_t*)popOp();
	pushOp(referencia[indice]);
	atualizaPc();
}

///
/// Coloca na pilha de operandos
/// um byte / booleano extraido
/// de um array
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc popOp
void baload() {
	int32_t* referencia;
	int32_t indice = popOp();
	referencia = (int32_t*)popOp();
	int8_t* binary = (int8_t*)referencia[indice];
	pushOp((int32_t)binary);
	atualizaPc();
}

///
/// Coloca na pilha de operandos
/// um char extraido de um array
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc popOp
void caload() {
	char* tipo = "C";
  tipoGlobal = tipo;
	int32_t* referencia;
	int32_t indice = popOp();
	referencia = (int32_t*)popOp();
	int16_t* charTemp = (int16_t*)referencia[indice];
	pushOp((int32_t)charTemp);
	atualizaPc();
}

///
/// Coloca na pilha de operandos
/// um short extraido de um array
///
/// @param Nenhum
/// @return @c void
/// @see pushOp atualizaPc popOp
void saload() {
	int32_t* referencia;
	int32_t indice = popOp();
	referencia = (int32_t*)popOp();
	int16_t* shortTemp = (int16_t*)referencia[indice];
	pushOp((int32_t)shortTemp);
	atualizaPc();
}

///
/// Coloca no array de variaveis
/// locais o inteiro desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void istore() {
  int32_t indice;
  int32_t valor;
  indice = frameCorrente->code[frameCorrente->pc + 1];
  valor = popOp();
  frameCorrente->fields[indice] = valor;
  atualizaPc();
}

///
/// Coloca no array de variaveis
/// locais o long desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void lstore() {
  int32_t indice;
  int32_t parteAlta, parteBaixa;
  indice = frameCorrente->code[frameCorrente->pc + 1];
  parteBaixa = popOp();
  parteAlta = popOp();
  frameCorrente->fields[indice ] = parteAlta;
  frameCorrente->fields[indice + 1] = parteBaixa;
  atualizaPc();
}
///
/// Coloca no array de variaveis
/// locais o float desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void fstore() {
  int32_t indice;
  int32_t valor;
  indice = frameCorrente->code[frameCorrente->pc + 1];
  valor = popOp();
  frameCorrente->fields[indice] = valor;
  atualizaPc();
}
///
/// Coloca no array de variaveis
/// locais o double desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void dstore() {
  int32_t indice;
  int32_t parteAlta, parteBaixa;
  indice = frameCorrente->code[frameCorrente->pc + 1];
  parteBaixa = popOp();
  parteAlta = popOp();
  frameCorrente->fields[indice ] = parteAlta;
  frameCorrente->fields[indice + 1] = parteBaixa;
  atualizaPc();
}
///
/// Coloca no array de variaveis
/// locais uma referencia desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void astore() {
  int32_t indice;
  int32_t valor;
  indice = frameCorrente->code[frameCorrente->pc + 1];
  valor = popOp();
  frameCorrente->fields[indice] = valor;
  atualizaPc();
}
///
/// Coloca no array de variaveis
/// locais na posicao 0 um inteiro desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void istore_0() {
  int32_t indice;
  int32_t valor;
  indice = 0;
  valor = popOp();
  frameCorrente->fields[indice] = valor;
  atualizaPc();
}
///
/// Coloca no array de variaveis
/// locais na posicao 1 um inteiro desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void istore_1() {
	int32_t indice;
  uint32_t valor;
	indice = 1;
  valor = popOp();
  frameCorrente->fields[1] = valor;
  atualizaPc();
}
///
/// Coloca no array de variaveis
/// locais na posicao 2 um inteiro desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void istore_2() {
  int32_t indice;
  int32_t valor;
  indice = 2;
  valor = popOp();
  frameCorrente->fields[indice] = valor;
  atualizaPc();
}
///
/// Coloca no array de variaveis
/// locais na posicao 3 um inteiro desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void istore_3() {
  int32_t indice;
  int32_t valor;
  indice = 3;
  valor = popOp();
  frameCorrente->fields[indice] = valor;
  atualizaPc();
}
///
/// Coloca no array de variaveis
/// locais na posicao 0 um long desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void lstore_0() {
  int32_t indice;
  int32_t parteAlta, parteBaixa;
  indice = 0;
  parteBaixa = popOp();
  parteAlta = popOp();
  frameCorrente->fields[indice] = parteAlta;
  frameCorrente->fields[indice + 1] = parteBaixa;
  atualizaPc();
}
///
/// Coloca no array de variaveis
/// locais na posicao 1 um long desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void lstore_1() {
  int32_t indice;
  int32_t parteAlta, parteBaixa;
  indice = 1;
  parteBaixa = popOp();
  parteAlta = popOp();
  frameCorrente->fields[indice] = parteAlta;
  frameCorrente->fields[indice + 1] = parteBaixa;
  atualizaPc();
}
///
/// Coloca no array de variaveis
/// locais na posicao 2 um long desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void lstore_2() {
  int32_t indice;
  int32_t parteAlta, parteBaixa;
  indice = 2;
  parteBaixa = popOp();
  parteAlta = popOp();
  frameCorrente->fields[indice] = parteAlta;
  frameCorrente->fields[indice + 1] = parteBaixa;
  atualizaPc();
}
///
/// Coloca no array de variaveis
/// locais na posicao 3 um long desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void lstore_3() {
  int32_t indice;
  int32_t parteAlta, parteBaixa;
  indice = 3;
  parteBaixa = popOp();
  parteAlta = popOp();
  frameCorrente->fields[indice] = parteAlta;
  frameCorrente->fields[indice + 1] = parteBaixa;
  atualizaPc();
}

///
/// Coloca no array de variaveis
/// locais na posicao 0 um float desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void fstore_0() {
  int32_t indice;
  int32_t valor;
  indice = 0;
  valor = popOp();
  frameCorrente->fields[indice] = valor;
  atualizaPc();
}


///
/// Coloca no array de variaveis
/// locais na posicao 1 um float desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void fstore_1() {
  int32_t indice;
  int32_t valor;
  indice = 1;
  valor = popOp();
  frameCorrente->fields[indice] = valor;
  atualizaPc();
}

///
/// Coloca no array de variaveis
/// locais na posicao 2 um float desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void fstore_2() {
  int32_t indice;
  int32_t valor;
  indice = 2;
  valor = popOp();
  frameCorrente->fields[indice] = valor;
  atualizaPc();
}

///
/// Coloca no array de variaveis
/// locais na posicao 3 um float desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void fstore_3() {
  int32_t indice;
  int32_t valor;
  indice = 3;
  valor = popOp();
  frameCorrente->fields[indice] = valor;
  atualizaPc();
}

///
/// Coloca no array de variaveis
/// locais na posicao 0 um double desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void dstore_0() {
  int32_t indice;
  int32_t parteAlta, parteBaixa;
  indice = 0;
  parteBaixa = popOp();
  parteAlta = popOp();
  frameCorrente->fields[indice] = parteAlta;
  frameCorrente->fields[indice + 1] = parteBaixa;
  atualizaPc();
}

///
/// Coloca no array de variaveis
/// locais na posicao 1 um double desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void dstore_1() {
  int32_t indice;
  int32_t parteAlta, parteBaixa;
  indice = 1;
  parteBaixa = popOp();
  parteAlta = popOp();
  frameCorrente->fields[indice] = parteAlta;
  frameCorrente->fields[indice + 1] = parteBaixa;
  atualizaPc();
}


///
/// Coloca no array de variaveis
/// locais na posicao 2 um double desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void dstore_2() {
  int32_t indice;
  int32_t parteAlta, parteBaixa;
  indice = 2;
  parteBaixa = popOp();
  parteAlta = popOp();
  frameCorrente->fields[indice] = parteAlta;
  frameCorrente->fields[indice + 1] = parteBaixa;
  atualizaPc();
}

///
/// Coloca no array de variaveis
/// locais na posicao 3 um double desempilhado
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void dstore_3() {
  int32_t indice;
  int32_t parteAlta, parteBaixa;
  indice = 3;
  parteBaixa = popOp();
  parteAlta = popOp();
  frameCorrente->fields[indice] = parteAlta;
  frameCorrente->fields[indice + 1] = parteBaixa;
  atualizaPc();
}

///
/// Coloca no array de variaveis
/// locais na posicao 0 uma referencia desempilhada
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void astore_0() {
    int32_t indice;
    int32_t valor;
    indice = 0;
    valor = popOp();
    frameCorrente->fields[indice] = valor;
    atualizaPc();
}
///
/// Coloca no array de variaveis
/// locais na posicao 1 uma referencia desempilhada
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void astore_1() {
    int32_t indice;
    int32_t valor;
    indice = 1;
    valor = popOp();
    frameCorrente->fields[indice] = valor;
    atualizaPc();
}
///
/// Coloca no array de variaveis
/// locais na posicao 2 uma referencia desempilhada
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void astore_2() {
    int32_t indice;
    int32_t valor;
    indice = 2;
    valor = popOp();
    frameCorrente->fields[indice] = valor;
    atualizaPc();
}
///
/// Coloca no array de variaveis
/// locais na posicao 3 uma referencia desempilhada
/// da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void astore_3() {
    int32_t indice;
    int32_t valor;
    indice = 3;
    valor = popOp();
    frameCorrente->fields[indice] = valor;
    atualizaPc();
}
///
/// Coloca um inteiro em um array.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void iastore() {
	int32_t* ref;
	int32_t indice, valor;
	valor = popOp();
	indice = popOp();
	ref = (int32_t*)popOp();
	ref[indice] = valor;
	atualizaPc();
}
///
/// Coloca um long em um array.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void lastore() {
	int32_t alta,baixa;
	baixa = popOp();
	alta = popOp();
	int32_t indice = popOp();
	int32_t* referencia;
	referencia = (int32_t*) popOp();
	referencia[2*indice] = baixa;
	referencia[(2*indice) + 1] = alta;
	atualizaPc();
}

///
/// Coloca um float em um array.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void fastore() {
	char* tipo = "F";
    tipoGlobal = tipo;
	int32_t* ref;
	int32_t indice,valor;
	valor = popOp();
	indice = popOp();
	ref = (int32_t*)popOp();
	ref[indice] = valor;
	atualizaPc();
}
///
/// Coloca um double em um array.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void dastore() {
	int32_t alta,baixa;

	baixa = popOp();
	alta = popOp();

	int32_t indice = popOp();
	int32_t* referencia;
	referencia = (int32_t*) popOp();

	referencia[(2*indice)] = baixa;
	referencia[(2*indice) + 1] = alta;

	atualizaPc();
}
///
/// Coloca uma referencia em um array.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void aastore() {
	int32_t* ref;
	int32_t indice,valor;
	valor = popOp();
	indice = popOp();
	ref = (int32_t*)popOp();
	ref[indice] = valor;
	atualizaPc();
}
///
/// Coloca um byte/booleano em um array.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void bastore() {
	int32_t* ref;
	int32_t indice,valor;
	valor = popOp();
	indice = popOp();
	ref = (int32_t*)popOp();
	ref[indice] = (int8_t)valor;
	atualizaPc();
}
///
/// Coloca um char em um array.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void castore() {
	int32_t* ref;
	int32_t indice,valor;
	valor = popOp();
	indice = popOp();
	ref = (int32_t*)popOp();
	ref[indice] = (int16_t)valor;
	atualizaPc();
}
///
/// Coloca um short em um array.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void sastore() {
	int32_t* ref;
	int32_t indice,valor;
	valor = popOp();
	indice = popOp();
	ref = (int32_t*)popOp();
	ref[indice] = (int16_t)valor;
	atualizaPc();
}
///
/// Retira o valor do topo da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void pop() {
	popOp();
	atualizaPc();
}
///
/// Retira os dois primeiros valores do topo da pilha
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp
void pop2() {
	popOp();
	popOp();
	atualizaPc();
}
///
/// Duplica o valor do topo da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void dup() {
	int32_t topo;

	topo = popOp();
	pushOp(topo);
	pushOp(topo);
	atualizaPc();
}
///
/// Insere uma copia do valor do topo da pilha duas posicoes abaixo do topo da pilha
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void dup_x1() {
	int32_t topo, aux;

	topo = popOp();
	aux = popOp();

	pushOp(topo);
	pushOp(aux);
	pushOp(topo);

	atualizaPc();
}
///
/// Insere uma copia do valor do topo da pilha tres posicoes abaixo do topo da pilha
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void dup_x2() {
	int32_t topo, aux, aux2;
	topo = popOp();
	aux = popOp();
	aux2 = popOp();
	pushOp(topo);
	pushOp(aux2);
	pushOp(aux);
	pushOp(topo);
	atualizaPc();
}
///
/// Duplica os dois primeiros valores da pilha
/// (duas primeiras words) como se o valor no topo
/// fosse um long
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void dup2() {
	int32_t topo, topo2;
	topo = popOp();
	topo2 = popOp();
	pushOp(topo2);
	pushOp(topo);
	pushOp(topo2);
	pushOp(topo);
	atualizaPc();
}
///
/// Insere uma copia dos dois primeiros valores da pilha
/// (duas primeiras words) como se o valor no topo
/// fosse um long duas posicoes abaixo da pilha.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void dup2_x1() {
	int32_t topo, topo2, aux;
	topo = popOp();
	topo2 = popOp();
	aux = popOp();
	pushOp(topo2);
	pushOp(topo);
	pushOp(aux);
	pushOp(topo2);
	pushOp(topo);
	atualizaPc();
}
///
/// Insere uma copia dos dois primeiros valores da pilha
/// (duas primeiras words) como se o valor no topo
/// fosse um long tres posicoes abaixo da pilha.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void dup2_x2() {
	int32_t topo, topo2, aux, aux2;
	topo = popOp();
	topo2 = popOp();
	aux = popOp();
	aux2 = popOp();
	pushOp(topo2);
	pushOp(topo);
	pushOp(aux2);
	pushOp(aux);
	pushOp(topo2);
	pushOp(topo);
	atualizaPc();
}
///
/// Troca o valor do topo da pilha com
/// o segundo valor da pilha
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void swap() {
	int32_t val1,val2;
	val1 = popOp();
	val2 = popOp();
	pushOp(val1);
	pushOp(val2);
	atualizaPc();
}
///
/// Soma dois valores inteiros e empilha o
/// resultado na pilha.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void iadd() {
	int32_t v1,v2;
	v2 = popOp();
	v1 = popOp();
	pushOp(v1+v2);
	atualizaPc();
}
///
/// Soma dois valores long e empilha o
/// resultado na pilha.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void ladd() {
	int32_t baixa,alta;
	baixa = popOp();
	alta = popOp();

	int64_t val1 = alta;
	val1 <<= 32;
	val1 = val1 + baixa;

	baixa = popOp();
	alta = popOp();

	int64_t val2 = alta;
	val2 <<= 32;
	val2 = val2 + baixa;

	int64_t resultado = val2 + val1;

	alta = resultado >> 32;
	baixa = resultado & 0xffffffff;

	pushOp(alta);
	pushOp(baixa);

	atualizaPc();
}
///
/// Soma dois valores float e empilha o
/// resultado na pilha.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void fadd() {
	float fVal1,fVal2;

	int32_t aux1 = popOp();
	int32_t aux2 = popOp();

	memcpy(&fVal1, &aux1, sizeof(int32_t));
	memcpy(&fVal2, &aux2, sizeof(int32_t));

	float resultado = fVal1 + fVal2;

	int32_t retPilha;
	memcpy(&retPilha, &resultado, sizeof(int32_t));

	pushOp(retPilha);

	atualizaPc();

}
///
/// Soma dois valores double e empilha o
/// resultado na pilha.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void dadd() {
	int32_t alta;
	int32_t baixa;

	baixa = popOp();
	alta = popOp();

	int64_t dVal = alta;
	dVal <<= 32;
	dVal = dVal + baixa;

	double valorDouble1;
	memcpy(&valorDouble1, &dVal, sizeof(int64_t));

	baixa = popOp();
	alta = popOp();

	dVal = alta;
	dVal <<= 32;
	dVal = dVal + baixa;

	double valorDouble2;
	memcpy(&valorDouble2, &dVal, sizeof(int64_t));

	double doubleSomado = valorDouble1 + valorDouble2;

	int64_t valorPilha;
	memcpy(&valorPilha, &doubleSomado, sizeof(int64_t));

	alta = valorPilha >> 32;
	baixa = valorPilha & 0xffffffff;

	pushOp(alta);
	pushOp(baixa);

	atualizaPc();
}
///
/// Subtrai dois valores inteiros e empilha o
/// resultado na pilha.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void isub() {
	int32_t v1,v2;
	v2 = popOp();
	v1 = popOp();
	pushOp(v1-v2);
	atualizaPc();
}
///
/// Subtrai dois valores long e empilha o
/// resultado na pilha.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void lsub() {
	int32_t baixa,alta;

	baixa = popOp();
	alta = popOp();

	int64_t lVal = alta;
	lVal <<= 32;
	lVal = lVal + baixa;

	baixa = popOp();
	alta = popOp();

	int64_t lVal1 = alta;
	lVal1 <<= 32;
	lVal1 = lVal1 + baixa;

	int64_t resultado = lVal1 - lVal;

	alta = resultado >> 32;
	baixa = resultado & 0xffffffff;

	pushOp(alta);
	pushOp(baixa);
	atualizaPc();
}
///
/// Subtrai dois valores float e empilha o
/// resultado na pilha.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void fsub() {
	float fVal1,fVal2;

	int32_t aux2 = popOp();
	int32_t aux1 = popOp();

	memcpy(&fVal1, &aux1, sizeof(int32_t));
	memcpy(&fVal2, &aux2, sizeof(int32_t));

	float resultado = fVal1 - fVal2;

	int32_t retPilha;
	memcpy(&retPilha, &resultado, sizeof(int32_t));

	pushOp(retPilha);
	atualizaPc();
}
///
/// Subtrai dois valores double e empilha o
/// resultado na pilha.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void dsub() {
	int32_t alta;
	int32_t baixa;

	baixa = popOp();
	alta = popOp();

	int64_t dVal = alta;
	dVal <<= 32;
	dVal = dVal + baixa;

	double valorDouble1;
	memcpy(&valorDouble1, &dVal, sizeof(int64_t));

	baixa = popOp();
	alta = popOp();

	dVal = alta;
	dVal <<= 32;
	dVal = dVal + baixa;

	double valorDouble2;
	memcpy(&valorDouble2, &dVal, sizeof(int64_t));

	double doubleSubtraido = valorDouble2 - valorDouble1;

	int64_t valorPilha;
	memcpy(&valorPilha, &doubleSubtraido, sizeof(int64_t));

	alta = valorPilha >> 32;
	baixa = valorPilha & 0xffffffff;

	pushOp(alta);
	pushOp(baixa);
	atualizaPc();
}

///
/// Desempilha dois inteiros da pilha de
/// de operandos e multiplica um pelo outro.
/// Do resultado, sao considerados apenas
/// os 32bits da parte baixa.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void imul() {
	int32_t v1 = popOp();
	int32_t v2 = popOp();
	pushOp((int32_t)(v1 * v2));
	atualizaPc();
}

///
/// Desempilha dois longs da pilha de
/// de operandos e multiplica um pelo outro.
/// Do resultado, sao considerados apenas
/// os 64bits da parte baixa, que sao
/// dividos em dois valores de 32bits
/// para serem salvos na pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void lmul() {
	int32_t parteBaixa, parteAlta;

	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorLong = parteAlta;
	valorLong = (valorLong << 32) + parteBaixa;

	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorLong2 = parteAlta;
	valorLong2 = (valorLong2 << 32) + parteBaixa;

	int64_t resultado = valorLong2 * valorLong;

	parteAlta = resultado >> 32;
	parteBaixa = resultado & 0xfffffff;

	pushOp(parteAlta);
	pushOp(parteBaixa);
	atualizaPc();
}

///
/// Desempilha dois floats da pilha de
/// de operandos e multiplica um pelo outro.
/// O resultado entao eh salvo na pilha de
/// operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void fmul() {
	float valorFloat, valorFloat2;

	int32_t valor1 = popOp();
	int32_t valor2 = popOp();

	memcpy(&valorFloat, &valor1, sizeof(int32_t));
	memcpy(&valorFloat2, &valor2, sizeof(int32_t));

	float resultado = valorFloat * valorFloat2;

	int32_t resPilha;
	memcpy(&resPilha, &resultado, sizeof(int32_t));

	pushOp(resPilha);
	atualizaPc();
}

///
/// Desempilha dois doubles da pilha de
/// de operandos e multiplica um pelo outro.
/// O resultado entao eh salvo na pilha
/// de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void dmul() {
	int32_t parteAlta;
	int32_t parteBaixa;

	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorDouble = parteAlta;
	valorDouble = (valorDouble << 32) + parteBaixa;

	double valorDouble1;
	memcpy(&valorDouble1, &valorDouble, sizeof(int64_t));

	parteBaixa = popOp();
	parteAlta = popOp();

	valorDouble = parteAlta;
	valorDouble = (valorDouble << 32) + parteBaixa;

	double valorDouble2;
	memcpy(&valorDouble2, &valorDouble, sizeof(int64_t));

	double resultado = valorDouble1 * valorDouble2;

	int64_t valorPilha;
	memcpy(&valorPilha, &resultado, sizeof(int64_t));

	parteAlta = valorPilha >> 32;
	parteBaixa = valorPilha & 0xffffffff;

	pushOp(parteAlta);
	pushOp(parteBaixa);
	atualizaPc();
}

///
/// Desempilha dois inteiros da pilha
/// de operandos e divide um pelo outro.
/// O resultado entao eh salvo na pilha
/// de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void idiv() {
	int32_t valor1 = popOp();
	int32_t valor2 = popOp();
	pushOp((int32_t)(valor2 / valor1));
	atualizaPc();
}

///
/// Desempilha dois longs da pilha de
/// de operandos e divide um pelo outro.
/// O resultado entao eh salvo na pilha
/// de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void _ldiv() {
	int32_t parteBaixa, parteAlta;

	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorLong = parteAlta;
	valorLong = (valorLong << 32) + parteBaixa;

	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorLong2 = parteAlta;
	valorLong2 = (valorLong2 << 32) + parteBaixa;

	int64_t resultado = valorLong2 / valorLong;

	parteAlta = resultado >> 32;
	parteBaixa = resultado & 0xffffffff;

	pushOp(parteAlta);
	pushOp(parteBaixa);
	atualizaPc();
}

///
/// Desempilha dois floats da pilha de
/// de operandos e divide um pelo outro.
/// O resultado entao eh salvo na pilha
/// de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void fdiv() {
	float valorFloat, valorFloat2;

	int32_t valor = popOp();
	int32_t valor2 = popOp();

	memcpy(&valorFloat, &valor, sizeof(int32_t));
	memcpy(&valorFloat2, &valor2, sizeof(int32_t));

	float resultado = valorFloat2 / valorFloat;

	int32_t valorPilha;
	memcpy(&valorPilha, &resultado, sizeof(int32_t));

	pushOp(valorPilha);
	atualizaPc();
}

///
/// Desempilha dois doubles da pilha de
/// de operandos e divide um pelo outro.
/// O resultado entao eh salvo na pilha
/// de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void ddiv() {
	int32_t parteAlta, parteBaixa;

	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorDouble = parteAlta;
	valorDouble = (valorDouble << 32) + parteBaixa;

	double valor1;
	memcpy(&valor1, &valorDouble, sizeof(double));

	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorDouble2 = parteAlta;
	valorDouble2 = (valorDouble2 << 32) + parteBaixa;

	double valor2;
	memcpy(&valor2, &valorDouble2, sizeof(double));

	double resultado = valor2 / valor1;

	int64_t valorPilha;
	memcpy(&valorPilha, &resultado, sizeof(int64_t));

	parteAlta = valorPilha >> 32;
	parteBaixa = valorPilha & 0xffffffff;

	pushOp(parteAlta);
	pushOp(parteBaixa);
	atualizaPc();
}

///
/// Desempilha dois inteiros da pilha de
/// de operandos e calcula o resto da divisao
/// de um pelo outro. O resultado entao eh
/// salvo na pilha de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void irem() {
	int32_t valor1 = popOp();
	int32_t valor2 = popOp();

	int32_t resultado = valor2 - ((valor2/valor1) * valor1);

	pushOp((int32_t) resultado);
	atualizaPc();
}

///
/// Desempilha dois longs da pilha de
/// de operandos e calcula o resto da divisao
/// de um pelo outro. O resultado entao eh
/// salvo na pilha de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void lrem() {
	int32_t parteAlta, parteBaixa;

	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorLong1 = parteAlta;
	valorLong1 = (valorLong1 << 32) + parteBaixa;

	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorLong2 = parteAlta;
	valorLong2 = (valorLong2 << 32) + parteBaixa;

	int64_t resultado = valorLong2%valorLong1;
	parteAlta = resultado >> 32;
	parteBaixa = resultado & 0xffffffff;

	pushOp(parteAlta);
	pushOp(parteBaixa);
	atualizaPc();
}

///
/// Desempilha dois floats da pilha de
/// de operandos e calcula o resto da divisao
/// de um pelo outro. O resultado entao eh
/// salvo na pilha de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void frem() {
	float valorFloat, valorFloat2;

	int32_t valor1 = popOp();
	int32_t valor2 = popOp();

	memcpy(&valorFloat, &valor1, sizeof(int32_t));
	memcpy(&valorFloat2, &valor2, sizeof(int32_t));

	float resultado = fmod(valorFloat2,valorFloat);
	int32_t retPilha;
	memcpy(&retPilha, &resultado, sizeof(int32_t));

	pushOp(retPilha);
	atualizaPc();
}

///
/// Desempilha dois doubles da pilha de
/// de operandos e calcula o resto da divisao
/// de um pelo outro. O resultado entao eh
/// salvo na pilha de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void _drem() {
	int32_t parteAlta, parteBaixa;

	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorDouble = parteAlta;
	valorDouble = (valorDouble << 32) + parteBaixa;

	double valorDouble1;
	memcpy(&valorDouble1, &valorDouble, sizeof(double));

	parteBaixa = popOp();
	parteAlta = popOp();

	valorDouble = parteAlta;
	valorDouble = (valorDouble << 32) + parteBaixa;

	double valorDouble2;
	memcpy(&valorDouble2, &valorDouble, sizeof(double));

	double resultado = fmod(valorDouble2, valorDouble1);

	int64_t valorPilha;
	memcpy(&valorPilha, &resultado, sizeof(int64_t));

	parteAlta = valorPilha >> 32;
	parteBaixa = valorPilha & 0xffffffff;

	pushOp(parteAlta);
	pushOp(parteBaixa);
	atualizaPc();
}

///
/// Desempilha um inteiro e faz a negacao aritmetica
/// desse valor, salvando o resultado de volta
/// na pilha de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void ineg() {
	int32_t valor = popOp();
	pushOp(-valor);
	atualizaPc();
}

///
/// Desempilha um long e faz a negacao aritmetica
/// desse valor, salvando o resultado de volta
/// na pilha de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void lneg() {
	int32_t parteBaixa, parteAlta;

	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorLong = parteAlta;
	valorLong = (valorLong << 32) + parteBaixa;

	valorLong = -valorLong;

	parteAlta = valorLong >> 32;
	parteBaixa = valorLong & 0xffffffff;

	pushOp(parteAlta);
	pushOp(parteBaixa);
	atualizaPc();
  	flagLNEG = TRUE;
}

///
/// Desempilha um float e faz a negacao aritmetica
/// desse valor, salvando o resultado de volta
/// na pilha de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void fneg() {
	int32_t valorFloat = popOp();
  int32_t valorPilha;
	float valorNeg;
	memcpy(&valorNeg,&valorFloat,sizeof(float));
	valorNeg = -valorNeg;
	memcpy(&valorPilha,&valorNeg,sizeof(int32_t));
	pushOp(valorPilha);
	atualizaPc();
}

///
/// Desempilha um double e faz a negacao aritmetica
/// desse valor, salvando o resultado de volta
/// na pilha de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void dneg() {
	int32_t parteBaixa;
  int32_t parteAlta;

	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorDouble = parteAlta;
	valorDouble = (valorDouble << 32) + parteBaixa;

	if (valorDouble > 0){
		valorDouble |= 0x8000000000000000;
	} else {
		valorDouble &= 0x7fffffffffffffff;
	}

	parteAlta = valorDouble >> 32;
	parteBaixa = valorDouble & 0xffffffff;

	pushOp(parteAlta);
	pushOp(parteBaixa);
	atualizaPc();
}

///
/// Desempilha dois valores, um inteiro que sera manipulado
/// e um inteiro cujos 5bits menos significativos sao
/// a quantidade de bits a serem shiftados a
/// esquerda salvando o resultado de volta
/// na pilha de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void ishl() {
	int32_t valorShift = popOp();
	int32_t resultado = popOp();

	valorShift &= 0x1f;
	resultado <<= valorShift;

	pushOp(resultado);
	atualizaPc();
}

///
/// Desempilha dois valores, um long que sera manipulado
/// e um inteiro cujos 6bits menos significativos sao
/// a quantidade de bits a serem shiftados a
/// esquerda salvando o resultado de volta
/// na pilha de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void lshl() {
	int32_t valorShift = popOp();
	int32_t parteAlta, parteBaixa;
	int64_t resultado;

	valorShift &= 0x3f;

	parteBaixa = popOp();
	parteAlta = popOp();

	resultado = parteAlta;
	resultado = (resultado << 32) + parteBaixa;

	resultado <<= valorShift;

	parteAlta = resultado >> 32;
	parteBaixa = resultado & 0xffffffff;

	pushOp(parteAlta);
	pushOp(parteBaixa);
	atualizaPc();
}

///
/// Desempilha dois valores, um inteiro que sera manipulado
/// e um inteiro cujos 5bits menos significativos sao
/// a quantidade de bits a serem shiftados a
/// direita salvando o resultado de volta
/// na pilha de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void ishr() {

	int32_t valorShift = popOp();
	int32_t resultado = popOp();
	int32_t i = 0;

	valorShift &= 0x1f;

	while(i < valorShift) {
		resultado = resultado / 2;
		i += 1;
	}

	pushOp(resultado);
	atualizaPc();
}

///
/// Desempilha dois valores, um long que sera manipulado
/// e um inteiro cujos 6bits menos significativos sao
/// a quantidade de bits a serem shiftados a
/// direita salvando o resultado de volta
/// na pilha de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void lshr() {
	int32_t valorShift = popOp();
	int32_t parteAlta, parteBaixa;
	int64_t resultado;

	valorShift &= 0x3f;

	parteBaixa = popOp();
	parteAlta = popOp();

	resultado = parteAlta;
	resultado = (resultado << 32) + parteBaixa;

	resultado >>= valorShift;

	parteAlta = resultado >> 32;
	parteBaixa = resultado & 0xffffffff;

	pushOp(parteAlta);
	pushOp(parteBaixa);
	atualizaPc();
}
///
/// Desempilha dois valores, um inteiro que sera manipulado
/// e um inteiro cujos 5bits menos significativos sao
/// a quantidade de bits a serem shiftados a
/// direita salvando o resultado de volta
/// na pilha de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void iushr() {

	int32_t valorShift = popOp();
	int32_t resultado = popOp();

	valorShift &= 0x1f;
	resultado >>= valorShift;

	pushOp(resultado);
	atualizaPc();
}

///
/// Desempilha dois valores, um long que sera manipulado
/// e um inteiro cujos 6bits menos significativos sao
/// a quantidade de bits a serem shiftados a
/// direita salvando o resultado de volta
/// na pilha de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void lushr() {
	int32_t valorShift = popOp();
	int32_t parteBaixa = popOp();
	int32_t parteAlta = popOp();
	int64_t valorLong = parteAlta;

	valorLong = (valorLong << 32) + parteBaixa;

	valorShift &= 0x3f;
	valorLong >>= valorShift;

	parteAlta = valorLong >> 32;
	parteBaixa = valorLong & 0xffffffff;

	pushOp(parteAlta);
	pushOp(parteBaixa);
	atualizaPc();
}

///
/// Realiza a operacao and entre os dois primeiros
/// inteiros da pilha e guarda o resultado
/// na pilha.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp
void iand() {
	int32_t pop1 = popOp();
	int32_t pop2 = popOp();
	int32_t resultado = pop1 & pop2;
	pushOp(resultado);
	frameCorrente->pc++;
}
///
/// Realiza a operacao and entre os dois primeiros
/// longs da pilha e guarda o resultado
/// na pilha.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void land() {
	int32_t baixa,alta;

	baixa = popOp();
	alta = popOp();

	int64_t val1 = alta;
	val1 <<= 32;
	val1 = val1 + baixa;

	baixa = popOp();
	alta = popOp();

	int64_t val2 = alta;
	val2 <<= 32;
	val2 = val2 + baixa;

	int64_t resultado = val2 & val1;

	alta = resultado >> 32;
	baixa = resultado & 0xffffffff;

	pushOp(alta);
	pushOp(baixa);
	atualizaPc();
}
///
/// Realiza a operacao or entre os dois primeiros
/// inteiros da pilha e guarda o resultado
/// na pilha.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp
void ior() {
	int32_t pop1 = popOp();
	int32_t pop2 = popOp();
	int32_t resultado = pop1 | pop2;
	pushOp(resultado);
	frameCorrente->pc++;
}
///
/// Realiza a operacao or entre os dois primeiros
/// longs da pilha e guarda o resultado
/// na pilha.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void lor() {
	int32_t baixa,alta;

	baixa = popOp();
	alta = popOp();

	int64_t val1 = alta;
	val1 <<= 32;
	val1 = val1 + baixa;

	baixa = popOp();
	alta = popOp();

	int64_t val2 = alta;
	val2 <<= 32;
	val2 = val2 + baixa;

	int64_t resultado = val2 | val1;

	alta = resultado >> 32;
	baixa = resultado & 0xffffffff;

	pushOp(alta);
	pushOp(baixa);

	inicializaDecodificador(dec);
	int numBytes = dec[frameCorrente->code[frameCorrente->pc]].bytes;

	atualizaPc();
}
///
/// Realiza a operacao xor entre os dois primeiros
/// inteiros da pilha e guarda o resultado
/// na pilha.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp
void ixor() {
	int32_t pop1 = popOp();
	int32_t pop2 = popOp();
	int32_t resultado = pop1 ^ pop2;
	pushOp(resultado);
	frameCorrente->pc++;
}
///
/// Realiza a operacao xor entre os dois primeiros
/// longs da pilha e guarda o resultado
/// na pilha.
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc popOp pushOp
void lxor() {
	int32_t baixa,alta;

	baixa = popOp();
	alta = popOp();

	int64_t val1 = alta;
	val1 <<= 32;
	val1 = val1 + baixa;

	baixa = popOp();
	alta = popOp();

	int64_t val2 = alta;
	val2 <<= 32;
	val2 = val2 + baixa;

	int64_t resultado = val2 ^ val1;

	alta = resultado >> 32;
	baixa = resultado & 0xffffffff;

	pushOp(alta);
	pushOp(baixa);
	atualizaPc();
}
///
/// Incrementa uma variavel do array de variaveis locais
/// apontada por um indice passado como parametro com um
/// byte tambem passado como parametro
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc
void iinc() {
	int8_t indice = frameCorrente->code[frameCorrente->pc + 1];
	int32_t valor = frameCorrente->fields[indice];
	int8_t byte = frameCorrente->code[frameCorrente->pc + 2];

	frameCorrente->fields[indice] = byte + valor;
	atualizaPc();
}
///
/// Converte um valor inteiro para um valor long
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc pushOp
void i2l() {
  char* tipo = "L";
  tipoGlobal = tipo;
  int32_t alta, baixa;
  int32_t val = popOp();
  int64_t long_num = (int64_t) val;
	alta = long_num >> 32;
	baixa = long_num & 0xffffffff;
  pushOp(alta);
  pushOp(baixa);
  atualizaPc();
}
///
/// Converte um valor inteiro para um valor float
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc pushOp
void i2f() {
  char* tipo = "F";
  tipoGlobal = tipo;
	int32_t val = (int32_t) popOp();
	float valF = (float) val;
	int32_t valPilha;
	memcpy(&valPilha, &valF, sizeof(int32_t));
	pushOp(valPilha);
	atualizaPc();
}
///
/// Converte um valor inteiro para um valor double
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc pushOp
void i2d() {
  	
  	char* tipo = "D";
  	tipoGlobal = tipo;
	int32_t retPilha = popOp();

	double dVal = (double) retPilha;
	
	int64_t pilhaVal;
	memcpy(&pilhaVal, &dVal, sizeof(int64_t));
	
	int32_t alta;
	int32_t baixa;
	alta = pilhaVal >> 32;
	baixa = pilhaVal & 0xffffffff;
	
	pushOp(alta);
	pushOp(baixa);
	atualizaPc();
}
///
/// Converte um valor long para um valor inteiro
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc pushOp
void l2i() {
  char* tipo = "I";
  tipoGlobal = tipo;
	int32_t alta,baixa;
	baixa = popOp();
	alta = popOp();
	pushOp(baixa);
	atualizaPc();
}
///
/// Converte um valor long para um valor float
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc pushOp
void l2f() {
  char* tipo = "F";
  tipoGlobal = tipo;

	int32_t baixa,alta;
	baixa = popOp();
	alta = popOp();

	int64_t lVal = alta;
	lVal <<= 32;
	lVal = lVal | baixa;

	float fVal;
  fVal = (float) lVal;

	int32_t valPilha;
	memcpy(&valPilha, &fVal, sizeof(int32_t));

	pushOp(valPilha);
	atualizaPc();
}
///
/// Converte um valor long para um valor double
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc pushOp
void l2d() {
  char* tipo = "D";
  tipoGlobal = tipo;

	int32_t baixa,alta;
	baixa = popOp();
	alta = popOp();

	int64_t lVal = alta;
	lVal <<= 32;
	lVal = lVal + baixa;

	double dVal;
	memcpy(&dVal, &lVal, sizeof(double));

	int64_t valorPilha;
	memcpy(&valorPilha, &dVal, sizeof(int64_t));

	alta = valorPilha >> 32;
	baixa = valorPilha & 0xffffffff;

	pushOp(alta);
	pushOp(baixa);
	atualizaPc();
}
///
/// Converte um valor float para um valor inteiro
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc pushOp
void f2i() {
  char* tipo = "I";
  tipoGlobal = tipo;
	int32_t retPilha = popOp();
	float fVal;
	memcpy(&fVal, &retPilha, sizeof(int32_t));
	pushOp((int32_t)fVal);
	atualizaPc();
}
///
/// Converte um valor float para um valor long
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc pushOp
void f2l() {
  char* tipo = "L";
  tipoGlobal = tipo;

	int32_t retPilha = popOp();
	float fVal;
	memcpy(&fVal, &retPilha, sizeof(int32_t));

	int64_t lVal = (int64_t) fVal;

	int32_t alta;
	int32_t baixa;

	alta = lVal >> 32;
	baixa = lVal & 0xffffffff;

	pushOp(alta);
	pushOp(baixa);
	atualizaPc();
}
///
/// Converte um valor float para um valor double
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc pushOp
void f2d() {
  char* tipo = "D";
  tipoGlobal = tipo;

	int32_t retPilha = popOp();
	float fVal;
	memcpy(&fVal, &retPilha, sizeof(int32_t));

	double dVal = (double) fVal;

	int64_t pilhaVal;
	memcpy(&pilhaVal, &dVal, 2*sizeof(int32_t));

	int32_t alta;
	int32_t baixa;

	alta = pilhaVal >> 32;

	baixa = pilhaVal & 0xffffffff;

	pushOp(alta);
	pushOp(baixa);
	atualizaPc();
}
///
/// Converte um valor double para um valor inteiro
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc pushOp
void d2i() {
  char* tipo = "I";
  tipoGlobal = tipo;

	int32_t baixa,alta;
	baixa = popOp();
	alta = popOp();

	int64_t dVal = alta;
	dVal <<= 32;
	dVal = dVal + baixa;

	double v1;
	memcpy(&v1, &dVal, sizeof(double));

	pushOp((int32_t)v1);
	atualizaPc();
}
///
/// Converte um valor double para um valor long
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc pushOp
void d2l() {
  char* tipo = "L";
  tipoGlobal = tipo;

	int32_t baixa,alta;
	baixa = popOp();
	alta = popOp();

	int64_t dVal = alta;
	dVal <<= 32;
	dVal = dVal + baixa;

	double v1;
	memcpy(&v1, &dVal, sizeof(double));

  int64_t long_num = (int64_t) v1;
	alta = long_num >> 32;
	baixa = long_num & 0xffffffff;

  pushOp(alta);
  pushOp(baixa);
	atualizaPc();
}
///
/// Converte um valor double para um valor float
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc pushOp
void d2f() {
  char* tipo = "F";
  tipoGlobal = tipo;

	int32_t alta,baixa;
	baixa = popOp();
	alta = popOp();

	int64_t dVal = alta;
	dVal <<= 32;
	dVal = dVal + baixa;

	double v1;
	memcpy(&v1, &dVal, sizeof(double));

	float fVal = (float) v1;

	int32_t pilhaVal;
	memcpy(&pilhaVal,&fVal,sizeof(float));

	pushOp(pilhaVal);
	atualizaPc();
}
///
/// Converte um valor inteiro para um valor byte/booleano
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc pushOp
void i2b() {
	int32_t valPilha = popOp();
	int8_t bVal = (int8_t) valPilha;
	pushOp((int32_t) bVal);
	atualizaPc();
}
///
/// Converte um valor inteiro para um valor char
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc pushOp
void i2c() {
  char* tipo = "C";
  tipoGlobal = tipo;
	int32_t valPilha = popOp();
	int16_t cVal = (int16_t) valPilha;
	pushOp((int32_t) cVal);
	atualizaPc();
}
///
/// Converte um valor inteiro para um valor short
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc pushOp
void i2s() {
	int32_t valPilha = popOp();
	int16_t cVal = (int16_t) valPilha;
	pushOp((int32_t) cVal);
	atualizaPc();
}

///
/// Desempilha dois valores long da pilha de operandos
/// e os compara, apos serem convertidos. Se valor1 > valor2
/// entao o valor inteiro 1 eh empilhado. Se valor1 = valor2
/// entao o valor inteiro 0 eh empilhado. Se valor1 < valor2
/// entao o valor inteiro -1 eh empilhado. Se valor1 ou valor2
/// sao NanN, entao o valor inteiro -1 eh empilhado.
///
/// @param Nenhum
/// @return @c void
/// @see popOp atualizaPc
void lcmp() {
	int32_t parteBaixa, parteAlta;
	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorLong = parteAlta;
	valorLong = (valorLong << 32) | parteBaixa;

	long longCmp;
	memcpy(&longCmp, &valorLong, sizeof(long));

	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorDouble2 = parteAlta;
	valorDouble2 = (valorDouble2 << 32) | parteBaixa;

	long longCmp2;
	memcpy(&longCmp2, &valorDouble2, sizeof(long));

	if(longCmp2 > longCmp) {
		pushOp((int32_t) 1);
	} else if(longCmp2 == longCmp) {
		pushOp((int32_t) 0);
	} else if(longCmp2 < longCmp) {
		pushOp((int32_t) -1);
	}
	atualizaPc();
}

///
/// Desempilha dois valores float da pilha de operandos
/// e os compara, apos serem convertidos. Se valor1 > valor2
/// entao o valor inteiro 1 eh empilhado. Se valor1 = valor2
/// entao o valor inteiro 0 eh empilhado. Se valor1 < valor2
/// entao o valor inteiro -1 eh empilhado. Se valor1 ou valor2
/// sao NanN, entao o valor inteiro -1 eh empilhado.
///
/// @param Nenhum
/// @return @c void
/// @see popOp atualizaPc
void fcmpl() {
	float valorFloat, valorFloat2;
	int32_t valor;

	valor = popOp();
	memcpy(&valorFloat,&valor,sizeof(float));

	valor = popOp();
	memcpy(&valorFloat2,&valor,sizeof(float));

	if(valorFloat2 == valorFloat) {
		pushOp((int32_t)0);
	} else if(valorFloat2 > valorFloat) {
		pushOp((int32_t)1);
	} else if(valorFloat2 < valorFloat) {
		pushOp((int32_t)-1);
	} else {
		printf("NaN!!\n");
		pushOp((int32_t)-1);
	}
	atualizaPc();
}

///
/// Desempilha dois valores float da pilha de operandos
/// e os compara, apos serem convertidos. Se valor1 > valor2
/// entao o valor inteiro 1 eh empilhado. Se valor1 = valor2
/// entao o valor inteiro 0 eh empilhado. Se valor1 < valor2
/// entao o valor inteiro -1 eh empilhado. Se valor1 ou valor2
/// sao NanN, entao o valor inteiro 1 eh empilhado.
///
/// @param Nenhum
/// @return @c void
/// @see popOp atualizaPc
void fcmpg() {
	float valorFloat, valorFloat2;
	int32_t valor;

	valor = popOp();
	memcpy(&valorFloat,&valor,sizeof(float));

	valor = popOp();
	memcpy(&valorFloat2,&valor,sizeof(float));

	if(valorFloat2 == valorFloat) {
		pushOp((int32_t)0);
	} else if(valorFloat2 > valorFloat) {
		pushOp((int32_t)1);
	} else if(valorFloat2 < valorFloat) {
		pushOp((int32_t)-1);
	} else {
		printf("NaN!!\n");
		pushOp((int32_t)1);
	}
	atualizaPc();
}

///
/// Desempilha dois valores double da pilha de operandos
/// e os compara, apos serem convertidos. Se valor1 > valor2
/// entao o valor inteiro 1 eh empilhado. Se valor1 = valor2
/// entao o valor inteiro 0 eh empilhado. Se valor1 < valor2
/// entao o valor inteiro -1 eh empilhado. Se valor1 ou valor2
/// sao NanN, entao o valor inteiro -1 eh empilhado.
///
/// @param Nenhum
/// @return @c void
/// @see popOp atualizaPc
void dcmpl() {
	int32_t parteBaixa, parteAlta;
	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorDouble = parteAlta;
	valorDouble = (valorDouble << 32) | parteBaixa;

	double doubleCmp;
	memcpy(&doubleCmp, &valorDouble, sizeof(double));

	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorDouble2 = parteAlta;
	valorDouble2 = (valorDouble2 << 32) | parteBaixa;

	double doubleCmp2;
	memcpy(&doubleCmp2, &valorDouble2, sizeof(double));

	if(doubleCmp2 > doubleCmp) {
		pushOp((int32_t) 1);
	} else if(doubleCmp2 == doubleCmp) {
		pushOp((int32_t) 0);
	} else if(doubleCmp2 < doubleCmp) {
		pushOp((int32_t) -1);
	} else {
		printf("NaN!\n");
		pushOp((int32_t) -1);
	}
	atualizaPc();
}

///
/// Desempilha dois valores double da pilha de operandos
/// e os compara, apos serem convertidos. Se valor1 > valor2
/// entao o valor inteiro 1 eh empilhado. Se valor1 = valor2
/// entao o valor inteiro 0 eh empilhado. Se valor1 < valor2
/// entao o valor inteiro -1 eh empilhado. Se valor1 ou valor2
/// sao NanN, entao o valor inteiro 1 eh empilhado.
///
/// @param Nenhum
/// @return @c void
/// @see popOp atualizaPc
void dcmpg() {
	int32_t parteBaixa, parteAlta;
	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorDouble = parteAlta;
	valorDouble = (valorDouble << 32) | parteBaixa;

	double doubleCmp;
	memcpy(&doubleCmp, &valorDouble, sizeof(double));

	parteBaixa = popOp();
	parteAlta = popOp();

	int64_t valorDouble2 = parteAlta;
	valorDouble2 = (valorDouble2 << 32) | parteBaixa;

	double doubleCmp2;
	memcpy(&doubleCmp2, &valorDouble2, sizeof(double));

	if(doubleCmp2 > doubleCmp) {
		pushOp((int32_t) 1);
	} else if(doubleCmp2 == doubleCmp) {
		pushOp((int32_t) 0);
	} else if(doubleCmp2 < doubleCmp) {
		pushOp((int32_t) -1);
	} else {
		printf("NaN!\n");
		pushOp((int32_t) 1);
	}
	atualizaPc();
}

///
/// Desempilha um valor inteiro da pilha de operandos
/// e compara com 0. Se valor = 0, pula para a
/// instrucao a partir do offset calculado com os
/// dois bytes de argumento. O valor de PC
/// eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void ifeq() {
	uint8_t branchbyte1,branchbyte2;
	int16_t offset;

	branchbyte1 = frameCorrente->code[frameCorrente->pc + 1];
	branchbyte2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = branchbyte1;
	offset = (branchbyte1 << 8) | branchbyte2;

	int32_t retPilha = popOp();

	if(retPilha == 0) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Desempilha um valor inteiro da pilha de operandos
/// e compara com 0. Se valor != 0, pula para a
/// instrucao a partir do offset calculado com os
/// dois bytes de argumento. O valor de PC
/// eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void ifne() {
	uint8_t branchbyte1,branchbyte2;
	int16_t offset;

	branchbyte1 = frameCorrente->code[frameCorrente->pc + 1];
	branchbyte2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = branchbyte1;
	offset = (branchbyte1 << 8) | branchbyte2;

	int32_t retPilha = popOp();

	if(retPilha != 0) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Desempilha um valor inteiro da pilha de operandos
/// e compara com 0. Se valor < 0, pula para a
/// instrucao a partir do offset calculado com os
/// dois bytes de argumento. O valor de PC
/// eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void iflt() {
	uint8_t branchbyte1,branchbyte2;
	int16_t offset;

	branchbyte1 = frameCorrente->code[frameCorrente->pc + 1];
	branchbyte2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = branchbyte1;
	offset = (branchbyte1 << 8) | branchbyte2;

	int32_t retPilha = popOp();

	if(retPilha < 0) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Desempilha um valor inteiro da pilha de operandos
/// e compara com 0. Se valor >= 0, pula para a
/// instrucao a partir do offset calculado com os
/// dois bytes de argumento. O valor de PC
/// eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void ifge() {
	uint8_t branchbyte1,branchbyte2;
	int16_t offset;

	branchbyte1 = frameCorrente->code[frameCorrente->pc + 1];
	branchbyte2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = branchbyte1;
	offset = (branchbyte1 << 8) | branchbyte2;

	int32_t retPilha = popOp();

	if(retPilha >= 0) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Desempilha um valor inteiro da pilha de operandos
/// e compara com 0. Se valor > 0, pula para a
/// instrucao a partir do offset calculado com os
/// dois bytes de argumento. O valor de PC
/// eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void ifgt() {
	uint8_t branchbyte1,branchbyte2;
	int16_t offset;

	branchbyte1 = frameCorrente->code[frameCorrente->pc + 1];
	branchbyte2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = branchbyte1;
	offset = (branchbyte1 << 8) | branchbyte2;

	int32_t retPilha = popOp();

	if(retPilha > 0) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Desempilha um valor inteiro da pilha de operandos
/// e compara com 0. Se valor <= 0, pula para a
/// instrucao a partir do offset calculado com os
/// dois bytes de argumento. O valor de PC
/// eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void ifle() {
	uint8_t branchbyte1,branchbyte2;
	int16_t offset;

	branchbyte1 = frameCorrente->code[frameCorrente->pc + 1];
	branchbyte2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = branchbyte1;
	offset = (branchbyte1 << 8) | branchbyte2;

	int32_t retPilha = popOp();

	if(retPilha <= 0) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Desempilha dois valores inteiros da pilha de operandos
/// e compara. Se valor1 = valor2, pula para a
/// instrucao a partir do offset calculado com os
/// dois bytes de argumento. O valor de PC
/// eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void if_icmpeq() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	int32_t retPilha1 = popOp();
	int32_t retPilha2 = popOp();

	if(retPilha1 == retPilha2) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Desempilha dois valores inteiros da pilha de operandos
/// e compara. Se valor1 != valor2, pula para a
/// instrucao a partir do offset calculado com os
/// dois bytes de argumento. O valor de PC
/// eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void if_icmpne() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	int32_t retPilha1 = popOp();
	int32_t retPilha2 = popOp();

	if(retPilha1 != retPilha2) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Desempilha dois valores inteiros da pilha de operandos
/// e compara. Se valor1 < valor2, pula para a
/// instrucao a partir do offset calculado com os
/// dois bytes de argumento. O valor de PC
/// eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void if_icmplt() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	int32_t retPilha1 = popOp();
	int32_t retPilha2 = popOp();

	if(retPilha2 < retPilha1) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Desempilha dois valores inteiros da pilha de operandos
/// e compara. Se valor1 >= valor2, pula para a
/// instrucao a partir do offset calculado com os
/// dois bytes de argumento. O valor de PC
/// eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void if_icmpge() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	int32_t retPilha1 = popOp();
	int32_t retPilha2 = popOp();

	if(retPilha2 >= retPilha1) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Desempilha dois valores inteiros da pilha de operandos
/// e compara. Se valor1 > valor2, pula para a
/// instrucao a partir do offset calculado com os
/// dois bytes de argumento. O valor de PC
/// eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void if_icmpgt() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	int32_t retPilha1 = popOp();
	int32_t retPilha2 = popOp();

	if(retPilha2 > retPilha1) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Desempilha dois valores inteiros da pilha de operandos
/// e compara. Se valor1 <= valor2, pula para a
/// instrucao a partir do offset calculado com os
/// dois bytes de argumento. O valor de PC
/// eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void if_icmple() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	int32_t retPilha1 = popOp();
	int32_t retPilha2 = popOp();

	if(retPilha2 <= retPilha1) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Desempilha dois valores de referencia da pilha
/// de operandos e compara. Se forem iguais, pula para a
/// instrucao a partir do offset calculado com os
/// dois bytes de argumento. O valor de PC
/// eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void if_acmpeq() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	int32_t retPilha1 = popOp();
	int32_t retPilha2 = popOp();

	if(retPilha2 == retPilha1) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Desempilha dois valores de referencia da pilha
/// de operandos e compara. Se forem iguais, pula para a
/// instrucao a partir do offset calculado com os
/// dois bytes de argumento. O valor de PC
/// eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void if_acmpne() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	int32_t retPilha1 = popOp();
	int32_t retPilha2 = popOp();

	if(retPilha2 != retPilha1) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Pula para a instrucao a partir do offset calculado
/// com os dois bytes de argumento. O valor de PC
/// eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
void _goto() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	frameCorrente->pc += offset;
}

///
/// Pula para uma subrotina, salvando o endereco da proxima
/// instrucao na pilha de operandos. Os dois bytes argumentos
/// compoe um offset para a instrucao que sera executada.
/// O valor de PC eh somado com esse offset.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp
void jsr() {
	pushOp(frameCorrente->pc+3);

	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	frameCorrente->pc += offset;
}

///
/// Retorna de uma subrotina.
///
/// @param Nenhum
/// @return @c void
void ret() {
	return;
}

///
/// Função responsavel pela operação switch case
/// Com valores variados.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void tableswitch() {
    uint32_t bytes_preench;
    int32_t indice;
    int32_t default_v;
    int32_t low;
    int32_t high;
    int32_t npairs;
    uint32_t pc_novo;
    uint32_t pc_aux;
    int32_t qtd_offset;
    int32_t offset;
    int32_t posicao;
    uint32_t temp;

    int definido = FALSE;

    pc_aux = frameCorrente->pc;

    indice = popOp();

    bytes_preench = (4 - ((pc_aux + 1) % 4) ) % 4;
    pc_aux += bytes_preench;
    pc_aux++;

    default_v = 0;
    for (int l = 0; l < 4; l++) {
        default_v = (default_v << 8) + frameCorrente->code[pc_aux];
        pc_aux++;
    }

    low = 0;
    for (int l = 0; l < 4; l++) {
        low = (low << 8) + frameCorrente->code[pc_aux];
        pc_aux++;
    }

    if (indice < low && !definido) {
        definido = TRUE;
        pc_novo = frameCorrente->pc + default_v;
    }

    high = 0;
    for (int l = 0; l < 4; l++) {
        high = (high << 8) + frameCorrente->code[pc_aux];
        pc_aux++;
    }

    if (indice > high && !definido) {
        definido = TRUE;
        pc_novo = frameCorrente->pc + default_v;
    }

    qtd_offset = 1 + high - low;
    posicao = indice - low;
    for (int32_t l = 0; l < qtd_offset; l++) {
        if (l == posicao) {
            offset = 0;
            for (int i = 0; i < 4; i++) {
                offset = (offset << 8) + frameCorrente->code[pc_aux];
                pc_aux++;
            }
            pc_novo = frameCorrente->pc + offset;
            definido = TRUE;
            break;
        } else {
            for (int i = 0; i < 4; i++) {
                pc_aux++;
            }
        }
    }

    frameCorrente->pc = pc_novo;
}

///
/// Função responsavel pela operação switch case
/// Com valores sequenciais
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void lookupswitch() {
    uint32_t pc_aux;
    uint32_t pc_novo;
    uint32_t bytes_preench;
    uint32_t offset;
    int32_t default_v;
    int32_t npairs;
    int32_t match;
    int32_t key;

    int definido = FALSE;

    pc_aux = frameCorrente->pc;

    key = popOp();

    bytes_preench = (4 - ((pc_aux + 1) % 4) ) % 4;
    pc_aux += bytes_preench;
    pc_aux++;

    default_v = 0;
    for (int l = 0; l < 4; l++) {
        default_v = (default_v << 8) + frameCorrente->code[pc_aux];
        pc_aux++;
    }

    npairs = 0;
    for (int l = 0; l < 4; l++) {
        npairs = (npairs << 8) + frameCorrente->code[pc_aux];
        pc_aux++;
    }

    for (int32_t l = 0; l < npairs; l++) {
        match = 0;
        for (int l = 0; l < 4; l++) {
            match = (match << 8) + frameCorrente->code[pc_aux];
            pc_aux++;
        }

        if (key == match) {
            offset = 0;
            for (int l = 0; l < 4; l++) {
                offset = (offset << 8) + frameCorrente->code[pc_aux];
                pc_aux++;
            }

            pc_novo = frameCorrente->pc + offset;

            definido = TRUE;
        } else {
            for(int i = 0; i < 4; i++) {
                pc_aux++;
            }
        }
     }

    if (!definido) {
        pc_novo = frameCorrente->pc + default_v;
    }

    frameCorrente->pc = pc_novo;
}

///
/// Retorna um valor inteiro finalizando
/// o metodo
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void ireturn() {
  retorno = popOp();
	flagRet = 1;
  frameCorrente->pc = frameCorrente->codeLength + 1;
}

///
/// Retorna um valor long finalizando
/// o metodo
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void lreturn() {
	int32_t alta,baixa;

	baixa = popOp();
	alta = popOp();

	flagRet = 2;

	retAlta = alta;
	retBaixa = baixa;

  frameCorrente->pc = frameCorrente->codeLength + 1;
}

///
/// Retorna um valor float finalizando
/// o metodo
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void freturn() {
	retorno = popOp();
	flagRet = 1;
  frameCorrente->pc = frameCorrente->codeLength + 1;
}

///
/// Retorna um valor double finalizando
/// o metodo
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void dreturn() {
	int32_t alta,baixa;

	baixa = popOp();
	alta = popOp();

	flagRet = 2;

	retAlta = alta;
	retBaixa = baixa;

  	frameCorrente->pc = frameCorrente->codeLength + 1;
}

///
/// Retorna um valor de referencia
///  finalizando o metodo
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void areturn() {
	retorno = popOp();
	flagRet = 1;
  frameCorrente->pc = frameCorrente->codeLength + 1;
}

///
/// Retorna void de um metodo
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc
void _return() {
	retorno = 0;
	flagRet = 0;
	atualizaPc();
}

///
/// Recupera um atributo static
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc
void getstatic() {
  
	/*uint32_t indice = frameCorrente->code[frameCorrente->pc + 2];
	int32_t indiceClasse = frameCorrente->constantPool[indice-1].info.Fieldref.classIndex;
	char* nomeClasse = retornaNome(frameCorrente->classe, frameCorrente->constantPool[indiceClasse-1].info.Class.nameIndex);
	uint16_t nomeTipoIndice = frameCorrente->constantPool[indice-1].info.Fieldref.nameAndTypeIndex;
	char* nome = retornaNome(frameCorrente->classe, frameCorrente->constantPool[nomeTipoIndice-1].info.NameAndType.nameIndex);
	char* tipo = retornaNome(frameCorrente->classe, frameCorrente->constantPool[nomeTipoIndice-1].info.NameAndType.descriptorIndex);
	tipoGlobal = tipo;

 	int32_t indiceField = buscaCampo(nomeClasse, nome, tipo);
 	uint32_t indiceNome = frameCorrente->classe->fields[indiceField].nameIndex;


 	int32_t i;
 	for (i = 0; i < frameCorrente->classe->fields[indiceNome].attributesCount; ++i);
 	uint16_t valor = frameCorrente->classe->fields[indiceNome].attributes[i].constantValueIndex;

	printf("%u\n", valor);
	exit(0);


 	//pushOp(val);
	atualizaPc();	
*/


  frameCorrente->pilhaOp->depth += 1;
	atualizaPc();
}

///
/// Seta o atributo static
///
/// @param Nenhum
/// @return @c void
/// @see atualizaPc
void putstatic() {
  atualizaPc();
}

///
/// Pega o field de uma classe a partir
/// do indice para uma constante do Constant Pool
/// desempilhado da pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see retornaNome popOp buscaCampo pushOp atualizaPc
void getfield() {
	
	uint32_t indice = frameCorrente->code[frameCorrente->pc + 2];
	int32_t indiceClasse = frameCorrente->constantPool[indice-1].info.Fieldref.classIndex;
	char* nomeClasse = retornaNome(frameCorrente->classe, frameCorrente->constantPool[indiceClasse-1].info.Class.nameIndex);
	uint16_t nomeTipoIndice = frameCorrente->constantPool[indice-1].info.Fieldref.nameAndTypeIndex;
	char* nome = retornaNome(frameCorrente->classe, frameCorrente->constantPool[nomeTipoIndice-1].info.NameAndType.nameIndex);
	char* tipo = retornaNome(frameCorrente->classe, frameCorrente->constantPool[nomeTipoIndice-1].info.NameAndType.descriptorIndex);
	tipoGlobal = tipo;

 	Objeto* obj = (Objeto*) popOp();

 	int32_t indiceField = buscaCampo(nomeClasse, nome, tipo);
 	uint32_t indiceNome = frameCorrente->classe->fields[indiceField].nameIndex;

 	if(tipo[0] == 'J' || tipo[0] == 'D') {
 		int32_t i;
		for(i = 0;obj->indiceCampos[i] != indiceNome; i++);

		int32_t baixa = obj->campos[i];
		int32_t alta = obj->campos[i+1];

		pushOp(alta);
		pushOp(baixa);
		atualizaPc();
 	} else {

		int32_t i = 0;
	    while(obj->indiceCampos[i] != indiceNome) {
	    	i++;
	    }

	 	uint32_t val = obj->campos[i];

	 	pushOp(val);
		atualizaPc();
	}
}

///
/// Atribui o valor desempilhado da piha de operandos
/// a um field de uma classe, a partir dos argumentos
/// que geram um indice para uma constante do
/// Constant Pool
///
/// @param Nenhum
/// @return @c void
/// @see retornaNome popOp buscaCampo atualizaPc
void putfield() {
	uint32_t indice = frameCorrente->code[frameCorrente->pc + 2];
	int32_t indiceClasse = frameCorrente->constantPool[indice-1].info.Fieldref.classIndex;
	char* nomeClasse = retornaNome(frameCorrente->classe, frameCorrente->constantPool[indiceClasse-1].info.Class.nameIndex);
	uint16_t nomeTipoIndice = frameCorrente->constantPool[indice-1].info.Fieldref.nameAndTypeIndex;
	char* nome = retornaNome(frameCorrente->classe, frameCorrente->constantPool[nomeTipoIndice-1].info.NameAndType.nameIndex);
	char* tipo = retornaNome(frameCorrente->classe, frameCorrente->constantPool[nomeTipoIndice-1].info.NameAndType.descriptorIndex);
 	int32_t indiceField = buscaCampo(nomeClasse, nome, tipo);
 	uint32_t indiceNome = frameCorrente->classe->fields[indiceField].nameIndex;

 	if(tipo[0] == 'J' || tipo[0] == 'D') {
 		int32_t alta;
    int32_t baixa;
 		int32_t val1 = popOp();
 		int32_t val2 = popOp();
 		Objeto* obj = (Objeto*)popOp();

		int64_t dVal = val2;
		dVal <<= 32;
		dVal = dVal + val1;

		double valorDouble1;
		memcpy(&valorDouble1, &dVal, sizeof(int64_t));

		int i;
		for(i = 0; obj->indiceCampos[i] != indiceNome; i++);

		int64_t valorPilha;
		memcpy(&valorPilha, &valorDouble1, sizeof(int64_t));

		alta = valorPilha >> 32;
		baixa = valorPilha & 0xffffffff;
		obj->campos[i] = baixa;
		obj->campos[i+1] = alta;
 	} else {
	 	int32_t val = popOp();
	 	Objeto* obj = (Objeto*)popOp();
	 	int i;
	 	for(i = 0; obj->indiceCampos[i] != indiceNome; i++);
		obj->campos[i] = val;
	}

	atualizaPc();
}

///
/// Invoca um metodo a partir da referencia
/// encontrada usando os dois argumentos para
/// construir o indice para a Constant Pool.
/// O metodo nao pode ser de inicializacao
/// de classe ou de inicializacao de interface
///
/// @param Nenhum
/// @return @c void
/// @see retornaNome popOp buscaCampo atualizaPc
void invokevirtual() {
	MethodInfo* metodoInvocado;
  char* nomeClasse;
  char* nomeMetodo;
  char* descricaoMetodo;
  uint16_t nomeMetodoAux;
  uint16_t descricaoMetodoAux;
  uint16_t nomeTipoAux;
  int32_t resultado;
  int32_t resultado2;
  int32_t resultado_string;
  int32_t classeIndice;
  uint8_t* string = NULL;

  uint32_t pcAux = frameCorrente->code[frameCorrente->pc + 2];

  classeIndice = frameCorrente->constantPool[pcAux - 1].info.Methodref.classIndex;
  nomeClasse = retornaNome(frameCorrente->classe, frameCorrente->constantPool[classeIndice - 1].info.Class.nameIndex);
  nomeTipoAux = frameCorrente->constantPool[pcAux - 1].info.Methodref.nameAndTypeIndex;
  nomeMetodoAux = frameCorrente->constantPool[nomeTipoAux - 1].info.NameAndType.nameIndex;
  descricaoMetodoAux = frameCorrente->constantPool[nomeTipoAux - 1].info.NameAndType.descriptorIndex;
  nomeMetodo = retornaNome(frameCorrente->classe, nomeMetodoAux);
  descricaoMetodo = retornaNome(frameCorrente->classe, descricaoMetodoAux);

	if((strcmp(nomeClasse, "java/io/PrintStream") == 0) && (strcmp(nomeMetodo,"println") == 0)) {
    if (strcmp(descricaoMetodo, "()V") == 0) {
        printf("\n");
    } else {
      resultado = popOp();

      if (tipoGlobal == NULL) {
          string = frameCorrente->constantPool[resultado].info.Utf8.bytes;
      }

      if (string != NULL) {
          printf("%s\n",string);
      } else if(strcmp(tipoGlobal, "Z") == 0) {
          if(resultado) {
          	printf("TRUE\n");
          } else {
          	printf("FALSE\n");
          }
      } else if(strcmp(tipoGlobal, "F") == 0) {
          float valDesemp;
          memcpy(&valDesemp, &resultado, sizeof(float));
          printf("%f\n",valDesemp);
      } else if(strcmp(tipoGlobal, "D") == 0) {
          resultado2 = popOp();
          double resultado_double;
          int64_t temp;
          temp = resultado2;
          temp <<= 32;
          temp += resultado;
          memcpy(&resultado_double, &temp, sizeof(int64_t));
          printf("%f\n", resultado_double);

      } else if(strcmp(tipoGlobal, "L") == 0) {
          resultado2 = popOp();
          int64_t long_num;
          long long result;
          long_num= resultado2;
          long_num <<= 32;
          long_num |= resultado;
          memcpy(&result, &long_num, sizeof(long));
          flagLNEG = FALSE;

          if (!flagLNEG) {
              printf("%" PRId64 "\n", long_num);
          } else {
              printf("%" PRId64 "\n", result);
          }
      } else if (strcmp(tipoGlobal, "I") == 0) {
          printf("%d\n", resultado);
      } else if (strcmp(tipoGlobal, "C") == 0) {
          printf("%c\n", resultado);
      } else {
          printf("erro no invoke_virtual, tipoGlobal ainda nao setado");
          exit(1);
      }
    }

    flagLNEG = FALSE;
		atualizaPc();
		return;
	}

	classeIndice = carregaClasseParaMemoria(nomeClasse);
	ClassFile* classe = buscaClassPorIndice(classeIndice);

	metodoInvocado = buscaMetodo(frameCorrente->classe,classe,nomeTipoAux);
	if(metodoInvocado == NULL){
		printf("Método não Encontrado!\n");
		exit(0);
	}

	int32_t numeroParametros = retornaNumeroParametros(classe,metodoInvocado);
	uint32_t* fields = calloc(sizeof(uint32_t),numeroParametros + 1);

	for(int32_t i = 0; i <= numeroParametros; i++){
		fields[i] = popOp();
	}

	empilhaMetodo(metodoInvocado, classe);

	for(int32_t i = 0; i <= numeroParametros; i++) {
    frameCorrente->fields[i] = fields[numeroParametros - i];
	}

	executaFrameCorrente();
	flagLNEG = FALSE;
	atualizaPc();
	return;
}

///
/// Invoca um metodo a partir da referencia
/// encontrada usando os dois argumentos para
/// construir o indice para a Constant Pool.
/// O metodo deve ser de superclasse ou de
/// inicializacao
///
/// @param Nenhum
/// @return @c void
/// @see retornaNome popOp buscaCampo atualizaPc
void invokespecial() {
	MethodInfo* metodoInvocado;

	uint32_t indice = frameCorrente->code[frameCorrente->pc + 2];
	uint32_t indiceClasse = (frameCorrente->constantPool[indice-1]).info.Methodref.classIndex;

	char* nomeClasse = retornaNome(frameCorrente->classe,(frameCorrente->constantPool[indiceClasse-1]).info.Class.nameIndex);

  if(strcmp("java/lang/Object",nomeClasse) == 0) {
		carregaClasseParaMemoria(nomeClasse);
		atualizaPc();
		return;
	}

	int32_t indexClasse = carregaClasseParaMemoria(nomeClasse);
	ClassFile* classe = buscaClassPorIndice(indexClasse);
	uint16_t nomeTipoIndice = frameCorrente->constantPool[indice-1].info.Methodref.nameAndTypeIndex;
	metodoInvocado = buscaMetodo(frameCorrente->classe,classe,nomeTipoIndice);
	int32_t numeroParametros = retornaNumeroParametros(classe,metodoInvocado);
	uint32_t* fields = calloc(sizeof(uint32_t),numeroParametros + 1);

	for(int32_t i = 0; i <= numeroParametros; i++) {
		fields[i] = popOp();
	}

	empilhaMetodo(metodoInvocado, classe);

	for(int32_t i = 0; i <= numeroParametros; i++) {
			frameCorrente->fields[i] = fields[numeroParametros - i];
	}

	executaFrameCorrente();
	atualizaPc();
}

///
/// Invoca um metodo a partir da referencia
/// encontrada usando os dois argumentos para
/// construir o indice para a Constant Pool.
/// O metodo deve ser static
///
/// @param Nenhum
/// @return @c void
/// @see retornaNome popOp buscaCampo atualizaPc
void invokestatic() {
	MethodInfo* metodoInvocado;
  char* nomeMetodo;
  char* descricaoMetodo;
  uint16_t nomeMetodoAux;
  uint16_t descricaoMetodoAux;
  uint16_t nomeTipoAux;
	uint32_t indice = frameCorrente->code[frameCorrente->pc + 2];
	uint32_t indiceClasse = (frameCorrente->constantPool[indice-1]).info.Methodref.classIndex;
	char* nomeClasse = retornaNome(frameCorrente->classe,(frameCorrente->constantPool[indiceClasse-1]).info.Class.nameIndex);
	nomeTipoAux = frameCorrente->constantPool[indice - 1].info.Methodref.nameAndTypeIndex;
  nomeMetodoAux = frameCorrente->constantPool[nomeTipoAux - 1].info.NameAndType.nameIndex;
	descricaoMetodoAux = frameCorrente->constantPool[nomeTipoAux - 1].info.NameAndType.descriptorIndex;
  nomeMetodo = retornaNome(frameCorrente->classe, nomeMetodoAux);
  descricaoMetodo = retornaNome(frameCorrente->classe, descricaoMetodoAux);

	if((strcmp(nomeClasse, "java/lang/System") == 0) && (strcmp(nomeMetodo,"exit") == 0)) {
		if(strstr(descricaoMetodo, "(I)V") != NULL) {
			int32_t retPilha = popOp();
			exit(retPilha);
      atualizaPc();
      return;
		}
	}

	if((strcmp(nomeClasse, "java/lang/Math") == 0) && (strcmp(nomeMetodo,"sqrt") == 0)) {
		if(strstr(descricaoMetodo, "(D)D") != NULL) {
			int32_t baixa = popOp();
			int32_t alta = popOp();

			int64_t dVal = alta;
			dVal <<= 32;
			dVal = dVal + baixa;

			double valorDouble1;
			memcpy(&valorDouble1, &dVal, sizeof(int64_t));

			valorDouble1 = sqrt (valorDouble1);

			int64_t aux;
			memcpy(&aux, &valorDouble1, sizeof(int64_t));

			alta = aux >> 32;
			baixa = aux & 0xffffffff;

			pushOp(alta);
			pushOp(baixa);
      atualizaPc();
      return;
		}
	}

	int32_t indexClasse = carregaClasseParaMemoria(nomeClasse);
	ClassFile* classe = buscaClassPorIndice(indexClasse);
	uint16_t nomeTipoIndice = frameCorrente->constantPool[indice-1].info.Methodref.nameAndTypeIndex;
	metodoInvocado = buscaMetodo(frameCorrente->classe,classe,nomeTipoIndice);
	int32_t numeroParametros = retornaNumeroParametros(classe,metodoInvocado);
	uint32_t* fields = calloc(sizeof(uint32_t),numeroParametros + 1);

	for(int32_t i = 0; i < numeroParametros; i++) {
		fields[i] = popOp();
	}

	empilhaMetodo(metodoInvocado, classe);

	for(int32_t i = 0; i < numeroParametros; i++) {
			frameCorrente->fields[i] = fields[numeroParametros - i - 1];
	}

	executaFrameCorrente();
	atualizaPc();
}

///
/// Invoca um metodo a partir da referencia
/// encontrada usando os dois argumentos para
/// construir o indice para a Constant Pool.
/// O metodo deve ser de uma Interface
///
/// @param Nenhum
/// @return @c void
/// @see retornaNome popOp buscaCampo atualizaPc
void invokeinterface() {
	MethodInfo* metodoInvocado;
	char* nomeMetodo;
	char* descricaoMetodo;
	uint16_t nomeMetodoAux;
  uint16_t descricaoMetodoAux;
  uint16_t nomeTipoAux;

	uint32_t indice = frameCorrente->code[frameCorrente->pc + 2];
	uint32_t indiceClasse = (frameCorrente->constantPool[indice-1]).info.Methodref.classIndex;
	char* nomeClasse = retornaNome(frameCorrente->classe,(frameCorrente->constantPool[indiceClasse-1]).info.Class.nameIndex);
	nomeTipoAux = frameCorrente->constantPool[indice - 1].info.Methodref.nameAndTypeIndex;
  nomeMetodoAux = frameCorrente->constantPool[nomeTipoAux - 1].info.NameAndType.nameIndex;
	descricaoMetodoAux = frameCorrente->constantPool[nomeTipoAux - 1].info.NameAndType.descriptorIndex;
  nomeMetodo = retornaNome(frameCorrente->classe, nomeMetodoAux);
  descricaoMetodo = retornaNome(frameCorrente->classe, descricaoMetodoAux);

	int32_t indexClasse = carregaClasseParaMemoria(nomeClasse);
	ClassFile* classe = buscaClassPorIndice(indexClasse);
	uint16_t nomeTipoIndice = frameCorrente->constantPool[indice-1].info.Methodref.nameAndTypeIndex;
	metodoInvocado = buscaMetodo(frameCorrente->classe,classe,nomeTipoIndice);
	int32_t numeroParametros = retornaNumeroParametros(classe,metodoInvocado);
	uint32_t* fields = calloc(sizeof(uint32_t),numeroParametros + 1);

	for(int32_t i = 0; i < numeroParametros; i++) {
		fields[i] = popOp();
	}

	empilhaMetodo(metodoInvocado, classe);

	for(int32_t i = 0; i < numeroParametros; i++) {
			frameCorrente->fields[i] = fields[numeroParametros - i - 1];
	}

	executaFrameCorrente();
	atualizaPc();
}

///
/// Instancia um novo objeto buscando a
/// classe pelo indice recupera dos
/// argumentos e, se a classe esta carregada,
/// instancia um objeto
///
/// @param Nenhum
/// @return @c void
/// @see retornaNome popOp buscaCampo atualizaPc retornaIndiceDaClassePorNome
void _new() {
	uint32_t indice;
	int32_t aux;
	char* nomeClasse;
	Objeto* objeto;
	ClassFile* classe;

	indice = frameCorrente->code[2+(frameCorrente->pc)];
	nomeClasse = retornaNome(frameCorrente->classe, frameCorrente->constantPool[indice-1].info.Class.nameIndex);
  aux = retornaIndiceDaClassePorNome(nomeClasse);

  if(aux == -1) {
    char opcao;
    printf("\n\n--- ERRO ---\n\nNome da classe invalido, nao condiz com o nome do arquivo .class\n\n");
    exit(0);
  } 
  	classe = buscaClassPorIndice(aux);
	objeto = criaObjeto(classe);
	
	if(objeto == NULL) {
		printf("Objeto não foi corretamente alocado\n");
	}

	pushOp((int32_t) objeto);
	atualizaPc();
}

///
/// Cria um array do tipo especificado
/// pelo primeiro argumento e de
/// tamanho especificado por um valor
/// inteiro desempilhado da pilha de
/// operandos. Apos alocar, salva uma
/// referencia na pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void newarray() {
	int32_t tamanhoBytes;
	int32_t tamanhoArray = popOp();
	int8_t tipoArray = frameCorrente->code[(frameCorrente->pc)+1];

	if(tipoArray == 11) {
		tamanhoBytes = 8;
	}
	if(tipoArray == 7) {
		tamanhoBytes = 8;
	}

	if(tipoArray == 6) {
		tamanhoBytes = 4;
	}
	if(tipoArray == 0) {
		tamanhoBytes = 4;
	}
	if(tipoArray == 10) {
		tamanhoBytes = 4;
	}

	if(tipoArray == 5) {
		tamanhoBytes = 2;
	}
	if(tipoArray == 9) {
		tamanhoBytes = 2;
	}

	if(tipoArray == 4) {
		tamanhoBytes = 1;
	}
	if(tipoArray == 8) {
		tamanhoBytes = 1;
	}

	int32_t* vetor = calloc(tamanhoBytes,tamanhoArray);

	qtdArrays++;
	arrayVetores = realloc (arrayVetores, sizeof(struct Vector)*qtdArrays);
	arrayVetores[qtdArrays-1].tamanho = tamanhoArray;
	arrayVetores[qtdArrays-1].referencia = (int32_t)vetor;
	arrayVetores[qtdArrays-1].tipo = tipoArray;

	pushOp((int32_t)vetor);
  atualizaPc();
}

///
/// Cria um array de referencias e de
/// tamanho especificado por um valor
/// inteiro desempilhado da pilha de
/// operandos. Apos alocar, salva uma
/// referencia na pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void anewarray() {
	int32_t tamanhoBytes;
	int32_t tamanhoArray = popOp();
	int8_t tipoArray = frameCorrente->code[(frameCorrente->pc)+1];

	if(tipoArray == 11) {
		tamanhoBytes = 8;
	}
	if(tipoArray == 7) {
		tamanhoBytes = 8;
	}
	if(tipoArray == 6) {
		tamanhoBytes = 4;
	}
	if(tipoArray == 0) {
		tamanhoBytes = 4;
	}
	if(tipoArray == 10) {
		tamanhoBytes = 4;
	}
	if(tipoArray == 5) {
		tamanhoBytes = 2;
	}
	if(tipoArray == 9) {
		tamanhoBytes = 2;
	}
	if(tipoArray == 4) {
		tamanhoBytes = 1;
	}
	if(tipoArray == 8) {
		tamanhoBytes = 1;
	}

	int32_t* vetor = calloc(tamanhoBytes,tamanhoArray);

	qtdArrays++;
	arrayVetores = realloc (arrayVetores, sizeof(struct Vector)*qtdArrays);
	arrayVetores[qtdArrays-1].tamanho = tamanhoArray;
	arrayVetores[qtdArrays-1].referencia = (int32_t)vetor;
	arrayVetores[qtdArrays-1].tipo = tipoArray;

	pushOp((int32_t)vetor);
  atualizaPc();
}

///
/// Desempilha uma referencia para um
/// array e empilha o tamanho do mesmo
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void arraylength() {
	int32_t arrayRef = popOp();
	int i = 0;

	while(i < qtdArrays) {
		if(arrayVetores[i].referencia == arrayRef) {
			int32_t length = arrayVetores[i].tamanho;
			pushOp(length);
			atualizaPc();
			return;
		}
		i++;
	}

  printf("O metodo tentou acessar uma referencia de array nula");
	exit(0);
}

///
/// Checa se o objeto e do tipo
/// da referencia desempilhada da
/// pilha de operandos
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp atualizaPc
void checkcast() {
	int16_t indice;
	int8_t offset1;
  int8_t offset2;

	offset1 = frameCorrente->code[(frameCorrente->pc)+1];
	offset2 = frameCorrente->code[(frameCorrente->pc)+2];

	indice = (offset1 << 8) | offset2;

	Objeto* Objeto = (struct Objeto*) popOp();

	if(Objeto == NULL) {
		printf("Objeto nulo!\n");
    atualizaPc();
    return;
	}

	char* nomeClasse = retornaNomeClass(Objeto->classe);
	char* nomeIndice = retornaNome(frameCorrente->classe, indice);

	if(strcmp(nomeClasse,nomeIndice) == 0) {
		printf("Objeto é do tipo: %s\n", nomeIndice);
    pushOp((int32_t)Objeto);
	}

	atualizaPc();
}

///
/// Recupera uma referencia de objeto da pilha de operandos
/// e verifica o tipo do objeto. Em caso nulo, empilha um
/// valor 0. Caso contrario, os 2 argumentos sao usados
/// para encontrar uma constante no Constant Pool que contem
/// o tipo do objeto (class, array, interface) e o valor 1
/// eh empilhado na pilha de operandos.
///
/// @param Nenhum
/// @return @c void
/// @see popOp pushOp retornaNomeClasse retornaNome
void instanceof() {
	int16_t indice;
	int8_t offset1,offset2;

	offset1 =  frameCorrente->code[(frameCorrente->pc)+1];
	offset2 =  frameCorrente->code[(frameCorrente->pc)+2];

	indice = (offset1 << 8) | offset2;

	Objeto* Objeto = (struct Objeto*) popOp();

	if(Objeto == NULL) {
		printf("Objeto nulo!\n");
		pushOp(0);
	}

	char* nomeClasse = retornaNomeClass(Objeto->classe);
	char* nomeIndice = retornaNome(frameCorrente->classe,indice);

	if(strcmp(nomeClasse,nomeIndice) == 0) {
		printf("Objeto é do tipo: %s\n",nomeIndice);
		pushOp(1);
	}

	atualizaPc();
}

///
/// Modifica o comportamento de outras instrucoes para receber
/// mais bytes como argumento, que serao usados para compor
/// um indice do array de variaveis locais.
///
/// <b> Essa instruncao nao foi implementada!</b>
///
/// @param Nenhum
/// @return @c void
void wide() {
	printf("\nERRO\n");
	printf("Instrucao \"wide\" nao implementada! \n");
	exit(0);
}

///
/// Cria um array multidimensional onde os 2 primeiros argumentos
/// sao compostos para ser usados como indice de uma classe
/// no Constant Pool. O ultimo argumento eh usado para definir
/// a quantidade de dimensoes no array.
///
/// <b> Essa instruncao nao foi implementada!</b>
///
/// @param Nenhum
/// @return @c void
void multianewarray() {
	printf("\nERRO\n");
	printf("Instrucao \"multianewarray\" nao implementada! \n");
	exit(0);
}

///
/// Recupera um valor da pilha de operandos e
/// verifica se eh uma referencia. Em caso positivo,
/// usa os 2 argumentos para compor um offset de 16bits
/// da subrotina que deve ser executada em seguida e
/// avanca o PC, somando o offset calculado.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void ifnull() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	int32_t retPilha = popOp();

	if(retPilha == 0) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Recupera um valor da pilha de operandos e verifica se
/// eh uma referencia nao nula. Em caso positivo, usa
/// os 2 argumentos para compor um offset de 16bits
/// da subrotina que deve ser executada em seguida e
/// avanca o PC, somando o offset calculado.
///
/// @param Nenhum
/// @return @c void
/// @see popOp
void ifnonnull() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	int32_t retPilha = popOp();

	if(retPilha != 0) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

///
/// Vai para a subrotina usando os 4 argumentos
/// para compor um offset de 32bits da subrotina
/// que deve ser executada em seguida e avanca
/// o PC, somando o offset calculado.
///
/// @param Nenhum
/// @return @c void
void goto_w() {
	int32_t deslocamento,offset1,offset2,offset3,offset4;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset3 = frameCorrente->code[frameCorrente->pc + 3];
	offset4 = frameCorrente->code[frameCorrente->pc + 4];

	deslocamento  = (offset1 & 0xFF) << 24;
	deslocamento |= (offset2 & 0xFF) << 16;
	deslocamento |= (offset3 & 0xFF) << 8;
	deslocamento |= (offset4 & 0xFF);

	frameCorrente->pc += deslocamento;
}

///
/// Pula para a subrotina usando os 4 argumentos
/// para compor um endereco de 32bits da subrotina
/// que deve ser executada e avanca o PC.
///
/// @param Nenhum
/// @return @c void
/// @see pushOp
void jsr_w() {

	int32_t deslocamento;
  int32_t offset1;
  int32_t offset2;
  int32_t offset3;
  int32_t offset4;

	pushOp(frameCorrente->code[frameCorrente->pc + 5]);

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset3 = frameCorrente->code[frameCorrente->pc + 3];
	offset4 = frameCorrente->code[frameCorrente->pc + 4];

	deslocamento  = (offset1 & 0xFF)<<24;
	deslocamento |= (offset2 & 0xFF)<<16;
	deslocamento |= (offset3 & 0xFF)<<8;
	deslocamento |= (offset4 & 0xFF);

	frameCorrente->pc += deslocamento;
}
