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

// maximo numero de variaveis um objeto pode ter
#define MAX_VAR 256

// um campo que o objeto pode armazenar
union var {
    int inteiro;
    char caractere; 
    float pFlutuante; 
}; 

/** 
 * struct para a area de metodos
 * 
 */
typedef struct AreaMetodos {
    // array de classfiles, contem as classes ja carregadas
    ClassFile** arrayClasses; 

    // numero de classes carregadas em memoria 
    int numClasses;  

    // cada classe tem uma referencia ao heap, precisamos entao de uma array de referencias ao heap 
    //heap** heap;

} AreaMetodos; 

/** 
 * struct que representa um objeto. Usada na instrução new.
 * 
 */
typedef struct Objeto{
    ClassFile* classe;
    struct objeto* superClasse;
    uint32_t* campos;
    uint32_t* indiceCampos;
} Objeto;

//Array de referencias para objetos.
Objeto** heap;

#endif 
