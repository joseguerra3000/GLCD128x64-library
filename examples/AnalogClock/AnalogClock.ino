/**
 * @Project: GLCD128x64 - Analog Clock
 * This example demostrate how use a 128x64 GLCD display to display both 
 * characters and graphics at the same time. The GLCD128x64 Library work 
 * with 128x64 GLCD display based on ST920 chip.
 * 
 * This sketch uses a bigest part of the routines availables on the
 * library.
 * 
 * @Example Dependencias:
 *  - Time Library
 *  - DS1302 Library
 *  
 * @Circuit:
 *  - GLCD RS to digital pin 17
 *  - GLCD RW to digital pin 16
 *  - GLCD EN to digital pin 18
 *  - GLCD PBS to digital pin 14
 *  - GLCD D7 to digital pin 7
 *  - GLCD D6 to digital pin 6
 *  - GLCD D5 to digital pin 5
 *  - GLCD D4 to digital pin 4
 *  - GLCD D3 to digital pin 11
 *  - GLCD D2 to digital pin 10
 *  - GLCD D1 to digital pin 9
 *  - GLCD D0 to digital pin 8
 *  
 */
#include <Time.h>
#include <DS1302RTC.h>
// include the library
#include <GLCD128x64.h>

// Initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
// GLCD( RS, RW, EN, PBS, d7, d6, d5, d4, d3, d2, d1, d0 )
GLCD lcd( 17, 16, 18, 14, 7, 6, 5, 4, 11, 10, 9, 8 );
DS1302RTC RTC(27, 29, 31);

void setup() {

  Serial.begin( 9600 );

  //set up the GLCD
  lcd.begin();


  if (RTC.haltRTC())
    Serial.print("Clock stopped!");
  else
    Serial.print("Clock working.");

  if (RTC.writeEN())
    Serial.print("Write allowed.");
  else
    Serial.print("Write protected.");
  delay ( 2000 );

  Serial.print("RTC Sync");
  setSyncProvider(RTC.get);          // the function to get the time from the RTC
  if(timeStatus() == timeSet)
    Serial.print(" Ok!");
  else
    Serial.print(" FAIL!");
  
}

void loop() {
  // put the glcd on text mode
  lcd.textMode();
  // clear the Text 
  lcd.clear();
  delay(1);
  // set the cursor on the first line and row 4 (half of the screen)
  lcd.setCursor( 4,0 );
  //print text
  lcd.print( "Analog" );
  // set the cursor on the second line and row 4 (half of the screen)
  lcd.setCursor( 4,1 );
  //print text
  lcd.print( "Clock" );
  // set the cursor on the third line and row 4 (half of the screen)
  lcd.setCursor( 4,2 );
  // print one integer
  lcd.print( hour() );
  //print text
  lcd.print( ":" );
  // print one integer
  lcd.print( minute() );
  //print text
  lcd.print( ":" );
  // print one integer
  lcd.print( second() );

  // put the glcd on graphic mode
  lcd.graphicMode();
  // draw one line form (63; 0)  to (63;63)
  // split the screen 
  lcd.drawLine( 63, 0, 63, 63 );
  //draw a analog clock 
  drawClock( 31, 31, 30, hour(), minute(), second() );
  // render the screen quadrant 2
  lcd.renderScreenBuffer(SCREEN_QUADRANT_2);
  // render the screen quadrant 3
  lcd.renderScreenBuffer(SCREEN_QUADRANT_3);
  // clear the screen buffer
  lcd.clearScreenBuffer();
  delay(250);
}


void drawClock( uint8_t x0, uint8_t y0, uint16_t radius, uint8_t h, uint8_t m, uint8_t s ){
    lcd.drawCircle( x0, y0, radius ); 
    lcd.drawCircle( x0, y0, 1 ); 
    lcd.drawCircle( x0, y0, 2 ); 

    uint8_t len = radius*8/10;
    lcd.drawLine( x0 + len, y0, x0 + radius , y0 ); 
    lcd.drawLine( x0 - len, y0, x0 - radius , y0 ); 
    lcd.drawLine( x0, y0 + len, x0, y0 + radius ); 
    lcd.drawLine( x0, y0 - len, x0, y0 - radius ); 

    uint8_t hourLen = radius*6/10, 
            minLen  = radius*7/10, 
            secLen  = radius*8/10;
    // draw hour
    double hourAngle = double(M_PI*( 60*h + m ))/360;
    double dx = hourLen*cos( M_PI/2.0 - hourAngle ),
           dy = -hourLen*cos( hourAngle );

    lcd.drawLine( x0, y0, x0 + (int8_t)dx, y0 + (int8_t)dy ); 
    
    // draw min
    double minAngle = double(M_PI*m)/30;
    dx = minLen*cos( M_PI/2.0 - minAngle );
    dy = -minLen*cos( minAngle );

    lcd.drawLine( x0, y0, x0 + (int8_t)dx, y0 + (int8_t)dy ); 

    // draw sec
    double secAngle = double(M_PI*s)/30;
    dx = secLen*cos( M_PI/2.0 - secAngle );
    dy = -secLen*cos( secAngle );

    lcd.drawLine( x0, y0, x0 + (int8_t)dx, y0 + (int8_t)dy ); 


}



