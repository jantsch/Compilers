/*
UFRGS - Compiladores B - Marcelo Johann - 2014/1 - Etapa 1

Alunos: Rodrigo Jantsch e Gustavo Spier.

Matr�culas: 207301 e 213991.
*/

int main(int argv, char **argc)
{
    int token;
    yyin = fopen("teste.txt","r");
    while(running)
	{
        token = yylex();
        if(!running) break;
    }

    hash_print();
    return 0;
}

