#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estoque.h"
#include "produto.h"
#include "estoque_repositorio.h"
#include "utils.h"

int entrada_de_produtos()
{
    int codigo;
    int quantidade;
    limpar_buffer();
    printf("Entrada de produtos\n\n");
    printf("Codigo do produto que deseja adicionar: ");
    scanf("%d", &codigo);
    listar_codigo(codigo);

    limpar_buffer();

    char opt;
    printf("Deseja adicionar esse produto?(s/n)\n\n>>");
    scanf("%c", &opt);
    switch (opt)
    {
    case 's':
        limpar_buffer();
        printf("Digite a quantiade de entrada:\n\n>>");
        scanf("%d", &quantidade);
        entrada_produtos(codigo, quantidade);
        break;
    case 'n':
        return 0;
    default:
        break;
    }

    return 0;
}

int saida_de_produtos()
{
    printf("saida de produtos\n\n");
    int codigo;
    int quantidade;
    limpar_buffer();
    printf("Codigo do produto irá sair: ");
    scanf("%d", &codigo);
    listar_codigo(codigo);

    limpar_buffer();

    char opt;
    printf("O produto está correto?(s/n)\n\n>>");
    scanf("%c", &opt);
    switch (opt)
    {
    case 's':
        limpar_buffer();
        printf("Digite a quantiade de saida:\n\n>>");
        scanf("%d", &quantidade);
        saida_produtos(codigo, quantidade);
        break;
    case 'n':
        return 0;
    default:
        break;
    }
    return 0;
}

int listar_produto_codigo()
{
    limpar_buffer();
    int codigo;
    printf("Insira o codigo do produto: \n>>");
    scanf("%d", &codigo);
    listar_codigo(codigo);
    return 0;
}

int listar_todos_produtos()
{
    listar_todos();
    return 0;
}

int relatorio_de_vendas_por_produto()
{
    printf("Relatorio de vendas por produto\n\n");
    return 0;
}

int cadastro_de_produtos()
{
    system("clear");
    limpar_buffer();
    printf("Cadastro...\n\n");
    Produto p;
    char descricao[51];
    char input[100];

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
