
#ifndef FORNECEDORES
#define FORNECEDORES

#include "produto.h"

typedef struct
{
    int id;
    char cnpj[15];
    char razao_social[51];
    char nome_fantasia[21];
    char produtos[];
} Fornecedor;


int cadastrar_fornecedores();

int historico_de_compras();

#endif
