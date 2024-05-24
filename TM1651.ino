/* Re-Used Gotek LEDC68/TM1651 based 3-digit LED board demo.
 *  Written for the Arduino Uno/Nano/Mega.
 * (c) Ian Neill 2024
 * GPL(v3) Licence
 *
 * Built on work by Derek Cooper. Thank you for the jump start!
 * References:
 *    https://www.instructables.com/Re-use-LEDC86-Old-Gotek-Display/
 */

#include "TM1651.h"

// Pin definitions for the TM1651 - this might look like I2C, but it is not!
#define CLKPIN 2                  // Clock.
#define DIOPIN 3                  // Data In/Out.

// Instantiate a TM1651 display.
TM1651 myDisplay(CLKPIN, DIOPIN); // Defaults: Brightness = 2, display cleared (all segments OFF), decimal points OFF.

void setup() {
  testDisplay();
}

void loop() {
  // Hex number count 0x00 - 0xFF, 1 count/125ms.
  countHex8(125);
  // Hex number count 0x00 - 0xFFF, 1 count/125ms.
  countHex12(125);
  // Decimal number count 0 - 999, 1 count/100ms
  countUp(999, 100);
  // Decimal number count 999 - 0, 1 count/1000ms.
  countDown(999, 1000);
  // A 10 minute timer.
  countXMins(10);
  // A 10 minute timer with flashing decimal points.
  countXMinsDP(10);
}

void testDisplay() {
  byte brightness, digit, character;
  // Display brightness test.
  myDisplay.displayInit();
  for(brightness = BRIGHT_DARKEST; brightness <= BRIGHTEST; brightness++) {
    myDisplay.displaySet(brightness);
    myDisplay.displayInteger(brightness * 111);
    delay(1000);
  }
  // Display all characters on each digit.
  myDisplay.displayInit(); // Defaults: Brightness = 2, display cleared (all segments OFF), decimal points OFF.
  for(digit = 0; digit <= 2; digit++) {
    // Cycle through each code in the character table, exceeding the table size by 1 to finish on a default space (0x00).
    for(character = 0; character <= myDisplay.charTableSize; character++) {
      myDisplay.displayChar(digit, character);
      delay(200);
    }
  }
  // Decimal points ON/OFF test.
  myDisplay.displayClear();
  myDisplay.displayDP(true);
  delay(500);
  myDisplay.displayDP(false);
  delay(500);
  }

void countHex8(uint32_t interval) {
  byte counter = 0;
  do {
    myDisplay.displayHexByte(counter);
    delay(interval);
  } while(++counter != 0);
}

void countHex12(uint32_t interval) {
  uint16_t counter = 0;
  do {
    myDisplay.displayInteger(counter, false);
    delay(interval);
  } while(++counter != 0x1000);
}

void countUp(uint16_t number, uint32_t interval) {
  int16_t counter;
  for(counter = 0; counter <= number; counter++) {
    myDisplay.displayInteger(counter);
    delay(interval);
  }
}

void countDown(uint16_t number, uint32_t interval) {
  int16_t counter;
  for(counter = number; counter >= 0; counter--) {
    myDisplay.displayInteger(counter);
    delay(interval);
  }
}

void countXMins(byte minutesMax) {
  byte minutes, seconds;
  if(minutesMax > 16) {
    minutesMax = 16;
  }
  for(minutes = 0; minutes < minutesMax; minutes++) {
    for(seconds = 0; seconds < 60; seconds++) {
      myDisplay.displayChar(0, minutes);
      myDisplay.displayChar(1, seconds / 10);
      myDisplay.displayChar(2, seconds % 10);
      delay(1000);
    }
  }
}

void countXMinsDP(byte minutesMax) {
  bool dPoint = false;
  byte minutes = 0, seconds = 0;
  unsigned long timeNow, timeMark;
  timeMark = millis();
  if(minutesMax > 16) {
    minutesMax = 16;
  }
  do {
    timeNow = millis();
    if(timeNow - timeMark >= 500) {
      timeMark = timeNow;
      // Toggle the decimal points.
      myDisplay.displayDP(dPoint);
      // Increment the time.
      if(dPoint) {
        myDisplay.displayChar(0, minutes);
        myDisplay.displayChar(1, seconds / 10);
        myDisplay.displayChar(2, seconds % 10);
        if(++seconds == 60) {
          seconds = 0;
          minutes++;
        }
      }
      dPoint = !dPoint;
    }
  } while (minutes != minutesMax);
}

// EOF