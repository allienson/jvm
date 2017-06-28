///
///		@file leitor.c
///		@headerfile leitor.h "leitor.h"
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
///		Recebe o caminho do arquivo ".class" e le todas
///		as estruturas do ".class", byte a byte, de forma sequencial.
///

#include "leitor.h"

#include <stdlib.h>
#include <string.h>

///
/// Funcao principal que comeca a leitura de um classfile.
///
/// @param char* String com o caminho do arquivo .class a ser lido
/// @return @c ClassFile* Uma estrutura ClassFile preenchida
/// @see leClassFile
ClassFile* inicializaLeitor(char* caminhoClasse) {
    FILE* fp;
    fp = fopen(caminhoClasse, "rb");

    if(fp == NULL) {
        printf("Erro ao abrir o arquivo (nao encontrado) ! \n");
        exit(0);
    }

    ClassFile* classFile = NULL;
    classFile = (ClassFile*) calloc(sizeof(ClassFile), 1);

    if(classFile == NULL) {
        printf("Erro ao alocar espaço para o arquivo ! \n");
        exit(0);
    }

    leClassFile(fp, classFile);
    fclose(fp);
    return classFile;
}

///
/// Funcao que coordena a leitura do arquivo .class
/// e coordena as chamadas das funcoes que leem cada
/// parte do classfile individualmente e preenchem
/// a estrutura recebida como parametro.
///
/// @param FILE* Descritor do arquivo .class que foi aberto para leitura
/// @param ClassFile* Ponteiro para a estrutura ClassiFile vazia (porem ja alocada)
/// @return @c void
/// @see cafeBabeValido le2Bytes leConstantPool leInterfaceInfo leFieldInfo leMethodInfo leAttributeInfo
void leClassFile(FILE* fp, ClassFile* classFile) {
    if(cafeBabeValido(fp, classFile)) {

        classFile->minorVersion = le2Bytes(fp);
        classFile->majorVersion = le2Bytes(fp);
        classFile->constantPoolCount = le2Bytes(fp);//lendo errado aqui
        leConstantPool(fp, classFile);

        classFile->accessFlags = le2Bytes(fp);
        classFile->thisClass = le2Bytes(fp);
        classFile->superClass = le2Bytes(fp);
        classFile->interfacesCount = le2Bytes(fp);
        leInterfaceInfo(fp, classFile);

        classFile->fieldsCount = le2Bytes(fp);
        leFieldInfo(fp, classFile);

        classFile->methodsCount = le2Bytes(fp);
        leMethodInfo(fp, classFile);//morre aqui dentro

        classFile->attributesCount = le2Bytes(fp);
        leAttributeInfo(fp,classFile);

    } else {
        printf("Arquivo .class invalido !! \n");
        exit(0);
    }
}

///
/// Funcao que avalia se o arquivo aberto eh de fato
/// um classfile e se os bytes de control 0xCAFEBABE
/// estao corretamente no inicio do arquivo.
///
/// @param FILE* Descritor do arquivo .class que foi aberto para leitura
/// @param ClassFile* Ponteiro para a estrutura ClassiFile
/// @return @c int Resultado da verificacao (valor booleano)
/// @see le4Bytes
int cafeBabeValido(FILE* fp, ClassFile* classFile) {
    classFile->magic = le4Bytes(fp);
    return classFile->magic == 0xCAFEBABE;
}

