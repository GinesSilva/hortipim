#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "vendas.h"

int emitir_nota_fiscal() {
    printf("Nota fiscal emitida...\n\n");
    return 0;
}

int registrar_venda() {
    //TODO
    printf("Registrar venda\n\n");
    emitir_nota_fiscal();
    return 0;
}

int relatorio_venda_dia() {
    
    time_t t;
    struct tm *tm_info;
    char buffer[20];
    time(&t);
    tm_info = localtime(&t);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", tm_info);
    //strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", tm_info);
    printf("Data e hora atual: %s\n", buffer);
    printf("Relatorio de venda do dia 01/01/2000\n\n");
    return 0;
}
