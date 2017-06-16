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

#define NULL_REF NULL
#define POS_BAIXA 1
#define POS_ALTA 0

Vector* arrayVetores = NULL;

int32_t qtdArrays = 0;

extern struct Frame* frameCorrente;

int naoEmpilhaFlag = 0;

Decodificador dec[NUM_INSTRUCAO];

void atualizaPc() {
	inicializaDecodificador(dec);
	int numBytes = dec[frameCorrente->code[frameCorrente->pc]].bytes;

	for(int8_t i = 0; i < numBytes + 1; i++) {
		frameCorrente->pc++;
	}
}

int obtemUtfEq(CpInfo* cp, int posPool) {
    int tag;
    tag = cp[posPool].tag;

    if (tag == CONSTANT_Utf8) {
        return posPool;
    }

    switch(tag) {
        case CONSTANT_Class:
            return obtem_utf_eq(cp, cp[posPool].info.Class.nameIndex - 1);
        case CONSTANT_String:
            return obtem_utf_eq(cp, cp[posPool].info.String.stringIndex - 1);
        case CONSTANT_Integer:
            return obtem_utf_eq(cp, cp[posPool].info.String.stringIndex - 1);
        case CONSTANT_Float:
            return obtem_utf_eq(cp, cp[posPool].info.String.stringIndex - 1);
    }
}

