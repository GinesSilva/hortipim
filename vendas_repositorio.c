#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "sqlite3.h"

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
            fprintf(stderr, "Nao foi possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
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
        fprintf(stderr, "Nao foi possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
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
    strcpy(nome_arquivo, "nf_");
    strcat(nome_arquivo, venda->data_venda);
    strcat(nome_arquivo, ".txt");
    replace(nome_arquivo, ' ', '_');
    replace(nome_arquivo, '/', '_');
    replace(nome_arquivo, ':', '_');
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
    fprintf(file, "%-4s %-50s %-10s\n", "Item", "Descricao", "Valor");
    while (curr != NULL)
    {
        fprintf(file, "%-4d %-37s x %-7.3f %8.2f\n", curr->index, curr->pc.descricao, curr->pc.quantidade, curr->pc.preco);
        curr = curr->prox;
    }

    fclose(file);
    return 0;
}

int relatorio_venda_dia_banco(char *data)
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
    const char *sql = "SELECT v.data_venda, v.total, v.troco FROM vendas v WHERE CAST(data_venda AS CHAR) LIKE ?;";
    double total_dia = 0;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    else
    {
        char data_param[25];
        strcpy(data_param, data);
        strcat(data_param, "%");
        sqlite3_bind_text(stmt, 1, data_param, -1, SQLITE_STATIC);
        limpar_terminal();
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            printf("%-25s | %-6s | %5s\n", "Data", "Total", "Troco");
            printf("--------------------------------------------------------\n");
            const unsigned char *data = sqlite3_column_text(stmt, 0);
            int dia, mes, ano, h, m, s;
            size_t length = strlen((const char *)data);
            unsigned char *datafIn = malloc(length + 1);
            unsigned char *datafOut = malloc(length + 1);
            if (datafIn)
            {
                strcpy((char *)datafIn, (const char *)data);
            }
            sscanf(datafIn, "%d-%d-%d %d:%d:%d", &ano, &mes, &dia, &h, &m, &s);
            sprintf(datafOut, "%02d/%02d/%d %02d:%02d:%02d", dia, mes, ano, h, m, s);
            double total = sqlite3_column_double(stmt, 1);
            double troco = sqlite3_column_double(stmt, 2);
            printf("%-25s | %-6.2f | %-6.2f\n", datafOut, total, troco);
            total_dia += total;
            free(datafIn);
            free(datafOut);
        }
        else
        {
            printf("Nao foi feita nenhuma venda hoje: %s\n\n", data);
            return -1;
        }
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            const unsigned char *data = sqlite3_column_text(stmt, 0);
            int dia, mes, ano, h, m, s;
            size_t length = strlen((const char *)data);
            unsigned char *datafIn = malloc(length + 1);
            unsigned char *datafOut = malloc(length + 1);
            if (datafIn)
            {
                strcpy((char *)datafIn, (const char *)data);
            }
            sscanf(datafIn, "%d-%d-%d %d:%d:%d", &ano, &mes, &dia, &h, &m, &s);
            sprintf(datafOut, "%02d/%02d/%d %02d:%02d:%02d", dia, mes, ano, h, m, s);
            double total = sqlite3_column_double(stmt, 1);
            double troco = sqlite3_column_double(stmt, 2);
            printf("%-25s | %-6.2f | %-6.2f\n", datafOut, total, troco);
            total_dia += total;
            free(datafIn);
            free(datafOut);
        }
    }
    printf("\n\n");
    printf("Total do dia: R$ %.2f", total_dia);
    printf("\n\n");

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

int relatorio_periodo(char *inicio, char *final)
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
    const char *sql = "SELECT v.data_venda, v.total, v.troco FROM vendas v WHERE data_venda BETWEEN ? AND ?;";
    double total_dia = 0;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    else
    {
        char inicio_param[25];
        char final_param[25];
        strcpy(inicio_param, inicio);
        strcat(inicio_param, " 00:00:00");
        strcpy(final_param, final);
        strcat(final_param, " 23:59:59");
        sqlite3_bind_text(stmt, 1, inicio_param, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, final_param, -1, SQLITE_STATIC);
        limpar_terminal();
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            printf("%-25s | %-6s | %5s\n", "Data", "Total", "Troco");
            printf("--------------------------------------------------------\n");
            const unsigned char *data = sqlite3_column_text(stmt, 0);
            int dia, mes, ano, h, m, s;
            size_t length = strlen((const char *)data);
            unsigned char *datafIn = malloc(length + 1);
            unsigned char *datafOut = malloc(length + 1);
            if (datafIn)
            {
                strcpy((char *)datafIn, (const char *)data);
            }
            sscanf(datafIn, "%d-%d-%d %d:%d:%d", &ano, &mes, &dia, &h, &m, &s);
            sprintf(datafOut, "%02d/%02d/%d %02d:%02d:%02d", dia, mes, ano, h, m, s);
            double total = sqlite3_column_double(stmt, 1);
            double troco = sqlite3_column_double(stmt, 2);
            printf("%-25s | %-6.2f | %-6.2f\n", datafOut, total, troco);
            total_dia += total;
        }
        else {
            printf("Sem vendas nesse periodo de datas!\n\n");
            return -1;
        }
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            const unsigned char *data = sqlite3_column_text(stmt, 0);
            int dia, mes, ano, h, m, s;
            size_t length = strlen((const char *)data);
            unsigned char *datafIn = malloc(length + 1);
            unsigned char *datafOut = malloc(length + 1);
            if (datafIn)
            {
                strcpy((char *)datafIn, (const char *)data);
            }
            sscanf(datafIn, "%d-%d-%d %d:%d:%d", &ano, &mes, &dia, &h, &m, &s);
            sprintf(datafOut, "%02d/%02d/%d %02d:%02d:%02d", dia, mes, ano, h, m, s);
            double total = sqlite3_column_double(stmt, 1);
            double troco = sqlite3_column_double(stmt, 2);
            printf("%-25s | %-6.2f | %-6.2f\n", datafOut, total, troco);
            total_dia += total;
        }
    }
    printf("\n\n");
    printf("Total do periodo: R$ %.2f", total_dia);
    printf("\n\n");

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}