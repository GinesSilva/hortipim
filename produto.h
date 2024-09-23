#ifndef PRODUTO
#define PRODUTO

typedef struct
{
    int codigo;
    int fornecedor_id;
    char descricao[51];
    float preco_de_compra;
    float preco_de_venda;
    float quantidade;
} Produto;

#endif