///
/// Funcao que le os bytes que compoe o Constant Pool e
/// os armazena nas devidas posicoes da estrutura CpInfo,
/// dentro do ClassFile recebido como parametro.
///
/// @param FILE* Descritor do arquivo .class que foi aberto para leitura
/// @param ClassFile* Ponteiro para a estrutura ClassiFile
/// @return @c void
/// @see le1Byte le2Bytes le4Bytes
void leConstantPool(FILE* fp, ClassFile* classFile) {
    classFile->constantPool = (CpInfo*) malloc((classFile->constantPoolCount-1) * sizeof(CpInfo));
    CpInfo* cpInfo;

    int i = 0;
    for(cpInfo = classFile->constantPool; i < (classFile->constantPoolCount-1); cpInfo++) {
        cpInfo->tag = le1Byte(fp);
        switch(cpInfo->tag) {
            case CONSTANT_Class:
                cpInfo->info.Class.nameIndex = le2Bytes(fp);
                break;
            case CONSTANT_Fieldref:
                cpInfo->info.Fieldref.classIndex = le2Bytes(fp);
                cpInfo->info.Fieldref.nameAndTypeIndex = le2Bytes(fp);
                break;
            case CONSTANT_NameAndType:
                cpInfo->info.NameAndType.nameIndex = le2Bytes(fp);
                cpInfo->info.NameAndType.descriptorIndex = le2Bytes(fp);
                break;
            case CONSTANT_Utf8:
                cpInfo->info.Utf8.length = le2Bytes(fp);
                cpInfo->info.Utf8.bytes = (uint8_t*) calloc ((cpInfo->info.Utf8.length) + 1 ,sizeof(uint8_t));
                fread(cpInfo->info.Utf8.bytes,1,cpInfo->info.Utf8.length,fp);
                cpInfo->info.Utf8.bytes[cpInfo->info.Utf8.length] = '\0';
                break;
            case CONSTANT_Methodref:
                cpInfo->info.Methodref.classIndex = le2Bytes(fp);
                cpInfo->info.Methodref.nameAndTypeIndex = le2Bytes(fp);
                break;
            case CONSTANT_InterfaceMethodref:
                cpInfo->info.InterfaceMethodref.classIndex = le2Bytes(fp);
                cpInfo->info.InterfaceMethodref.nameAndTypeIndex = le2Bytes(fp);
                break;
            case CONSTANT_String:
                cpInfo->info.String.stringIndex = le2Bytes(fp);
                break;
            case CONSTANT_Integer:
                cpInfo->info.Integer.bytes = le4Bytes(fp);
                break;
            case CONSTANT_Float:
                cpInfo->info.Float.bytes = le4Bytes(fp);
                break;
            case CONSTANT_Double:
                cpInfo->info.Double.highBytes = le4Bytes(fp);
                cpInfo->info.Double.lowBytes = le4Bytes(fp);
                cpInfo++;
                i++;
                break;
            case CONSTANT_Long:
                cpInfo->info.Long.highBytes = le4Bytes(fp);
                cpInfo->info.Long.lowBytes = le4Bytes(fp);
                cpInfo++;
                i++;
                break;
            default:
                break;
        }
        i++;
    }
}

///
/// Funcao que le os bytes que compoe as Informacoes de Interface e
/// os armazena nas devidas posicoes da estrutura InterfaceInfo,
/// dentro do ClassFile recebido como parametro.
///
/// @param FILE* Descritor do arquivo .class que foi aberto para leitura
/// @param ClassFile* Ponteiro para a estrutura ClassiFile
/// @return @c void
/// @see le2Bytes
void leInterfaceInfo(FILE* fp, ClassFile* classFile) {
    if(classFile->interfacesCount == 0) {
        return;
    } else {
        classFile->interfaces = (uint16_t*) malloc((classFile->interfacesCount) * sizeof(uint16_t));
        for (int i = 0; i < classFile->interfacesCount; i++) {
            classFile->interfaces[i] = le2Bytes(fp);
        }
    }
}

///
/// Funcao que le os bytes que compoe as Informacoes de Fields e
/// os armazena nas devidas posicoes da estrutura FieldInfo,
/// dentro do ClassFile recebido como parametro.
///
/// @param FILE* Descritor do arquivo .class que foi aberto para leitura
/// @param ClassFile* Ponteiro para a estrutura ClassiFile
/// @return @c void
/// @see le2Bytes le4Bytes
void leFieldInfo(FILE* fp, ClassFile* classFile) {
    if(classFile->fieldsCount == 0) {
        return;
    } else {
        classFile->fields = (FieldInfo*) malloc(classFile->fieldsCount * sizeof(FieldInfo));

        for (int i = 0; i < classFile->fieldsCount; i++) {
            classFile->fields[i].accessFlags = le2Bytes(fp);
            classFile->fields[i].nameIndex = le2Bytes(fp);
            classFile->fields[i].descriptorIndex = le2Bytes(fp);
            classFile->fields[i].attributesCount = le2Bytes(fp);
            classFile->fields[i].attributes = (CvInfo*) malloc(classFile->fields[i].attributesCount * sizeof(CvInfo));

            for (int j = 0; j < classFile->fields[i].attributesCount; j++) {
                classFile->fields[i].attributes->attributeNameIndex = le2Bytes(fp);
                classFile->fields[i].attributes->attributeLength = le4Bytes(fp);
                classFile->fields[i].attributes->constantValueIndex = le2Bytes(fp);
            }
        }
    }
}

