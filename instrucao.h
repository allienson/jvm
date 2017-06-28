///
///		@file instrucao.h
///		@class instrucao.c "instrucao.c"
///
///		@date 28/06/17.
///		@copyright Copyright © 2017 GrupoSB. All rights reserved.
///
///		@authors
///		Allisson Barros			12/0055619\n
///		Daniel Luz				13/0007714\n
///		Luiz Fernando Vieira	13/0013757\n
///		Mariana Pannunzio		12/0018276\n
///		Mateus Denucci			12/0053080
///
///		@brief
///		Define, interpreta e executa as instruções do programa executado.
///

#ifndef INSTRUCAO_H
#define INSTRUCAO_H

#include <stdint.h>

#define FALSE 0
#define TRUE 1

void (*instrucao[256]) ();

char* tipoGlobal;

int flagLNEG;

void inicializaInstrucoes();
void atualizaPc();

void nop();
void aconst_null();
void iconst_m1();
void iconst_0();
void iconst_1();
void iconst_2();
void iconst_3();
void iconst_4();
void iconst_5();
void lconst_0();
void lconst_1();
void fconst_0();
void fconst_1();
void fconst_2();
void dconst_0();
void dconst_1();
void bipush();
void sipush();
void ldc();
void ldc_w();
void ldc2_w();
void iload();
void lload();
void fload();
void dload();
void aload();
void iload_0();
void iload_1();
void iload_2();
void iload_3();
void lload_0();
void lload_1();
void lload_2();
void lload_3();
void fload_0();
void fload_1();
void fload_2();
void fload_3();
void dload_0();
void dload_1();
void dload_2();
void dload_3();
void aload_0();
void aload_1();
void aload_2();
void aload_3();
void iaload();
void laload();
void faload();
void daload();
void aaload();
void baload();
void caload();
void saload();
void istore();
void lstore();
void fstore();
void dstore();
void astore();
void istore_0();
void istore_1();
void istore_2();
void istore_3();
void lstore_0();
void lstore_1();
void lstore_2();
void lstore_3();
void fstore_0();
void fstore_1();
void fstore_2();
void fstore_3();
void dstore_0();
void dstore_1();
void dstore_2();
void dstore_3();
void astore_0();
void astore_1();
void astore_2();
void astore_3();
void iastore();
void lastore();
void fastore();
void dastore();
void aastore();
void bastore();
void castore();
void sastore();
void pop();
void pop2();
void dup();
void dup_x1();
void dup_x2();
void dup2();
void dup2_x1();
void dup2_x2();
void swap();
void iadd();
void ladd();
void fadd();
void dadd();
void isub();
void lsub();
void fsub();
void dsub();
void imul();
void lmul();
void fmul();
void dmul();
void idiv();
void _ldiv();
void fdiv();
void ddiv();
void irem();
void lrem();
void frem();
void _drem();
void ineg();
void lneg();
void fneg();
void dneg();
void ishl();
void lshl();
void ishr();
void lshr();
void iushr();
void lushr();
void iand();
void land();
void ior();
void lor();
void ixor();
void lxor();
void iinc();
void i2l();
void i2f();
void i2d();
void l2i();
void l2f();
void l2d();
void f2i();
void f2l();
void f2d();
void d2i();
void d2l();
void d2f();
void i2b();
void i2c();
void i2s();
void lcmp();
void fcmpl();
void fcmpg();
void dcmpl();
void dcmpg();
void ifeq();
void ifne();
void iflt();
void ifge();
void ifgt();
void ifle();
void if_icmpeq();
void if_icmpne();
void if_icmplt();
void if_icmpge();
void if_icmpgt();
void if_icmple();
void if_acmpeq();
void if_acmpne();
void _goto();
void jsr();
void ret();
void tableswitch();
void lookupswitch();
void ireturn();
void lreturn();
void freturn();
void dreturn();
void areturn();
void _return();
void getstatic();
void putstatic();
void getfield();
void putfield();
void invokevirtual();
void invokespecial();
void invokestatic();
void invokeinterface();
void _new();
void newarray();
void anewarray();
void arraylength();
void checkcast();
void instanceof();
void wide();
void multianewarray();
void ifnull();
void ifnonnull();
void goto_w();
void jsr_w();

#endif
