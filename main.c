///
///  @file main.c
///
///  @headerfile carregador.h "carregador.h"
///  @headerfile metodo.h "metodo.h"
///  @headerfile areaMetodos.h "areaMetodos.h"
///  @headerfile exibidor.h "exibidor.h"
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
///  @brief Arquivo principal que inicia a execucacao da JVM
///
///  Inicializa o programa, pegando o caminho do arquivo
///  ".class" e uma flag para impressao na tela. Ambos por
///  por linha de comando, ou por interface. Entao, chama
///  o Carregador para processar o arquivo e carregar a
///  area de metodos (AreaMetodos) para a memoria. Entao, comeca a
///  execucaco do metodo main da classe principal e encerra
///  quando a pilha de frames estiver vazia, imprimindo as
///  estruturas na tela (caso o usuario tenha solicitado).
///

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

///
/// Funcao principal que controla o inicio do  programa.
///
/// @param int Numero de argumentos passados via linha de comando
/// @param char* Array que contem as strings passadas via linha de comando
/// @return @c int Retorna 0 caso a execucao seja bem sucedida.
/// @see leParamsEntrada carregaClasseParaMemoria preparaMetodoMain empilhaMetodo executaFrameCorrente exibeArrayClasses
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

///
/// Le os paramentros inseridos via linha de comando.
///
/// @param int Numero de argumentos passados via linha de comando
/// @param char* Array que contem as strings passadas via linha de comando
/// @return @c void
void leParamsEntrada(int argc, char* argv[]) {
  caminhoArquivo = calloc(TAMANHO_ARQUIVO, sizeof(argv[1]));
  if (argc < 3) {
    printf("\n\nInforme o caminho completo do arquivo \".class\" que contem o metodo main:\n");
    scanf("%s", caminhoArquivo);
    getchar();
    exibeClassFile = 1;
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

///
/// Busca a classe que possui o metodo main e depois busca dentro dessa classe o metodo main.
///
/// @param Nenhum
/// @return @c void
/// @see buscaClassPorIndice buscaMetodoMain
void preparaMetodoMain() {
  classeMain = buscaClassPorIndice(1);
  metodoMain = buscaMetodoMain(classeMain);
  if (metodoMain == NULL) {
    printf("O arquivo \".class\" informado nao possui metodo Main!");
    exit(0);
  }
}

///
/// Mostra o  bytecode da classe lida no terminal e criar um arquivo de texto "log.txt"
/// com o bytecode da classe caso a flag printaParaArquivo tenha sido setada.
///
/// @param Nao possui paramentros
/// @return @c void
/// @see printaClassFile
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
