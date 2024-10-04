#include <stdio.h>
#include <stdlib.h>

#include "vendas_repositorio.h"
#include "vendas.h"
#include "lista.h"

void add(struct lista **head, ProdutoCheckout produto_checkout)
{
    struct lista *nova_lista = (struct lista *)malloc(sizeof(struct lista));
    if (nova_lista == NULL)
    {
        printf("Erro ao alocar memória!\n\n");
    }

    nova_lista->pc = produto_checkout;
    nova_lista->prox = NULL;

    if (*head == NULL)
    {
        *head = nova_lista;
    }
    else 
    {
        struct lista *atual = *head;
        while (atual->prox != NULL) 
        {
            atual = atual->prox;
        }
        atual->prox = nova_lista;
    }
}