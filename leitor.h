///
///		@file leitor.h
///		@class leitor.c "leitor.c"
///		@headerfile decodificador.h "decodificador.h"
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
///		Define as estruturas do Classfile e as funcoes que fazem
///   a leitura de um arquivo .class e populam as estruturas.
///

#ifndef LEITOR_h
#define LEITOR_h

#include <stdio.h>
#include <stdint.h>

#include "decodificador.h"

#define CONSTANT_Class 7
#define CONSTANT_Fieldref 9
#define CONSTANT_Methodref 10
#define CONSTANT_InterfaceMethodref 11
#define CONSTANT_String 8
#define CONSTANT_Integer 3
#define CONSTANT_Float 4
#define CONSTANT_Long 5
#define CONSTANT_Double 6
#define CONSTANT_NameAndType 12
#define CONSTANT_Utf8 1

#define NUM_INSTRUCAO 256

#define TABLESWITCH 170
#define LOOKUPSWITCH 171
#define WIDE 196
#define BIPUSH 16
#define SIPUSH 16
#define LDC 18
#define LDC_W 19
#define LDC2_W 20
#define ILOAD 21
#define FLOAD 23
#define ALOAD 25
#define LLOAD 22
#define DLOAD 24
#define ISTORE 54
#define FSTORE 56
#define ASTORE 58
#define LSTORE 55
#define DSTORE 57
#define IINC 132
#define IFEQ 153
#define IFNE 154
#define IFLT 155
#define IFGE 156
#define IFGT 157
#define IFLE 158
#define IF_ICMPEQ 159
#define IF_ICMPNE 160
#define IF_ICMPLT 161
#define IF_ICMPGE 162
#define IF_ICMPGT 163
#define IF_ICMPLE 164
#define IF_ACMPEQ 165
#define IF_ACMPNE 166
#define GOTO 167
#define RET 169
#define NEWARRAY 188
#define IFNULL 198
#define IFNONNULL 199


typedef struct CpInfo {
  uint8_t tag;
  union {
    struct {
      uint16_t nameIndex;
    } Class;
    struct {
      uint16_t classIndex;
      uint16_t nameAndTypeIndex;
    } Fieldref;
    struct {
      uint16_t classIndex;
      uint16_t nameAndTypeIndex;
     } Methodref;
    struct {
      uint16_t classIndex;
      uint16_t nameAndTypeIndex;
    } InterfaceMethodref;
    struct {
      uint16_t stringIndex;
    } String;
    struct {
      uint32_t bytes;
    } Integer;
    struct {
      uint32_t bytes;
    } Float;
    struct {
      uint32_t highBytes;
      uint32_t lowBytes;
    } Long;
    struct {
      uint32_t highBytes;
      uint32_t lowBytes;
    } Double;
    struct {
      uint16_t nameIndex;
      uint16_t descriptorIndex;
    } NameAndType;
    struct {
      uint16_t length;
      uint8_t* bytes;
    } Utf8;
  } info;
} CpInfo;

typedef struct ExceptionsAttribute {
  uint16_t attributeNameIndex;
  uint32_t attributeLength;
  uint16_t numberExceptions;
  uint16_t* exceptionIndexTable;
} ExceptionsAttribute;

typedef struct CvInfo {
  uint16_t attributeNameIndex;
  uint32_t attributeLength;
  uint16_t constantValueIndex;
} CvInfo;

typedef struct FieldInfo {
  uint16_t accessFlags;
  uint16_t nameIndex;
  uint16_t descriptorIndex;
  uint16_t attributesCount;
  CvInfo* attributes;
} FieldInfo;

typedef struct AttributeInfo {
  uint16_t attributeNameIndex;
  uint32_t attributeLength;
  uint8_t* info;
} AttributeInfo;

typedef struct ExceptionTable {
  uint16_t startPc;
  uint16_t endPc;
  uint16_t catchType;
} ExceptionTable;

typedef struct CodeAttribute {
  uint16_t attributeNameIndex;
  uint32_t attributeLength;
  uint16_t maxStack;
  uint16_t maxLocals;
  uint32_t codeLength;
  uint8_t* code;
  uint16_t exceptionTableLength;
  ExceptionTable* exceptionTable;
  uint16_t attributesCount;
  AttributeInfo* attributes;
} CodeAttribute;

typedef struct MethodInfo {
  uint16_t accessFlags;
  uint16_t nameIndex;
  uint16_t descriptorIndex;
  uint16_t attributesCount;
  CodeAttribute* cdAtrb;
  ExceptionsAttribute* excAtrb;
} MethodInfo;

typedef struct ClassFile {
  uint32_t magic;
  uint16_t minorVersion;
  uint16_t majorVersion;
  uint16_t constantPoolCount;
  CpInfo* constantPool;
  uint16_t accessFlags;
  uint16_t thisClass;
  uint16_t superClass;
  uint16_t interfacesCount;
  uint16_t* interfaces;
  uint16_t fieldsCount;
  FieldInfo* fields;
  uint16_t methodsCount;
  MethodInfo* methods;
  uint16_t attributesCount;
  AttributeInfo* attributes;
} ClassFile;

ClassFile* inicializaLeitor(char*);
void leClassFile(FILE*, ClassFile*);
int cafeBabeValido(FILE*, ClassFile*);
void leConstantPool(FILE*, ClassFile*);
void leInterfaceInfo(FILE*, ClassFile*);
void leFieldInfo(FILE*, ClassFile*);
void leMethodInfo(FILE*, ClassFile*);
void leAttributeInfo(FILE*, ClassFile*);
void leExc(FILE*, ExceptionsAttribute**, uint16_t, uint32_t);
void leCode(FILE*, CodeAttribute**, uint16_t, uint32_t);

uint8_t le1Byte(FILE*);
uint16_t le2Bytes(FILE*);
uint32_t le4Bytes(FILE*);
void salvaInstrucoes(FILE*, CodeAttribute**);

#endif
