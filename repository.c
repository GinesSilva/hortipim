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

    if (sqlite3_open("hortifruti.db", &db) != SQLITE_OK)
    {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sprintf(sql, "INSERT INTO produtos(codigo, fornecedor_id, descricao, preco_de_compra, preco_de_venda, quantidade) VALUES(NULL, %d, '%s', %.2f, %.2f, %.2f);",
            p.fornecedor_id, p.descricao, p.preco_de_compra, p.preco_de_venda, p.quantidade);

    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao executar a consulta: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
        printf("Produto inserido com sucesso.\n");
    }

    sqlite3_close(db);

    return 0;
}

int listar_todos()
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
    const char *sql = "SELECT * FROM produtos;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        system("clear");
        printf("%-6s | %-11s | %-53s | %-6s | %-6s | %-14s\n", "Codigo", "Fornecedor", "Descrição", "Compra", "Venda", "Quantidade Kg");
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int codigo = sqlite3_column_int(stmt, 0);
            int fornecedor_id = sqlite3_column_int(stmt, 1);
            const unsigned char *descricao = sqlite3_column_text(stmt, 2);
            double preco_compra = sqlite3_column_double(stmt, 3);
            double preco_venda = sqlite3_column_double(stmt, 4);
            double quantidade = sqlite3_column_double(stmt, 5);

            printf("%-6d | %-11d | %-51s | %-6.2f | %-6.2f | %-14.3f\n", codigo, fornecedor_id, descricao, preco_compra, preco_venda, quantidade);
        }
    }

    printf("\n\n");

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int listar_codigo(int codigo)
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
    const char *sql = "SELECT * FROM produtos WHERE codigo = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        sqlite3_bind_int(stmt, 1, codigo);
        system("clear");
        printf("%-6s | %-11s | %-53s | %-6s | %-6s | %-14s\n", "Codigo", "Fornecedor", "Descrição", "Compra", "Venda", "Quantidade Kg");
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int codigo = sqlite3_column_int(stmt, 0);
            int fornecedor_id = sqlite3_column_int(stmt, 1);
            const unsigned char *descricao = sqlite3_column_text(stmt, 2);
            double preco_compra = sqlite3_column_double(stmt, 3);
            double preco_venda = sqlite3_column_double(stmt, 4);
            double quantidade = sqlite3_column_double(stmt, 5);

            printf("%-6d | %-11d | %-51s | %-6.2f | %-6.2f | %-14.3f\n", codigo, fornecedor_id, descricao, preco_compra, preco_venda, quantidade);
        }
    }

    printf("\n\n");

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int entrada_produtos(int codigo, int quantidadeEntrada)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = 0;
    int rc;

    rc = sqlite3_open("hortifruti.db", &db);
    if (rc)
    {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    const char *sql = "UPDATE produtos SET quantidade = quantidade + ? WHERE codigo = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar o SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    sqlite3_bind_int(stmt, 1, quantidadeEntrada);
    sqlite3_bind_int(stmt, 2, codigo);  

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Erro ao executar o SQL: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("Atualização realizada com sucesso!\n");
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);
    return 0;
}

int saida_produtos(int codigo, int quantidadeEntrada)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = 0;
    int rc;

    rc = sqlite3_open("hortifruti.db", &db);
    if (rc)
    {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    const char *sql = "UPDATE produtos SET quantidade = quantidade - ? WHERE codigo = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar o SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    sqlite3_bind_int(stmt, 1, quantidadeEntrada);
    sqlite3_bind_int(stmt, 2, codigo);  

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Erro ao executar o SQL: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("Atualização realizada com sucesso!\n");
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);
    return 0;
}