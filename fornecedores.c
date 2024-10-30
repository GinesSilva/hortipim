#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fornecedores.h"
#include "fornecedores_repositorio.h"
#include "utils.h"

int cadastrar_fornecedores()
{
    bool cont = true;
    do
    {
        limpar_terminal();
        limpar_buffer();
        char cnpj[15];
        char ie[14];
        char razao_social[256];
        char fantasia[256];
        printf("Cadastrar fornecedor...\n\n");
        printf("Insira o cnpj (apenas números)\n\n>>");
        fgets(cnpj, sizeof(cnpj), stdin);
        limpar_buffer();
        printf("Insira a inscrição estadual (apenas números)\n\n>>");
        fgets(ie, sizeof(ie), stdin);
        ie[strcspn(ie, "\n")] = 0;
        printf("Insira a razão social\n\n>>");
        fgets(razao_social, sizeof(razao_social), stdin);
        razao_social[strcspn(razao_social, "\n")] = 0;
        printf("Insira o nome fantasia\n\n>>");
        fgets(fantasia, sizeof(fantasia), stdin);
        fantasia[strcspn(fantasia, "\n")] = 0;

        printf("Os dados estão corretos?(s/n)\n\n");
        printf("cnpj: %s\nI.E.: %s\nrazão social: %s\nnome fantasia: %s\n\n>>", cnpj, ie, razao_social, fantasia);

        char opt;
        scanf("%c", &opt);

        switch (opt)
        {
        case 's':
            Fornecedor f;
            strcpy(f.cnpj, cnpj);
            strcpy(f.nome_fantasia, fantasia);
            strcpy(f.razao_social, razao_social);
            strcpy(f.inscricao_estadual, ie);
            cont = false;
            cadastro_fornecedor(f);
            break;
        case 'n':
            cont = true;
            break;
        default:
            cont = true;
            break;
        }
    } while(cont == true);

    return 0;
}

int historico_de_compras()
{
    char cnpj[15];
    limpar_terminal();
    limpar_buffer();
    printf("Insira o cnpj (apenas numeros)\n\n>>");
    fgets(cnpj, sizeof(cnpj), stdin);
    int id = buscar_por_cnpj(cnpj);
    if (id <= 0)
    {
        printf("Fornecedor não encontrado!\n\n");
        return -1;
    }
    listar_historico_compra_fornecedor(id);
    return 0;
}

int listar_fornecedores()
{
    listar_todos_fornecedores();
    return 0;
}
