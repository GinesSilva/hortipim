#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

int produtos() {
  sqlite3 *db;
  char sql[512];

  if (sqlite3_open("hortifruti.db", &db) != SQLITE_OK)
  {
      fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
      return -1;
  }

  sprintf(sql, "CREATE TABLE IF NOT EXISTS produtos(codigo INTEGER PRIMARY KEY, fornecedor_id INTEGER, descricao VARCHAR(50), preco_de_compra DECIMAL(10,2), preco_de_venda DECIMAL(10,2), quantidade DECIMAL(10,3));");

  char *err_msg = NULL;
  if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK)
  {
      fprintf(stderr, "Erro ao executar a consulta: %s\n", err_msg);
      sqlite3_free(err_msg);
  }
  else
  {
      printf("Tabela criada com sucesso.\n");
  }

  sqlite3_close(db);
  return 0;
}

int fornecedores() {
  sqlite3 *db;
  char sql[512];

  if (sqlite3_open("hortifruti.db", &db) != SQLITE_OK)
  {
      fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
      return -1;
  }

  sprintf(sql, "CREATE TABLE IF NOT EXISTS fornecedores(id INTEGER PRIMARY KEY, cnpj VARCHAR(14) UNIQUE, inscricao_estadual VARCHAR(12) razao_social VARCHAR(50) UNIQUE, nome_fantasia VARCHAR(20));");

  char *err_msg = NULL;
  if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK)
  {
      fprintf(stderr, "Erro ao executar a consulta: %s\n", err_msg);
      sqlite3_free(err_msg);
  }
  else
  {
      printf("Tabela criada com sucesso.\n");
  }

  sqlite3_close(db);
  return 0;
}

int vendas() {
  sqlite3 *db;
  char sql[512];

  if (sqlite3_open("hortifruti.db", &db) != SQLITE_OK)
  {
      fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
      return -1;
  }

  sprintf(sql, "CREATE TABLE IF NOT EXISTS vendas(id INTEGER PRIMARY KEY, data_venda DATETIME, documento_cliente VARCHAR(14), total DECIMAL(10,2), troco DECIMAL(10,2));");

  char *err_msg = NULL;
  if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK)
  {
      fprintf(stderr, "Erro ao executar a consulta: %s\n", err_msg);
      sqlite3_free(err_msg);
  }
  else
  {
      printf("Tabela criada com sucesso.\n");
  }

  sqlite3_close(db);
  return 0;
}

int produtos_vendidos_tabela() {
  sqlite3 *db;
  char sql[512];

  if (sqlite3_open("hortifruti.db", &db) != SQLITE_OK)
  {
      fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
      return -1;
  }

  sprintf(sql, "CREATE TABLE IF NOT EXISTS produtos_vendidos(id INTEGER PRIMARY KEY, codigo INTEGER, venda_id INTEGER, quantidade DECIMAL(10,3), valor_total DECIMAL(10,2));");

  char *err_msg = NULL;
  if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK)
  {
      fprintf(stderr, "Erro ao executar a consulta: %s\n", err_msg);
      sqlite3_free(err_msg);
  }
  else
  {
      printf("Tabela criada com sucesso.\n");
  }

  sqlite3_close(db);
  return 0;
}

int compras() {
  sqlite3 *db;
  char sql[512];

  if (sqlite3_open("hortifruti.db", &db) != SQLITE_OK)
  {
      fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
      return -1;
  }

  sprintf(sql, "CREATE TABLE IF NOT EXISTS compras(id INTEGER PRIMARY KEY, fornecedor_id INTEGER, quantidade DECIMAL(10,3), valor_total DECIMAL(10,2), data_compra DATE);");

  char *err_msg = NULL;
  if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK)
  {
      fprintf(stderr, "Erro ao executar a consulta: %s\n", err_msg);
      sqlite3_free(err_msg);
  }
  else
  {
      printf("Tabela criada com sucesso.\n");
  }

  sqlite3_close(db);
  return 0;
}

void criarBanco() {
  sqlite3 *db;
  char sql[512];

  if (sqlite3_open("hortifruti.db", &db) != SQLITE_OK)
  {
      fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
  }
  sqlite3_close(db);

  produtos();
  fornecedores();
  vendas();
  produtos_vendidos_tabela();
  compras();
}
