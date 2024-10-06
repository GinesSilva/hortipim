#include <stdio.h>
#include <stdlib.h>

#include "vendas_repositorio.h"
#include "vendas.h"
#include "colecoes.h"

void add(struct map_checkout **head, ProdutoCheckout produto_checkout, int index)
{
    struct map_checkout *nova_lista = (struct map_checkout *)malloc(sizeof(struct map_checkout));
    if (nova_lista == NULL)
    {
        printf("Erro ao alocar memória!\n\n");
    }

    nova_lista->index = index;
    nova_lista->pc = produto_checkout;
    nova_lista->prox = NULL;

    if (*head == NULL)
    {
        *head = nova_lista;
    }
    else 
    {
        struct map_checkout *atual = *head;
        while (atual->prox != NULL) 
        {
            atual = atual->prox;
        }
        atual->prox = nova_lista;
    }
}

void remover_elemento(struct map_checkout **head, int index) {
    MapCheckout *atual = *head;
    MapCheckout *anterior = NULL;

    while (atual != NULL && atual->index != index) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Elemento com index %d não encontrado na lista.\n", index);
        return;
    }

    if (anterior == NULL) {
        *head = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
}

void add_map(struct map_produto **head_map, int codigo, double quantidade, double preco, int index) {
    struct map_produto *novo_map = (struct map_produto *)malloc(sizeof(struct map_produto));
    if (novo_map == NULL)
    {
        printf("Erro ao alocar memória!\n\n");
    }

    novo_map->index = index;
    novo_map->codigo = codigo;
    novo_map->quantidade = quantidade;
    novo_map->preco = preco;
    novo_map->prox = NULL;

    if (*head_map == NULL)
    {
        *head_map = novo_map;
    }
    else 
    {
        struct map_produto *atual = *head_map;
        while (atual->prox != NULL) 
        {
            atual = atual->prox;
        }
        atual->prox = novo_map;
    }
}

void remover_elemento_produto(struct map_produto **head, int index) {
    Map *atual = *head;
    Map *anterior = NULL;

    while (atual != NULL && atual->index != index) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Elemento com index %d não encontrado na lista.\n", index);
        return;
    }

    if (anterior == NULL) {
        *head = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
}