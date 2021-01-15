/*
 * driver.c
 *
 *  Created on: april, 2020
 *      Author: ilter SAVAS
 * 				ilteer.savas@gmail.com
 * 
 * this driver contains Character lcd IO pin definition, 
 *                      IO settings,
 *                      Character lcd init function,
 *                      
 */

#include "driver.h"

/*
 for example @TI am4372 ARM® Cortex®-A9 32-Bit RISC Processor  or beaglebone 
	exp. gpioX_Y   Pin value = 32*X+Y
*/
// @Begin 20x4 character LCD PinOut
// this pinout my custom am4372 board pinout. you must be declared your pinout.
#define RS_PIN		 148 	  /* gpio4 20 */
#define RW_PIN		 180 	  /* gpio4 20 */
#define EN_PIN		 139	  /* gpio4 11 */
#define VE_PIN		 /*0 */		//	contrast controlled by pot
#define DATA0		 144      /* gpio4 16 */ 
#define DATA1		 143      /* gpio4 15 */ 
#define DATA2		 132      /* gpio4 04 */ 
#define DATA3		 142      /* gpio4 14 */ 
#define DATA4        134      /* gpio4 19 */
#define DATA5  		 133      /* gpio4 05 */
#define DATA6        138      /* gpio4 10 */
#define DATA7        137      /* gpio4 09 */

#define LCDPOWER     129	  /* gpio4 01 */	
// @End 20x4 character LCD PinOut

/* Commands*/
#define sClearDisp        	    ( 0x01 )
#define sReturnHome          	( 0x02 )
#define sEntryMode        	 	( 0x04 )
#define sDispControl      		( 0x08 )
#define sShiftControl         	( 0x10 )
#define sFunctionSet         	( 0x20 )
#define sWakeUp                 ( 0x30 )
#define sCgram        			( 0x40 )
#define sDdram        			( 0x80 )

/* Flags for display entry mode */
#define KS0066_ENTRYRIGHT          0x00
#define KS0066_ENTRYLEFT           0x02
#define KS0066_ENTRYSHIFTINCREMENT 0x01
#define KS0066_ENTRYSHIFTDECREMENT 0x00

/* Flags for display on/off control */
#define KS0066_DISPLAYON           0x04
#define KS0066_CURSORON            0x02
#define KS0066_BLINKON             0x01

/* Flags for display/cursor shift */
#define KS0066_DISPLAYMOVE         0x08
#define KS0066_CURSORMOVE          0x00
#define KS0066_MOVERIGHT           0x04
#define KS0066_MOVELEFT            0x00

/* Flags for function set */
#define KS0066_8BITMODE            0x10
#define KS0066_4BITMODE            0x00
#define KS0066_2LINE               0x08
#define KS0066_1LINE               0x00
#define KS0066_5x10DOTS            0x04
#define KS0066_5x8DOTS             0x00


typedef struct {
	uint8_t displayControl;
	uint8_t displayFunction;
	uint8_t DisplayMode;
	uint8_t Rows;
	uint8_t Cols;
	uint8_t currentX;
	uint8_t currentY;
	unsigned char ram[80];
	uint8_t bitMode;
} tsParams;

static tsParams tsOptions;

#define RS_LOW     gpio_set_value(RS_PIN, 0)   
#define RS_HIGH    gpio_set_value(RS_PIN, 1)
#define EN_LOW	   gpio_set_value(EN_PIN, 0)
#define EN_HIGH    gpio_set_value(EN_PIN, 1)


#define RW_LOW	   gpio_set_value(RW_PIN, 0)
#define RW_HIGH    gpio_set_value(RW_PIN, 1)

#define KS0066_E_BLINK             EN_HIGH; Wait(100); EN_LOW; Wait(300)
#define Wait(x)            usleep(x)

int lcdDataPins, m6800Value;
void KS0066_Wait()
{
	RW_HIGH;
	RS_LOW;
	PIN_DIRECTION gpio_direction = INPUT_PIN;
	gpio_set_direction(DATA7, gpio_direction);  // set GPIO pin direction
	m6800Value = 0;
	gpio_get_value(DATA7, &lcdDataPins);
	m6800Value |= lcdDataPins & 1<<7;


	while(m6800Value & 0x80);
	gpio_direction = OUTPUT_PIN;
	gpio_set_direction(DATA7, gpio_direction);  // set GPIO pin direction

	RS_HIGH;
	RW_LOW;
}

