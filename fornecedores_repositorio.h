#ifndef FORNECEDORES_REPOSITORIO
#define FORNECEDORES_REPOSITORIO

#include "fornecedores.h"
#include <stdbool.h>

int cadastro_fornecedor(Fornecedor fornecedor);

char* buscar_id(int id);

bool buscar_fornecedor(int id);

#endif