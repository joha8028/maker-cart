/*Maker Cart Integrated System Code
Original Author: Jordan Hammer
**************************************
MOST RECENT UPDATE: 1/25/16 6PM          IF YOU UPDATE THIS CODE PLEASE UPDATE THIS SECTION WITH THE DATE, TIME AND YOUR NAME
CURRENT AUTHOR: JORDAN HAMMER                      
**************************************
Portions of this code based on the following:
RGB Panel Paint 
   by: Jim Lindblom
       SparkFun Electronics
   date: December 9, 2013
   license: Beerware. Please use, reuse, and modify this sketch.
   If you find it useful, and we meet some day, we can share a beer.
   
   Check out our hookup guide for help wiring it all up:
     https://learn.sparkfun.com/tutorials/rgb-panel-hookup-guide
   
   You'll need to download and install the RGBmatrixPanel and
   Adafruit_GFX libraries:
     https://github.com/adafruit/RGB-matrix-Panel
     https://github.com/adafruit/Adafruit-GFX-Library
   
   This sketch uses the RGBmatrixPanel and Adafruit_GFX libraries,
   written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
   for Adafruit Industries. Released under the BSD license.
*/

//////////////////////
// Library Includes //
//////////////////////
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
//images to load later
#include "bitmap.h"
#include "bitmap1.h"
#include "bitmap2.h"
//#include "pitches.h"// for intro theme (NOT CURRENTLY USING JAN16)

/////////////////////
// Hardware Hookup //
/////////////////////
// R0, G0, B0, R1, G1, B1 should be connected to pins 
// 2, 3, 4, 5, 6, and 7 respectively. Their pins aren't defined,
// because they're controlled directly in the library. These pins
// can be moved (somewhat):
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3 
// CLK can be moved but must remain on PORTB(8, 9, 10, 11, 12, 13)
#define CLK 11  // MUST be on PORTB!
#define BLINK_RATE 250 // blink rate in ms

// Instantiate the RGBmatrixPanel class.
// For 32x32 LED panels:
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false); // 32x32
// this is done to make our lives easier later 
// this sets up everything for the drawing mode 
enum direction {
  UP,    // 0
  DOWN,  // 1
  LEFT,  // 2
  RIGHT  // 3
};

int8_t cursorX = 0;  // Cursor x position, initialize left
int8_t cursorY = 0;  // Cursor y position, initialize top
int16_t oldColor = 0; // Keep track of color under cursor

  int drawingMode=0;
  int colorCount;
  float color = matrix.Color333(0,0,0);

//declaration of variables for sensor array
  int sensorSetting=0;
  
// The setup() function initializes the matrix, and clears the 
// screen. It also starts Serial. And declares all of the different pins
void setup()
{
  matrix.begin();       // Initialize the matrix.
  
  //prints a welcome message to the matrix
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.setTextSize(1);
  matrix.print("Mobil");
 matrix.setTextColor(matrix.Color333(0,7,0));
  matrix.print("Maker");
 matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print("Cart"); 
  
  Serial.begin(9600);   // Start serial
  
  tone(44, 2000, 250);    //audio indication of reset
  
  //declaration of connected buttons
  pinMode(2, INPUT_PULLUP); //image 1 
  pinMode(3, INPUT_PULLUP); // //slide sensor and third image                      
  pinMode(4, INPUT_PULLUP); // proximity sensor
  pinMode(5, INPUT_PULLUP); // mode switch                                                         
  pinMode(7, INPUT_PULLUP); //activate force sensor-button and second image           
  pinMode(8, INPUT_PULLUP); //acticvate flex sensor-button   
  pinMode(44, OUTPUT); //piezo speaker output
  pinMode(54, OUTPUT); //speaker output
  //note pinMode not needed for analog input pins unless being used as digital pins
}

