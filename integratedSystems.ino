/*Maker Cart Integrated System Code
Author: Jordan Hammer
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
#include <Adafruit_NeoPixel.h> // Allows neo pixles to be used for under shelf lighting
//images to load later
#include "bitmap.h"
#include "bitmap1.h"
#include "bitmap2.h"
#include "pitches.h"// for intro theme

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


// Instantiate the RGBmatrixPanel class.
// For 32x32 LED panels:
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false); // 32x32

// instatiate NeoPixel class for lighting 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, 50, NEO_GRB + NEO_KHZ800);

//declaration of variables for sensor array
  int forceSensor=0;
  int flexSensor=0;
  int proximitySensor=0;
  int slideSensor=0;
  
// The setup() function initializes the matrix, and clears the 
// screen. It also starts Serial. And declares all of the different pins
void setup()
{
  matrix.begin();       // Initialize the matrix.
  strip.begin();        // Initialize lighting
  
  //prints a welcome message to the matrix
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.setTextSize(1);
  matrix.print("Mobil");
 matrix.setTextColor(matrix.Color333(0,7,0));
  matrix.print("Maker");
 matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print("Cart"); 
  
  Serial.begin(9600);   // Start serial
  
tone(44, 2000, 250);
  
  //declaration of connected buttons
  pinMode(2, INPUT_PULLUP); //image 1 
  pinMode(3, INPUT_PULLUP); // //slide sensor and third image                      
  pinMode(4, INPUT_PULLUP); // proximity sensor
  pinMode(5, INPUT_PULLUP); // mode switch                                                        
  pinMode(6, INPUT_PULLUP); // 
  pinMode(7, INPUT_PULLUP); //activate force sensor-button and second image           
  pinMode(8, INPUT_PULLUP); //acticvate flex sensor-button   
  pinMode(44, OUTPUT); //piezo speaker output
  pinMode(48, INPUT_PULLUP); //activate proximity sensor-button 
  pinMode(49, INPUT_PULLUP); //activate slide sensor -button 
  pinMode(54, OUTPUT); //speaker output
  //note pinMode not needed for analog input pins unless being used as digital pins
}

void loop()
{
//leave the lights on\
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
    forceSensor=1;
    flexSensor=0;
    proximitySensor=0;
    slideSensor=0;
   }
  //use button 6 to select flex sensor
   if(digitalRead(8)==0){
    forceSensor=0;
    flexSensor=1;
    proximitySensor=0;
    slideSensor=0;
   }
  //use button 4 to select proximity sensor
   if(digitalRead(4)==0){
    forceSensor=0;
    flexSensor=0;
    proximitySensor=1;
    slideSensor=0;
   }
  //use button 3 to select slide sensor
   if(digitalRead(3)==0){
    forceSensor=0;
    flexSensor=0;
    proximitySensor=0;
    slideSensor=1;
   } 
//let's do something with our sensors
//force sensor fill
  if(forceSensor==1){
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
  if(flexSensor==1){
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
  if(proximitySensor==1){
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
  if(slideSensor==1){
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
}


//end of code 
