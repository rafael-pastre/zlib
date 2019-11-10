/*
 *	Biblioteca com definicao de tipos padrao para Baixo Nivel
 *	Versao em C
 *
 *	Feito por: setor de Software Baixo Nivel - Zenith Aerospace
 *	Criado em: 27 ago 2019
 */

#ifndef LIB_ZENITH_STDTYPES_H
#define LIB_ZENITH_STDTYPES_H

#include <stdint.h>
/*
 *	Definicao das Palavras Genericas
 *
 *	Palavras sao estuturas que auxiliam a reinterpretacao e tratamento
 *	de valores em binario/hexadecimal que necessitam de uma mudanca de
 *	tamanho, ou de uma mudanca de signed/unsigned.
 *	As palavras nao realizam conversao, mas mantem o valor binario.
 *
 *	OBS: VERIFICAR A ORDENACAO DA DIVISAO DE VARIAVEIS DAS PALAVRAS
 */
 
/* Palavra de 8 bits */
typedef union _word8{
	uint8_t		u;				// Valor interpretado como inteiro de 8 bits positivo
	int8_t		sg;				// Valor interpretado como inteiro de 8 bits com sinal
}word8_t;

/* Palavra de 16 bits */
typedef union _word16{
	uint16_t	u;				// Valor interpretado como inteiro de 16 bits positivo
	int16_t		sg;				// Valor interpretado como inteiro de 16 bits com sinal
	uint8_t		uvet8[2];		// Valor dividido em inteiros de 8 bits positivos
	int8_t		svet8[2];		// Valor dividido em inteiros de 8 bits com sinal
}word16_t;

/* Palavra de 32 bits */
typedef union _word32{
	uint32_t	u;				// Valor interpretado como inteiro de 32 bits positivo
	int32_t		sg;				// Valor interpretado como inteiro de 32 bits com sinal
	float		flt;			// Valor interpretado como ponto flutuante
	uint8_t		uvet16[2];		// Valor dividido em inteiros de 16 bits positivos
	int8_t		svet16[2];		// Valor dividido em inteiros de 16 bits com sinal
	uint8_t		uvet8[4];		// Valor dividido em inteiros de 8 bits positivos
	int8_t		svet8[4];		// Valor dividido em inteiros de 8 bits com sinal
}word32_t;

/* Palavra de 64 bits */
typedef union _word64{
	uint64_t	u;				// Valor interpretado como inteiro de 64 bits positivo
	int64_t		sg;				// Valor interpretado como inteiro de 64 bits com sinal
	double		dbl;			// Valor interpretado como ponto flutuante
	uint8_t		uvet32[2];		// Valor dividido em inteiros de 32 bits positivos
	int8_t		svet32[2];		// Valor dividido em inteiros de 32 bits com sinal
	uint8_t		uvet16[4];		// Valor dividido em inteiros de 16 bits positivos
	int8_t		svet16[4];		// Valor dividido em inteiros de 16 bits com sinal
	uint8_t		uvet8[8];		// Valor dividido em inteiros de 8 bits positivos
	int8_t		svet8[8];		// Valor dividido em inteiros de 8 bits com sinal
}word64_t;

/* Ascii Character */
typedef uint8_t	ascii_t;		// ascii character type. must have 8 bits length.

/* Ascii Interval*/
typedef struct _ascii_interval{
	ascii_t begin;				// intervals are inclusive by default
	ascii_t end;				// to exclusive intervals use +1 or -1
}ascii_interval_t;

/* Flag */
typedef enum _flag{
	FLAG_OFF = 0x00,
	FLAG_ON	 = 0x01
}flag_t;

/* Strict Comparation Result */
typedef enum _scmp{
	LOWER	= -1,
	EQUAL	= 0,
	GREATER	= 1
}scmp_t;

#endif//LIB_ZENITH_STDTYPES_H
