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
//
//  Inicializa o programa, pegando o caminho do arquivo
//  ".class" e disparando a execuacao do leitor. Após
//  a leitura, executa a funcao de apresentacao das
//  estruturas lidas e formatadas.

#include "carregador.h"
#include "metodo.h"
#include "areaMetodos.h"
#include "exibidor.h"

#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_ARQUIVO 100

MethodInfo* metodoMain;
ClassFile* classeMain;
char* caminhoArquivo;
int exibeClassFile;
int printaParaArquivo;

void leParamsEntrada();
void preparaMetodoMain();
void exibeArrayClasses();

int main(int argc, char* argv[]) {
  leParamsEntrada(argc, argv);
  carregaClasseParaMemoria("java/lang/Object");
  carregaClasseParaMemoria(caminhoArquivo);
  preparaMetodoMain();
  empilhaMetodo(metodoMain, classeMain);
  executaFrameCorrente();
  exibeArrayClasses();
  free(caminhoArquivo);
  return 0;
}

void leParamsEntrada(int argc, char* argv[]) {
  caminhoArquivo = calloc(TAMANHO_ARQUIVO, sizeof(argv[1]));
  if (argc < 3) {
    printf("\n\nInforme o caminho completo do arquivo \".class\" que contem o metodo main:\n");
    scanf("%s", caminhoArquivo);
    getchar();
    exibeClassFile = 0;
  } else {
    strcpy(caminhoArquivo, argv[1]);
    if(*argv[2] == '1') {
      exibeClassFile = 1;
    } else if(*argv[2] == '2') {
      exibeClassFile = 1;
      printaParaArquivo = 1;
    }
  }
}

void preparaMetodoMain() {
  classeMain = buscaClassPorIndice(1);
  metodoMain = buscaMetodoMain(classeMain);
  if (metodoMain == NULL) {
    printf("O arquivo \".class\" informado nao possui metodo Main!");
    exit(0);
  }
}

void exibeArrayClasses() {
  if (exibeClassFile) {
    FILE *saved;
    if (printaParaArquivo) {
      saved = freopen("log.txt", "w+", stdout);
    }
    for (int i = 1; i < areaMetodos.numClasses; i++) {
      printaClassFile(areaMetodos.arrayClasses[i]);
    }
    if (printaParaArquivo) {
      fclose(saved);
    }
  }
}
