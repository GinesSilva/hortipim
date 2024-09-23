# INSTRUÇÕES

## Instalação sqlite

<h4>Debian</h4>

``` bash
    apt-get install sqlite3 libsqlite3-dev
```

<h4>Windows</h4>

<p>Tem esse vídeo com tutorial</p>
[Instalação sqlite](https://www.youtube.com/watch?v=gsZmB_MMKyc&t=315s)

## Tabelas Banco de dados

```sql
CREATE TABLE produtos(
    codigo INTEGER PRIMARY KEY,
    fornecedor_id INTEGER,
    descricao VARCHAR(50),
    preco_de_compra DECIMAL(10,2),
    preco_de_venda DECIMAL(10,2),
    quantidade DECIMAL(10,3)
);

CREATE TABLE fornecedores(
    id INTEGER PRIMARY KEY,
    cnpj VARCHAR(14),
    razao_social VARCHAR(50),
    nome_fantasia VARCHAR(20)
);
```


## Compilação 

<h4>Debian</h4>

<p>Para compilar e executar basta rodar esse comando na pasta do projeto</p>

``` bash
    gcc *.c -o main.o -lsqlite3 && ./main.o
```

<h4>Windows</h4>

<p>Para compilar no windows</p>
<h4>Não foi testado</h4>
<p>* Lembrar de alterar os caminhos no comando</p>

``` cmd
    gcc *.c -o main.exe -I"C:\caminho\para\sqlite\include" -L"C:\caminho\para\sqlite\lib" -lsqlite3
```