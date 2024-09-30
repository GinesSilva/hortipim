#ifndef FORNECEDORES_REPOSITORIO
#define FORNECEDORES_REPOSITORIO

#include "fornecedores.h"
#include <stdbool.h>

int cadastro_fornecedor(Fornecedor fornecedor);

char* buscar_id(int id);

bool buscar_fornecedor(int id);

int listar_todos_fornecedores();

int buscar_por_cnpj(char *cnpj);

int listar_historico_compra_fornecedor(int id);

#endif