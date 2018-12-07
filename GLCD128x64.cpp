/**
  GLCD Driver API Header File for Arduino-like Boards

  @Author
    Jose Guerra Carmenate

  @File Name
    GLCD128x64.cpp

  @Summary
    This is the header file for GLCD (based on ST7920 chip) driver using Arduin

  @Description
    This source file contains the APIs Implementation for drive GLCDs based on ST7920 chip.
*/


/**
 * Section: Incude Macros 
 */

#include "GLCD128x64.h"


const uint8_t offsetLine[] = { 0x80u, 0x90u, 0x88u, 0x98u };



/**
 * Section: Local Vars 
 */

uint8_t RS;// = 17;
uint8_t RW;// = 16;
uint8_t EN;// = 18;
uint8_t PBS;// = 14;

uint8_t D[8];// = { 8,9,10,11,4,5,6,7 };

uint8_t ScreenBuffer[32][32];

uint8_t _displayControl;
uint8_t _displayFunction;

/**
 * Section: Private Functions 
 */

inline void GLCD128x64::outputPins( uint8_t rs, uint8_t rw, uint8_t data ){
  digitalWrite( RS, rs );
  digitalWrite( RW, rw );
  for( uint8_t i = 0; i < 8u; i++ ){
    if( data & (1u<<i) )
      digitalWrite( D[i], 1u );
    else
      digitalWrite( D[i], 0u );
  }
}

inline void GLCD128x64::strobe(){
  digitalWrite(EN,1u);  
  delayMicroseconds(1u);
  digitalWrite(EN,0u);    
}

void  GLCD128x64::sendCommand( uint8_t cmd ){
//  digitalWrite( EN, 1 );
//  delayMicroseconds( 80 )
  outputPins( 0u, 0u, cmd );
  strobe();
}

void GLCD128x64::sendData( char d ){
  outputPins( 1u, 0u, d );  
  strobe();
}
// general functions end


/**
 * Section: API Functions 
 */

GLCD128x64::GLCD128x64( uint8_t rs, uint8_t rw, uint8_t en, uint8_t pbs, uint8_t d7, uint8_t d6, uint8_t d5, uint8_t d4, uint8_t d3, uint8_t d2, uint8_t d1, uint8_t d0 ){
	RS = rs;
	RW = rw;
	EN = en;
	PBS = pbs;
	D[7] = d7;
	D[6] = d6;
	D[5] = d5;
	D[4] = d4;
	D[3] = d3;
	D[2] = d2;
	D[1] = d1;
	D[0] = d0;
	
	pinMode( RS, OUTPUT );
  pinMode( RW, OUTPUT );
  pinMode( EN, OUTPUT );
  for( uint8_t i = 0; i < 8; i++ )
    pinMode( D[i], OUTPUT );
  
  pinMode( PBS, OUTPUT );
	digitalWrite( PBS, HIGH );
}

/**
 * Section: API functions for Text Mode
 */
void GLCD128x64::textMode( ){
  //Wait time >40ms
  //	delay(50); 
  // XRESET LOW -> HIGH
  // No lo hago

  //Function Set
  sendCommand( LCD_FunctionSet | LCD_INTERFACE_8BITS );
  //Wait time > 100uS
  delayMicroseconds(150);
  
  //Function Set
  sendCommand( LCD_FunctionSet | LCD_INTERFACE_8BITS );
  //Wait time > 37uS
  delayMicroseconds(60);

  //Display ON/OFF  Control
  sendCommand( LCD_DisplayControl | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF );
  //Wait time > 100uS
  delayMicroseconds(150);

  sendCommand( LCD_ReturnHome );
  //Wait time > 10mS
  delay(20);
  
  sendCommand( LCD_EntryMode | LCD_CURSOR_MOVE_RIGHT | LCD_DISPLAY_NO_SHIFT );

  delay(50);
}

void GLCD128x64::noDisplay(){
	_displayControl &= ~(LCD_DISPLAY_ON);
	sendCommand( LCD_DisplayControl | _displayControl );
}

