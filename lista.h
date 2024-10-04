
#ifndef LISTA
#define LISTA

#include "produto.h"

typedef struct lista
{
    ProdutoCheckout pc;
    struct lista *prox;
} Lista;

void add(struct lista **head, ProdutoCheckout produto_checkout);

#endif