/*
UFRGS - Compiladores B - Marcelo Johann - 2014/1 - Etapa 1

Alunos: Rodrigo Jantsch e Gustavo Spier.

Matrículas: 207301 e 213991.
*/

#include <stdio.h>
/*#include "hash.c"*/

typedef struct nome_interno{
    int token;
    char *text;
    struct nome_interno *next;

}HASH_NODE;

void hash_init(void);
int hash_address(char *text);
HASH_NODE *hash_find(char *text);
HASH_NODE *hash_insert(char *text, int token);
void hash_print(void);
