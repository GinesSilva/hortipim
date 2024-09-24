#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sqlite3.h>

#include "fornecedores.h"

int cadastro_fornecedor(Fornecedor fornecedor) {
    sqlite3 *db;
    char sql[512];

    if (sqlite3_open("hortifruti.db", &db) != SQLITE_OK)
    {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sprintf(sql, "INSERT INTO fornecedores(id, cnpj, razao_social, nome_fantasia) VALUES(NULL, '%s', '%s', '%s');",
            fornecedor.cnpj, fornecedor.razao_social, fornecedor.nome_fantasia);

    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao executar a consulta: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
        printf("Fornecedor cadastrado com sucesso.\n");
    }

    sqlite3_close(db);

    return 0;
}