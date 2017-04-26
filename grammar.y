%{
	#include <stdio.h>
	#include "data.h"
	extern int yyerror(char* err);
	extern int yylex(void);
	extern FILE *yyin;

	lst_formel* lstformel = NULL;
	int varcounter = 0;
%}

%start seq

%token VAR
%token FUNC
%token PRAEDIKAT
%token ARROW
%token UND
%token KOMMA
%token TRUE
%token FALSE
%token KLAMMERO
%token KLAMMERC
%token NEWLINE


%union {
	term *t;
	atom *a;
	formel *f;
	lst_term *lt;
	lst_atom *la;
	lst_formel *lf;
	char* text;
}

%%

term: VAR {
			term *_term = (term*) malloc(sizeof(term));
			
			char* _var = (char*) malloc(sizeof($<text>1));
			strcpy(_var, $<text>1);
			append_char(_var, (char)(varcounter+ 48));
			
			_term->varorfunc = _var;
			_term->isvar = true;
			_term->liste = NULL;
			$<t>$ = _term;
		}
	| FUNC {
			term *_term = (term*) malloc(sizeof(term));
			_term->varorfunc = $<text>1;
			_term->isvar = false;
			_term->liste = NULL;
			$<t>$ = _term;
		}
	| FUNC KLAMMERO params KLAMMERC {
			term *_term = (term*) malloc(sizeof(term));
			_term->varorfunc = $<text>1;
			_term->isvar = false;
			_term->liste = $<lt>3;
			$<t>$ = _term;
		}

//params = liste von termen
params: term { $<lt>$ = create_lst_term($<t>1);}
	| term KOMMA params { $<lt>$ = append_lst_term($<lt>3, $<t>1);}

atom: PRAEDIKAT { $<a>$ = create_atom($<text>1, NULL);}
	| PRAEDIKAT KLAMMERO params KLAMMERC {$<a>$ = create_atom($<text>1, $<lt>3);}

//body == liste von atomen
body: atom {$<la>$ = create_lst_atom($<a>1);}
	| body UND atom {$<la>$ = append_lst_atom($<la>1, $<a>3);}

formel: body ARROW atom { $<f>$ = create_formel($<a>3, $<la>1); varcounter++;}
	| body ARROW FALSE { $<f>$ = create_formel(NULL, $<la>1); varcounter++;}
	| TRUE ARROW atom { $<f>$ = create_formel($<a>3, NULL); varcounter++;}

seq: formel { lstformel = append_lst_formel(lstformel, $<f>1);}
	| formel NEWLINE seq { lstformel = append_lst_formel(lstformel, $<f>1);}
	| formel NEWLINE { lstformel = append_lst_formel(lstformel, $<f>1);}

%%

int yyerror(char* err)
{
	printf("Error: %s\n", err);
	return 0;
}

int main( int argc, char **argv )       
{
	++argv, --argc;
	if(argc > 0) 
	{
		yyin = fopen(argv[0], "r");
	}
	else
	{
		yyin = stdin;
	}
	
	yyparse();

	print_lst_formel(lstformel);
	
	printf("\n\n START RESULUTION\n\n");

	if(satisfiable(lstformel,lstformel))
	{
		printf("\n\nunsatisfiable\n\n");
		return 1;
	}
	else
	{
		printf("\n\nsatisfiable\n\n");
	}

	return 0;
}