///
/// Funcao que le os bytes que compoe as Informacoes de Metodos e
/// os armazena nas devidas posicoes da estrutura MethodInfo,
/// dentro do ClassFile recebido como parametro. Isso tudo incluindo
/// codigo (instrucoes) e excecoes de cada metodo.
///
/// @param FILE* Descritor do arquivo .class que foi aberto para leitura
/// @param ClassFile* Ponteiro para a estrutura ClassiFile
/// @return @c void
/// @see le1Byte le2Bytes le4Bytes leCode leExc
void leMethodInfo(FILE* fp, ClassFile* classFile) {
  uint16_t nameIndex;
  uint32_t attributesCount;

  if(classFile->methodsCount == 0) {
    return;
  } else {
    classFile->methods = (MethodInfo*) malloc(classFile->methodsCount * sizeof(MethodInfo));
    MethodInfo* methodInfo = classFile->methods;
    for(int i = 0; i < classFile->methodsCount; methodInfo++) {
      methodInfo->accessFlags = le2Bytes(fp);
      if(methodInfo->accessFlags == 0x010a ||methodInfo->accessFlags == 0x0101||methodInfo->accessFlags == 0x0111){
        methodInfo->nameIndex = le2Bytes(fp);
        methodInfo->descriptorIndex = le2Bytes(fp);
        methodInfo->attributesCount = le2Bytes(fp);
        i++;
        for (int j = 0; j < methodInfo->attributesCount; j++) {
          int64_t temp, temp2;
          // pega atributo name index do metodo
          temp = le2Bytes(fp);
          // pega attributo length do metodo
          temp = le4Bytes(fp);
          // vai lendo info
          for (int k = 0; k < temp; k++) {
            temp2 = le1Byte(fp);
          }
        }
        continue;
      }
      methodInfo->nameIndex = le2Bytes(fp);
      methodInfo->descriptorIndex = le2Bytes(fp);
      methodInfo->attributesCount = le2Bytes(fp);
      for(int j = 0; j < methodInfo->attributesCount; j++) {
        nameIndex = le2Bytes(fp);
        attributesCount = le4Bytes(fp);
        if (strcmp( (char*) classFile->constantPool[nameIndex - 1].info.Utf8.bytes, "Code") == 0) {
          methodInfo->cdAtrb = (CodeAttribute*) malloc(sizeof(CodeAttribute));
          leCode(fp, &(methodInfo->cdAtrb), nameIndex, attributesCount);
        } else if (strcmp( (char*) classFile->constantPool[nameIndex - 1].info.Utf8.bytes, "Exceptions") == 0) {
          methodInfo->excAtrb = (ExceptionsAttribute*) malloc(sizeof(ExceptionsAttribute));
          leExc(fp, &(methodInfo->excAtrb), nameIndex, attributesCount);
        }
      }
      i++;
    }
  }
}

///
/// Funcao que le os bytes que compoe as Informacoes de Atributos e
/// os armazena nas devidas posicoes da estrutura AttributeInfo,
/// dentro do ClassFile recebido como parametro.
///
/// @param FILE* Descritor do arquivo .class que foi aberto para leitura
/// @param ClassFile* Ponteiro para a estrutura ClassiFile
/// @return @c void
/// @see le2Bytes le4Bytes
void leAttributeInfo(FILE* fp, ClassFile* classFile) {
    if(classFile->attributesCount == 0) {
        return;
    } else {
        classFile->attributes = (AttributeInfo*) malloc(classFile->attributesCount * sizeof(AttributeInfo));
        AttributeInfo* attributeInfo = classFile->attributes;

        for(int i = 0; i < classFile->attributesCount; attributeInfo++){
            attributeInfo->attributeNameIndex = le2Bytes(fp);
            attributeInfo->attributeLength = le4Bytes(fp);
            attributeInfo->info = (uint8_t*) malloc((attributeInfo->attributeLength) * sizeof(uint8_t));


            for(uint32_t j = 0; j < attributeInfo->attributeLength; j++){
                fread(&attributeInfo->info[j], 1, 1, fp);
            }
            i++;
        }
    }
}