void loop()
{
//go into drawing mode
if(digitalRead(2) == 0 && digitalRead(4) == 0){
  drawingMode=1;
}
if(drawingMode == 1){
    // These variables keep track of the cursor status. lastUpdate
  // remembers when we last did something to the cursor. 
  // cursorStatus remembers if the cursor is on or off.
  static uint32_t lastUpdate = 0;
  static boolean cursorStatus = true;
  // If enough time has passed, let's blink the cursor:
  if (millis() > lastUpdate + BLINK_RATE)
  {
    lastUpdate = millis(); // get a new lastUpdate value
    blinkCursor(cursorStatus); // blinkCursor on or off
    cursorStatus = !cursorStatus; // Update cursorStatus
  }
  
  // draw dot or select color depending on switch position
  if(digitalRead(5)==0){ //switch is off
    if(digitalRead(2)==0){
      drawDot(true);
  }
}
  if(digitalRead(5)==1){ //switch is off
  //use button 2 to pick a color
  if(digitalRead(2)==0 && colorCount==1 ){
    color = matrix.Color333(7,0,0);
    colorCount = 2;
    delay(500);
  }else  if(digitalRead(2)==0 && colorCount==2 ){
    color = matrix.Color333(0,7,0);
    colorCount = 3;
    delay(500);
  }else  if(digitalRead(2)==0 && colorCount==3 ){
    color = matrix.Color333(0,0,7);
    colorCount = 4;
    delay(500);
  }else  if(digitalRead(2)==0 && colorCount==4 ){
    color = matrix.Color333(7,7,0);
    colorCount = 5;
    delay(500);
  }else  if(digitalRead(2)==0 && colorCount==5 ){
    color = matrix.Color333(0,7,7);
    colorCount = 6;
    delay(500);
  }else  if(digitalRead(2)==0 && colorCount==6 ){
    color = matrix.Color333(7,7,7);
    colorCount = 1;
    delay(500);
  }
  }//end of draw dot or color pick

//movement of cursor controls 
  if(digitalRead(7)==0){
    moveCursor(UP, 1);
    Serial.println(cursorX, cursorY); 
  }else if(digitalRead(3)==0){
    moveCursor(DOWN, 1);
  }else if(digitalRead(8)){
    moveCursor(RIGHT, 1);
  }else if(digitalRead(4)==0){
    moveCursor(LEFT, 1);
  }
}//end of drawing Mode

  //use a switch to control the mode the board is in: image or sensor
if(digitalRead(5)==0){    
 Serial.println("IMAGE MODE!!"); 
  if(digitalRead(2)==0){
    loadBitmap();
  }
  if(digitalRead(3)==0){
    loadBitmap1();
  }
  if(digitalRead(7)==0){
    loadBitmap2();
  }
}else  //tells the arduino to turn on the sensors and turn off the images 
  {
 blankEasel();//clears the display so the sensors work correctly 
   
//use button 5 to select force sensor
   if(digitalRead(7)==0){
    sensorSetting=1;
   }
  //use button 6 to select flex sensor
   if(digitalRead(8)==0){
    sensorSetting=2;
   }
  //use button 4 to select proximity sensor
   if(digitalRead(4)==0){
    sensorSetting=3;
   }
  //use button 3 to select slide sensor
   if(digitalRead(3)==0){
    sensorSetting=4;
   } 
//let's do something with our sensors
//force sensor fill
  if(sensorSetting == 1){
    Serial.println("Force Sensor Active");
 int   forceReading = analogRead(A8);
    delay(1);
    forceReading = map(forceReading, 0, 1000, 0, 6);
    if(forceReading == 1){
      matrix.fillScreen(matrix.Color333(0,0,4));
      delay(300);
    }
    if(forceReading == 2){
      matrix.fillScreen(matrix.Color333(4,0,4));
      delay(300);
    }
    if(forceReading == 3){
      matrix.fillScreen(matrix.Color333(0,4,0));
      delay(300);
    }
    if(forceReading == 4){
      matrix.fillScreen(matrix.Color333(4,4,0));
      delay(300);
    }
    if(forceReading == 5){
      matrix.fillScreen(matrix.Color333(6,3,0));
      delay(300);
    }
    if(forceReading == 6){
      matrix.fillScreen(matrix.Color333(7,0,0));
      delay(300);
    }
  }    
//flex sensor fill
  if(sensorSetting == 2){
    Serial.println("Flex Sensor Active"); 
 int flexReading = analogRead(A9);
 delay(1);
     flexReading = map(flexReading, 250, 500, 0, 6);
    if(flexReading == 1){
      matrix.fillScreen(matrix.Color333(0,0,4));
      delay(300);
    }
    if(flexReading == 2){
      matrix.fillScreen(matrix.Color333(4,0,4));
      delay(300);
    }
    if(flexReading == 3){
      matrix.fillScreen(matrix.Color333(0,4,0));
      delay(300);
    }
    if(flexReading == 4){
      matrix.fillScreen(matrix.Color333(4,4,0));
      delay(300);
    }
    if(flexReading == 5){
      matrix.fillScreen(matrix.Color333(6,3,0));
      delay(300);
    }
    if(flexReading == 6){
      matrix.fillScreen(matrix.Color333(7,0,0));
      delay(300);
    }

  }
//proximity sensor fill
  if(sensorSetting == 3){
    Serial.println("Prox Sensor Active");
    int proximityReading = analogRead(A10);
      delay(1);
        proximityReading = map(proximityReading, 0, 600, 0, 6);
    if(proximityReading == 1){
      matrix.fillScreen(matrix.Color333(0,0,4));
      delay(300);
    }
    if(proximityReading == 2){
      matrix.fillScreen(matrix.Color333(4,0,4));
      delay(300);
    }
    if(proximityReading == 3){
      matrix.fillScreen(matrix.Color333(0,4,0));
      delay(300);
    }
    if(proximityReading == 4){
      matrix.fillScreen(matrix.Color333(4,4,0));
      delay(300);
    }
    if(proximityReading == 5){
      matrix.fillScreen(matrix.Color333(6,3,0));
      delay(300);
    }
    if(proximityReading == 6){
      matrix.fillScreen(matrix.Color333(7,0,0));
      delay(300);
    }

  }
//slide sensor fill
  if(sensorSetting == 4){
    Serial.println("Slide Sensor Active");
    int slideReading = analogRead(A11);
    delay(1);
    slideReading = map(slideReading, 0, 1022, 0, 6);
    if(slideReading == 1){
      matrix.fillScreen(matrix.Color333(0,0,4));
      delay(300);
    }
    if(slideReading == 2){
      matrix.fillScreen(matrix.Color333(4,0,4));
      delay(300);
    }
    if(slideReading == 3){
      matrix.fillScreen(matrix.Color333(0,4,0));
      delay(300);
    }
    if(slideReading == 4){
      matrix.fillScreen(matrix.Color333(4,4,0));
      delay(300);
    }
    if(slideReading == 5){
      matrix.fillScreen(matrix.Color333(6,3,0));
      delay(300);
    }
    if(slideReading == 6){
      matrix.fillScreen(matrix.Color333(7,0,0));
      delay(300);
    }
  }
  }
} // end of void loop()


