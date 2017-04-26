#include "data.h"
#include <stdlib.h>
#include <stdio.h>

//TERM
lst_term* create_lst_term(term* _term)
{
	lst_term *_lst = (lst_term*) malloc(sizeof(lst_term));
	_lst->data = _term;
	_lst->next = NULL;
	return _lst;
}


lst_term* append_lst_term(lst_term* _lstterm, term* _term)
{
	lst_term *_lst = (lst_term*) malloc(sizeof(lst_term));
	_lst->data = _term;
	_lst->next = _lstterm;

	return _lst;
}

//ATOM
atom* create_atom(char* _text, lst_term* _lstterm)
{
	atom* _atom = (atom*) malloc(sizeof(atom));
	_atom->praedikat = _text;
	_atom->liste = _lstterm;
	return _atom;
}

lst_atom* create_lst_atom(atom* _atom)
{
	lst_atom *_lst = (lst_atom*) malloc(sizeof(lst_atom));
	_lst->data = _atom;
	_lst->next = NULL;
	return _lst;
}

lst_atom* append_lst_atom(lst_atom* _lstatom, atom* _atom)
{
	lst_atom *_lst = (lst_atom*) malloc(sizeof(lst_atom));
	_lst->data = _atom;
	_lst->next = _lstatom;

	return _lst;
}

//FORMEL
formel* create_formel(atom* _atom, lst_atom* _lstatom)
{
	formel* _formel = (formel*) malloc(sizeof(formel));
	_formel->kopf = _atom;
	_formel->liste = _lstatom;
	return _formel;
}

lst_formel* append_lst_formel(lst_formel* _lstformel, formel* _formel)
{
	lst_formel* _lst = (lst_formel*) malloc(sizeof(lst_formel));
	_lst->data = _formel;
	_lst->next = _lstformel;

	return _lst;
}

//PRINT:
void print_lst_formel(lst_formel* _lstformel)
{
	print_formel(_lstformel->data);
	if (_lstformel->next)
	{
		print_lst_formel(_lstformel->next);
	}
}

void print_formel(formel *_formel)
{
	printf("%s", "Kopf:");
	if (_formel->kopf)
	{
		print_atom(_formel->kopf);
	}
	else
	{
		printf(" FALSE\n");
	}

	printf("%s", "Body:");
	if (_formel->liste)
	{
		print_lst_atom(_formel->liste);
	}
	else
	{
		printf("=TRUE\n");
	}
}

void print_atom(atom *_atom)
{
	printf("Atom: %s\n", _atom->praedikat);
	if (_atom->liste)
	{
		print_lst_term(_atom->liste);
	}
}

void print_lst_atom(lst_atom* _lstatom)
{
	print_atom(_lstatom->data);
	if (_lstatom->next)
	{
		print_lst_atom(_lstatom->next);
	}
}

void print_lst_term(lst_term* _lstterm)
{
	if (_lstterm->data)
	{
		print_term(_lstterm->data);
	}
	else
	{
		printf("ERROR: Term data NULL\n");
	}

	if (_lstterm->next)
	{
		print_lst_term(_lstterm->next);
	}
}

void print_term(term* _term)
{
	if (_term)
	{
		printf("Term: %s\n", _term->varorfunc);
		if (_term->liste)
		{
			printf("Term Liste:\n");
			print_lst_term(_term->liste);
		}
	}
}

int deepth = 0;
//#resulution
bool satisfiable(lst_formel* _lstformel_remain, lst_formel* _lstformel)
{
	if (!_lstformel_remain->data->kopf)
	{

		printf("FOUND unifiable:\n");
		print_formel(_lstformel_remain->data);

		if (resulution(_lstformel_remain->data->liste, _lstformel))
		{
			return true;
		}
	}

	if (_lstformel_remain->next)
	{
		return satisfiable(_lstformel_remain->next, _lstformel);
	}

	return false;
}

bool resulution(lst_atom* _lstatom, lst_formel* _lstformel)
{
	if (deepth > 100)
	{
		printf("!!!Schachtelung größer als 100. Keine Aussage möglich!");
		deepth --;
		return false;
	}

	deepth ++;


	//check only not empty
	if (_lstatom && _lstatom->data)
	{
		lst_formel* formelpointer = _lstformel;
		while(formelpointer)
		{
			//make backup of data?
			if(compare_atom(_lstatom->data, formelpointer->data->kopf))
			{
				if (!formelpointer->data->liste)
				{
					deepth--;
					return true;
				}


				return resulution(formelpointer->data->liste, _lstformel);
			}
			//restore backup of data ?

			formelpointer = formelpointer->next;
		}
	}

	deepth --;
	return false;
}

