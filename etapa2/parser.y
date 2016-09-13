/*
UFRGS - Compiladores B - Marcelo Johann - 2014/1 - Etapa 1

Alunos: Rodrigo Jantsch e Gustavo Spier.

Matrículas: 207301 e 213991.
*/


%{

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

%}

%union
{
HASH_NODE *symbol;
int number;
}


%token KW_WORD		256
%token KW_BOOL		258
%token KW_BYTE		259
%token KW_IF		261
%token KW_THEN		262
%token KW_ELSE		263
%token KW_LOOP		264
%token KW_INPUT		266
%token KW_OUTPUT	267
%token KW_RETURN	268

%token OPERATOR_LE	270
%token OPERATOR_GE	271
%token OPERATOR_EQ	272
%token OPERATOR_NE	273
%token OPERATOR_AND	274
%token OPERATOR_OR	275

%token<symbol> TK_IDENTIFIER	280
%token<number> LIT_INTEGER	281
%token<symbol> LIT_FALSE	283
%token<symbol> LIT_TRUE		284
%token<symbol> LIT_CHAR		285
%token<symbol> LIT_STRING	286

%token TOKEN_ERROR	290

%left OPERATOR_OR OPERATOR_AND
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_NE
%left '+' '-'
%left '*' '/'
%nonassoc '$' '&' '!'


%%

program: decl_global
	| decl_global program
	| function
	| function program
	;


// Declarations

decl_global: decl
	| decl_vector
	| decl_pointer
	;

decl: type identifier ':' init ';'
	;

init: LIT_INTEGER
	| LIT_FALSE
	| LIT_TRUE
	| LIT_CHAR
	| LIT_STRING
	;

decl_vector: type identifier '[' LIT_INTEGER ']' ':' init_vector ';'
	| type identifier '[' LIT_INTEGER ']' ';'
	;

init_vector: init
	| init init_vector
	;

decl_pointer: type '$' identifier ':' init ';'
	;


//Function

function: type identifier '(' n_param ')' command ';'
	;

n_param: 
	| param n_param_2
	;

n_param_2:
	| ',' param n_param_2
	;

param: type identifier
	| type '$' identifier
	;

// Commands
command: simple_command
	| block
       ;

block: '{' command_block '}'
	| '{' '}'
	;

command_block: simple_command
	| simple_command command_block
	;

simple_command:attribution
	| if
	| loop
	| KW_INPUT identifier
	| KW_OUTPUT out
	| KW_RETURN expression
	;

attribution: identifier '=' expression
	| identifier '[' expression ']' '=' expression
	;

out:expression
	| expression ',' out
	;


//control flow

if: KW_IF '(' expression ')' KW_THEN command
	| KW_IF '(' expression ')' KW_ELSE command KW_THEN command
	;

loop: KW_LOOP command '(' expression ')'
	;


// types

identifier: TK_IDENTIFIER
	;

type:KW_WORD 
	| KW_BYTE
	| KW_BOOL
	;

expression: element
	| identifier '[' expression ']'
	| expression '+' expression
	| expression '-' expression
	| expression '/' expression
	| expression '*' expression
	| expression '<' expression
	| expression '>' expression
	| expression OPERATOR_LE expression
	| expression OPERATOR_GE expression
	| expression OPERATOR_EQ expression
	| expression OPERATOR_NE expression
	| expression OPERATOR_AND expression
	| expression OPERATOR_OR expression
	| identifier '(' n_param_ref ')'
	| '(' expression ')'
	| '!' expression
	| '&' expression
	| '$' expression
	;

n_param_ref: 
	| expression n_param_ref2
	;

n_param_ref2:
	| ',' expression n_param_ref2
	;

element: identifier
	| LIT_INTEGER
	| LIT_FALSE
	| LIT_TRUE
	| LIT_CHAR
	| LIT_STRING
	;

%%

int yyerror (char *s)
{
	fprintf(stderr, "Error in line %d: %s.\n", getLineNumber(), s);
	exit(3);
}
