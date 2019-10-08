/*
 *	Biblioteca de funcoes genericas
 *	Versao em C
 *
 *	Feito por: setor de Software Baixo Nivel - Zenith Aerospace
 *	Criado em: 18 set 2019
 */

#include "zgenerics.h"

//TO CHECK
void swap(void* pa, void* pb, size_t s){
	uint8_t t;
	uint8_t* a = (uint8_t*)pa;
	uint8_t* b = (uint8_t*)pb;

	while(s--){
		t = a[s];
		a[s] = b[s];
		b[s] = t;
	}
}

//TO CHECK: pensar em swap com palavras de 16/32 ao inves de bytes
void swap_bytes(void* str, size_t s){
	/*
	 *	Generic function for inverting bytes order in a given data
	 *	Example: int32_t in  = b3,B2,B1,b0 -> int32_t out = b0,B1,B2,b3
	 *	Example: uint8_t in[3]  = c[0],c[1],c[2] -> int32_t out = c[2],c[1],c[0]
	 */
	uint8_t b;
	for(size_t i = 0; i < s/2; i++){
		b = *((uint8_t*)(str+s-i-1));
		*((uint8_t*)(str+s-i-1)) = *((uint8_t*)(str+i));
		*((uint8_t*)(str+i)) = b;
	}
}