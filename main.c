#include <stdio.h>
#include <stdlib.h>
#include "estoque.h"
#include "fornecedores.h"
#include "vendas.h"

int main()
{
    entrada_de_produtos();
    saida_de_produtos();
    relatorio_de_vendas_por_produto();
    cadastro_de_produtos();
    registrar_venda();
    relatorio_venda_dia();
    cadastrar_fornecedores();
    historico_de_compras(4);
    return 0;
}
