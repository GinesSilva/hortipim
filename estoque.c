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
    int res;
    do
    {
        limpar_buffer();
        printf("Entrada de produtos\n\n");
        printf("Codigo do produto que deseja adicionar ou 0 para sair: ");
        scanf("%d", &codigo);
        if (codigo <= 0)
        {
            limpar_terminal();
            return -1;
        }
        res = listar_codigo(codigo);
        limpar_buffer();
        bool cont = false;

        if (res == 0)
        {
            do
            {
                char opt;
                printf("Deseja adicionar esse produto?(s/n)\n>>");
                scanf("%c", &opt);
                switch (opt)
                {
                case 's':{
                    bool negativo = true;
                    limpar_buffer();
                    do
                    {
                        printf("Digite a quantidade de entrada:\n>>");
                        fgets(input, sizeof(input), stdin);
                        quantidade = atof(input);
                        if (quantidade <= 0)
                        {
                            printf("A quantidade de entrada deve ser maior que zero\n\n");
                        }
                        else
                        {
                            negativo = false;
                        }
                    } while (negativo);
                    negativo = true;
                    do
                    {
                        printf("Digite o preco:\n>>");
                        fgets(input, sizeof(input), stdin);
                        preco = atof(input);
                        if (preco <= 0)
                        {
                            printf("O preco deve ser maior que zero\n\n");
                        }
                        else
                        {
                            negativo = false;
                        }
                    } while (negativo);

                    entrada_produtos(codigo, quantidade, preco);
                    break;
                    } 
                case 'n':
                    cont == false;
                    break;
                default:
                    limpar_terminal();
                    listar_codigo(codigo);
                    printf("Digite um valor valido!\n");
                    break;
                }
            } while (cont == true);
        }
        else
        {
            limpar_terminal();
            printf("Produto nao encontrado!\n\nPressione ENTER para continuar...\n\n");
        }

    } while (res == 0);
    return 0;
}

int saida_de_produtos()
{
    int codigo;
    float quantidade;
    int res;
    do
    {
        limpar_buffer();
        printf("Saida de produtos (QUEBRA/PERDA)\n\n");
        printf("Codigo do produto ira quebrar ou 0 para sair: ");
        scanf("%d", &codigo);
        if (codigo == 0)
        {
            limpar_terminal();
            return -1;
        }
        res = listar_codigo(codigo);
        limpar_buffer();
        bool cont = false;

        if (res == 0)
        {
            do
            {
                char opt;
                printf("O produto esta correto?(s/n)\n>>");
                scanf("%c", &opt);
                switch (opt)
                {
                case 's':
                    limpar_buffer();
                    printf("Digite a quantidade de saida:\n>>");
                    scanf("%f", &quantidade);
                    saida_produtos(codigo, quantidade);
                    break;
                case 'n':
                    cont = false;
                    break;
                default:
                    limpar_terminal();
                    listar_codigo(codigo);
                    printf("Digite um valor valido!\n");
                    break;
                }
            } while (cont == true);
        }
        else
        {
            limpar_terminal();
            printf("Produto nao encontrado!\n\nPressione ENTER para continuar...\n\n");
        }
    } while (res == 0);

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
    limpar_terminal();
    int codigo;
    printf("Relatorio de vendas por produto\n\n");
    printf("Codigo do produto que deseja obter o relatorio ou 0 para sair: ");
    limpar_buffer();
    scanf("%d", &codigo);
    if (codigo == 0)
    {
        limpar_terminal();
        return -1;
    }

    relatorio_produto(codigo);
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
        printf("Codigo do fornecedor: ");
        fgets(input, sizeof(input), stdin);
        p.fornecedor_id = atoi(input);
        if (buscar_fornecedor(p.fornecedor_id) == false)
        {
            bool cont = true;
            do
            {
                printf("Fornecedor nao esta cadastrado\n\n\n");
                printf("(c) cancelar (n) novo fornecedor (d) digitar codigo\n>>");
                char res;
                scanf("%c", &res);
                limpar_buffer();
                switch (res)
                {
                case 'c':
                {
                    limpar_terminal();
                    return -1;
                }
                case 'd':
                {
                    cont = false;
                    break;
                }
                case 'n':
                {
                    cont = false;
                    printf("Pressione ENTER");
                    cadastrar_fornecedores();
                    break;
                }

                default:
                    limpar_terminal();
                    printf("Digite um valor valido\n");
                    cont = true;
                    break;
                }
            } while (cont == true);
        }
    } while (buscar_fornecedor(p.fornecedor_id) == false || p.fornecedor_id <= 0);

    bool negativos = true;
    printf("Descricao: ");
    fgets(descricao, sizeof(descricao), stdin);
    descricao[strcspn(descricao, "\n")] = 0;

    do
    {
        printf("Preco de compra: ");
        fgets(input, sizeof(input), stdin);
        p.preco_de_compra = atof(input);
        if (p.preco_de_compra <= 0)
        {
            printf("Inserir um valor maior que zero para o preco de compra do produto!\n\n");
        }
        else
        {
            negativos = false;
        }
    } while (negativos);

    negativos = true;
    do
    {
        printf("Preco de venda: ");
        fgets(input, sizeof(input), stdin);
        p.preco_de_venda = atof(input);

        if (p.preco_de_venda <= 0)
        {
            printf("Inserir um valor maior que zero para o preco de venda do produto!\n\n");
        }
        else
        {
            negativos = false;
        }

    } while (negativos);

    strcpy(p.descricao, descricao);
    cadastrar_produto(p);
    return 0;
}

int alterar_preco()
{
    printf("Alteracao de precos\n\n");
    int codigo;
    float preco;
    limpar_buffer();
    printf("Codigo do produto ira alterar: ");
    scanf("%d", &codigo);
    listar_codigo(codigo);

    limpar_buffer();

    char opt;
    printf("O produto esta correto?(s/n)\n>>");
    scanf("%c", &opt);
    switch (opt)
    {
    case 's':{
        bool negativo = true;
        do
        {
            limpar_buffer();
            printf("Digite o novo preco: R$");
            scanf("%f", &preco);
            if (preco <= 0)
            {
                printf("O preco do produto nao pode ser igual ou menor que zero\n\n");
            }
            else
            {
                negativo = false;
            }
        } while (negativo);
        atualizar_preco(codigo, preco);
        break;
    case 'n':
        return 0;
    default:
        break;
    }
    return 0;
}
