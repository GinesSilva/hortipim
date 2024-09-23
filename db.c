#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>

#include "produto.h"

int cadastrar_produto(Produto p)
{
    sqlite3 *db;
    char sql[512];
    
    if (sqlite3_open("hortifruti.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sprintf(sql, "INSERT INTO produtos(codigo, fornecedor_id, descricao, preco_de_compra, preco_de_venda, quantidade) VALUES(NULL, %d, '%s', %.2f, %.2f, %.2f);",
            p.fornecedor_id, p.descricao, p.preco_de_compra, p.preco_de_venda, p.quantidade);

    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Erro ao executar a consulta: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Produto inserido com sucesso.\n");
    }

    sqlite3_close(db);

    return 0;
}