void newInstrucoes() {
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
	instrucao[109] = ins_ldiv;
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
	instrucao[167] = ins_goto;
	instrucao[168] = jsr;
	instrucao[169] = ret;
	instrucao[170] = tableswitch;
	instrucao[171] = lookupswitch;
	instrucao[172] = ireturn;
	instrucao[173] = lreturn;
	instrucao[174] = freturn;
	instrucao[175] = dreturn;
	instrucao[176] = areturn;
	instrucao[177] = ins_return;
	instrucao[178] = getstatic;
	instrucao[179] = putstatic;
	instrucao[180] = getfield;
	instrucao[181] = putfield;
	instrucao[182] = invokevirtual;
	instrucao[183] = invokespecial;
	instrucao[184] = invokestatic;
	instrucao[185] = invokeinterface;
	instrucao[187] = ins_new;
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

void nop() {
	frameCorrente->pc++;
}

void aconst_null() {
    push((int32_t)NULL_REF);

	frameCorrente->pc++;
}

void iconst_m1() {
    char* tipo = "I";
    tipoGlobal = tipo;

    push(-1);

    frameCorrente->pc++;
}

void iconst_0() {
    char* tipo = "I";
    tipoGlobal = tipo;

	push((int32_t) 0);

	atualizaPc();
}

void iconst_1() {
    char* tipo = "I";
    tipoGlobal = tipo;

    push(1);

    frameCorrente->pc++;
}

void iconst_2() {
    char* tipo = "I";
    tipoGlobal = tipo;

    push(2);

    frameCorrente->pc++;
}

void iconst_3() {
    char* tipo = "I";
    tipoGlobal = tipo;

    push(3);
    frameCorrente->pc++;
}

void iconst_4() {
    char* tipo = "I";
    tipoGlobal = tipo;

    push(4);

    frameCorrente->pc++;
}

void iconst_5() {
    char* tipo = "I";
    tipoGlobal = tipo;

    push(5);

    frameCorrente->pc++;
}

void lconst_0() {
    char* tipo = "L";
    tipoGlobal = tipo;


    int64_t long0 = 0;
    int32_t parte_alta;
    int32_t parte_baixa;

	parte_alta = long0 >> 32;
	parte_baixa = long0 & 0xffffffff;

    push(parte_alta);

    push(parte_baixa);

    frameCorrente->pc++;
}

void lconst_1() {
    char* tipo = "L";
    tipoGlobal = tipo;

    int64_t long1 = 1;
    int32_t parte_alta;
    int32_t parte_baixa;

	parte_alta = long1 >> 32;
	parte_baixa = long1 & 0xffffffff;

    push(parte_alta);

    push(parte_baixa);

    frameCorrente->pc++;
}

void fconst_0() {
    char* tipo = "F";
    tipoGlobal = tipo;


	int32_t* valPilha;

	float valF = 0.0;

	valPilha = (int32_t*) malloc(sizeof(int32_t));

	memcpy(valPilha, &valF, sizeof(int32_t));

	push(*valPilha);

	atualizaPc();
}

void fconst_1() {
    char* tipo = "F";
    tipoGlobal = tipo;

	int32_t* valPilha;

	float valF = 1.0;

	valPilha = (int32_t*) malloc(sizeof(int32_t));

	memcpy(valPilha, &valF, sizeof(int32_t));

	push(*valPilha);

	atualizaPc();
}

void fconst_2() {
    char* tipo = "F";
    tipoGlobal = tipo;

	int32_t* valPilha;

	float valF = 2.0;

	valPilha = (int32_t*) malloc(sizeof(int32_t));

	memcpy(valPilha, &valF, sizeof(int32_t));

	push(*valPilha);

	atualizaPc();
}

void dconst_0() {
    char* tipo = "D";
    tipoGlobal = tipo;

    double double0 = 0.0;
    int64_t temp;
    int32_t parte_alta;
    int32_t parte_baixa;

	memcpy(&temp, &double0, sizeof(int64_t));

	parte_alta = temp >> 32;
	parte_baixa = temp & 0xffffffff;

    push(parte_alta);
    push(parte_baixa);

    frameCorrente->pc++;
}

void dconst_1() {
    char* tipo = "D";
    tipoGlobal = tipo;

    double double1 = 1.0;
    int64_t temp;
    int32_t parte_alta;
    int32_t parte_baixa;

	memcpy(&temp, &double1, sizeof(int64_t));

	parte_alta = temp >> 32;
	parte_baixa = temp & 0xffffffff;

    push(parte_alta);
    push(parte_baixa);

    frameCorrente->pc++;
}

void bipush() {
	int8_t argumento = (int8_t) frameCorrente->code[frameCorrente->pc + 1];

	push((int32_t)argumento);

	atualizaPc();
}

void sipush() {
    int32_t byte1, byte2;
    int32_t valor;
    int16_t short_temp;

	byte1 = frameCorrente->code[(frameCorrente->pc + 1)];

	byte2 = frameCorrente->code[(frameCorrente->pc + 2)];

    short_temp = (byte1 << 8) + byte2;
    valor = (int32_t) short_temp;

    push(valor);
    atualizaPc();
}

void ldc() {
    uint32_t indice;
    tipoGlobal = NULL;

    indice = frameCorrente->code[frameCorrente->pc + 1];

    if (frameCorrente->constantPool[indice - 1].tag == CONSTANT_Float || \
        frameCorrente->constantPool[indice - 1].tag == CONSTANT_Integer) {

        if (frameCorrente->constantPool[indice - 1].tag == CONSTANT_Float) {
            push(frameCorrente->constantPool[indice - 1].info.Float.bytes);
        } else {
            push(frameCorrente->constantPool[indice - 1].info.Integer.bytes);
        }
    } else if (frameCorrente->constantPool[indice - 1].tag == CONSTANT_String) {
        uint32_t indice_utf;
        indice_utf = obtemUtfEq(frameCorrente->constantPool, indice-1);
        push(indice_utf);
    } else if (frameCorrente->constantPool[indice - 1].tag == CONSTANT_String) {
        printf("a implementar\n");
        exit(1);
    } else {
        printf("erro na instrucao ldc\n");
        exit(1);
    }
    atualizaPc();
}

void ldc_w() {
    uint32_t indice;

	inicializaDecodificador(dec);
	int numBytes = dec[frameCorrente->code[frameCorrente->pc]].bytes;

    indice = (frameCorrente->code[frameCorrente->pc + 1] << 8 + frameCorrente->code[frameCorrente->pc + 2]);

    if (frameCorrente->constantPool[indice - 1].tag == CONSTANT_Float || \
            frameCorrente->constantPool[indice - 1].tag == CONSTANT_Integer) {
        if (frameCorrente->constantPool[indice - 1].tag == CONSTANT_Float) {
            push(frameCorrente->constantPool[indice - 1].info.Float.bytes);
        } else {
            push(frameCorrente->constantPool[indice - 1].info.Integer.bytes);
        }
    } else if (frameCorrente->constantPool[indice - 1].tag == CONSTANT_String) {
        uint32_t indice_utf;
        indice_utf = obtemUtfEq(frameCorrente->constantPool, indice-1);
        push(indice_utf);
    } else if (frameCorrente->constantPool[indice - 1].tag == CONSTANT_String) {
    } else {
        printf("erro na instrucao ldc\n");
        exit(1);
    }
	for(int8_t i = 0; i < numBytes + 1; i++) {
		frameCorrente->pc++;
	}
}

void ldc2_w() {

	uint8_t indice = frameCorrente->code[frameCorrente->pc + 2];

	uint8_t tag = (frameCorrente->constantPool[indice-1]).tag;

	if(tag == 5) {
		uint32_t alta = frameCorrente->constantPool[indice-1].info.Long.highBytes;
		uint32_t baixa = frameCorrente->constantPool[indice-1].info.Long.lowBytes;
		push(alta);
		push(baixa);
	}

	if(tag == 6) {
		uint32_t alta = frameCorrente->constantPool[indice-1].info.Double.highBytes;
		uint32_t baixa = frameCorrente->constantPool[indice-1].info.Double.lowBytes;
		push(alta);
		push(baixa);
	}

	atualizaPc();
    foi_lneg = false;

}

void iload() {

    char* tipo = "I";
    tipoGlobal = tipo;

	int32_t argumento = (int32_t) frameCorrente->code[frameCorrente->pc + 1];
	int32_t aux = frameCorrente->fields[argumento];
	push(aux);

	atualizaPc();

}

void lload() {
	char* tipo = "L";
    tipoGlobal = tipo;

    int32_t indice;
    int32_t parte_alta, parte_baixa;

    indice = frameCorrente->code[frameCorrente->pc + 1];

    parte_alta = frameCorrente->fields[indice + POS_ALTA];
    push(parte_alta);

    parte_baixa = frameCorrente->fields[indice + POS_BAIXA];
    push(parte_baixa);

    atualizaPc();

}

void fload() {

	char* tipo = "F";
    tipoGlobal = tipo;

    int32_t indice, valor;

    indice = frameCorrente->code[frameCorrente->pc + 1];

    valor = frameCorrente->fields[indice];
    push(valor);

    atualizaPc();
}

void dload() {

    int32_t indice;
    int32_t parte_alta, parte_baixa;
    char* tipo = "D";
    tipoGlobal = tipo;

    indice = frameCorrente->code[frameCorrente->pc + 1];

    parte_alta = frameCorrente->fields[indice + POS_ALTA];
    push(parte_alta);

    parte_baixa = frameCorrente->fields[indice + POS_BAIXA];
    push(parte_baixa);

    atualizaPc();
}

void aload() {

    int32_t indice, valor;

    indice = frameCorrente->code[frameCorrente->pc + 1];

    valor = frameCorrente->fields[indice];
    push(valor);

    atualizaPc();

}

void iload_0() {

	char* tipo = "I";
    tipoGlobal = tipo;

    int32_t valor;

    valor = frameCorrente->fields[0];

    push(valor);

	atualizaPc();
}

void iload_1() {
	char* tipo = "I";
    tipoGlobal = tipo;

    int32_t valor;

    valor = frameCorrente->fields[1];

    push(valor);
    atualizaPc();
}

void iload_2() {

	char* tipo = "I";
    tipoGlobal = tipo;

    int32_t valor;

    valor = frameCorrente->fields[2];

    push(valor);

    atualizaPc();
}

void iload_3() {

    int32_t valor;
    char* tipo = "I";
    tipoGlobal = tipo;

    valor = frameCorrente->fields[3];

    push(valor);

    atualizaPc();

}

void lload_0() {

	char* tipo = "L";
    tipoGlobal = tipo;

    int32_t indice;
    int32_t parte_alta, parte_baixa;

    indice = 0;

    parte_alta = frameCorrente->fields[indice + POS_ALTA];
    push(parte_alta);

    parte_baixa = frameCorrente->fields[indice + POS_BAIXA];
    push(parte_baixa);

	atualizaPc();

}

void lload_1() {
    int32_t indice;
    int32_t parte_alta, parte_baixa;

    char* tipo = "L";
    tipoGlobal = tipo;

    indice = 1;

    parte_alta = frameCorrente->fields[indice + POS_ALTA];
    push(parte_alta);

    parte_baixa = frameCorrente->fields[indice + POS_BAIXA];
    push(parte_baixa);

    atualizaPc();

}

void lload_2() {

	char* tipo = "L";
    tipoGlobal = tipo;

    int32_t indice;
    int32_t parte_alta, parte_baixa;

    indice = 2;

    parte_alta = frameCorrente->fields[indice + POS_ALTA];
    push(parte_alta);

    parte_baixa = frameCorrente->fields[indice + POS_BAIXA];
    push(parte_baixa);

    atualizaPc();

}

void lload_3() {

	char* tipo = "L";
    tipoGlobal = tipo;

    int32_t indice;
    int32_t parte_alta, parte_baixa;

    indice = 3;

    parte_alta = frameCorrente->fields[indice + POS_ALTA];
    push(parte_alta);

    parte_baixa = frameCorrente->fields[indice + POS_BAIXA];
    push(parte_baixa);

    atualizaPc();

}

void fload_0() {

	char* tipo = "F";
    tipoGlobal = tipo;

    int32_t indice, valor;

    indice = 0;

    valor = frameCorrente->fields[indice];
    push(valor);

   atualizaPc();
}

void fload_1() {

	char* tipo = "F";
    tipoGlobal = tipo;

    int32_t indice, valor;

    indice = 1;

    valor = frameCorrente->fields[indice];
    push(valor);

    atualizaPc();
}

void fload_2() {

	char* tipo = "F";
    tipoGlobal = tipo;

    int32_t indice, valor;

    indice = 2;

    valor = frameCorrente->fields[indice];
    push(valor);

    atualizaPc();
}

void fload_3() {

	char* tipo = "F";
    tipoGlobal = tipo;

    int32_t indice, valor;

    indice = 3;

    valor = frameCorrente->fields[indice];
    push(valor);

    atualizaPc();

}

void dload_0() {

	char* tipo = "D";
    tipoGlobal = tipo;

    int32_t indice;
    int32_t parte_alta, parte_baixa;

    indice = 0;

    parte_alta = frameCorrente->fields[indice + POS_ALTA];
    push(parte_alta);

    parte_baixa = frameCorrente->fields[indice + POS_BAIXA];
    push(parte_baixa);

    atualizaPc();

}

void dload_1() {
    int32_t indice;
    int32_t parte_alta, parte_baixa;

    char* tipo = "D";
    tipoGlobal = tipo;

    indice = 1;

    parte_alta = frameCorrente->fields[indice + POS_ALTA];
    push(parte_alta);

    parte_baixa = frameCorrente->fields[indice + POS_BAIXA];
    push(parte_baixa);

    atualizaPc();

}

void dload_2() {
    int32_t indice;
    int32_t parte_alta, parte_baixa;
    char* tipo = "D";
    tipoGlobal = tipo;

    indice = 2;

    parte_alta = frameCorrente->fields[indice + POS_ALTA];
    push(parte_alta);

    parte_baixa = frameCorrente->fields[indice + POS_BAIXA];
    push(parte_baixa);

    atualizaPc();

}

void dload_3() {
    int32_t indice;
    int32_t parte_alta, parte_baixa;

    char* tipo = "D";
    tipoGlobal = tipo;

    indice = 3;

    parte_alta = frameCorrente->fields[indice + POS_ALTA];
    push(parte_alta);

    parte_baixa = frameCorrente->fields[indice + POS_BAIXA];
    push(parte_baixa);

    atualizaPc();
}

void aload_0() {
	push(frameCorrente->fields[0]);
	atualizaPc();
}

void aload_1() {
    int32_t indice, valor;

    indice = 1;

    valor = frameCorrente->fields[indice];
    push(valor);
    atualizaPc();
}

void aload_2() {
    int32_t indice, valor;

    indice = 2;

    valor = frameCorrente->fields[indice];
    push(valor);

   atualizaPc();
}

void aload_3() {
    int32_t indice, valor;

    indice = 3;

    valor = frameCorrente->fields[indice];
    push(valor);

    atualizaPc();

}

void iaload() {
	int32_t* referencia;
	int32_t indice = popOp();
	referencia = (int32_t*)popOp();
	push(referencia[indice]);

	atualizaPc();
}

void laload() {
	static int16_t countPos = 0;
	char* tipo = "L";
    tipoGlobal = tipo;
	int32_t indice = popOp();
	int32_t* referencia;
	referencia = (int32_t*)popOp();

	push(referencia[countPos + indice+1]);
	push(referencia[countPos + indice]);
	countPos += 2;
	atualizaPc();
}

void faload() {
	char* tipo = "F";
    tipoGlobal = tipo;
	int32_t* referencia;
	int32_t indice = popOp();
	referencia = (int32_t*)popOp();
	int32_t valPilha;
	memcpy(&valPilha, &((float *)referencia)[indice], sizeof(int32_t));
	push(valPilha);

	atualizaPc();
}

void daload() {
	static int16_t countPos = 0;
	char* tipo = "D";
    tipoGlobal = tipo;
	int32_t indice = popOp();
	int32_t* referencia;
	referencia = (int32_t*)popOp();

	push(referencia[countPos + indice+1]);
	push(referencia[countPos + indice]);
	countPos += 2;
	atualizaPc();
}

void aaload() {
	int32_t* referencia;
	int32_t indice = popOp();
	referencia = (int32_t*)popOp();
	push(referencia[indice]);

	atualizaPc();
}

void baload() {
	int32_t* referencia;
	int32_t indice = popOp();
	referencia = (int32_t*)popOp();
	int8_t* binary = (int8_t*)referencia[indice];
	push((int32_t)binary);

	atualizaPc();
}

void caload() {
	char* tipo = "C";
    tipoGlobal = tipo;
	int32_t* referencia;
	int32_t indice = popOp();
	referencia = (int32_t*)popOp();
	int16_t* binary = (int16_t*)referencia[indice];
	push((int32_t)binary);

	atualizaPc();
}

void saload() {
	int32_t* referencia;
	int32_t indice = popOp();
	referencia = (int32_t*)popOp();
	int16_t* binary = (int16_t*)referencia[indice];
	push((int32_t)binary);

	atualizaPc();
}

/*** STORES ***/

void istore() {

    int32_t indice;
    int32_t valor;

    indice = frameCorrente->code[frameCorrente->pc + 1];

    valor = popOp();

    frameCorrente->fields[indice] = valor;

    atualizaPc();
}

void lstore() {

    int32_t indice;
    int32_t parte_alta, parte_baixa;

    indice = frameCorrente->code[frameCorrente->pc + 1];

    parte_baixa = popOp();

    parte_alta = popOp();

    frameCorrente->fields[indice + POS_ALTA] = parte_alta;
    frameCorrente->fields[indice + POS_BAIXA] = parte_baixa;

    atualizaPc();
}

void fstore() {

    int32_t indice;
    int32_t valor;

    indice = frameCorrente->code[frameCorrente->pc + 1];

    valor = popOp();

    frameCorrente->fields[indice] = valor;

    atualizaPc();
}

void dstore() {
    int32_t indice;
    int32_t parte_alta, parte_baixa;

    indice = frameCorrente->code[frameCorrente->pc + 1];

    parte_baixa = popOp();

    parte_alta = popOp();

    frameCorrente->fields[indice + POS_ALTA] = parte_alta;
    frameCorrente->fields[indice + POS_BAIXA] = parte_baixa;

    atualizaPc();
}

void astore() {

    int32_t indice;
    int32_t valor;

    indice = frameCorrente->code[frameCorrente->pc + 1];

    valor = popOp();

    frameCorrente->fields[indice] = valor;

    atualizaPc();
}

void istore_0() {

    int32_t indice;
    int32_t valor;

    indice = 0;

    valor = popOp();

    frameCorrente->fields[indice] = valor;

    atualizaPc();
}

void istore_1() {
    uint32_t valor;

    valor = popOp();

    frameCorrente->fields[1] = valor;

    atualizaPc();
}

void istore_2() {
    int32_t indice;
    int32_t valor;

    indice = 2;

    valor = popOp();

    frameCorrente->fields[indice] = valor;

    atualizaPc();
}

void istore_3() {
    int32_t indice;
    int32_t valor;

    indice = 3;

    valor = popOp();

    frameCorrente->fields[indice] = valor;

    atualizaPc();
}

void lstore_0() {

    int32_t indice;
    int32_t parte_alta, parte_baixa;

    indice = 0;

    parte_baixa = popOp();

    parte_alta = popOp();

    frameCorrente->fields[indice + POS_ALTA] = parte_alta;
    frameCorrente->fields[indice + POS_BAIXA] = parte_baixa;

    atualizaPc();
}

void lstore_1() {
    int32_t indice;
    int32_t parte_alta, parte_baixa;

    indice = 1;

    parte_baixa = popOp();

    parte_alta = popOp();

    frameCorrente->fields[indice + POS_ALTA] = parte_alta;
    frameCorrente->fields[indice + POS_BAIXA] = parte_baixa;

    atualizaPc();
}

void lstore_2() {
    int32_t indice;
    int32_t parte_alta, parte_baixa;

    indice = 2;

    parte_baixa = popOp();

    parte_alta = popOp();

    frameCorrente->fields[indice + POS_ALTA] = parte_alta;
    frameCorrente->fields[indice + POS_BAIXA] = parte_baixa;

    atualizaPc();
}

void lstore_3() {
    int32_t indice;
    int32_t parte_alta, parte_baixa;

    indice = 3;

    parte_baixa = popOp();

    parte_alta = popOp();

    frameCorrente->fields[indice + POS_ALTA] = parte_alta;
    frameCorrente->fields[indice + POS_BAIXA] = parte_baixa;

    atualizaPc();
}

void fstore_0() {
    int32_t indice;
    int32_t valor;

    indice = 0;

    valor = popOp();

    frameCorrente->fields[indice] = valor;

    atualizaPc();

}

void fstore_1() {
    int32_t indice;
    int32_t valor;

    indice = 1;

    valor = popOp();

    frameCorrente->fields[indice] = valor;

    atualizaPc();
}

void fstore_2() {
    int32_t indice;
    int32_t valor;

    indice = 2;

    valor = popOp();

    frameCorrente->fields[indice] = valor;

    atualizaPc();

}

void fstore_3() {
    int32_t indice;
    int32_t valor;

    indice = 3;

    valor = popOp();

    frameCorrente->fields[indice] = valor;

    atualizaPc();

}

void dstore_0() {
    int32_t indice;
    int32_t parte_alta, parte_baixa;

    indice = 0;

    parte_baixa = popOp();

    parte_alta = popOp();

    frameCorrente->fields[indice + POS_ALTA] = parte_alta;
    frameCorrente->fields[indice + POS_BAIXA] = parte_baixa;

    atualizaPc();
}

void dstore_1() {
    int32_t indice;
    int32_t parte_alta, parte_baixa;

    indice = 1;

    parte_baixa = popOp();

    parte_alta = popOp();

    frameCorrente->fields[indice + POS_ALTA] = parte_alta;
    frameCorrente->fields[indice + POS_BAIXA] = parte_baixa;

    atualizaPc();
}

void dstore_2() {
    int32_t indice;
    int32_t parte_alta, parte_baixa;

    indice = 2;

    parte_baixa = popOp();

    parte_alta = popOp();

    frameCorrente->fields[indice + POS_ALTA] = parte_alta;
    frameCorrente->fields[indice + POS_BAIXA] = parte_baixa;

    atualizaPc();

}

void dstore_3() {
    int32_t indice;
    int32_t parte_alta, parte_baixa;

    indice = 3;

    parte_baixa = popOp();

    parte_alta = popOp();

    frameCorrente->fields[indice + POS_ALTA] = parte_alta;
    frameCorrente->fields[indice + POS_BAIXA] = parte_baixa;

    atualizaPc();
}

void astore_0() {
    int32_t indice;
    int32_t valor;

    indice = 0;

    valor = popOp();

    frameCorrente->fields[indice] = valor;

    atualizaPc();
}

void astore_1() {
    int32_t indice;
    int32_t valor;

    indice = 1;

    valor = popOp();
    frameCorrente->fields[indice] = valor;

    atualizaPc();

}

void astore_2() {
    int32_t indice;
    int32_t valor;

    indice = 2;

    valor = popOp();
    frameCorrente->fields[indice] = valor;

    atualizaPc();

}

void astore_3() {
    int32_t indice;
    int32_t valor;

    indice = 3;

    valor = popOp();

    frameCorrente->fields[indice] = valor;

    atualizaPc();

}

void iastore() {
	int32_t* ref;
	int32_t indice,valor;
	valor = popOp();
	indice = popOp();
	ref = (int32_t*)popOp();
	ref[indice] = valor;

	atualizaPc();
}

void lastore() {
	static int16_t countPos = 0;
	int32_t alta,baixa;

	baixa = popOp();
	alta = popOp();

	int32_t indice = popOp();

	int32_t* referencia;
	referencia = (int32_t*) popOp();

	referencia[countPos + indice] = baixa;
	referencia[countPos + indice + 1] = alta;
	countPos += 2;
	atualizaPc();
}

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

void dastore() {
	static int16_t countPos = 0;
	int32_t alta,baixa;

	baixa = popOp();
	alta = popOp();

	int32_t indice = popOp();

	int32_t* referencia;
	referencia = (int32_t*) popOp();

	referencia[countPos + indice] = baixa;
	referencia[countPos + indice + 1] = alta;
	countPos += 2;
	atualizaPc();
}

void aastore() {
	int32_t* ref;
	int32_t indice,valor;
	valor = popOp();
	indice = popOp();
	ref = (int32_t*)popOp();
	ref[indice] = valor;

	atualizaPc();
}

void bastore() {
	int32_t* ref;
	int32_t indice,valor;
	valor = popOp();
	indice = popOp();
	ref = (int32_t*)popOp();
	ref[indice] = (int8_t)valor;

	atualizaPc();
}

void castore() {
	int32_t* ref;
	int32_t indice,valor;
	valor = popOp();
	indice = popOp();
	ref = (int32_t*)popOp();
	ref[indice] = (int16_t)valor;

	atualizaPc();
}

void sastore() {
	int32_t* ref;
	int32_t indice,valor;
	valor = popOp();
	indice = popOp();
	ref = (int32_t*)popOp();
	ref[indice] = (int16_t)valor;

	atualizaPc();
}

void pop() {
	popOp();

	atualizaPc();
}

void pop2() {
	popOp();
	popOp();

	atualizaPc();
}

void dup() {
	int32_t retPilha;

	retPilha = popOp();

	push(retPilha);
	push(retPilha);
	atualizaPc();
}

void dup_x1() {
	int32_t aux1, aux2;

	aux1 = popOp();

	aux2 = popOp();

	push(aux1);

	push(aux2);

	push(aux1);

	atualizaPc();
}

void dup_x2() {


	int32_t aux1, aux2, aux3;

	aux1 = popOp();

	aux2 = popOp();

	aux3 = popOp();

	push(aux1);
	push(aux3);
	push(aux2);
	push(aux1);

	atualizaPc();
}

void dup2() {

	int32_t aux1, aux2, aux3;

	aux1 = popOp();

	aux2 = popOp();

	push(aux2);
	push(aux1);
	push(aux2);
	push(aux1);

	atualizaPc();
}

void dup2_x1() {

	int32_t aux1, aux2, aux3;

	aux1 = popOp();

	aux2 = popOp();

	aux3 = popOp();

	push(aux2);
	push(aux1);
	push(aux3);
	push(aux2);
	push(aux1);

	atualizaPc();

}

void dup2_x2() {

	int32_t aux1, aux2, aux3, aux4;

	aux1 = popOp();

	aux2 = popOp();

	aux3 = popOp();

	aux4 = popOp();

	push(aux2);
	push(aux1);
	push(aux4);
	push(aux3);
	push(aux2);
	push(aux1);

	atualizaPc();
}

void swap() {
	int32_t val1,val2;

	val1 = popOp();
	val2 = popOp();

	push(val1);
	push(val2);

	atualizaPc();
}

void iadd() {
	int32_t v1,v2;
	v2 = popOp();
	v1 = popOp();

	push(v1+v2);

	atualizaPc();
}

void ladd() {
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

	int64_t resultado = lVal1 + lVal;


	alta = resultado >> 32;
	baixa = resultado & 0xffffffff;

	push(alta);
	push(baixa);

	atualizaPc();
}

void fadd() {
	float fVal1,fVal2;

	int32_t aux1 = popOp();
	int32_t aux2 = popOp();

	memcpy(&fVal1, &aux1, sizeof(int32_t));
	memcpy(&fVal2, &aux2, sizeof(int32_t));

	float resultado = fVal1 + fVal2;

	int32_t retPilha;
	memcpy(&retPilha, &resultado, sizeof(int32_t));

	push(retPilha);

	atualizaPc();

}

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

	push(alta);
	push(baixa);

	atualizaPc();
}

