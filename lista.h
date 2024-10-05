
#ifndef LISTA
#define LISTA

#include "produto.h"

typedef struct lista
{
    ProdutoCheckout pc;
    struct lista *prox;
} Lista;

typedef struct map_produto
{
    int codigo;
    double quantidade;
    struct map_produto *prox;
} Map;

void add(struct lista **head, ProdutoCheckout produto_checkout);

void add_map(struct map_produto **head, int codigo, double quantidade);

#endif