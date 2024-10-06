#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sqlite3.h>

#include "utils.h"
#include "vendas.h"
#include "colecoes.h"
#include "estoque_repositorio.h"

int produtos_vendidos(struct map_checkout *map, int id_venda)
{
    struct map_checkout *curr = map;
    while (curr != NULL)
    {
        sqlite3 *db;
        char sql[512];

        if (sqlite3_open("hortifruti.db", &db) != SQLITE_OK)
        {
            fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
            return -1;
        }

        sprintf(sql, "INSERT INTO produtos_vendidos(id, venda_id, quantidade, valor_total, codigo) VALUES(NULL, '%d', %.3f, %.2f, %d);",
                id_venda, curr->pc.quantidade, curr->pc.preco, curr->pc.codigo);

        char *err_msg = NULL;
        if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK)
        {
            fprintf(stderr, "Erro ao executar a consulta: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(db);
            return -1;
        }
        else
        {
            limpar_terminal();
        }

        sqlite3_close(db);
        saida_produtos(curr->pc.codigo, curr->pc.quantidade);
        curr = curr->prox;
    }

    return 0;
}

int registrar_nova_venda(struct venda *venda)
{
    sqlite3 *db;
    char sql[512];

    if (sqlite3_open("hortifruti.db", &db) != SQLITE_OK)
    {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sprintf(sql, "INSERT INTO vendas(id, data_venda, documento_cliente, total, troco) VALUES(NULL, '%s', '%s', %.2f, %.2f);",
            venda->data_venda, venda->documento_cliente, venda->total, venda->troco);

    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao executar a consulta: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    }
    else
    {
        sqlite3_int64 last_id = sqlite3_last_insert_rowid(db);
        int id = last_id;
        produtos_vendidos(venda->produtos, id);
        limpar_terminal();
        printf("Venda finalizada.\n");
    }

    sqlite3_close(db);
    return 0;
}

int emitir_nota_fiscal(struct venda *venda)
{
    char nome_arquivo[100];
    split(nome_arquivo, ' ', '_');
    split(nome_arquivo, '/', '_');
    split(nome_arquivo, ':', '_');
    printf("%s", nome_arquivo);
    FILE *file = fopen(nome_arquivo, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Erro ao criar o arquivo.\n");
        return 1;
    }

    fprintf(file, "--- Nota Fiscal ---\n");
    fprintf(file, "Emitente: HORTIFRUTI PIM\n\n\n");
    fprintf(file, "CNPJ: 23.456.789/0001-78\n");
    fprintf(file, "Endereço: Rua da Faculdade, 1000\n");
    fprintf(file, "Bairro: Centro        ");
    fprintf(file, "Cidade: Araraquara - SP\n");
    fprintf(file, "Cep: 14765-432\n");
    fprintf(file, "\nDestinatário:\n");
    fprintf(file, "CPF/CNPJ: %-14s", venda->documento_cliente);
    fprintf(file, "\nTotal: R$ %-8.2f\n\n", venda->total);

    struct map_checkout *curr = venda->produtos;
    fprintf(file, "%-4s %-50s %-10s\n", "Item", "Descrição", "Valor");
    while (curr != NULL)
    {
        fprintf(file, "%-4d %-37s x %-7.3f %8.2f\n", curr->index, curr->pc.descricao, curr->pc.quantidade, curr->pc.preco);
        curr = curr->prox;
    }

    fclose(file);
    return 0;
}