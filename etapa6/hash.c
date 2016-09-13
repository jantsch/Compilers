/*
// UFRGS - Compiladores B - Marcelo Johann - 2014/1 - Etapa 6
//
// Alunos: Rodrigo Jantsch e Gustavo Spier.
//
// Matrículas: 207301 e 213991.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "astree.h"
#include "hash.h"
#include "y.tab.h"

static int hash_i = 1;

extern HASH_TABLE Table;

void hashInit (HASH_TABLE *Table)
{
	int i;

	for (i = 0; i < hash_i*HASH_SIZE; ++i)
	{
		Table -> node[i] = 0;
	}
	Table -> usedEntries = 0;
}

int hashAddress (HASH_TABLE *Table, char *text)
{
	int address;
	int i;

	address = 1;
	for (i = 0; i < strlen(text); ++i)
	{
		address = (address * text[i]) % (hash_i*HASH_SIZE + 1);
	}
	return address - 1;
}

void hashResize(HASH_TABLE *Table)
{
	int i;
	hash_i = hash_i*2;
	for(i = HASH_SIZE; i<hash_i*HASH_SIZE; ++i)
	{
		Table -> node[i] = 0;
	}
//	printf("Table resized, now size is %d\n", hash_i*HASH_SIZE);
}

HASH_NODE *hashInsert (HASH_TABLE *Table, char *text, int type, int lineNumber)
{
	HASH_NODE *node;
	int address;

	// Check if the table is getting full and resize it
	if (Table->usedEntries > hash_i*HASH_SIZE/2)
	{
		hashResize(Table);
	}

	address = 0;
	node  = hashFind(Table, text, type);

	// First check if it is in the hash
	if (node == 0)
	{
		address = hashAddress(Table, text);
		HASH_NODE *newNode;
		newNode = (HASH_NODE*)calloc(1, sizeof(HASH_NODE));
		newNode -> type = type;
		newNode -> lineNumber = lineNumber;
		newNode -> text = (char*)calloc(sizeof(strlen(text)+1), sizeof(strlen(text)+1));
		strcpy(newNode -> text, text);
		newNode -> next = Table -> node[address];
		Table -> node[address] = newNode;
		Table -> usedEntries++;
		return newNode;
	}
	else
	{
		return node;
	}

}

HASH_NODE *hashFind (HASH_TABLE *Table, char *text, int type)
{
	int address;
	int i;
	HASH_NODE *pt;

	address = hashAddress(Table, text);

	for (i = 0; i < hash_i*HASH_SIZE; ++i)
	{
		for (pt = Table -> node[i]; pt; pt = pt -> next)
		{
			if (!strcmp(pt -> text, text)){
				return pt;
			}
		}
	}

	return 0;
}

void hashPrint (HASH_TABLE *Table)
{
	int i;
	HASH_NODE *pt;

	printf("\n");
	for (i = 0; i < hash_i*HASH_SIZE; ++i)
	{
		for (pt = Table -> node[i]; pt; pt = pt -> next)
		{
			printf("Token[%d]: %s \n", i, pt -> text);
		}
	}
	printf("Table has %d entries\n", Table->usedEntries);
}

void hashCheckUndeclared(HASH_TABLE *Table, int * errors)
{
	int i;
	HASH_NODE *pt;

	for (i = 0; i < hash_i*HASH_SIZE; ++i)
	{
		for (pt = Table -> node[i]; pt; pt = pt -> next)
		{
			if (pt->type == SYMBOL_IDENTIFIER)
			{
				*errors = *errors + 1;
				printf("LINE: %d - Undeclared Identifier (%s)\n", pt->lineNumber, pt->text);
			}
		}
	}

}

HASH_NODE *makeLabel()
{
    static int nextLabel = 0;
    char buffer[128];

    sprintf(buffer, "__Label%d", nextLabel);
    nextLabel++;
    return hashInsert (&Table, buffer, SYMBOL_LABEL, 0);
}
HASH_NODE *makeTemp()
{
    static int nextLabel = 0;
    char buffer[128];

    sprintf(buffer, "__Temp%d", nextLabel);
    nextLabel++;
    return hashInsert (&Table, buffer, SYMBOL_VARIABLE, 0);
}

void gen_declarations (FILE *output, HASH_TABLE *table)
{
	int i;
	HASH_NODE *node = 0;

 	for (i = 0; i < HASH_SIZE+1; ++i) 
 	{
        for (node = table->node[i]; node != NULL; node = node->next) 
        {
    		int aux=0;
			switch(node->type)
			{

				case SYMBOL_VECTOR:
					//aux = 4*atoi(node->arraySize);
					//aux = 4*10; //por enquanto
					//fprintf(output, 
					//		"\n\t.comm	%s,%d,32\n"
					//		, node->lexem, aux);	
					break;
				case TK_IDENTIFIER: 
							/*switch(node->tacType){
								case TAC_VETREAD:
									break;
								case TAC_LABEL:
									break;
							*/
								//default:
								fprintf(output, 
								"\n"
								"%s: \n"	
								"\t.long	0\n"
								, node->type);	
								break;		

							//}
					break;			
				default: break;
			}
		}
	}
}