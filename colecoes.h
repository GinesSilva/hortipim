
#ifndef COLECOES
#define COLECOES

#include "produto.h"
#include "vendas.h"

typedef struct map_checkout
{
    int index;
    ProdutoCheckout pc;
    struct map_checkout *prox;
} MapCheckout;

typedef struct map_produto
{
    int index;
    int codigo;
    double quantidade;
    double preco;
    struct map_produto *prox;
} Map;

void add(struct map_checkout **head, ProdutoCheckout produto_checkout, int index);

void add_map(struct map_produto **head, int codigo, double quantidade, double preco, int index);

void remover_elemento(struct map_checkout **head, int index);

void remover_elemento_produto(struct map_produto **head, int index);

#endif