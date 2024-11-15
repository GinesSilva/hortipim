#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include <stdbool.h>
#include <time.h>

#include "produto.h"
#include "fornecedores_repositorio.h"
#include "utils.h"
#include "vendas.h"
#include "estoque_repositorio.h"

double obter_quantidade(int codigo)
{
    sqlite3 *db;
    int rc;
    sqlite3_stmt *stmt;
    double quantidade;
    rc = sqlite3_open("hortifruti.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Nao foi possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    const char *sql = "SELECT * FROM produtos WHERE codigo = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    else
    {
        sqlite3_bind_int(stmt, 1, codigo);
        limpar_terminal();
        int res = sqlite3_step(stmt);
        if (res != SQLITE_ROW)
        {
            printf("Produto nao encontrado!\n");
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return -1;
        }
        else
        {
            quantidade = sqlite3_column_double(stmt, 5);
        }
    }

    printf("\n\n");

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return quantidade;
}
int cadastrar_produto(Produto p)
{
    sqlite3 *db;
    char sql[512];

    if (sqlite3_open("hortifruti.db", &db) != SQLITE_OK)
    {
        fprintf(stderr, "Nao foi possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    if (buscar_fornecedor(p.fornecedor_id) == false)
    {
        printf("Fornecedor nao esta cadastrado");
        return -1;
    };
    sprintf(sql, "INSERT INTO produtos(codigo, fornecedor_id, descricao, preco_de_compra, preco_de_venda, quantidade) VALUES(NULL, %d, '%s', %.2f, %.2f, %.3f);",
            p.fornecedor_id, p.descricao, p.preco_de_compra, p.preco_de_venda, 0.0);

    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao executar a consulta: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
        limpar_terminal();
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
        fprintf(stderr, "Nao foi possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    const char *sql = "SELECT * FROM produtos;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        limpar_terminal();
        printf("%-6s | %-20s | %-53s | %-6s | %-6s | %-14s\n", "Codigo", "Fornecedor", "Descrição", "Compra", "Venda", "Quantidade Kg");
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int codigo = sqlite3_column_int(stmt, 0);
            const unsigned char *fornecedor_id = buscar_id(sqlite3_column_int(stmt, 1));
            const unsigned char *descricao = sqlite3_column_text(stmt, 2);
            double preco_compra = sqlite3_column_double(stmt, 3);
            double preco_venda = sqlite3_column_double(stmt, 4);
            double quantidade = sqlite3_column_double(stmt, 5);

            printf("%-6d | %-20s | %-51s | %-6.2f | %-6.2f | %-14.3f\n", codigo, fornecedor_id, descricao, preco_compra, preco_venda, quantidade);
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
        fprintf(stderr, "Nao foi possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    const char *sql = "SELECT * FROM produtos WHERE codigo = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    else
    {
        sqlite3_bind_int(stmt, 1, codigo);
        limpar_terminal();
        int res = sqlite3_step(stmt);
        if (res != SQLITE_ROW)
        {
            printf("Produto nao encontrado!\n");
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
        else
        {
            printf("%-6s | %-20s | %-53s | %-6s | %-6s | %-14s\n", "Codigo", "Fornecedor", "Descrição", "Compra", "Venda", "Quantidade Kg");
            while (res == SQLITE_ROW)
            {
                int codigo = sqlite3_column_int(stmt, 0);
                const unsigned char *fornecedor_id = buscar_id(sqlite3_column_int(stmt, 1));
                const unsigned char *descricao = sqlite3_column_text(stmt, 2);
                double preco_compra = sqlite3_column_double(stmt, 3);
                double preco_venda = sqlite3_column_double(stmt, 4);
                double quantidade = sqlite3_column_double(stmt, 5);

                printf("%-6d | %-20s | %-51s | %-6.2f | %-6.2f | %-14.3f\n", codigo, fornecedor_id, descricao, preco_compra, preco_venda, quantidade);
                res = SQLITE_DONE;
            }
        }
    }

    printf("\n\n");

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int compra(int fornecedor_id, float quantidade, float preco)
{
    time_t t;
    struct tm *tm_info;
    char buffer[11];
    time(&t);
    tm_info = localtime(&t);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", tm_info);
    sqlite3 *db;
    char sql[512];
    float valor_total = quantidade * preco;
    if (sqlite3_open("hortifruti.db", &db) != SQLITE_OK)
    {
        fprintf(stderr, "Nao foi possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    if (buscar_fornecedor(fornecedor_id) == false)
    {
        printf("Fornecedor nao esta cadastrado");
        return -1;
    };
    sprintf(sql, "INSERT INTO compras(id, fornecedor_id, quantidade, valor_total, data_compra) VALUES(NULL, '%d', %.3f, %.2f, '%s');", fornecedor_id, quantidade, valor_total, buffer);

    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao executar a consulta: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
        limpar_terminal();
        printf("compra cadastrada com sucesso.\n");
    }

    sqlite3_close(db);

    return 0;
}

int id_fornecedor(int codigo)
{
    sqlite3 *db;
    int rc;
    sqlite3_stmt *stmt;
    rc = sqlite3_open("hortifruti.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Nao foi possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
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
        limpar_terminal();
        printf("%-6s | %-20s | %-53s | %-6s | %-6s | %-14s\n", "Codigo", "Fornecedor", "Descrição", "Compra", "Venda", "Quantidade Kg");
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int fornecedor_id = sqlite3_column_int(stmt, 1);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return fornecedor_id;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

int entrada_produtos(int codigo, float quantidadeEntrada, float preco)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = 0;
    int rc;

    rc = sqlite3_open("hortifruti.db", &db);
    if (rc)
    {
        fprintf(stderr, "Nao foi possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao iniciar a transacao: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return rc;
    }

    const char *sql = "UPDATE produtos SET quantidade = quantidade + ?, preco_de_compra = ? WHERE codigo = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar o SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    sqlite3_bind_double(stmt, 1, quantidadeEntrada);
    sqlite3_bind_double(stmt, 2, preco);
    sqlite3_bind_int(stmt, 3, codigo);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Erro ao executar o SQL: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        int id = id_fornecedor(codigo);
        if (id <= 0)
        {
            return -1;
        }
        else
        {
            rc = sqlite3_exec(db, "COMMIT;", 0, 0, &errMsg);
            if (rc != SQLITE_OK)
            {
                fprintf(stderr, "Erro ao finalizar a transacao: %s\n", errMsg);
                sqlite3_free(errMsg);
                return rc;
            }
            else
            {
                sqlite3_close(db);
                compra(id, quantidadeEntrada, preco);
            }
        }
    }
    sqlite3_close(db);
    sqlite3_finalize(stmt);
    return 0;
}

int saida_produtos(int codigo, float saida)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = 0;
    int rc;

    double qtde = obter_quantidade(codigo);

    if(qtde - saida < 0) {
        printf("A quantidade de saida nao pode ser maior que %.2f", qtde);
        return -1;
    }

    rc = sqlite3_open("hortifruti.db", &db);
    if (rc)
    {
        fprintf(stderr, "Nao foi possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
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

    sqlite3_bind_double(stmt, 1, saida);
    sqlite3_bind_int(stmt, 2, codigo);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {

        fprintf(stderr, "Erro ao executar o SQL: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("Produto jogado na quebra!\n\n");
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);
    return 0;
}

int atualizar_preco(int codigo, float novo_preco)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = 0;
    int rc;

    rc = sqlite3_open("hortifruti.db", &db);
    if (rc)
    {
        fprintf(stderr, "Nao foi possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    const char *sql = "UPDATE produtos SET preco_de_venda = ? WHERE codigo = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar o SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    sqlite3_bind_double(stmt, 1, novo_preco);
    sqlite3_bind_int(stmt, 2, codigo);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Erro ao executar o SQL: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("Atualizacao realizada com sucesso!\n");
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);
    return 0;
}

Resultado buscar_produto(int codigo)
{
    ProdutoCheckout p;
    sqlite3 *db;
    int rc;
    sqlite3_stmt *stmt;
    rc = sqlite3_open("hortifruti.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Nao foi possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        Resultado r;
        r.cod = -1;
        return r;
    }
    const char *sql = "SELECT * FROM produtos WHERE codigo = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        Resultado r;
        r.cod = -1;
        return r;
    }
    else
    {
        sqlite3_bind_int(stmt, 1, codigo);
        limpar_terminal();
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            const unsigned char *descricao = sqlite3_column_text(stmt, 2);
            double preco_venda = sqlite3_column_double(stmt, 4);
            double quantidade = sqlite3_column_double(stmt, 5);
            p.preco = preco_venda;
            p.quantidade = quantidade;
            strcpy(p.descricao, descricao);
        }
        else
        {
            Resultado r;
            r.cod = -1;
            return r;
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    Resultado r;
    r.p = p;
    r.cod = 0;
    return r;
}

int relatorio_produto(int codigo)
{
    sqlite3 *db;
    int rc;
    sqlite3_stmt *stmt;
    rc = sqlite3_open("hortifruti.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Nao foi possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    const char *sql = "SELECT pv.quantidade, pv.valor_total AS valor, v.data_venda, v.total, p.descricao FROM produtos_vendidos pv INNER JOIN vendas v ON v.id == pv.venda_id INNER JOIN produtos p ON p.codigo = ? WHERE pv.codigo = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    else
    {
        sqlite3_bind_int(stmt, 1, codigo);
        sqlite3_bind_int(stmt, 2, codigo);
        limpar_terminal();
        printf("%-10s | %-10s | %-25s | %-30s | %-6s | %13s\n", "Quantidade", "valor", "Data", "Descricao", "Total", "Total Compra");
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            double quantidade = sqlite3_column_double(stmt, 0);
            double valor = sqlite3_column_double(stmt, 1);
            const unsigned char *data = sqlite3_column_text(stmt, 2);
            double total = valor * quantidade;
            double total_compra = sqlite3_column_double(stmt, 3);
            const unsigned char *descricao = sqlite3_column_text(stmt, 4);

            printf("%-10.3f | %-10.2f | %-25s | %-30s | %-6.2f | %-6.2f\n", quantidade, valor, data, descricao, total, total_compra);
        }
    }
    printf("\n\n");

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}