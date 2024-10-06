#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sqlite3.h>

#include "utils.h"
#include "vendas.h"
#include "colecoes.h"

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

        sprintf(sql, "INSERT INTO produtos_vendidos(id, venda_id, quantidade, valor_total) VALUES(NULL, '%d', %.3f, %.2f);",
                id_venda ,curr->pc.quantidade, curr->pc.preco);

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