bool compare_atom(atom* _atom1, atom* _atom2)
{
	if (_atom1 && _atom2 && strcmp(_atom1->praedikat, _atom2->praedikat) == 0)
	{
		if (!_atom1->liste && !_atom2->liste)
		{	
			return true;
		}
		else if (!_atom1->liste || !_atom2->liste)
		{
			return false;
		}
		else
		{
			//if lst_term is not equal -> unification
			return compare_lst_term(_atom1->liste, _atom2->liste);
		}
	}

	return false;
}

int compare_term(term* _term1, term* _term2)
{	
	if (_term1 && _term2 && strcmp(_term1->varorfunc, _term2->varorfunc) == 0)
	{
		if (!_term1->liste && !_term2->liste)
		{
			return 1;
		}
		else if(!_term1->liste || !_term2->liste)
		{
			return 0;
		}
		else
		{
			return compare_lst_term(_term1->liste, _term2->liste);
		}
	}
	else
	{
		//unification	
		if (_term1->isvar == true)
		{
			if(isVarInTerm(_term1->varorfunc, _term2->liste) == true)
			{
				return 0;
			}

			term *unify = has_unification(_term1);
			if(unify)
			{
				return compare_term(unify, _term2);
			}
			else
			{
				add_unification(_term1, _term2);
				return 2;
			}

		} 

		if (_term2->isvar == true)
		{
			if(isVarInTerm(_term2->varorfunc, _term1->liste) == true)
			{
				return 0;
			}

			term *unify = has_unification(_term2);
			if(unify)
			{
				return compare_term(_term1, unify);
			}
			else
			{
				add_unification(_term2, _term1);
				return 2;
			}
		}
	}

	return 0;
}

bool compare_lst_term(lst_term* _lstterm1, lst_term* _lstterm2)
{
	if (_lstterm1 && _lstterm2)
	{

		//make backup of terms and unifylist
		int retcompare = compare_term(_lstterm1->data, _lstterm2->data);
		//if ret 0 -> restore backup


		if(retcompare >= 1)
		{
			if (retcompare == 2)
			{
				_lstterm1->data = _lstterm2->data;

				//TODO ersetzen

				print_unification();

			}

			if (!_lstterm1->next && !_lstterm2->next)
			{
				return true;
			}
			else if(_lstterm1->next && _lstterm2->next)
			{
				return compare_lst_term(_lstterm1->next, _lstterm2->next);
			}
		}
	}

	return false;
}

//Helper
void append_char(char* s, char c) {
	int len = strlen(s);
	if(realloc(s, sizeof(s) + sizeof(s)))
	{
		s[len] = c;
		s[len+1] = '\0';	
	}
}

lst_unification* _lstunification = NULL;
void add_unification(term* from, term* to)
{
	unification* new_unification = (unification*) malloc(sizeof(unification));
	new_unification->from = from;
	new_unification->to = to;

	lst_unification* new_lst_unification = (lst_unification*) malloc(sizeof(lst_unification));
	
	new_lst_unification->data = new_unification;
	new_lst_unification->next = _lstunification;

	_lstunification = new_lst_unification;
}

term* has_unification(term* from)
{
	lst_unification* uni = _lstunification;

	while(uni)
	{
		if (uni->data)
		{
			if (uni->data->from == from)
			{
				return uni->data->to;
			}
		}

		uni = uni->next;
	}
	
	return NULL;
}

void print_unification()
{
	lst_unification* uni = _lstunification;

	while(uni)
	{
		if (uni->data)
		{
			printf("\nUNIFICATION: ");
			print_term(uni->data->from);
			print_term(uni->data->to);
		}

		uni = uni->next;
	}
}

bool isVarInTerm(char* var, lst_term* _termlst) 
{
	lst_term* _termlsttemp = _termlst;

	while(_termlsttemp)
	{
		if (strcmp(_termlsttemp->data->varorfunc, var) == 0)
		{
			return true;
		}
		_termlsttemp = _termlsttemp->next;
	}

	return false;
}
