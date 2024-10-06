#ifndef VENDAS_REPOSITORIO
#define VENDAS_REPOSITORIO

#include "vendas.h"

int registrar_nova_venda(struct venda *venda);

int emitir_nota_fiscal(struct venda *venda);

int relatorio_venda_dia_banco(char *data);


#endif