void KS0066_Init(uint8_t cols, uint8_t rows, uint8_t bitMode) {
	//input pinout
	KS0066_InitPins();

	//at least 45ms
	Wait(45000);

	tsOptions.Rows = rows;
	tsOptions.Cols = cols;
	tsOptions.bitMode = bitMode;

	/* Set cursor pointer to beginning for LCD */
	tsOptions.currentX = 0;
	tsOptions.currentY = 0;

	if( tsOptions.bitMode == 8 )
	{
		tsOptions.displayFunction = KS0066_8BITMODE | KS0066_5x8DOTS | KS0066_2LINE;
		if (rows > 1) {
			tsOptions.displayFunction |= KS0066_2LINE;
		}
		KS0066_Cmd8bit(0x03);
		Wait(4500);
		
		KS0066_Cmd8bit(0x03);
		Wait(4500);
		
		KS0066_Cmd8bit(0x03);
		Wait(4500);	
		
		/* Set 8-bit interface */
		KS0066_Cmd8bit(0x02);
		Wait(100);
		
		KS0066_Cmd(sFunctionSet | tsOptions.displayFunction);
	}
	else {

		tsOptions.displayFunction = KS0066_4BITMODE | KS0066_5x8DOTS | KS0066_2LINE;
		if (rows > 1) {
			tsOptions.displayFunction |= KS0066_2LINE;
		}
		KS0066_Cmd4bit(0x03);
		Wait(4500);
		
		KS0066_Cmd4bit(0x03);
		Wait(4500);
		
		KS0066_Cmd4bit(0x03);
		Wait(4500);	
		
		/* Set 4-bit interface */
		KS0066_Cmd4bit(0x02);
		Wait(100);
		
		KS0066_Cmd(sFunctionSet | tsOptions.displayFunction);
	}
	
	/* Turn the display on with no cursor or blinking default */
	KS0066_DisplayOn();

	/* Clear lcd */
	KS0066_Cmd(sClearDisp);

	/* Default font directions */
	tsOptions.DisplayMode = KS0066_ENTRYLEFT | KS0066_ENTRYSHIFTDECREMENT;
	KS0066_Cmd(sEntryMode | tsOptions.DisplayMode);

	/* Delay */
	Wait(4500);

	KS0066_CursorSet(0, 0);

}
void KS0066_DisplayOn(void){
	tsOptions.displayControl |= KS0066_DISPLAYON;
	KS0066_Cmd(sDispControl | tsOptions.displayControl);
}

void KS0066_DisplayOff(void){
		tsOptions.displayControl &= ~KS0066_DISPLAYON;
	KS0066_Cmd(sDispControl | tsOptions.displayControl);
}

void KS0066_Clear(void) {
	//KS0066_Puts(0, 0, "                    ");
	//Wait( 37 );
	//KS0066_Puts(0, 1, "                    ");
	//Wait( 37 );
	//KS0066_Puts(0, 2, "                    ");
	//Wait( 37 );
	KS0066_Cmd(sClearDisp);
	Wait(3000);
}

void KS0066_Puts(uint8_t x, uint8_t y, char* str)
{
	
	KS0066_CursorSet(x, y);
	Wait(100);

	while (*str) {
		KS0066_Data(*str);
		//strncpy(&tsOptions.ram[tsOptions.currentX + tsOptions.currentY*20], str, 1);
		tsOptions.currentX++;
		str++;
	}
}

void KS0066_Print(uint8_t x, uint8_t y, const char * ptrdata, ...){
	va_list args;
    char array[ 128 ];
    int length;

    va_start( args, ptrdata );
    length = vsprintf( array, ptrdata, args );

    if( length > 0 )
    {
        KS0066_Puts(x, y, (char *)&array);
    }
    
    va_end( args );
}

void KS0066_BlinkOn(void){

	tsOptions.displayControl |= KS0066_BLINKON;
	KS0066_Cmd(sDispControl | tsOptions.displayControl);
}

void KS0066_BlinkOff(void){

	tsOptions.displayControl &= ~KS0066_BLINKON;
	KS0066_Cmd(sDispControl | tsOptions.displayControl);
}

void KS0066_CursorOn(void){

	tsOptions.displayControl |= KS0066_CURSORON;
	KS0066_Cmd(sDispControl | tsOptions.displayControl);
}

void KS0066_CursorOff(void){

	tsOptions.displayControl &= ~KS0066_CURSORON;
	KS0066_Cmd(sDispControl | tsOptions.displayControl);
}

void KS0066_ScrollLeft(void){
		KS0066_Cmd(sShiftControl | KS0066_DISPLAYMOVE | KS0066_MOVELEFT);
}

void KS0066_ScrollRight(void){
		KS0066_Cmd(sShiftControl | KS0066_DISPLAYMOVE | KS0066_MOVERIGHT);
}

void KS0066_CreateChar(uint8_t location, const uint8_t* data){
		uint8_t i;
	/* We have 8 locations available for custom characters */
	location &= 0x07;
	KS0066_Cmd(sCgram | (location << 3));
	Wait(100);
	
	for (i = 0; i < 8; i++) {
		KS0066_Data(data[i]);
	}
}

void KS0066_PutCustom(uint8_t x, uint8_t y, uint8_t location){
	KS0066_CursorSet(x, y);
	Wait( 100 );
	KS0066_Data(location);
}

void KS0066_Cmd(uint8_t cmd) {
	/* Command mode */
	RS_LOW;
	if( tsOptions.bitMode == 8 ) {
		KS0066_Cmd8bit(cmd);
	}
	else {
		/* High nibble */
		KS0066_Cmd4bit(cmd >> 4);
		/* Low nibble */
		KS0066_Cmd4bit(cmd & 0x0F);
	}
}

