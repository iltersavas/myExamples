#ifndef DRIVER_H
#define DRIVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "simplegpio.h"
#include <stdarg.h>
char printbuff[80];

void KS0066_Init(uint8_t cols, uint8_t rows, uint8_t bitMode);
void KS0066_DisplayOn(void);
void KS0066_DisplayOff(void);
void KS0066_Clear(void);
void KS0066_Puts(uint8_t x, uint8_t y, char* str);
void KS0066_Print(uint8_t x, uint8_t y, const char * ptrdata, ...);
void KS0066_BlinkOn(void);
void KS0066_BlinkOff(void);
void KS0066_CursorOn(void);
void KS0066_CursorOff(void);
void KS0066_ScrollLeft(void);
void KS0066_ScrollRight(void);
void KS0066_CreateChar(uint8_t location, const uint8_t *data);
void KS0066_PutCustom(uint8_t x, uint8_t y, uint8_t location);
void KS0066_Cmd(uint8_t cmd);
void KS0066_Data(uint8_t data);
void KS0066_Cmd4bit(uint8_t cmd);
void KS0066_Cmd8bit(uint8_t cmd);
void KS0066_CursorSet(uint8_t col, uint8_t row);
void KS0066_InitPins(void);
void KS0066_BackLightOff(void);
void KS0066_BackLightOn(void);
void gpioInit( void );
void KS0066_Update( void );

#endif
