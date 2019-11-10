/*
 *	Biblioteca de funcoes genericas
 *	Versao em C
 *
 *	Feito por: setor de Software Baixo Nivel - Zenith Aerospace
 *	Criado em: 18 set 2019
 */
 
#ifndef LIB_ZENITH_GENERICS
#define LIB_ZENITH_GENERICS

#include <stdint.h>

/* Directives */
#define MAX(x,y) (x > y ? x : y)
#define MIN(x,y) (x > y ? y : x)
#define IS_MAIUSCULE(c) ('A' <= c && c <='Z')
#define IS_MINUSCULE(c) ('a' <= c && c <='z')
#define IS_ASCII_LETTER(c) (IS_MAIUSCULE(c) || IS_MINUSCULE(c))
#define IS_ASCII_NUMBER(c) ('0' <= c && c <= '9')

/* Functions */
void swap(void* pa, void* pb, size_t s);	// Swaps elements pointed by "pa" and "pb". The elements must have "s" bytes.

void swap_nibbles(void* pa, size_t s);		// Swaps nibbles(4bits) of an element pointed by "pa". The element must have "s" bytes.

void swap_bytes(void* pa, size_t s);		// Swaps bytes of an element pointed by "pa". The element must have "s" bytes.

#endif//LIB_ZENITH_GENERICS