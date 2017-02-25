// Simple strand test for Adafruit Dot Star RGB LED strip.
// This is a basic diagnostic tool, NOT a graphics demo...helps confirm
// correct wiring and tests each pixel's ability to display red, green
// and blue and to forward data down the line.  By limiting the number
// and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!

#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...
// #include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET

#include <avr/pgmspace.h>
#include "fonts.h"

#define NUMPIXELS 16 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN    4
#define CLOCKPIN   5
Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
//Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);


const int xInput = A0;
const int yInput = A1;
//const int zInput = A2;

int xMax = 0;
int xMin = 1024;
int yMax = 0;
int yMin = 1024;

int xPrev;
int yPrev;
//int zPrev;

bool direction = true; // true is forwards, false is backwards


char text[32] = "Hilma ";

char *textPtr = &text[0];

int charDelay = 5;

void setup() {
  
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  analogReference(EXTERNAL);

  pinMode(xInput, INPUT);
  pinMode(yInput, INPUT);
//  pinMode(zInput, INPUT);

  xPrev = analogRead(xInput);
  yPrev = analogRead(yInput);
//  zPrev = analogRead(zInput);

  Serial.begin(115200);

  while(!Serial);
  Serial.println("Starting...");

  Serial.println(sizeof Ubuntu_Condensed12x16);
  
  strip.begin(); // Initialize pins for output
  strip.clear();
  strip.show();  // Turn all LEDs off ASAP

}

void loop() {

  int xRaw = analogRead(xInput);
  int yRaw = analogRead(yInput);

  yMax = yRaw > yMax ? yRaw : yMax;
  yMin = yRaw < yMin ? yMin : yMin;

  if(!direction && (yMax - yRaw) > 20) {
    direction = true;
    yMax = 0;
    textPtr = &text[0];
  }
    
  if(direction && (yMin - yRaw) < -20) {
    yMin = 1024;
    direction = false;
  }

  Serial.print(direction ? 400 : 600, DEC);
  Serial.print(" ");
  Serial.print(yRaw, DEC);
//  Serial.print(" ");
//  Serial.print(xRaw, DEC);
  Serial.println();

  xPrev = xRaw;
  yPrev = yRaw;
//  zPrev = zRaw;

  if(direction) {
    if(*textPtr != 0)
      displayLetter(*(textPtr++), 0x00ffff, 1, direction);
  } else {
    if(textPtr >= &text)
      displayLetter(*(textPtr--), 0x00ffff, 1, direction);
  }

  strip.clear();
  strip.show();
  
  delay(charDelay);
  
}

void displayLetter(int letter, uint32_t color, int lineDelay, bool direction) {
  if(direction) {
    int index = Ubuntu_Condensed12x16 + (letter - 32) * BYTES_PER_CHAR;
    //Serial.println(index);
    uint8_t rows = pgm_read_byte_near(index++);
    //Serial.println(rows);
    for( uint8_t i = 0; i < rows; i++) {
      uint8_t b = pgm_read_byte_near(index++);
      
      //Serial.print(b, HEX);
      if(b & 0x80) { strip.setPixelColor(8, color); } else { strip.setPixelColor(8, 0); }
      if(b & 0x40) { strip.setPixelColor(9, color); } else { strip.setPixelColor(9, 0); }
      if(b & 0x20) { strip.setPixelColor(10, color); } else { strip.setPixelColor(10, 0); }
      if(b & 0x10) { strip.setPixelColor(11, color); } else { strip.setPixelColor(11, 0); }
      if(b & 0x08) { strip.setPixelColor(12, color); } else { strip.setPixelColor(12, 0); }
      if(b & 0x04) { strip.setPixelColor(13, color); } else { strip.setPixelColor(13, 0); }
      if(b & 0x02) { strip.setPixelColor(14, color); } else { strip.setPixelColor(14, 0); }
      if(b & 0x01) { strip.setPixelColor(15, color); } else { strip.setPixelColor(15, 0); }
      
      b = pgm_read_byte_near(index++);
      //Serial.println(b, HEX);
      if(b & 0x80) { strip.setPixelColor(0, color); } else { strip.setPixelColor(0, 0); }
      if(b & 0x40) { strip.setPixelColor(1, color); } else { strip.setPixelColor(1, 0); }
      if(b & 0x20) { strip.setPixelColor(2, color); } else { strip.setPixelColor(2, 0); }
      if(b & 0x10) { strip.setPixelColor(3, color); } else { strip.setPixelColor(3, 0); }
      if(b & 0x08) { strip.setPixelColor(4, color); } else { strip.setPixelColor(4, 0); }
      if(b & 0x04) { strip.setPixelColor(5, color); } else { strip.setPixelColor(5, 0); }
      if(b & 0x02) { strip.setPixelColor(6, color); } else { strip.setPixelColor(6, 0); }
      if(b & 0x01) { strip.setPixelColor(7, color); } else { strip.setPixelColor(7, 0); }
  
      strip.show();
      delay(lineDelay);
    }
  } else {
    int index = Ubuntu_Condensed12x16 + (letter - 32) * BYTES_PER_CHAR;
    //Serial.println(index);
    uint8_t rows = pgm_read_byte_near(index);
    //Serial.println(rows);
    index += BYTES_PER_CHAR;
    for( uint8_t i = 0; i < rows; i++) {
      uint8_t b = pgm_read_byte_near(--index);
      
      //Serial.print(b, HEX);
      if(b & 0x80) { strip.setPixelColor(0, color); } else { strip.setPixelColor(0, 0); }
      if(b & 0x40) { strip.setPixelColor(1, color); } else { strip.setPixelColor(1, 0); }
      if(b & 0x20) { strip.setPixelColor(2, color); } else { strip.setPixelColor(2, 0); }
      if(b & 0x10) { strip.setPixelColor(3, color); } else { strip.setPixelColor(3, 0); }
      if(b & 0x08) { strip.setPixelColor(4, color); } else { strip.setPixelColor(4, 0); }
      if(b & 0x04) { strip.setPixelColor(5, color); } else { strip.setPixelColor(5, 0); }
      if(b & 0x02) { strip.setPixelColor(6, color); } else { strip.setPixelColor(6, 0); }
      if(b & 0x01) { strip.setPixelColor(7, color); } else { strip.setPixelColor(7, 0); }
  
      
      b = pgm_read_byte_near(--index);
      //Serial.println(b, HEX);
      if(b & 0x80) { strip.setPixelColor(8, color); } else { strip.setPixelColor(8, 0); }
      if(b & 0x40) { strip.setPixelColor(9, color); } else { strip.setPixelColor(9, 0); }
      if(b & 0x20) { strip.setPixelColor(10, color); } else { strip.setPixelColor(10, 0); }
      if(b & 0x10) { strip.setPixelColor(11, color); } else { strip.setPixelColor(11, 0); }
      if(b & 0x08) { strip.setPixelColor(12, color); } else { strip.setPixelColor(12, 0); }
      if(b & 0x04) { strip.setPixelColor(13, color); } else { strip.setPixelColor(13, 0); }
      if(b & 0x02) { strip.setPixelColor(14, color); } else { strip.setPixelColor(14, 0); }
      if(b & 0x01) { strip.setPixelColor(15, color); } else { strip.setPixelColor(15, 0); }
      
      strip.show();
      delay(lineDelay);
    }
  }
}

void blink(){
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
}

