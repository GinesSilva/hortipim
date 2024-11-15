#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "vendas_repositorio.h"
#include "utils.h"
#include "vendas.h"
#include "estoque_repositorio.h"
#include "colecoes.h"

char *dataAtual()
{
    time_t t;
    struct tm *tm_info;
    char *buffer = (char *)malloc(20 * sizeof(char));

    if (buffer == NULL)
    {
        return NULL;
    }

    time(&t);
    tm_info = localtime(&t);
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", tm_info);
    return buffer;
}

void displayList(struct map_checkout *head)
{
    double total = 0;
    struct map_checkout *curr = head;
    printf("%-7s %-20s %-10s %-10s %-10s\n", "Produto", "Descrição", "Quantidade", "Preço kg", "Total produto");
    while (curr != NULL)
    {
        total += curr->pc.quantidade * curr->pc.preco;
        printf("%-7d %-20s %-8.2f %-8.2f %8.2f\n", curr->index, curr->pc.descricao, curr->pc.quantidade, curr->pc.preco, curr->pc.quantidade * curr->pc.preco);
        curr = curr->prox;
    }
    printf("%-40sTotal R$%8.2f\n", " ", total);
}

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

float valor_compra(struct map_produto *head)
{
    double total = 0;
    struct map_produto *curr = head;
    while (curr != NULL)
    {
        total += curr->quantidade * curr->preco;
        curr = curr->prox;
    }
    return total;
}

int registrar_venda()
{
    struct map_checkout *head = NULL;
    struct map_produto *head_map = NULL;
    struct venda *venda = malloc(sizeof(Venda));
    if (venda == NULL)
    {
        printf("Erro ao alocar memoria para venda.\n");
        return 1;
    }
    char documento_cliente[16];
    char input[16];
    int cont = 0;
    double quantidade;
    char continuar;
    limpar_terminal();
    limpar_buffer();

    printf("cpf ou cnpj do cliente: ");
    fgets(documento_cliente, sizeof(input), stdin);
    documento_cliente[strcspn(documento_cliente, "\n")] = 0;
    int index = 1;
    do
    {
        quantidade = 0;
        ProdutoCheckout p;
        Resultado r;
        printf("Codigo do produto: ");
        fgets(input, sizeof(input), stdin);
        int codigo = atoi(input);
        printf("quantidade: ");
        fgets(input, sizeof(input), stdin);
        quantidade = atof(input);
        r = buscar_produto(codigo);
        if (r.cod != 0)
        {
            printf("Produto nao encontrado\n");
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
                    p.codigo = codigo;
                    add_map(&head_map, codigo, quantidade, r.p.preco, index);
                    add(&head, p, index);
                    index++;
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
            printf("\n\n\n(a) adicionar produto ao carrinho (f) finalizar compra (r) remover produto do carrinho (c) cancelar\n>>");
            scanf("%c", &continuar);
            switch (continuar)
            {
            case 'a':
                limpar_buffer();
                cont = 0;
                c2 = 1;
                break;
            case 'f':
                int conf_troco = 0;
                cont = 1;
                c2 = 1;
                char *dt = dataAtual();
                venda->documento_cliente = documento_cliente;
                venda->data_venda = dt;
                venda->produtos = head;
                venda->total = valor_compra(head_map);
                float valor_pago = 0;
                do
                {
                    limpar_buffer();
                    printf("Valor pago: ");
                    scanf("%f", &valor_pago);
                    if (valor_pago >= venda->total)
                    {
                        conf_troco = 1;
                        float troco = 0;
                        limpar_buffer();
                        troco = valor_pago - venda->total;
                        printf("Troco R$%.2f\n", troco);
                        venda->troco = troco;
                    }
                    else
                    {
                        printf("Valor menor que o da compra!\n\n");
                    }
                } while (conf_troco == 0);
                printf("Pressione ENTER para continuar...\n\n");
                getchar();
                emitir_nota_fiscal(venda);
                registrar_nova_venda(venda);
                break;
            case 'r':
                limpar_buffer();
                printf("Indice do protudo a ser removido: ");
                int index = 0;
                scanf("%d", &index);
                remover_elemento(&head, index);
                remover_elemento_produto(&head_map, index);
                limpar_terminal();
                limpar_buffer();
                displayList(head);
                break;
            case 'c':
                c2 = 1;
                cont = 1;
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
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", tm_info);
    printf("Relatorio de venda do dia %s\n\n", buffer);
    relatorio_venda_dia_banco(buffer);
    return 0;
}

int relatorio_venda_periodo()
{
    char input[20];
    char inicio[20];
    char final[20];

    limpar_buffer();

    printf("Data de inicio: (aaaa-mm-dd): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    strcpy(inicio, input);

    printf("Data final: (aaaa-mm-dd): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    strcpy(final, input);

    relatorio_periodo(inicio, final);

    return 0;
}