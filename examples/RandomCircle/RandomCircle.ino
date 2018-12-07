/**
 * @Project: GLCD128x64 - RandomCircles
 * This example demostrate how use a 128x64 GLCD display to draw 
 * circles. The GLCD128x64 Library work with 128x64 GLCD display 
 * based on ST920 chip.
 * 
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
#include <GLCD128x64.h>

GLCD glcd( 17, 16, 18, 14, 7, 6, 5, 4, 11, 10, 9, 8 );


void setup() {
  //set up the GLCD
  glcd.begin();
  // wait on second
  delay( 1000 );
  // put the lcd on graphic mode
  glcd.graphicMode();
  srand( analogRead(A2) );
}

void loop() {
  
  uint8_t x0 = random(0, 128),
          y0 = random( 0, 64 ),
          radius = random(2, 20);

  glcd.drawCircle( x0, y0, radius );
  
  glcd.renderScreenBuffer();  
  delay( 1000 );

}
