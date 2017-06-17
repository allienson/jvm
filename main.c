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

#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_ARQUIVO 100

MethodInfo* metodoMain;
char* nomeArquivo;

int main(int argc, char* argv[]) {
    nomeArquivo = calloc(TAMANHO_ARQUIVO, sizeof(argv[1]));
    int printPrompt = 0;

    if (argc < 3) {
    	printf("\n\nInforme o caminho completo do arquivo \".class\" que contem o metodo main:\n");
    	scanf("%s", nomeArquivo);
    	getchar();
    } else {
    	strcpy(nomeArquivo, argv[1]);
      if(*argv[2] == '1') {
        printPrompt = 1;
      }
    }
	carregaMemClass("java/lang/Object");
    carregaMemClass(nomeArquivo);

    ClassFile* classeMain = buscaClassPorIndice(1);

    metodoMain = buscaMetodoMain();

    if (metodoMain == NULL) {
        printf("O arquivo \".class\" informado nao possui metodo Main!");
        return 0;
    }


	empilhaMetodo(metodoMain, classeMain);
    executaFrameCorrente();

    free(nomeArquivo);
    return 0;
}
