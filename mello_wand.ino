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
#include "fonts.h"

#define NUMPIXELS 60 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
//#define DATAPIN    4
//#define CLOCKPIN   5
//Adafruit_DotStar strip = Adafruit_DotStar(
//  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);

void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif
  Serial.begin(9600);
  strip.begin(); // Initialize pins for output
  strip.clear();
  strip.show();  // Turn all LEDs off ASAP
}

// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)

void loop() {
  displayLetter('a', 0xff00ff);

  delay(200);
  
  displayLetter('B', 0xffff00);

  delay(200);
  
//  strip.setPixelColor(head, color); // 'On' pixel at head
//  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
//  strip.show();                     // Refresh strip
//  //delay(20);                        // Pause 20 milliseconds (~50 FPS)
//
//  if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
//    head = 0;                       //  Yes, reset head index to start
//    if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
//      color = 0xFF0000;             //   Yes, reset to red
//  }
//  if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
}

void displayLetter(int letter, uint32_t color) {
  uint8_t *rowPtr = &FreeSans12x15[(letter - 32) * BYTES_PER_CHAR];
  int rows = *rowPtr;
  Serial.println(rows);
  for( uint8_t i = 0; i < rows; i++) {
    uint8_t b = *(rowPtr++);
    Serial.println(b);
    if(b & 0x80) { strip.setPixelColor(8, color); b ^= 0x80; } else { strip.setPixelColor(8, 0); }
    if(b & 0x40) { strip.setPixelColor(9, color); b ^= 0x40; } else { strip.setPixelColor(9, 0); }
    if(b & 0x20) { strip.setPixelColor(10, color); b ^= 0x20; } else { strip.setPixelColor(10, 0); }
    if(b & 0x10) { strip.setPixelColor(11, color); b ^= 0x10; } else { strip.setPixelColor(11, 0); }
    if(b & 0x08) { strip.setPixelColor(12, color); b ^= 0x08; } else { strip.setPixelColor(12, 0); }
    if(b & 0x04) { strip.setPixelColor(13, color); b ^= 0x04; } else { strip.setPixelColor(13, 0); }
    if(b & 0x02) { strip.setPixelColor(14, color); b ^= 0x02; } else { strip.setPixelColor(14, 0); }
    if(b & 0x01) { strip.setPixelColor(15, color); b ^= 0x01; } else { strip.setPixelColor(15, 0); }
    
    b = *(rowPtr++);
    Serial.println(b);
    if(b & 0x80) { strip.setPixelColor(0, color); b ^= 0x80; } else { strip.setPixelColor(0, 0); }
    if(b & 0x40) { strip.setPixelColor(1, color); b ^= 0x40; } else { strip.setPixelColor(1, 0); }
    if(b & 0x20) { strip.setPixelColor(2, color); b ^= 0x20; } else { strip.setPixelColor(2, 0); }
    if(b & 0x10) { strip.setPixelColor(3, color); b ^= 0x10; } else { strip.setPixelColor(3, 0); }
    if(b & 0x08) { strip.setPixelColor(4, color); b ^= 0x08; } else { strip.setPixelColor(4, 0); }
    if(b & 0x04) { strip.setPixelColor(5, color); b ^= 0x04; } else { strip.setPixelColor(5, 0); }
    if(b & 0x02) { strip.setPixelColor(6, color); b ^= 0x02; } else { strip.setPixelColor(6, 0); }
    if(b & 0x01) { strip.setPixelColor(7, color); b ^= 0x01; } else { strip.setPixelColor(7, 0); }

    delay(500);
  }
}