void isub() {
	int32_t v1,v2;
	v2 = popOp();
	v1 = popOp();

	push(v1-v2);

	atualizaPc();
}

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

	push(alta);
	push(baixa);

	atualizaPc();
}

void fsub() {
	float fVal1,fVal2;

	int32_t aux2 = popOp();
	int32_t aux1 = popOp();

	memcpy(&fVal1, &aux1, sizeof(int32_t));
	memcpy(&fVal2, &aux2, sizeof(int32_t));

	float resultado = fVal1 - fVal2;

	int32_t retPilha;
	memcpy(&retPilha, &resultado, sizeof(int32_t));

	push(retPilha);

	atualizaPc();
}

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

	push(alta);
	push(baixa);

	atualizaPc();
}

void imul() {
	int32_t v1 = popOp();
	int32_t v2 = popOp();

	push((int32_t)(v1 * v2));

	atualizaPc();
}

void lmul() {

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

	int64_t resultado = lVal1 * lVal;

	alta = resultado >> 32;
	baixa = resultado & 0xffffffff;

	push(alta);
	push(baixa);

	atualizaPc();
}

void fmul() {
	float fVal1,fVal2;

	int32_t aux1 = popOp();
	int32_t aux2 = popOp();

	memcpy(&fVal1, &aux1, sizeof(int32_t));
	memcpy(&fVal2, &aux2, sizeof(int32_t));

	float resultado = fVal1 * fVal2;

	int32_t retPilha;
	memcpy(&retPilha, &resultado, sizeof(int32_t));

	push(retPilha);

	atualizaPc();
}