void KS0066_Data(uint8_t data) {
	/* Data mode */
	RS_HIGH;
	if( tsOptions.bitMode == 8 ) {
		KS0066_Cmd8bit(data);
	}
	else {
		/* High nibble */
		KS0066_Cmd4bit(data >> 4);
		/* Low nibble */
		KS0066_Cmd4bit(data & 0x0F);
	}
}

void KS0066_Cmd8bit(uint8_t cmd) {
	gpio_set_value(DATA7,(cmd >> 7 ) & 0x01 );
	gpio_set_value(DATA6,(cmd >> 6 ) & 0x01 );
	gpio_set_value(DATA5,(cmd >> 5 ) & 0x01 );
	gpio_set_value(DATA4,(cmd >> 4 ) & 0x01 );
	gpio_set_value(DATA3,(cmd >> 3 ) & 0x01 );
	gpio_set_value(DATA2,(cmd >> 2 ) & 0x01 );
	gpio_set_value(DATA1,(cmd >> 1 ) & 0x01 );
	gpio_set_value(DATA0,(cmd >> 0 ) & 0x01 );

	KS0066_E_BLINK;
}

void KS0066_Cmd4bit(uint8_t cmd) {
	gpio_set_value(DATA7,(cmd & 0x08) );
	gpio_set_value(DATA6,(cmd & 0x04) );
	gpio_set_value(DATA5,(cmd & 0x02) );
	gpio_set_value(DATA4,(cmd & 0x01) );

	KS0066_E_BLINK;
}

void KS0066_CursorSet(uint8_t col, uint8_t row) {
	uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	
	/* Go to beginning */
	if (row >= tsOptions.Rows) {
		row = 0;
	}
	/* Set current column and row */
	tsOptions.currentX = col;
	tsOptions.currentY = row;
	
	/* Set location address */
	KS0066_Cmd(sDdram | (col + row_offsets[row]));
}

void KS0066_InitPins(void) {
	PIN_DIRECTION gpio_direction = OUTPUT_PIN;


	// @ LCD power optioanl.
	gpio_export(LCDPOWER);			 // set GPIO pin export
	gpio_set_direction(LCDPOWER, gpio_direction);  // set GPIO pin direction
	gpio_set_value(LCDPOWER, 1);			 // set GPIO pin value
	Wait(100);

	gpio_export(RW_PIN);			 // set GPIO pin export
	gpio_set_direction(RW_PIN, gpio_direction);  // set GPIO pin direction
	gpio_set_value(RW_PIN, 0);			 // set GPIO pin value
	Wait(100);

	gpio_export(RS_PIN);			 // set GPIO pin export
	gpio_set_direction(RS_PIN, gpio_direction);  // set GPIO pin direction
	gpio_set_value(RS_PIN, 0);			 // set GPIO pin value
	Wait(100);
	gpio_export(EN_PIN);			 // set GPIO pin export
	gpio_set_direction(EN_PIN, gpio_direction);  // set GPIO pin direction
	gpio_set_value(EN_PIN, 0);			 // set GPIO pin value
	Wait(100);


	gpio_export(DATA7);			 // set GPIO pin export
	gpio_set_direction(DATA7, gpio_direction);  // set GPIO pin direction
	gpio_set_value(DATA7, 0);			 // set GPIO pin value
	Wait(100);
	gpio_export(DATA6);			 // set GPIO pin export
	gpio_set_direction(DATA6, gpio_direction);  // set GPIO pin direction
	gpio_set_value(DATA6, 0);			 // set GPIO pin value
	Wait(100);
	gpio_export(DATA5);			 // set GPIO pin export
	gpio_set_direction(DATA5, gpio_direction);  // set GPIO pin direction
	gpio_set_value(DATA5, 0);			 // set GPIO pin value
	Wait(100);
	gpio_export(DATA4);			 // set GPIO pin export
	gpio_set_direction(DATA4, gpio_direction);  // set GPIO pin direction
	gpio_set_value(DATA4, 0);			 // set GPIO pin value
	Wait(100);

	// @ LCD 8 bit mode optioanl.
	gpio_export(DATA3);			 // set GPIO pin export
	gpio_set_direction(DATA3, gpio_direction);  // set GPIO pin direction
	gpio_set_value(DATA3, 0);			 // set GPIO pin value
	Wait(100);
	gpio_export(DATA2);			 // set GPIO pin export
	gpio_set_direction(DATA2, gpio_direction);  // set GPIO pin direction
	gpio_set_value(DATA2, 0);			 // set GPIO pin value
	Wait(100);
	gpio_export(DATA1);			 // set GPIO pin export
	gpio_set_direction(DATA1, gpio_direction);  // set GPIO pin direction
	gpio_set_value(DATA1, 0);			 // set GPIO pin value
	Wait(100);
	gpio_export(DATA0);			 // set GPIO pin export
	gpio_set_direction(DATA0, gpio_direction);  // set GPIO pin direction
	gpio_set_value(DATA0, 0);			 // set GPIO pin value
}
