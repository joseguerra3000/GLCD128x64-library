/**
  GLCD Driver API Header File for Arduino-like Boards

  @Author
    Jose Guerra Carmenate

  @File Name
    GLCD128x64.h

  @Summary
    This is the header file for GLCD (based on ST7920 chip) driver using Arduino

  @Description
    This header file provides APIs for drive GLCDs based on ST7920 chip.
    This file contains all the constants, classes and data types needed to control GLCD based on the ST920 chip.
*/

#ifndef _GLCD_H
#define _GLCD_H


#include <Arduino.h>
#include <Print.h>


/**
 * Section: define Macros and constants 
 */

//basic commands
#define LCD_FunctionSet 		0x30u
#define LCD_DisplayControl 		0x08u
#define LCD_DisplayClear 		0x01u
#define LCD_ReturnHome			0x02u
#define LCD_EntryMode	 		0x04u
#define LCD_SetDdramAddress 	0x80u // or-ed with address

//extended commands
#define LCD_FunctionSetExtended 0x24u
#define LCD_FunctionScrollOrAddressSelect 0x02u
#define LCD_DisplayControlExtended 0x0Cu
#define LCD_SetGraphicAddress 0x80u // or-ed with address

//FunctionSet features
#define LCD_INTERFACE_8BITS 0x10u
#define LCD_INTERFACE_4BITS 0x00u
#define LCD_GRAPHIC_ON      0x02u // only for extended
#define LCD_GRAPHIC_OFF     0x00u // only for extended

//DisplayControl features
#define LCD_DISPLAY_ON 		0x04u
#define LCD_DISPLAY_OFF		0x00u
#define LCD_CURSOR_ON		0x02u
#define LCD_CURSOR_OFF		0x00u
#define LCD_BLINK_ON		0x01u
#define LCD_BLINK_OFF		0x00u

//EntryMode features
#define LCD_CURSOR_MOVE_RIGHT 0x02u
#define LCD_CURSOR_MOVE_LEFT  0x00u
#define LCD_DISPLAY_SHIFT	  0x01u
#define LCD_DISPLAY_NO_SHIFT  0x00u


// data types

enum SCREEN_SECTION{
  SCREEN_COMPLETE,
  SCREEN_TOP_HALF,
  SCREEN_BOTTOM_HALF,
  SCREEN_QUADRANT_1,
  SCREEN_QUADRANT_2,
  SCREEN_QUADRANT_3,
  SCREEN_QUADRANT_4  
};



class GLCD128x64 : public Print{
	public: 
		//functions
		
		GLCD128x64(){}
		
		GLCD128x64( uint8_t rs, uint8_t rw, uint8_t en, uint8_t pbs, uint8_t d7, uint8_t d6, uint8_t d5, uint8_t d4, uint8_t d3, uint8_t d2, uint8_t d1, uint8_t d0 );
		
		void begin();
		//text functions
		void textMode( ); // put the lcd on text mode

		void clear(){ sendCommand( LCD_DisplayClear ); }
		void home(){ sendCommand( LCD_ReturnHome ); }
		void noDisplay(); // hide the display data
		void display();   // show the display data
		void noBlink();	  // off the blinking cursor
		void blink();	  // on the blinking cursor
		void noCursor();  // hide the cursor
		void cursor();	  // show the cursor
		
		void setCursor(uint8_t,uint8_t); // set cursor position
		
		
		virtual inline size_t write( uint8_t x){ sendData(x); } // allow to use print and println routine
		
		// graphics functions
		void graphicMode();								// put the lcd
		void drawPixel( int8_t x, int8_t y );			// draw one pixel on the screen buffer
		void drawLine( int x0, int y0, int x1, int y1 );// draw one line on the screen buffer
		void drawCircle( int x0, int y0, int radius );  // draw one circle on the screen buffer
		void renderScreenBuffer( uint8_t screen = SCREEN_COMPLETE ); // flush the screen buffer to GLCD   
		void clearScreenBuffer();							// clear screen buffer 
		void clearScreenBuffer( uint8_t x0, uint8_t y0, uint8_t xf, uint8_t yf );
	private:
		inline void outputPins( uint8_t rs, uint8_t rw, uint8_t data );
		inline void strobe();
		void sendCommand( uint8_t cmd );
		void sendData( char d );
};

typedef GLCD128x64 GLCD;

#endif