void dmul() {
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

	double doubleMultiplicado = valorDouble1 * valorDouble2;

	int64_t valorPilha;
	memcpy(&valorPilha, &doubleMultiplicado, sizeof(int64_t));

	alta = valorPilha >> 32;
	baixa = valorPilha & 0xffffffff;

	push(alta);
	push(baixa);

	atualizaPc();
}

void idiv() {
	int32_t v2 = popOp();
	int32_t v1 = popOp();

	push((int32_t)(v1 / v2));

	atualizaPc();
}

void ins_ldiv() {
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

	int64_t resultado = lVal1 / lVal;

	alta = resultado >> 32;
	baixa = resultado & 0xffffffff;

	push(alta);
	push(baixa);

	atualizaPc();
}

void fdiv() {
	float fVal1,fVal2;

	int32_t aux2 = popOp();
	int32_t aux1 = popOp();

	memcpy(&fVal1, &aux1, sizeof(int32_t));
	memcpy(&fVal2, &aux2, sizeof(int32_t));

	float resultado = fVal1 / fVal2;

	int32_t retPilha;
	memcpy(&retPilha, &resultado, sizeof(int32_t));

	push(retPilha);

	atualizaPc();
}

void ddiv() {
	int32_t alta,baixa,alta1,baixa1;

	baixa1 = popOp();
	alta1 = popOp();

	baixa = popOp();
	alta = popOp();

	int64_t dVal = alta1;
	dVal <<= 32;
	dVal = dVal + baixa1;

	double v1;
	memcpy(&v1, &dVal, sizeof(double));

	dVal = alta;
	dVal <<= 32;
	dVal = dVal + baixa;

	double v2;
	memcpy(&v2, &dVal, sizeof(double));

	double resultado = v2 / v1;

	int64_t pilhaVal;
	memcpy(&pilhaVal, &resultado, sizeof(int64_t));

	alta = pilhaVal >> 32;
	baixa = pilhaVal & 0xffffffff;

	push(alta);
	push(baixa);

	atualizaPc();
}

