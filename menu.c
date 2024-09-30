#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "estoque.h"
#include "fornecedores.h"
#include "vendas.h"
#include "utils.h"
#include "estoque_repositorio.h"

#include "fornecedores_repositorio.h"

#define MAX_NOME 50
#define QTDE_SETORES 3
#define ESTOQUE_MENU 7
#define VENDAS_MENU 2
#define FORNECEDORES_MENU 3

typedef struct
{
    char nome[MAX_NOME];
} Programa;

typedef struct
{
    char nome[MAX_NOME];
} Setor;

Setor Setores[QTDE_SETORES];

int set_setores()
{
    strcpy(Setores[0].nome, "Estoque");
    strcpy(Setores[1].nome, "Vendas");
    strcpy(Setores[2].nome, "Fornecedores");
    return 0;
}

void set_estoque(Programa Programas_estoque[])
{
    strcpy(Programas_estoque[0].nome, "Entrada de produtos");
    strcpy(Programas_estoque[1].nome, "Saída de produtos");
    strcpy(Programas_estoque[2].nome, "relatório de vendas por produto");
    strcpy(Programas_estoque[3].nome, "Cadastro de produtos");
    strcpy(Programas_estoque[4].nome, "Buscar produto por código");
    strcpy(Programas_estoque[5].nome, "Listar todos os produtos");
    strcpy(Programas_estoque[6].nome, "Alteração de preços");
}

void set_vendas(Programa Programas_vendas[])
{
    strcpy(Programas_vendas[0].nome, "Nova venda");
    strcpy(Programas_vendas[1].nome, "Relatório de vendas do dia");
}

void set_fornecedores(Programa Programas_fornecedores[])
{
    strcpy(Programas_fornecedores[0].nome, "Cadastrar fornecedor");
    strcpy(Programas_fornecedores[1].nome, "Listar Fornecedores");
    strcpy(Programas_fornecedores[2].nome, "Histórico de compras");
}

int show_menu()
{
    limpar_terminal();
    set_setores();
    printf("Escolha a opção desejada\n\n");
    for (int i = 0; i < QTDE_SETORES; i++)
    {
        printf("[%d] - %s\n", i + 1, Setores[i]);
    }
    printf("\n[0] - Sair do programa\n\n>>");
    int selecionado;
    scanf("%d", &selecionado);
    return selecionado;
}

int show_estoque()
{
    int opt;
    do
    {
        Programa Programas_estoque[ESTOQUE_MENU];
        set_estoque(Programas_estoque);
        printf("ESTOQUE\n\n");
        printf("Escolha a opção desejada \n\n");
        for (int i = 0; i < ESTOQUE_MENU; i++)
        {
            printf("[%d] - %s\n", i + 1, Programas_estoque[i]);
        }
        printf("\n[9] - Limpar console\n");
        printf("[0] - Voltar\n\n>>");
        scanf("%d", &opt);
        switch (opt)
        {
        case 1:
            entrada_de_produtos();
            break;
        case 2:
            saida_de_produtos();
            break;
        case 3:
            relatorio_de_vendas_por_produto();
            break;
        case 4:
            cadastro_de_produtos();
            break;
        case 5:
            listar_produto_codigo();
            break;
        case 6:
            listar_todos_produtos();
            break;
        case 7:
            alterar_preco();
            break;
        case 9:
            limpar_terminal();
            break;
        case 0:
            opt = 0;
            break;
        }
    } while (opt != 0);
    return 0;
}

int show_vendas()
{
    int opt;
    do
    {
        Programa Programas_vendas[VENDAS_MENU];
        set_vendas(Programas_vendas);
        printf("VENDAS\n\n");
        printf("Escolha a opção desejada \n\n");
        for (int i = 0; i < VENDAS_MENU; i++)
        {
            printf("[%d] - %s\n", i + 1, Programas_vendas[i]);
        }
        printf("\n[9] - Limpar console\n");
        printf("[0] - Voltar\n\n>>");
        scanf("%d", &opt);
        switch (opt)
        {
        case 1:
            registrar_venda();
            break;
        case 2:
            relatorio_venda_dia();
            break;
        case 9:
            limpar_terminal();
            break;
        case 0:
            opt = 0;
            break;
        }

    } while (opt != 0);
    return 0;
}

int show_fornecedores()
{
    int opt;
    do
    {
        Programa Programas_fornecedores[FORNECEDORES_MENU];
        set_fornecedores(Programas_fornecedores);
        printf("FORNECEDORES\n\n");
        printf("Escolha a opção desejada \n\n");
        for (int i = 0; i < FORNECEDORES_MENU; i++)
        {
            printf("[%d] - %s\n", i + 1, Programas_fornecedores[i]);
        }
        printf("\n[9] - Limpar console\n");
        printf("[0] - Voltar\n\n>>");
        scanf("%d", &opt);
        switch (opt)
        {
        case 1:
            cadastrar_fornecedores();
            break;
        case 2:
            listar_fornecedores();
            break;
        case 3:
            historico_de_compras();
            break;
        case 9:
            limpar_terminal();
            break;
        case 0:
            opt = 0;
            break;
        }
    } while (opt != 0);
    return 0;
}

int selecionar_programa()
{
    int cont = 1;
    while (cont == 1)
    {
        int opt = show_menu();
        switch (opt)
        {
        case 1:
            show_estoque();
            break;
        case 2:
            show_vendas();
            break;
        case 3:
            show_fornecedores();
            break;
        case 9: // debug
            break;
        case 0:
            cont = 0;
            break;
        }
    }
    return 0;
}