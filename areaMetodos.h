///
///	@file areaMetodos.h
///	@headerfile leitor.h "leitor.h"
///
///	@date 26/06/17.
///	@copyright Copyright © 2017 GrupoSB. All rights reserved.
///
///	@authors
///	Allisson Barros         12/0055619\n
///	Daniel Luz              13/0007714\n
///	Luiz Fernando Vieira    13/0013757\n
///	Mariana Pannunzio       12/0018276\n
///	Mateus Denucci          12/0053080
///
///	@brief
///	Define a estrutura da Area de Metodos e Objeto, o espaço das variáveis e a heap.
///

#ifndef AREA_METODOS_H
#define AREA_METODOS_H

#include "leitor.h"

typedef struct AreaMetodos {
  ClassFile** arrayClasses;
  int numClasses;
} AreaMetodos;

typedef struct Objeto {
  ClassFile* classe;
  struct Objeto* superClasse;
  uint32_t* campos;
  uint32_t* indiceCampos;
} Objeto;

#endif