void irem() {
	int32_t v2 = popOp();
	int32_t v1 = popOp();

	push((int32_t)(v1 % v2));

	atualizaPc();
}

void lrem() {
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

	int64_t resultado = lVal1 % lVal;

	alta = resultado >> 32;
	baixa = resultado & 0xffffffff;

	push(alta);
	push(baixa);

	atualizaPc();
}

void frem() {
	float fVal1,fVal2;

	int32_t aux2 = popOp();
	int32_t aux1 = popOp();

	memcpy(&fVal1, &aux1, sizeof(int32_t));
	memcpy(&fVal2, &aux2, sizeof(int32_t));

	float resultado = fmodf(fVal1,fVal2);

	int32_t retPilha;
	memcpy(&retPilha, &resultado, sizeof(int32_t));

	push(retPilha);

	atualizaPc();
}

void _drem() {
	int32_t alta,baixa,alta1,baixa1;

	baixa1 = popOp();
	alta1 = popOp();

	baixa = popOp();
	alta = popOp();

	int64_t dVal = alta1;
	dVal <<= 32;
	dVal = dVal + baixa1;

	double v1;
	memcpy(&v1, &dVal, sizeof(double));

	dVal = alta;
	dVal <<= 32;
	dVal = dVal + baixa;

	double v2;
	memcpy(&v2, &dVal, sizeof(double));

	double resultado = fmod(v2,v1);

	int64_t pilhaVal;
	memcpy(&pilhaVal, &resultado, sizeof(int64_t));

	alta = pilhaVal >> 32;
	baixa = pilhaVal & 0xffffffff;

	push(alta);
	push(baixa);

	atualizaPc();
}

void ineg() {
	int32_t retPilha = popOp();
	int32_t aux = -retPilha;

	push(aux);

	atualizaPc();
}

void lneg() {
	int32_t baixa,alta;

	baixa = popOp();
	alta = popOp();

	int64_t lVal = alta;
	lVal <<= 32;
	lVal = lVal | baixa;

	lVal = - lVal;

	alta = lVal >> 32;
	baixa = lVal & 0xffffffff;

	push(alta);
	push(baixa);

	atualizaPc();
    foi_lneg = true;
}

void fneg() {
	float fVal;
	int32_t retPilha = popOp();

	memcpy(&fVal,&retPilha,sizeof(float));

	fVal = - fVal;

	memcpy(&retPilha,&fVal,sizeof(int32_t));

	push(retPilha);

	atualizaPc();
}

void dneg() {

	int32_t baixa,alta;

	baixa = popOp();
	alta = popOp();

	int64_t dVal = alta;
	dVal <<= 32;
	dVal = dVal + baixa;

	double valorDouble1;
	memcpy(&valorDouble1, &dVal, sizeof(int64_t));

	valorDouble1 = - valorDouble1;

	memcpy(&dVal, &valorDouble1, sizeof(int64_t));

	alta = dVal >> 32;
	baixa = dVal & 0xffffffff;

	push(alta);
	push(baixa);

	atualizaPc();
}

void ishl() {
	int32_t shift = popOp();
	shift = shift & 0x1f;

	int32_t sVal = popOp();
	sVal = sVal << shift;
	push(sVal);

	atualizaPc();
}

void lshl() {

	int32_t shift = popOp();
	shift = shift & 0x3f;

	int32_t baixa,alta;
	baixa = popOp();
	alta = popOp();

	int64_t lVal = alta;
	lVal <<= 32;
	lVal = lVal + baixa;

	lVal = lVal << shift;

	alta = lVal >> 32;
	baixa = lVal & 0xffffffff;

	push(alta);
	push(baixa);

	atualizaPc();
}

void ishr() {
	int32_t shift = popOp();
	shift = shift & 0x1f;

	int32_t sVal = popOp();

	int32_t i = 0;
	while(i < shift) {
		sVal = sVal / 2;
		i += 1;
	}

	push(sVal);

	atualizaPc();
}

void lshr() {

    int32_t v2 = popOp();

	int32_t baixa,alta;
	baixa = popOp();
	alta = popOp();
	int64_t lVal = alta;
	lVal <<= 32;
	lVal = lVal + baixa;

    lVal = lVal << v2;

	alta = lVal >> 32;
	baixa = lVal & 0xffffffff;
	push(alta);
	push(baixa);

	atualizaPc();
}

void iushr() {
	int32_t shift = popOp();
	shift = shift & 0x1f;

	int32_t sVal = popOp();
	sVal = sVal >> shift;
	push(sVal);

	atualizaPc();
}

void lushr() {
	int32_t shift = popOp();
	shift = shift & 0x3f;

	int32_t baixa,alta;
	baixa = popOp();
	alta = popOp();

	int64_t lVal = alta;
	lVal <<= 32;
	lVal = lVal + baixa;

	lVal = lVal >> shift;

	alta = lVal >> 32;
	baixa = lVal & 0xffffffff;

	push(alta);
	push(baixa);

	atualizaPc();
}

void iand() {
	int32_t pop1 = popOp();

	int32_t pop2 = popOp();

	int32_t aux = pop1 & pop2;

	push(aux);

	frameCorrente->pc++;
}

void land() {
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

	int64_t resultado = lVal1 & lVal;

	alta = resultado >> 32;
	baixa = resultado & 0xffffffff;

	push(alta);
	push(baixa);

	atualizaPc();
}

void ior() {
	int32_t pop1 = popOp();

	int32_t pop2 = popOp();

	int32_t aux = pop1 | pop2;

	push(aux);

	frameCorrente->pc++;
}

void lor() {
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

	int64_t resultado = lVal1 | lVal;

	alta = resultado >> 32;
	baixa = resultado & 0xffffffff;

	push(alta);
	push(baixa);

	inicializaDecodificador(dec);
	int numBytes = dec[frameCorrente->code[frameCorrente->pc]].bytes;

	atualizaPc();
}

void ixor() {
	int32_t pop1 = popOp();

	int32_t pop2 = popOp();

	int32_t aux = pop1 ^ pop2;

	push(aux);

	frameCorrente->pc++;
}

