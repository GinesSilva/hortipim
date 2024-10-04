#ifndef ESTOQUE_REPOSITORIO
#define ESTOQUE_REPOSITORIO

#include "produto.h"
#include "vendas.h"

typedef struct resultado
{
    ProdutoCheckout p;
    int cod;
} Resultado;

int cadastrar_produto(Produto produto);

int listar_todos();

int listar_codigo(int codigo);

int entrada_produtos(int codigo, float quantidadeEntrada, float preco);

int saida_produtos(int codigo, int quantidadeEntrada);

int atualizar_preco(int codigo, float novo_preco);

Resultado buscar_produto(int codigo);

#endif