///
///		@file decodificador.h
///		@class decodificador.c "decodificador.c"
///
///		@date 26/06/17.
///		@copyright Copyright © 2017 GrupoSB. All rights reserved.
///
///		@authors
///		Allisson Barros         12/0055619\n
///		Daniel Luz              13/0007714\n
///		Luiz Fernando Vieira    13/0013757\n
///		Mariana Pannunzio       12/0018276\n
///		Mateus Denucci          12/0053080
///
///  	@brief Recebe uma estrutura Decodificador vazia e preenche
///		com strings que sao as instrucoes presentes no bytecode.
///		Retorna uma estrutura Decodificador preenchida.

#ifndef DECODIFICADOR_H
#define DECODIFICADOR_H

#include <string.h> 

#define NOME_INSTRUCAO 30 

typedef struct Decodificador {
    char instrucao[NOME_INSTRUCAO];
    int bytes; 
} Decodificador; 

void inicializaDecodificador(Decodificador dec[]);

#endif 