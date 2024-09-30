#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "estoque.h"
#include "produto.h"
#include "estoque_repositorio.h"
#include "utils.h"
#include "fornecedores_repositorio.h"
#include "fornecedores.h"

int entrada_de_produtos()
{
    int codigo;
    float quantidade, preco;
    char input[100];

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
        printf("Digite a quantidade de entrada:\n\n>>");
        fgets(input, sizeof(input), stdin);
        quantidade = atof(input);
        printf("Digite o preço:\n\n>>");
        fgets(input, sizeof(input), stdin);
        preco = atof(input);
        entrada_produtos(codigo, quantidade, preco);
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
    // TODO
    printf("Relatorio de vendas por produto\n\n");
    return 0;
}

int cadastro_de_produtos()
{
    limpar_terminal();
    limpar_buffer();
    printf("Cadastro...\n\n");
    Produto p;
    char descricao[51];
    char input[100];

    printf("Insira os dados do produto a ser adicionado:\n\n");

    do
    {
        printf("Código do fornecedor: ");
        fgets(input, sizeof(input), stdin);
        p.fornecedor_id = atoi(input);
        if (buscar_fornecedor(p.fornecedor_id) == false)
        {
            printf("Fornecedor não está cadastrado deseja tentar de novo? (s/n)\n");
            char res;
            scanf("%c", &res);
            limpar_buffer();
            if (res != 's')
            {
                limpar_terminal();
                return -1;
            }
        }
    } while (buscar_fornecedor(p.fornecedor_id) == false || p.fornecedor_id <= 0);

    printf("Descrição: ");
    fgets(descricao, sizeof(descricao), stdin);
    descricao[strcspn(descricao, "\n")] = 0;

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