void GLCD128x64::display(){
	_displayControl |= (LCD_DISPLAY_ON);
	sendCommand( LCD_DisplayControl | _displayControl );
}

void GLCD128x64::noBlink(){
	_displayControl &= ~(LCD_BLINK_ON);
	sendCommand( LCD_DisplayControl | _displayControl );
}

void GLCD128x64::blink(){
	_displayControl |= (LCD_BLINK_ON);
	sendCommand( LCD_DisplayControl | _displayControl );
}

void GLCD128x64::noCursor(){
	_displayControl &= ~(LCD_CURSOR_ON);
	sendCommand( LCD_DisplayControl | _displayControl );
}

void GLCD128x64::cursor(){
	_displayControl |= (LCD_CURSOR_ON);
	sendCommand( LCD_DisplayControl | _displayControl );
}

void GLCD128x64::setCursor( uint8_t col, uint8_t row ){
	if( row >= 4u) 
		row = 3u;
	
	if( col >= 8u )
		col = 7u;
	
	sendCommand( LCD_SetDdramAddress | (offsetLine[row] + col ) );
}

// extended functions begin
void GLCD128x64::begin(){
  
  digitalWrite( PBS, HIGH );
    
    _displayControl = LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF;
    _displayFunction = LCD_INTERFACE_8BITS;
    
    
    sendCommand( LCD_FunctionSetExtended | _displayFunction | LCD_GRAPHIC_OFF );
    delay(1);
    sendCommand( LCD_FunctionSet | _displayFunction );
    delay(1);
    
    sendCommand( LCD_DisplayClear );
    delay(1);
    
    sendCommand( LCD_EntryMode | LCD_CURSOR_MOVE_RIGHT | LCD_DISPLAY_NO_SHIFT );
    delay(1);
    
    sendCommand( LCD_DisplayControl | _displayControl );

    delayMicroseconds(80);
    delayMicroseconds(80);
    
    sendCommand( LCD_FunctionSetExtended | _displayFunction | LCD_GRAPHIC_OFF );
    delay(1);
    sendCommand( LCD_FunctionScrollOrAddressSelect );
    delay(1);
	
	//Clear screen
    for( uint8_t i = 0; i < 32; i++ ){
       sendCommand( LCD_SetGraphicAddress | i ); // y
        delay(1);
       sendCommand( LCD_SetGraphicAddress | 0 ); // x
        delay(1);
        
       for( uint8_t j = 0; j < 32; j++ ){
        delay(1);
        sendData( 0u );
        }
    }
    
}

void GLCD128x64::graphicMode(){
  sendCommand( LCD_FunctionSetExtended | _displayFunction | LCD_GRAPHIC_OFF );  // functionSetExt with G = 0
	delayMicroseconds(80);
  sendCommand( LCD_FunctionSetExtended | _displayFunction | LCD_GRAPHIC_ON );  // functionSetExt with G = 1
}

uint8_t readData(  ){ // no funciona

  for( int i = 0; i < 8; i++ )
    pinMode( D[i], INPUT );
  
  digitalWrite( RW, HIGH );
  digitalWrite( EN, HIGH );
  
  delayMicroseconds(80);
  uint8_t res = 0;
  for( int i = 0; i < 8; i++ )
    res |= digitalRead( D[i] )?1u:0u<<i;
  
  digitalWrite( EN, LOW );
  
  delayMicroseconds(80);
  for( int i = 0; i < 8; i++ )
    pinMode( D[i], OUTPUT );
    
  return res;
}


void GLCD128x64::drawPixel( int8_t x, int8_t y ){
  if( y > 63 || y < 0 ){
     return;
    }
  if( x/8 + 16 > 32 || x < 0 ){
     return;
  }
  if( y < 32 )
    ScreenBuffer[ x/8 ][ y ] |= 1u<<( 8 -  x%8 -1);  
  else
    ScreenBuffer[ x/8 + 16 ][ y-32 ] |= 1u<<( 8 -  x%8 -1);  
  
}

