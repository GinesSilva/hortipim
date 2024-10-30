#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stdbool.h>

#include "fornecedores.h"
#include "utils.h"

int cadastro_fornecedor(Fornecedor fornecedor)
{
    sqlite3 *db;
    char sql[512];

    if (sqlite3_open("hortifruti.db", &db) != SQLITE_OK)
    {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sprintf(sql, "INSERT INTO fornecedores(id, cnpj, razao_social, nome_fantasia, inscricao_estadual) VALUES(NULL, '%s', '%s', '%s', '%s');",
            fornecedor.cnpj, fornecedor.razao_social, fornecedor.nome_fantasia, fornecedor.inscricao_estadual);

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

int listar_todos_fornecedores()
{
    limpar_buffer();
    limpar_terminal();
    sqlite3 *db;
    int rc;
    sqlite3_stmt *stmt;
    rc = sqlite3_open("hortifruti.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    const char *sql = "SELECT * FROM fornecedores;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        limpar_terminal();
        printf("%-6s | %-14s | %-51s | %-21s\n", "Id", "CNPJ", "Razão social", "Nome Fantasia");
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char *cnpj = sqlite3_column_text(stmt, 1);
            const unsigned char *razao_social = sqlite3_column_text(stmt, 3);
            const unsigned char *fantasia = sqlite3_column_text(stmt, 4);
            printf("%-6d | %-14s | %-51s | %-21s\n", id, cnpj, razao_social, fantasia);
        }
    }

    printf("\n\n");

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int buscar_por_cnpj(char *cnpj)
{
    sqlite3 *db;
    int rc;
    sqlite3_stmt *stmt;
    int id;
    rc = sqlite3_open("hortifruti.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    const char *sql = "SELECT * FROM fornecedores WHERE cnpj = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, cnpj, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        id = sqlite3_column_int(stmt, 0);
    }
    else
    {
        printf("Nenhum resultado encontrado.\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return id;
}

int listar_historico_compra_fornecedor(int id_entrada)
{
    sqlite3 *db;
    int rc;
    sqlite3_stmt *stmt;
    rc = sqlite3_open("hortifruti.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    const char *sql = "SELECT * FROM compras WHERE fornecedor_id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        sqlite3_bind_int(stmt, 1, id_entrada);
        limpar_terminal();
        printf("%-6s | %-20s | %-14s | %-6s  | %-10s\n", "Id", "Fornecedor","Quantidade Kg", "Total", "Data");
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char *fornecedor = buscar_id(sqlite3_column_int(stmt, 1));
            double quantidade = sqlite3_column_double(stmt, 2);
            double total = sqlite3_column_double(stmt, 3);
            const unsigned char *data_compra = sqlite3_column_text(stmt, 4);

            printf("%-6d | %-20s | %-14.3f | %-6.2f | %-10s\n", id ,fornecedor, quantidade, total, data_compra);

        }
    }

    printf("\n\n");

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int vendas_produto() {
    
}