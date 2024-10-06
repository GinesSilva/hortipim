#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *get_os()
{
#ifdef _WIN32
    return "Windows";
#elif defined(_WIN64)
    return "Windows 64-bit";
#elif defined(__linux__)
    return "Linux";
#elif defined(__APPLE__) && defined(__MACH__)
    return "macOS";
#elif defined(__unix__)
    return "Unix";
#else
    return "Desconhecido";
#endif
}

void limpar_terminal()
{
    if (strcmp(get_os(), "Windows") == 0 || strcmp(get_os(), "Windows 64-bit") == 0)
    {
        system("cls");
    }
    else
    {
        system("clear");
    }
}

void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void split(char *str, char antigo, char novo)
{
    while (*str)
    {
        if (*str == antigo)
        {               
            *str = novo; 
        }
        str++;
    }
}