void GLCD128x64::drawLine( int x0, int y0, int x1, int y1 ){ // draw a line using the Bresenham Algorithm
                                                 // based on LCD12864 implementation
  int dx = (x1 - x0); 
  int dy = (y1 - y0);
  bool steep = (abs(dy) >= abs(dx));
  if ( steep ) {
    x0 ^= y0;
    y0 ^= x0;
    x0 ^= y0;

    x1 ^= y1;
    y1 ^= x1;
    x1 ^= y1;

    // recompute dx, dy after swap
    dx = x1 - x0;
    dy = y1 - y0;
   }
   int xstep = 1;
   if (dx < 0) {
       xstep = -1;
       dx = -dx;
   }
   int ystep = 1;
   if (dy < 0) {
       ystep = -1;    
       dy = -dy; 
   }
   int TwoDy = 2*dy; 
   int TwoDyTwoDx = TwoDy - 2*dx; // 2*Dy - 2*Dx
   int E = TwoDy - dx; //2*Dy - Dx
   int y = y0;
   for (int x = x0 ; x != x1; x += xstep) {    
    if (steep) {     
      drawPixel(y, x);
    } else {     
      drawPixel(x, y);
    }
  
     // next point
    if (E > 0) {
      E += TwoDyTwoDx; //E += 2*Dy - 2*Dx;
      y += ystep;
    } else {
      E += TwoDy; //E += 2*Dy;
    }
   }
}

void GLCD128x64::drawCircle( int x0, int y0, int radius ){// draw a line using the Bresenham Algorithm
                                              // based on LCD12864 implementation
      int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;
 
    drawPixel(x0, y0 + radius);
    drawPixel(x0, y0 - radius);
    drawPixel(x0 + radius, y0);
    drawPixel(x0 - radius, y0);
 
    while(x < y)
    {
      if(f >= 0) 
      {
        y--;
        ddF_y += 2;
        f += ddF_y;
      }
      x++;
      ddF_x += 2;
      f += ddF_x;    
      drawPixel(x0 + x, y0 + y);
      drawPixel(x0 - x, y0 + y);
      drawPixel(x0 + x, y0 - y);
      drawPixel(x0 - x, y0 - y);
      
      drawPixel(x0 + y, y0 + x);
      drawPixel(x0 - y, y0 + x);
      drawPixel(x0 + y, y0 - x);
      drawPixel(x0 - y, y0 - x);
    }
  
  }


void GLCD128x64::renderScreenBuffer( uint8_t screen = SCREEN_COMPLETE ){
  uint8_t startX, // GDRAM Horizontal address [0, 15]
  		  cantX;  // bytes to write
  switch( screen ){
    case SCREEN_COMPLETE:
      startX = 0;
      cantX = 32;
      break;
    case SCREEN_TOP_HALF:
      startX = 0;
      cantX = 16;
      break;
    case SCREEN_BOTTOM_HALF:
      startX = 8;
      cantX = 16;
      break;
    case SCREEN_QUADRANT_1:
      startX = 4;
      cantX = 8;
      break;
    case SCREEN_QUADRANT_2:
      startX = 0;
      cantX = 8;
      break;
    case SCREEN_QUADRANT_3:
      startX = 8;
      cantX = 8;
      break;
    case SCREEN_QUADRANT_4:
      startX = 12;
      cantX = 8;
      break;
    }
  cantX += 2*startX;
  for( uint8_t i = 0; i < 32; i++ ){  // iterate for 'y' axis
       sendCommand( LCD_SetGraphicAddress | i ); // y
        delayMicroseconds(80);
        sendCommand( LCD_SetGraphicAddress | startX ); // x
        //delayMicroseconds(80);
        
       for( uint8_t j = 2*startX; j < cantX; j++ ){
        delayMicroseconds(80);
        sendData( ScreenBuffer[j][i] );
       }
  }
  
}

void GLCD128x64::clearScreenBuffer(){
	for( uint8_t i = 0; i < 32; i++ )
		for( uint8_t j = 0; j < 32; j++ )
			ScreenBuffer[i][j] = 0;
}


