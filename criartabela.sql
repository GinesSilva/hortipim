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
    cnpj VARCHAR(14) UNIQUE,
    razao_social VARCHAR(50) UNIQUE,
    nome_fantasia VARCHAR(20)
);

CREATE TABLE vendas(
    id INTEGER PRIMARY KEY,
    data_venda DATETIME,
    documento_cliente VARCHAR(14),
    total DECIMAL(10,2),
    troco DECIMAL(10,2)
);

CREATE TABLE produtos_vendidos(
    id INTEGER PRIMARY KEY,
    codigo INTEGER,
    venda_id INTEGER,
    quantidade DECIMAL(10,3),
    valor_total DECIMAL(10,2)
);

CREATE TABLE compras(
    id INTEGER PRIMARY KEY,
    fornecedor_id INTEGER,
    quantidade DECIMAL(10,3),
    valor_total DECIMAL(10,2),
    data_compra DATE
);