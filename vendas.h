#include <time.h>

#ifndef VENDAS
#define VENDAS

typedef struct
{
    char descricao[50];
    double quantidade;
    double preco;
} ProdutoCheckout;

typedef struct
{
    char *data_venda;
    char *documento_cliente;
    ProdutoCheckout *produtos;
} Venda;

int registrar_venda();

int relatorio_venda_dia();

#endif
