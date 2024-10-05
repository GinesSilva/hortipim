#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "vendas_repositorio.h"
#include "utils.h"
#include "vendas.h"
#include "estoque_repositorio.h"
#include "lista.h"

void displayList(struct lista *head)
{
    double total = 0;
    struct lista *curr = head;
    printf("%-20s %-10s %-10s %-10s\n", "Descrição", "Quantidade", "Preço kg", "Total produto");
    while (curr != NULL)
    {
        total += curr->pc.quantidade * curr->pc.preco;
        printf("%-20s %-8.2f %-8.2f %8.2f\n", curr->pc.descricao, curr->pc.quantidade, curr->pc.preco, curr->pc.quantidade * curr->pc.preco);
        curr = curr->prox;
    }
    printf("%-40sTotal R$%8.2f\n", " ", total);
}

// debug map

float quantidade_map(struct map_produto *head, int codigo)
{
    float quantidade = 0;
    struct map_produto *curr = head;
    while (curr != NULL)
    {
        if (codigo == curr->codigo)
        {
            quantidade += curr->quantidade;
        }
        curr = curr->prox;
    }
    return quantidade;
}

// fim

int emitir_nota_fiscal()
{
    printf("Nota fiscal emitida...\n\n");
    return 0;
}

int registrar_venda()
{
    struct lista *head = NULL;
    struct map_produto *head_map = NULL;
    char documento_cliente[14];
    char input[14];
    int cont = 0;
    double quantidade;
    char continuar;
    limpar_terminal();
    limpar_buffer();

    printf("cpf ou cnpj do cliente: ");
    fgets(documento_cliente, sizeof(input), stdin);
    documento_cliente[strcspn(documento_cliente, "\n")] = 0;

    do
    {
        quantidade = 0;
        ProdutoCheckout p;
        Resultado r;
        printf("Código do produto: ");
        fgets(input, sizeof(input), stdin);
        int codigo = atoi(input);
        printf("quantidade: ");
        fgets(input, sizeof(input), stdin);
        quantidade = atof(input);
        r = buscar_produto(codigo);
        if (r.cod != 0)
        {
            printf("Produto não encontrado\n");
            quantidade = 0;
        }
        else
        {
            if (r.p.quantidade >= quantidade)
            {
                if (r.p.quantidade >= quantidade_map(head_map, codigo) + quantidade)
                {
                    strcpy(p.descricao, r.p.descricao);
                    p.preco = r.p.preco;
                    p.quantidade = quantidade;
                    add_map(&head_map, codigo, quantidade);
                    add(&head, p);
                }
                else
                {
                    printf("Quantidade maior do que a que tem no estoque!\n");
                }
            }
            else
            {
                printf("Quantidade maior do que a que tem no estoque!\n");
            }
        }
        displayList(head);
        int c2 = 0;
        do
        {
            printf("Adicionar mais um produto?(s/n) ");
            scanf("%c", &continuar);
            switch (continuar)
            {
            case 's':
                limpar_buffer();
                cont = 0;
                c2 = 1;
                break;
            case 'n':
                limpar_buffer();
                cont = 1;
                c2 = 1;
                break;
            default:
                limpar_buffer();
                printf("Insira um valor valido:\n");
                break;
            }
        } while (c2 == 0);
    } while (cont == 0);

    return 0;
}

int relatorio_venda_dia()
{

    time_t t;
    struct tm *tm_info;
    char buffer[20];
    time(&t);
    tm_info = localtime(&t);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", tm_info);
    // strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", tm_info);
    printf("Data e hora atual: %s\n", buffer);
    printf("Relatorio de venda do dia 01/01/2000\n\n");
    return 0;
}