void lxor() {
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

	int64_t resultado = lVal1 ^ lVal;

	alta = resultado >> 32;
	baixa = resultado & 0xffffffff;

	push(alta);
	push(baixa);

	atualizaPc();
}

void iinc() {
	int8_t field = frameCorrente->code[frameCorrente->pc + 1];

	int32_t value = frameCorrente->fields[field];

	int8_t constant = frameCorrente->code[frameCorrente->pc + 2];

	int8_t sumVal = (int8_t) value;
	sumVal = sumVal + constant;

	frameCorrente->fields[field] = (int32_t) sumVal;

	atualizaPc();
}

void i2l() {
    char* tipo = "L";
    tipoGlobal = tipo;
    int32_t alta, baixa;

    int32_t val = popOp();

    int64_t long_num = (int64_t) val;
	alta = long_num >> 32;
	baixa = long_num & 0xffffffff;

    push(alta);
    push(baixa);

    atualizaPc();
}

void i2f() {
    char* tipo = "F";
    tipoGlobal = tipo;

	int32_t val = (int32_t) popOp();

	float valF = (float) val;

	int32_t valPilha;
	memcpy(&valPilha, &valF, sizeof(int32_t));

	push(valPilha);

	atualizaPc();
}

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

	push(alta);
	push(baixa);

	atualizaPc();
}

void l2i() {
    char* tipo = "I";
    tipoGlobal = tipo;
	int32_t alta,baixa;

	baixa = popOp();
	alta = popOp();

	push(baixa);
	atualizaPc();
}

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

	push(valPilha);

	atualizaPc();
}

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

	push(alta);
	push(baixa);

	atualizaPc();
}

void f2i() {
    char* tipo = "I";
    tipoGlobal = tipo;

	int32_t retPilha = popOp();

	float fVal;
	memcpy(&fVal, &retPilha, sizeof(int32_t));
	push((int32_t)fVal);
	atualizaPc();
}

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

	push(alta);
	push(baixa);

	atualizaPc();
}

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

	push(alta);
	push(baixa);

	atualizaPc();
}

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

	push((int32_t)v1);
	atualizaPc();
}

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

    push(alta);
    push(baixa);
	atualizaPc();
}

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

	push(pilhaVal);

	atualizaPc();
}

void i2b() {
	int32_t valPilha = popOp();
	int8_t bVal = (int8_t) valPilha;
	push((int32_t) bVal);
	atualizaPc();
}

void i2c() {
    char* tipo = "C";
    tipoGlobal = tipo;

	int32_t valPilha = popOp();
	int16_t cVal = (int16_t) valPilha;
	push((int32_t) cVal);
	atualizaPc();
}

void i2s() {
	int32_t valPilha = popOp();
	int16_t cVal = (int16_t) valPilha;
	push((int32_t) cVal);
	atualizaPc();
}

void lcmp() {
	int32_t baixa,alta;
	baixa = popOp();
	alta = popOp();

	int64_t lVal = alta;
	lVal <<= 32;
	lVal = lVal + baixa;

	baixa = popOp();
	alta = popOp();

	int64_t lVal2 = alta;
	lVal2 <<= 32;
	lVal2 = lVal2 + baixa;

	if(lVal2 == lVal) {
		push((int32_t)0);
	} else if(lVal2 > lVal) {
		push((int32_t)1);
	} else if(lVal2 < lVal) {
		push((int32_t)-1);
	}

	atualizaPc();
}

void fcmpl() {
	float val1,val2;
	int32_t retPilha;

	retPilha = popOp();
	memcpy(&val2,&retPilha,sizeof(float));

	retPilha = popOp();
	memcpy(&val1,&retPilha,sizeof(float));

	if(val1 == val2) {
		push((int32_t)0);
	} else if(val1 > val2) {
		push((int32_t)1);
	} else if(val1 < val2) {
		push((int32_t)-1);
	} else {
		printf("NaN!!\n");
		push((int32_t)-1);
	}
	atualizaPc();
}

void fcmpg() {
	float val1,val2;
	int32_t retPilha;

	retPilha = popOp();
	memcpy(&val2,&retPilha,sizeof(float));

	retPilha = popOp();
	memcpy(&val1,&retPilha,sizeof(float));

	if(val1 == val2) {
		push((int32_t)0);
	} else if(val1 > val2) {
		push((int32_t)1);
	} else if(val1 < val2) {
		push((int32_t)-1);
	} else {
		printf("NaN!!\n");
		push((int32_t)1);
	}
	atualizaPc();
}

void dcmpl() {
	int32_t baixa,alta;
	baixa = popOp();
	alta = popOp();

	int64_t dVal = alta;
	dVal <<= 32;
	dVal = dVal + baixa;

	double doubleCmpl;
	memcpy(&doubleCmpl, &dVal, sizeof(double));

	baixa = popOp();
	alta = popOp();

	int64_t dVal2 = alta;
	dVal2 <<= 32;
	dVal2 = dVal2 + baixa;

	double doubleCmpl2;
	memcpy(&doubleCmpl2, &dVal2, sizeof(double));

	if(doubleCmpl2 > doubleCmpl) {
		push((int32_t)1);
	} else if(doubleCmpl2 == doubleCmpl) {
		push((int32_t)0);
	} else if(doubleCmpl2 < doubleCmpl) {
		push((int32_t)-1);
	} else {
		printf("NaN!\n");
		push((int32_t) -1);
	}
	atualizaPc();
}

void dcmpg() {
	int32_t baixa,alta;
	baixa = popOp();
	alta = popOp();

	int64_t dVal = alta;
	dVal <<= 32;
	dVal = dVal + baixa;

	double doubleCmpl;
	memcpy(&doubleCmpl, &dVal, sizeof(double));

	baixa = popOp();
	alta = popOp();

	int64_t dVal2 = alta;
	dVal2 <<= 32;
	dVal2 = dVal2 + baixa;

	double doubleCmpl2;
	memcpy(&doubleCmpl2, &dVal2, sizeof(double));

	if(doubleCmpl2 > doubleCmpl) {
		push((int32_t)1);
	} else if(doubleCmpl2 == doubleCmpl) {
		push((int32_t)0);
	} else if(doubleCmpl2 < doubleCmpl) {
		push((int32_t)-1);
	} else {
		printf("NaN!\n");
		push((int32_t) 1);
	}
	atualizaPc();
}

void ifeq() {
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

void ifne() {
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

void iflt() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	int32_t retPilha = popOp();

	if(retPilha < 0) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

void ifge() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	int32_t retPilha = popOp();

	if(retPilha >= 0) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

void ifgt() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	int32_t retPilha = popOp();

	if(retPilha > 0) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

void ifle() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	int32_t retPilha = popOp();

	if(retPilha <= 0) {
		frameCorrente->pc += offset;
	} else {
		frameCorrente->pc += 3;
	}
}

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

void ins_goto() {
	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	frameCorrente->pc += offset;
}

void jsr() {
	push(frameCorrente->pc+3);

	uint8_t offset1,offset2;
	int16_t offset;

	offset1 = frameCorrente->code[frameCorrente->pc + 1];
	offset2 = frameCorrente->code[frameCorrente->pc + 2];
	offset = offset1;
	offset <<= 8;
	offset |= offset2;

	frameCorrente->pc += offset;
}

void ret() {
	return;
}

void tableswitch() {
    uint32_t bytes_preench;
    int32_t indice;
    int32_t default_v, low, high, npairs;
    uint32_t pc_novo, pc_aux;
    int32_t qtd_offset, offset, posicao;
    uint32_t temp;

    bool definido = false;

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
        definido = true;
        pc_novo = frameCorrente->pc + default_v;
    }

    high = 0;
    for (int l = 0; l < 4; l++) {
        high = (high << 8) + frameCorrente->code[pc_aux];
        pc_aux++;
    }

    if (indice > high && !definido) {
        definido = true;
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
            definido = true;

            break;
        } else {
            for (int i = 0; i < 4; i++) {
                pc_aux++;
            }
        }
    }

    frameCorrente->pc = pc_novo;
}

