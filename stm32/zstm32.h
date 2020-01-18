/*
 *	Biblioteca de funcoes genericas para o STM32
 *	Versao em C
 *
 *	Feito por: setor de Software Baixo Nivel - Zenith Aerospace
 *	Criado em: 20 set 2019
 */
 
#ifndef LIB_ZENITH_STM32_GENERICS
#define LIB_ZENITH_STM32_GENERICS

#include "zgenerics.h"
#include "stm32l4xx_hal.h"

#define RETURN_ON_ERROR(status);\
	if(status != HAL_OK){\
		return status;\
	}

#define SET_PIN(pin_label);\
	HAL_GPIO_WritePin(PP_CONCAT(pin_label, _GPIO_Port), PP_CONCAT(pin_label, _Pin), GPIO_PIN_SET);

#define RESET_PIN(pin_label);\
	HAL_GPIO_WritePin(PP_CONCAT(pin_label, _GPIO_Port), PP_CONCAT(pin_label, _Pin), GPIO_PIN_RESET);

#define TOGGLE_PIN(pin_label);\
	HAL_GPIO_TogglePin(PP_CONCAT(pin_label, _GPIO_Port), PP_CONCAT(pin_label, _Pin));

#endif//LIB_ZENITH_STM32_GENERICS
