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

#ifndef AREA_METODOS_H
#define AREA_METODOS_H

#include "leitor.h"

#define MAX_VAR 256

union var {
  int inteiro;
  char caractere;
  float pFlutuante;
};

typedef struct AreaMetodos {
  ClassFile** arrayClasses;
  int numClasses;
} AreaMetodos;

typedef struct Objeto {
  ClassFile* classe;
  struct objeto* superClasse;
  uint32_t* campos;
  uint32_t* indiceCampos;
} Objeto;

Objeto** heap;

#endif
