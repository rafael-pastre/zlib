/*
 *	Biblioteca para multiset de strings utilizando lista ligada
 *	Versao em C
 *
 *	Feito por: Rafael Pastre
 *	Criado em: 05 nov 2019
 */
 
#ifndef LIB_STR_MULTISET_H
#define LIB_STR_MULTISET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_MAX_LEN	100

// Node
typedef struct _str_multiset_node{
	char str[STR_MAX_LEN];
	int count;
    struct _str_multiset_node* next;
}StrMultiSetNode;

// Multiset
typedef StrMultiSetNode* StrMultiSet;

StrMultiSet* str_multiset_create();

int str_multiset_count(StrMultiSet* str_multiset);

int str_multiset_insert(StrMultiSet* str_multiset, const char* str);

int str_multiset_remove(StrMultiSet* str_multiset, const char* str);

void print_str_multiset(StrMultiSet* str_multiset);
#endif//LIB_STR_MULTISET_H