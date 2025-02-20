#ifndef DISPLAY_U8G2_H
#define DISPLAY_U8G2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdbool.h>

void draw (u8g2_t *u8g2);
void logicaMenu (bool botaocima, bool botaoselect, bool botaobaixo);
void atualiza_u8g2(u8g2_t* u8g2, bool botaocima, bool botaoselect, bool botaobaixo, unsigned long* delaydesenho);
void inicializa_u8g2(u8g2_t* u8g2, u8x8_msg_cb hw_spi, u8x8_msg_cb stm32);

#ifdef __cplusplus
}
#endif		
	
#endif