//***************************************************************
// THIS BEGINS ALL OF THE FUNCTIONS THAT THE ABOVE CODE RELIES ON
// PLEASE DO NOT EDIT, IF YOU HAVE A QUESTION PLEASE POST IT TO THE DISCUSSION
// AND I WILL GET BACK TO YOU
//**************************************************************



// loadBitmap loads up a stored bitmap into the matrix's data
// buffer. If you've printed an image out, use this to load it
// back onto the screen.
void loadBitmap()
{
  uint8_t *ptr = matrix.backBuffer(); // Get address of matrix data
  memcpy_P(ptr, bmp0, sizeof(bmp0));
}

//load the second bitmap
void loadBitmap1()
{
  uint8_t *ptr = matrix.backBuffer(); // Get address of matrix data
  memcpy_P(ptr, bmp1, sizeof(bmp1));  
}
//load bitmap 3
void loadBitmap2()
{
  uint8_t *ptr = matrix.backBuffer(); // Get address of matrix data
  memcpy_P(ptr, bmp2, sizeof(bmp2)); 
}

// Reset the screen. 
void blankEasel()
{
  matrix.fillScreen(0); // Blank screen
}

void moveCursor(uint8_t direction, uint8_t distance)
{
  // If there was a color under the cursor, put it back:
  matrix.drawPixel(cursorX, cursorY, oldColor);
  
  // For each direction, decrease it by distance, then check
  // to make sure it's not out of bounds. If it is, wrap around
  // to the other side.
  switch (direction)
  {
  case UP:
    cursorY-=distance;
    if (cursorY < 0) cursorY = matrix.height() - 1;
    break;
  case DOWN:
    cursorY+=distance;
    if (cursorY > matrix.height() - 1) cursorY = 0;
    break;
  case LEFT:
    cursorX-=distance;
    if (cursorX < 0) cursorX = matrix.width() - 1;
    break;
  case RIGHT:
    cursorX+=distance;
    if (cursorX > matrix.width() - 1) cursorX = 0;
    break;
  }
  // update oldColor before the loop() redraws our cursor.
  oldColor = getPixelColor(cursorX, cursorY);
}

