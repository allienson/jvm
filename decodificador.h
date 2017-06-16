//
//  decodificador.h
//  LeitorExibidor - Software Basico 2017-1
//
//  Created on 26/05/17.
//  Copyright © 2017 GrupoSB. All rights reserved.
//
//  Allisson Barros         12/0055619
//  Daniel Luz              13/0007714
//  Luiz Fernando Vieira    13/0013757
//  Mariana Pannunzio       12/0018276
//  Mateus Denucci          12/0053080

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