///
/// Funcao que le as excecoes (quando definidas) de um metodo da classe e
/// os armazena nas devidas posicoes da estrutura ExceptionsAttribute.
///
/// @param FILE* Descritor do arquivo .class que foi aberto para leitura
/// @param ExceptionsAttribute** Ponteiro para uma estrutura ExceptionsAttribute
/// @param uint16_t NameIndex do metodo cujas excecoes serao lidas
/// @param uint16_t Quantidade de atributos do metodo
/// @return @c void
/// @see le2Bytes
void leExc(FILE* fp, ExceptionsAttribute** excAtrb, uint16_t nameIndex, uint32_t attributesCount) {
    (*excAtrb)->attributeNameIndex = nameIndex;
    (*excAtrb)->attributeLength = attributesCount;
    (*excAtrb)->numberExceptions = le2Bytes(fp);
    (*excAtrb)->exceptionIndexTable = (uint16_t*) malloc((*excAtrb)->numberExceptions * sizeof(ExceptionTable));
    for (int k = 0; k < (*excAtrb)->numberExceptions; k++) {
        (*excAtrb)->exceptionIndexTable[k] = le2Bytes(fp);
    }
}

///
/// Funcao que le o codigo (mnemonicos) de um metodo e os
/// armazena nas devidas posicoes da estrutura CodeAttribute.
///
/// @param FILE* Descritor do arquivo .class que foi aberto para leitura
/// @param CodeAttribute** Ponteiro para uma estrutura CodeAttribute
/// @param uint16_t NameIndex do metodo cujas instrucoes serao lidas
/// @param uint16_t Quantidade de atributos do metodo
/// @return @c void
/// @see le1Byte le2Bytes le4Bytes salvaInstrucoes
void leCode(FILE* fp, CodeAttribute** cdAtrb, uint16_t nameIndex, uint32_t attributesCount) {
    int posicaoInicial = (int) ftell(fp);

    (*cdAtrb)->attributeNameIndex = nameIndex;
    (*cdAtrb)->attributeLength = attributesCount;
    (*cdAtrb)->maxStack = le2Bytes(fp);
    (*cdAtrb)->maxLocals = le2Bytes(fp);
    (*cdAtrb)->codeLength = le4Bytes(fp);

    salvaInstrucoes(fp, cdAtrb);

    (*cdAtrb)->exceptionTableLength = le2Bytes(fp);
    (*cdAtrb)->exceptionTable = (ExceptionTable*) malloc((*cdAtrb)->exceptionTableLength * sizeof(ExceptionTable));
    for (int k = 0; k < (*cdAtrb)->exceptionTableLength; k++) {
        (*cdAtrb)->exceptionTable[k].startPc = le2Bytes(fp);
        (*cdAtrb)->exceptionTable[k].endPc = le2Bytes(fp);
        (*cdAtrb)->exceptionTable[k].catchType = le2Bytes(fp);
    }

    (*cdAtrb)->attributesCount = le2Bytes(fp);
    (*cdAtrb)->attributes = (AttributeInfo*) malloc ((*cdAtrb)->attributesCount * sizeof(AttributeInfo));

    while (ftell(fp) - posicaoInicial < (int32_t)((*cdAtrb)->attributeLength)) {
        le1Byte(fp);
    }
}

