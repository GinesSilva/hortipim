#ifndef ESTOQUE_REPOSITORIO
#define ESTOQUE_REPOSITORIO

#include "produto.h"

int cadastrar_produto(Produto produto);

int listar_todos();

int listar_codigo(int codigo);

int entrada_produtos(int codigo, int quantidadeEntrada);

int saida_produtos(int codigo, int quantidadeEntrada);

#endif