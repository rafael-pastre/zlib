/*
 *	Biblioteca para debug de codigo no console
 *	Versao em C
 *
 *	Para desabilitar os prints desta biblioteca, ou seja, na compilacao
 *	da versao final do codigo, deve-se retirar o macro "DEBUG_MODE" definido no
 *	arquivo "zdebug.c"
 *
 *	Feito por: Rafael Pastre
 *	Criado em: 26 out 2019
 */

#ifndef LIB_ZENITH_DEBUG_H
#define LIB_ZENITH_DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define PROCESS_ERROR_MSG	"Falha no processamento do arquivo."
#define FILE_ERROR_MSG		"Falha no carregamento do arquivo."
#define FUNCT_ERROR_MSG		"Error: invalid functionality inserted.\n"
#define READING_ERROR_MSG 	"Erro de leitura.\n"

int dprintf(int tag, const char* format_str, ...);

int debug_ascii_file(int tag, FILE* fp);

void exit_if_null(void* p);

void mexit_if_null(void* p, const char* error_message);

#endif//LIB_ZENITH_DEBUG_H