///
/// Funcao que traduz os bytes de codigo lidos de um metodo e
/// os armazena nas devidas posicoes da estrutura CodeAttribute.
///
/// @param FILE* Descritor do arquivo .class que foi aberto para leitura
/// @param CodeAttribute** Ponteiro para uma estrutura CodeAttribute
/// @return @c void
/// @see inicializaDecodificador
void salvaInstrucoes(FILE* fp, CodeAttribute** cdAtrb) {
    int opcode, posReferencia;
    int bytesPreench, offsets;
    uint32_t defaultV, low, high, npairs;

    Decodificador dec[NUM_INSTRUCAO];
    inicializaDecodificador(dec);

    (*cdAtrb)->code = (uint8_t*) malloc((*cdAtrb)->codeLength * sizeof(uint8_t));

    for(uint32_t k = 0; k < (*cdAtrb)->codeLength; ) {
        fread(&((*cdAtrb)->code[k]), 1, 1, fp);

        opcode = (*cdAtrb)->code[k];
        k++;

        if (opcode == TABLESWITCH) {
            posReferencia = k - 1;

            bytesPreench = (4 - (k % 4)) % 4;
            for (int l = 0; l < bytesPreench; l++) {
                k++;
                fread(&((*cdAtrb)->code[k]), 1, 1, fp);
            }

            defaultV = 0;
            for (int l = 0; l < 4; l++) {
                fread(&((*cdAtrb)->code[k]), 1, 1, fp);
                defaultV = (defaultV << 8) + (*cdAtrb)->code[k];
                k++;
            }

            low = 0;
            for (int l = 0; l < 4; l++) {
                fread(&((*cdAtrb)->code[k]), 1, 1, fp);
                low = (low << 8) + (*cdAtrb)->code[k];
                k++;
            }

            high = 0;
            for (int l = 0; l < 4; l++) {
                fread(&((*cdAtrb)->code[k]), 1, 1, fp);
                high = (high << 8) + (*cdAtrb)->code[k];
                k++;
            }

            offsets = 1 + high - low;
            for (int l = 0; l < offsets; l++) {
                for (int i = 0; i < 4; i++) {
                    fread(&((*cdAtrb)->code[k]), 1, 1, fp);
                    k++;
                }
            }
        } else if (opcode == LOOKUPSWITCH) {
            posReferencia = k - 1;

            bytesPreench = (4 - (k % 4)) % 4;
            for (int l = 0; l < bytesPreench; l++) {
                k++;
                fread(&((*cdAtrb)->code[k]), 1, 1, fp);
            }

            defaultV = 0;
            for (int l = 0; l < 4; l++) {
                fread(&((*cdAtrb)->code[k]), 1, 1, fp);
                defaultV = (defaultV << 8) + (*cdAtrb)->code[k];
                k++;
            }

            npairs = 0;
            for (int l = 0; l < 4; l++) {
                fread(&((*cdAtrb)->code[k]), 1, 1, fp);
                npairs = (npairs << 8) + (*cdAtrb)->code[k];
                k++;
            }

            for (uint32_t l = 0; l < npairs; l++) {
                for (int i = 0; i < 4; i++) {
                    fread(&((*cdAtrb)->code[k]), 1, 1, fp);
                    k++;
                }

                for (int i = 0; i < 4; i++) {
                    fread(&((*cdAtrb)->code[k]), 1, 1, fp);
                    k++;
                }
            }
        } else if (opcode == WIDE) {

            fread(&((*cdAtrb)->code[k]), 1, 1, fp);
            opcode = (*cdAtrb)->code[k];
            k++;

            if (opcode == ILOAD || opcode == FLOAD || opcode == ALOAD || opcode == LLOAD ||
                opcode == DLOAD || opcode == ISTORE || opcode == FSTORE || opcode == ASTORE ||
                opcode == LSTORE || opcode == DSTORE || opcode == RET) { // mneRmonicos

                fread(&((*cdAtrb)->code[k]), 1, 1, fp);
                k++;

                fread(&((*cdAtrb)->code[k]), 1, 1, fp);
                k++;
            } else if (opcode == IINC) {

                fread(&((*cdAtrb)->code[k]), 1, 1, fp);
                k++;

                fread(&((*cdAtrb)->code[k]), 1, 1, fp);
                k++;

                fread(&((*cdAtrb)->code[k]), 1, 1, fp);
                k++;

                fread(&((*cdAtrb)->code[k]), 1, 1, fp);
                k++;
            } else {
                printf("arquivo .class invalido na instrucao wide");
                exit(1);
            }
        } else {
            int num_bytes = dec[opcode].bytes;
            for (int l = 0; l < num_bytes; l++) {
                fread(&((*cdAtrb)->code[k]), 1, 1, fp);
                k++;
            }
        }
    }
}

///
/// Funcao que le 1 byte e salva numa variavel unsigned de 8bits.
///
/// @param FILE* Descritor do arquivo .class que foi aberto para leitura
/// @return @c uint8_t O valor lido em um unsigned de 8bits
uint8_t le1Byte(FILE* fp) {
    uint8_t byte = getc(fp);
    return byte;
}

///
/// Funcao que le 2 byte e salva numa variavel unsigned de 16bits.
///
/// @param FILE* Descritor do arquivo .class que foi aberto para leitura
/// @return @c uint16_t O valor lido em um unsigned de 16bits
uint16_t le2Bytes(FILE* fp) {
    uint16_t bytes = getc(fp);
    bytes = (bytes << 8) | (getc(fp));
    return bytes;
}

///
/// Funcao que le 4 byte e salva numa variavel unsigned de 32bits.
///
/// @param FILE* Descritor do arquivo .class que foi aberto para leitura
/// @return @c uint32_t O valor lido em um unsigned de 32bits
uint32_t le4Bytes(FILE* fp) {
    uint32_t bytes = getc(fp);
    bytes = (bytes << 8) | (getc(fp));
    bytes = (bytes << 8) | (getc(fp));
    bytes = (bytes << 8) | (getc(fp));
    return bytes;
}