void lookupswitch() {
    uint32_t pc_aux, pc_novo;
    uint32_t bytes_preench;
    uint32_t offset;
    int32_t default_v, npairs;
    int32_t match;
    int32_t key;

    bool definido = false;

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

            definido = true;
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
void ireturn() {
  retorno = popOp();
	flagRet = 1;
  frameCorrente->pc = frameCorrente->codeLength + 1;
}

void lreturn() {
	int32_t alta,baixa;

	baixa = popOp();
	alta = popOp();

	flagRet = 2;

	retAlta = alta;
	retBaixa = baixa;

  frameCorrente->pc = frameCorrente->codeLength + 1;
}

void freturn() {
	retorno = popOp();
	flagRet = 1;
  frameCorrente->pc = frameCorrente->codeLength + 1;
}

void dreturn() {
	int32_t alta,baixa;

	baixa = popOp();
	alta = popOp();

	flagRet = 2;

	retAlta = alta;
	retBaixa = baixa;

  frameCorrente->pc = frameCorrente->codeLength + 1;
}

void areturn() {
	retorno = popOp();
	flagRet = 1;

    frameCorrente->pc = frameCorrente->codeLength + 1;
}

void ins_return() {
	retorno = 0;
	flagRet = 0;

	atualizaPc();
}

void getstatic() {
  frameCorrente->pilhaOp->depth += 1;
	atualizaPc();
}

void putstatic() {

}

void getfield() {
	uint32_t indice = frameCorrente->code[frameCorrente->pc + 2];
	int32_t indiceClasse = frameCorrente->constantPool[indice-1].info.Fieldref.classIndex;
	char* nomeClasse = retornaNome(frameCorrente->classe, frameCorrente->constantPool[indiceClasse-1].info.Class.nameIndex);
	uint16_t nomeTipoIndice = frameCorrente->constantPool[indice-1].info.Fieldref.nameAndTypeIndex;
	char* nome = retornaNome(frameCorrente->classe, frameCorrente->constantPool[nomeTipoIndice-1].info.NameAndType.nameIndex);
	char* tipo = retornaNome(frameCorrente->classe, frameCorrente->constantPool[nomeTipoIndice-1].info.NameAndType.descriptorIndex);
	tipoGlobal = tipo;

 	if((strcmp(tipo, "Ljava/util/Scanner;") == 0)) {
 		atualizaPc();
		return;
 	}

 	Objeto* obj = (Objeto*) popOp();

 	int32_t indiceField = buscaCampo(nomeClasse, nome, tipo);
 	uint32_t indiceNome = frameCorrente->classe->fields[indiceField].nameIndex;

 	if(tipo[0] == 'J' || tipo[0] == 'D') {
 		int32_t i;
		for(i = 0;obj->indiceCampos[i] != indiceNome; i++);

		int32_t baixa = obj->campos[i];
		int32_t alta = obj->campos[i+1];

		push(alta);
		push(baixa);
		atualizaPc();
 	} else {

	 	int32_t i;
		for(i = 0;obj->indiceCampos[i] != indiceNome; i++);
	 	uint32_t val = obj->campos[i];

	 	push(val);
		atualizaPc();
	}
}

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

void invokevirtual() {
	MethodInfo* metodoInvocado;
  char* nomeClasse;
  char* nomeMetodo;
  char* descricaoMetodo;
  uint16_t nomeMetodoAux;
  uint16_t descricaoMetodoAux;
  uint16_t nomeTipoAux;
  uint16_t stringAux;
  int32_t resultado;
  int32_t resultado2;
  int32_t resultado_string;
  int32_t classeIndice;
  uint8_t* string = NULL;
  static int8_t flagAppend = 0;

  uint32_t pcAux = frameCorrente->code[frameCorrente->pc + 2];

  classeIndice = frameCorrente->constantPool[pcAux - 1].info.Methodref.classIndex;
  nomeClasse = retornaNome(frameCorrente->classe, frameCorrente->constantPool[classeIndice - 1].info.Class.nameIndex);
  nomeTipoAux = frameCorrente->constantPool[pcAux - 1].info.Methodref.nameAndTypeIndex;
  nomeMetodoAux = frameCorrente->constantPool[nomeTipoAux - 1].info.NameAndType.nameIndex;

	descricaoMetodoAux = frameCorrente->constantPool[nomeTipoAux - 1].info.NameAndType.descriptorIndex;
  nomeMetodo = retornaNome(frameCorrente->classe, nomeMetodoAux);
  descricaoMetodo = retornaNome(frameCorrente->classe, descricaoMetodoAux);

  if((strcmp(nomeClasse, "java/lang/StringBuffer") == 0) && (strcmp(nomeMetodo,"append") == 0)) {
		flagAppend++;
	  foi_lneg = false;
		atualizaPc();
		return;
	}

	if((strcmp(nomeClasse, "java/lang/StringBuffer") == 0) && (strcmp(nomeMetodo,"toString") == 0)) {
		foi_lneg = false;
		atualizaPc();
		return;
	}

	if((strcmp(nomeClasse, "java/util/Scanner") == 0) && (strcmp(nomeMetodo,"next") == 0)) {
		int32_t aux;
		scanf("%d",&aux);
		push(aux);
		foi_lneg = false;
		atualizaPc();
		return;
	}

	if((strcmp(nomeClasse, "java/io/PrintStream") == 0) && (strcmp(nomeMetodo,"println") == 0)) {
        if (strcmp(descricaoMetodo, "()V") == 0) {
            printf("\n");
        } else if (flagAppend == 0) {
            resultado = popOp();
            if (tipoGlobal == NULL) {
                string = frameCorrente->constantPool[resultado].info.Utf8.bytes;
            }
        } else if(strcmp(tipoGlobal, "F") == 0) {
            float valDesemp;
            memcpy(&valDesemp, &resultado, sizeof(float));
            printf("%f\n",valDesemp);
        } else if(strcmp(tipoGlobal, "D") == 0) {
            resultado2 = pop_op();
            double resultado_double;
            int64_t temp;

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

            memcpy(&result, &long_num, sizeof(long));
            foi_lneg = false;
            if (!foi_lneg) {
                printf("%" PRId64 "\n", long_num);
            } else {
                printf("%" PRId64 "\n", result);
            }
        } else if (flagAppend == 2) {
            if(strcmp(tipoGlobal, "F") == 0) {
                resultado = popOp();
                resultado_string = popOp();

                string = frameCorrente->constantPool[resultado_string].info.Utf8.bytes;
                if (string != NULL) {
                    printf("%s",string);
                }
                float valDesemp;
                memcpy(&valDesemp,&resultado, sizeof(float));
                printf("%f\n",valDesemp);
            } else if(strcmp(tipoGlobal, "I") == 0) {
                resultado = popOp();
                resultado_string = popOp();

                string = frameCorrente->constantPool[resultado_string].info.Utf8.bytes;
                if (string != NULL) {
                    printf("%s",string);
                }
                printf("%d\n", resultado);
            } else if(strcmp(tipoGlobal, "D") == 0) {
                resultado = popOp();
                resultado2 = popOp();
                resultado_string = popOp();

            double resultado_double;
            int64_t temp;

            temp = resultado2;
            temp <<= 32;
            temp += resultado;

            if (string != NULL) {
                printf("%s",string);
            }

            memcpy(&resultado_double, &temp, sizeof(int64_t));
            printf("%lf\n", resultado_double);
        } else {
            printf("tipoGlobal ainda nao reconhecido");
            exit(1);
        }

        flagAppend = 0;
    } else {
    	return;
    }

        foi_lneg = false;
		atualizaPc();
		return;
	}

	classeIndice = carregaMemClass(nomeClasse);
	ClassFile* classe = buscaClasseIndice(classeIndice);

	metodoInvocado = buscaMetodo(frameCorrente->classe,classe,nomeTipoAux);
	if(metodoInvocado == NULL) {
		printf("Método não Encontrado!\n");
		exit(0);
	}

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

	foi_lneg = false;
	atualizaPc();
	return;
}

void invokespecial() {
	MethodInfo* metodoInvocado;

	uint32_t indice = frameCorrente->code[frameCorrente->pc + 2];

	uint32_t indiceClasse = (frameCorrente->constantPool[indice-1]).info.Methodref.classIndex;

	char* nomeClasse = retornaNome(frameCorrente->classe,(frameCorrente->constantPool[indiceClasse-1]).info.Class.nameIndex);

    if(strcmp("java/lang/Object",nomeClasse) == 0) {

		carregaMemClass(nomeClasse);

		atualizaPc();
		return;
	}

	if(strcmp("java/lang/StringBuffer",nomeClasse) == 0) {

		atualizaPc();
		return;
	}

	if(strcmp("java/util/Scanner",nomeClasse) == 0) {

		atualizaPc();
		return;
	}

	int32_t indexClasse = carregaMemClass(nomeClasse);

	ClassFile* classe = buscaClasseIndice(indexClasse);

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

void invokestatic() {

	MethodInfo* metodoInvocado;

    char* nomeMetodo;
    char* descricaoMetodo;
    uint16_t nomeMetodoAux, descricaoMetodoAux,nomeTipoAux,stringAux;

	uint32_t indice = frameCorrente->code[frameCorrente->pc + 2];

	uint32_t indiceClasse = (frameCorrente->constantPool[indice-1]).info.Methodref.classIndex;

	char* nomeClasse = retornaNome(frameCorrente->classe,(frameCorrente->constantPool[indiceClasse-1]).info.Class.nameIndex);

	nomeTipoAux = frameCorrente->constantPool[indice - 1].info.Methodref.nameAndTypeIndex;

    nomeMetodoAux = frameCorrente->constantPool[nomeTipoAux - 1].info.NameAndType.nameIndex;

	descricaoMetodoAux = frameCorrente->constantPool[nomeTipoAux - 1].info.NameAndType.descriptor_index;


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

	if((strcmp(nomeClasse, "java/lang/Integer") == 0) && (strcmp(nomeMetodo,"parseInt") == 0)) {

		int32_t retPilha = popOp();
		popOp();
		push(retPilha);
        atualizaPc();
        return;
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

			push(alta);
			push(baixa);

            atualizaPc();
            return;
		}
	}

	int32_t indexClasse = carregaMemClass(nomeClasse);

	ClassFile* classe = buscaClasseIndice(indexClasse);

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

void invokeinterface() {
	MethodInfo* metodoInvocado;

    char* nomeMetodo;
    char* descricaoMetodo;
    uint16_t nomeMetodoAux, descricaoMetodoAux,nomeTipoAux,stringAux;

	uint32_t indice = frameCorrente->code[frameCorrente->pc + 2];

	uint32_t indiceClasse = (frameCorrente->constantPool[indice-1]).info.Methodref.classIndex;

	char* nomeClasse = retornaNome(frameCorrente->classe,(frameCorrente->constantPool[indiceClasse-1]).info.Class.nameIndex);

	nomeTipoAux = frameCorrente->constantPool[indice - 1].info.Methodref.nameAndTypeIndex;

    nomeMetodoAux = frameCorrente->constantPool[nomeTipoAux - 1].info.NameAndType.nameIndex;

	descricaoMetodoAux = frameCorrente->constantPool[nomeTipoAux - 1].info.NameAndType.descriptor_index;

    nomeMetodo = retornaNome(frameCorrente->classe, nomeMetodoAux);

    descricaoMetodo = retornaNome(frameCorrente->classe, descricaoMetodoAux);

	int32_t indexClasse = carregaMemClass(nomeClasse);

	ClassFile* classe = buscaClasseIndice(indexClasse);

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

void ins_new() {
	uint32_t indice;
	int32_t aux;
	char* nomeClasse;
	ClassFile* classe;
	Objeto* Objeto;

	indice = frameCorrente->code[2+(frameCorrente->pc)];

	nomeClasse = retornaNome(frameCorrente->classe, frameCorrente->constantPool[indice-1].info.Class.nameIndex);

	if(strcmp("java/util/Scanner",nomeClasse) == 0) {
		naoEmpilhaFlag = 1;
		atualizaPc();
		return;
	}

	if(strcmp("java/lang/StringBuffer",nomeClasse) == 0) {
		naoEmpilhaFlag = 1;

		atualizaPc();
		return;
	}

	aux = carregaMemClass(nomeClasse);

	classe = buscaClasseIndice(aux);

	Objeto = criaObjeto(classe);

	if(Objeto == NULL) {
		printf("Objeto não foi corretamente alocado\n");
	}

	push((int32_t) Objeto);
	atualizaPc();
}

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
	arrayVetores = realloc (arrayVetores, sizeof(struct array)*qtdArrays);
	arrayVetores[qtdArrays-1].tamanho = tamanhoArray;
	arrayVetores[qtdArrays-1].referencia = (int32_t)vetor;
	arrayVetores[qtdArrays-1].tipo = tipoArray;

	push((int32_t)vetor);

    atualizaPc();
}

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
	arrayVetores = realloc (arrayVetores, sizeof(struct array)*qtdArrays);
	arrayVetores[qtdArrays-1].tamanho = tamanhoArray;
	arrayVetores[qtdArrays-1].referencia = (int32_t)vetor;
	arrayVetores[qtdArrays-1].tipo = tipoArray;

	push((int32_t)vetor);

    atualizaPc();
}

void arraylength() {
	int32_t arrayRef = popOp();
	int i = 0;
	while(i  < qtdArrays) {
		if(arrayVetores[i].referencia == arrayRef) {
			int32_t length = arrayVetores[i].tamanho;
			push(length);
			atualizaPc();
			return;
		}
		i++;
	}
	push(0);
	atualizaPc();
}

void checkcast() {
	int16_t indice;
	int8_t offset1,offset2;

	offset1 =  frameCorrente->code[(frameCorrente->pc)+1];
	offset2 =  frameCorrente->code[(frameCorrente->pc)+2];

	indice = (offset1 << 8) | offset2;

	Objeto* Objeto = (struct Objeto*) popOp();

	if(Objeto == NULL) {
		printf("Objeto nulo!\n");
	}

	char* nomeClasse = retornaNomeClasse(Objeto->classe);

	char* nomeIndice = retornaNome(frameCorrente->classe,indice);

	if(strcmp(nomeClasse,nomeIndice) == 0) {
		printf("Objeto é do tipo: %s\n",nomeIndice);
	}

	push((int32_t)Objeto);
	atualizaPc();
}

void instanceof() {
	int16_t indice;
	int8_t offset1,offset2;

	offset1 =  frameCorrente->code[(frameCorrente->pc)+1];
	offset2 =  frameCorrente->code[(frameCorrente->pc)+2];

	indice = (offset1 << 8) | offset2;

	Objeto* Objeto = (struct Objeto*) popOp();

	if(Objeto == NULL) {
		printf("Objeto nulo!\n");
		push(0);
	}

	char* nomeClasse = retornaNomeClasse(Objeto->classe);

	char* nomeIndice = retornaNome(frameCorrente->classe,indice);

	if(strcmp(nomeClasse,nomeIndice) == 0) {
		printf("Objeto é do tipo: %s\n",nomeIndice);
		push(1);
	}
	atualizaPc();
}

void wide() {

}

void multianewarray() {

}

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
	}else{
		frameCorrente->pc += 3;
	}
}

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

void goto_w() {
	int32_t deslocamento,offset1,offset2,offset3,offset4;

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

void jsr_w() {
	int32_t deslocamento,offset1,offset2,offset3,offset4;

	push(frameCorrente->code[frameCorrente->pc + 5]);

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
