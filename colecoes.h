
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
    int codigo;
    double quantidade;
    double preco;
    struct map_produto *prox;
} Map;

void add(struct map_checkout **head, ProdutoCheckout produto_checkout, int index);

void add_map(struct map_produto **head, int codigo, double quantidade, double preco);

void removerElemento(struct map_checkout **head, int index);

#endif