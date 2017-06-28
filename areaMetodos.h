///
///	@file areaMetodos.h
///	@headerfile leitor.h "leitor.h"
///
///	@date 28/06/17.
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
///	Define as estruturas Area de Metodos e Objeto.
///

#ifndef AREA_METODOS_H
#define AREA_METODOS_H

#include "leitor.h"

///
/// @brief Estrutura geral que contem todas as demais estruturas
///
/// Aqui ficam guaradas as classes carregadas e todas as subestrutures
/// dessas classes.
typedef struct AreaMetodos {
  ClassFile** arrayClasses;			
  int numClasses;								
} AreaMetodos;

///
/// @brief Estrutura para um objeto java
///
/// Essa estrutura guarda todas as informacoes especificas de cada
/// objeto instanciado.
typedef struct Objeto {
  ClassFile* classe;						
  struct Objeto* superClasse;		
  uint32_t* campos;							
  uint32_t* indiceCampos;				
} Objeto;

#endif
