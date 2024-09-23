#include<stdio.h>
#include<stdlib.h>

#include "estoque.h"
#include "produto.h"
#include "repository.h"

#include <string.h>

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int entrada_de_produtos() {
    printf("Entrada de produtos\n\n");
    return 0;
}

int saida_de_produtos(int codigo, int quantidade) {
    printf("saida de produtos\n\n");
    return 0;
}

int listar_produto_id(int id) {
    printf("Produto %d\n\n", id);
    return 0;
}

int listar_todos_produtos(){
    listar_todos();
    return 0;
}

int relatorio_de_vendas_por_produto() {
    printf("Relatorio de vendas por produto\n\n");
    return 0;
}

int cadastro_de_produtos() {
    system("clear");
    limpar_buffer();
    printf("Cadastro...\n\n");
    Produto p;
    char descricao[51];
    char input[100]; // Buffer para entrada

    printf("Insira os dados do produto a ser adicionado:\n\n");
    
    printf("Descrição: ");
    fgets(descricao, sizeof(descricao), stdin);
    descricao[strcspn(descricao, "\n")] = 0;

    printf("Código do fornecedor: ");
    fgets(input, sizeof(input), stdin);
    p.fornecedor_id = atoi(input);

    printf("Quantidade: ");
    fgets(input, sizeof(input), stdin);
    p.quantidade = atof(input); 

    printf("Preço de compra: ");
    fgets(input, sizeof(input), stdin);
    p.preco_de_compra = atof(input);

    printf("Preço de venda: ");
    fgets(input, sizeof(input), stdin);
    p.preco_de_venda = atof(input); 

    strcpy(p.descricao, descricao);
    cadastrar_produto(p);
    return 0;
}
