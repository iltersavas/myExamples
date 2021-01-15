#include <stdio.h>
#include "driver.h"

const uint8_t pause_symbol[] 	= { 0x00, 0x00, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B };
const uint8_t play_symbol[] 	= { 0x00, 0x10, 0x18, 0x1C, 0x1E, 0x1C, 0x18, 0x10 };
const uint8_t okkey[] 			= { 0x00, 0x01, 0x02, 0x14, 0x08, 0x00, 0x00, 0x00 };
const uint8_t up_down_symbol[] 	= { 0x04, 0x0E, 0x1F, 0x00, 0x1F, 0x0E, 0x04, 0x00 };
const uint8_t up_symbol[] 		= { 0x04, 0x0E, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00 };
const uint8_t down_symbol[] 	= { 0x00, 0x00, 0x00, 0x00, 0x1F, 0x0E, 0x04, 0x00 };
//

int main(int argc, char const *argv[])
{
	KS0066_Init(20, 4, 4);
	KS0066_Puts(3, 1, "its Working hello ");

	KS0066_PutCustom(3, 2, 0);
	KS0066_CreateChar(0, pause_symbol);

	KS0066_PutCustom(4, 2, 1);
	KS0066_CreateChar(1, play_symbol);

	KS0066_PutCustom(5, 2, 2);
	KS0066_CreateChar(2, okkey);

	KS0066_PutCustom(6, 2, 3);
	KS0066_CreateChar(3, up_symbol);

	KS0066_PutCustom(7, 2, 4);
	KS0066_CreateChar(4, down_symbol);

	return 0;
}