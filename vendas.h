#include <time.h>
#include "colecoes.h"

#ifndef VENDAS
#define VENDAS

typedef struct
{
    char descricao[50];
    double quantidade;
    double preco;
} ProdutoCheckout;

typedef struct venda
{
    double total;
    double troco;
    char *data_venda;
    char *documento_cliente;
    struct map_checkout *produtos;
} Venda;

int registrar_venda();

int relatorio_venda_dia();

#endif