void blinkCursor(boolean stat)
{
  if (stat) // Blink on:
    matrix.drawPixel(cursorX, cursorY, color);
  else      // Blink off
    matrix.drawPixel(cursorX, cursorY, 0);
}

void drawDot(boolean draw)
{
  // We need to both update the oldColor varaiable, which keeps
  // track of a color under the cursor. And send the drawPixel
  // command. First update oldColor:
  if (draw) // if 1 Draw pixel
    oldColor = color;
  else      // else Erase
    oldColor = 0;
  // Then draw the desired color:
  matrix.drawPixel(cursorX, cursorY, oldColor);
}

uint16_t getPixelColor(uint8_t x, uint8_t y)
{
  uint8_t * ptr; // A movable pointer to read data from
  uint16_t r=0, g=0, b=0; // We'll store rgb values in these
  uint8_t nPlanes = 4; // 4 planes. This is static in the library
  uint8_t nRows = matrix.height() / 2; // 16 for 32 row, 8 for 16
  
  // Get a pointer to the matrix backBuffer, where the pixel/
  // color data is stored:
  uint8_t * buf = matrix.backBuffer();
  
  // Data for upper half is stored in lower bits of each byte
  if (y < nRows)
  {
    ptr = &buf[y * matrix.width() * (nPlanes - 1) + x];
    // get data from plane 0:
    if (ptr[64] & 0x01)
      r = 1; 
    if (ptr[64] & 0x02)
      g = 1;
    if (ptr[32] & 0x01)
      b = 1;
    // get data from other planes:
    for (uint8_t i=2; i < (1<<nPlanes);  i <<= 1)
    {
      if (*ptr & 0x04)
        r |= i;
      if (*ptr & 0x08)
        g |= i;
      if (*ptr & 0x10)
        b |= i;
      ptr += matrix.width();
    }
  }
  else
  {
    ptr = &buf[(y - nRows) * matrix.width() * (nPlanes - 1) + x];
    // get data from plane 0
    if (ptr[32] & 0x02)
      r = 1;
    if (*ptr & 0x01)
      g = 1;
    if (*ptr & 0x02)
      b = 1;
    for (uint8_t i=2; i < (1<<nPlanes); i <<= 1)
    {
      if (*ptr & 0x20)
        r |= i;
      if (*ptr & 0x40)
        g |= i;
      if (*ptr & 0x80)
        b |= i;
      ptr += matrix.width();
    }
  }
  return (r << 12) | (g << 7) | (b << 1);
}
// This is a slightly modified version of the dumpMatrix function
// from the RGBmatrixPanel library. Running out of SRAM! So some
// of the static serial prints are stored in flash ("F()"). Also
// do some extra formatting so it works with non-Serial monitor
// terminals.
void printBuffer()
{
  blinkCursor(false); // Turn cursor off first.
  
  int bufferSize = matrix.width() * (matrix.height() / 2) * 3;
  // Get a pointer to the data buffer:
  uint8_t * buf = matrix.backBuffer();
  
  // Print some stuff
  Serial.println();
  Serial.print(F("#include <avr/pgmspace.h>\r\n\r\n"));
  Serial.print(F("static const uint8_t PROGMEM bmp[] = {\r\n"));
  
  for (int i=0; i < bufferSize; i++)
  {
    Serial.print(F("0x"));
    if (buf[i] < 0x10) 
      Serial.print('0');
    Serial.print(buf[i], HEX);
    if (i < (bufferSize - 1))
    {
      if ((i & 7) == 7) 
        Serial.print(F(", \r\n "));
      else 
        Serial.print(',');
    }
  }
  Serial.println(F("\r\n};"));
}
//end of code 
