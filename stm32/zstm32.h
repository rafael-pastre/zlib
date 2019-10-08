/*
 *	Biblioteca de funcoes genericas para o STM32
 *	Versao em C
 *
 *	Feito por: setor de Software Baixo Nivel - Zenith Aerospace
 *	Criado em: 20 set 2019
 */
 
#ifndef LIB_ZENITH_GENERICS
#define LIB_ZENITH_GENERICS

#include "stm32l4xx_hal.h"

#define RETURN_ON_ERROR(status);\
	if(status != HAL_OK){\
		return status;\
	}

#endif//LIB_ZENITH_GENERICS
