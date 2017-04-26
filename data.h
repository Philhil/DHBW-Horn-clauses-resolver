#include <stdbool.h>
#include <string.h>

typedef struct term
{
	char* varorfunc;
	bool isvar;
	struct lst_term* liste;
} term;

typedef struct atom
{
	char* praedikat;
	struct lst_term* liste;
} atom;

typedef struct formel
{
	atom* kopf;
	struct lst_atom* liste;
} formel;

typedef struct unification
{
	term* from;
	term* to;
} unification;


/*Listen Verkettete Listen von:
Termen
Atomen
Formeln*/

typedef struct lst_term
{
	term *data;
	struct lst_term *next;
} lst_term;


typedef struct lst_atom
{
	atom *data;
	struct lst_atom *next;
} lst_atom;


typedef struct lst_formel
{
	formel *data;
	struct lst_formel *next;
} lst_formel;

typedef struct liste
{
	lst_term *terme;
	lst_atom *atome;
	lst_formel *formeln;
} liste;

typedef struct lst_unification
{
	unification* data;
	struct lst_unification* next;
} lst_unification;

//#Term
lst_term* create_lst_term(term*);
lst_term* append_lst_term(lst_term*, term*);

//#Atom
atom* create_atom(char*, lst_term*);
lst_atom* create_lst_atom(atom*);
lst_atom* append_lst_atom(lst_atom*, atom*);

//#Formel
formel* create_formel(atom*, lst_atom*);
lst_formel* append_lst_formel(lst_formel*, formel*);


//#Print
void print_lst_formel(lst_formel*);
void print_formel(formel*);
void print_atom(atom*);
void print_lst_atom(lst_atom*);
void print_lst_term(lst_term*);
void print_term(term*);
void print_unification();



//#resulution
bool satisfiable(lst_formel*,lst_formel*);
bool resulution(lst_atom*,lst_formel*);


bool compare_atom(atom*, atom*);
int compare_term(term*, term*);
bool compare_lst_term(lst_term*, lst_term*);

//helper
void append_char(char*, char);
void add_unification(term*, term*);
term* has_unification(term*);
bool isVarInTerm(char*, lst_term*); 