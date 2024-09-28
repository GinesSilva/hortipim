#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stdbool.h>

#include "fornecedores.h"

int cadastro_fornecedor(Fornecedor fornecedor)
{
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

bool buscar_fornecedor(int id)
{
    sqlite3 *db;
    int rc;
    sqlite3_stmt *stmt;

    rc = sqlite3_open("hortifruti.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return false;
    }

    const char *sql = "SELECT * FROM fornecedores WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    }
    else
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }
}

char *buscar_id(int id)
{
    sqlite3 *db;
    int rc;
    sqlite3_stmt *stmt;
    char *res = (char *)malloc(21 * sizeof(char));
    if (res == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória.\n");
        return "";
    }

    rc = sqlite3_open("hortifruti.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        free(res);
        return "";
    }

    const char *sql = "SELECT * FROM fornecedores WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        free(res);
        return "";
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const unsigned char *fantasia = sqlite3_column_text(stmt, 3);
        if (fantasia != NULL)
        {
            strncpy(res, (const char *)fantasia, 20);
            res[20] = '\0';
        }
        else
        {
            printf("Nenhum resultado encontrado.\n");
        }
    }
    else
    {
        printf("Nenhum resultado encontrado.\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return res;
}