#include <stdio.h>
#include <stdlib.h>
#include "estoque.h"


int main()
{
    entrada_de_produtos();
    saida_de_produtos();
    relatorio_de_vendas_por_produto();
    cadastro_de_produtos();
    registrar_venda();
    relatorio_venda_dia();
    return 0;
}
