/*  Gotek 3-digit LED display driver (LEDC68/TM1651).
 *  Written for the Arduino Uno/Nano/Mega.
 * (c) Ian Neill 2024
 * GPL(v3) Licence
 *
 * Built on work by Derek Cooper. Thank you for the jump start!
 * References:
 *    https://www.instructables.com/Re-use-LEDC86-Old-Gotek-Display/
 *    https://github.com/coopzone-dc/GotekLEDC68
 *    https://github.com/mworkfun/TM1650
 */

#include "TM1651.h"

// A table of simple character codes.
const static uint8_t charTable[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,             // Numbers : 0-9.
                                    0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71,                                     // Numbers : A, b, C, d, E, F.
                                    0x58, 0x6f, 0x74, 0x76, 0x38, 0x54, 0x37, 0x73, 0x50, 0x1c, 0x3e, 0x6e, // Chars   : c, g, h, H, L, n, N, P, u, U, y.
                                    0x01, 0x40, 0x08, 0x63, 0x00, 0x5c,                                     // Specials: uDash, mDash, lDash, uBox, Space, lBox.
                                    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};                              // Segments: SegA, SegB, SegC, SegD, SegE, SegF, SegG.

// TM1651 Class instantiation.
TM1651::TM1651(uint8_t clkPin, uint8_t dataPin) {
  _clkPin  = clkPin;
  _dataPin = dataPin;
  charTableSize = (sizeof(charTable)/sizeof(*charTable));
  pinMode(_clkPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);
  displayInit();
}

/**************************/
/* Public Class Functions */
/**************************/
// Initialise the display defaults.
void TM1651::displayInit(uint8_t brightness) {
  displaySet(brightness);
  displayClear();
}
// Display a character in a specific LED digit.
void TM1651::displayChar(uint8_t digit, uint8_t number, bool raw) {
  if(digit > 2) {                     // Ensure the digit position is in range 0 to 2.
    digit = 2;
  }
  if(raw) {                           // If this is a raw segment bit number, ensure there are only 7 bits.
    number &= 0x7f;
  }
  else {                              // If using the character table, ensure the number is within the character table.
    if(number >= charTableSize) {
      number = 0x20;                  // This is a 0x00 (space) in the character table.
    }
    number = charTable[number];       // Get the raw number from the character table.
  }
  start();                            // Send the start signal to the TM1651.
  writeByte(ADDR_FIXED);              // Cmd to set a specific address.
  stop();                             // Send the stop signal to the TM1651.
  start();
  writeByte(STARTADDR + digit);       // Set the address for the requested digit.
  writeByte(number);                  // Write the number to the digit.
  stop();
}
// Display a hex byte between 0x00 - 0xFF.
void TM1651::displayHexByte(uint8_t number) {
  start();                            // Send the start signal to the TM1651.
  writeByte(ADDR_AUTO);               // Cmd to auto increment the address.
  stop();                             // Send the stop signal to the TM1651.
  start();
  writeByte(STARTADDR);               // Start at the first digit address.
  writeByte(charTable[0x12]);         // Display an "h" in the lefthand digit.
  writeByte(charTable[number / 16]);  // Display the most significant nibble.
  writeByte(charTable[number % 16]);  // Display the least significant nibble.
  stop();
}
// Display a decimal integer between 0 - 999, or a hex integer between 0 - 0xFFF.
void TM1651::displayInteger(uint16_t number, bool useDec) {
  if(useDec) {
    if(number > 999) {
      number = 999;
    }
    else {
      if(number > 0xFFF) {
        number = 0xFFF;
      }
    }
  }
  start();                            // Send the start signal to the TM1651.
  writeByte(ADDR_AUTO);               // Cmd to auto increment the address.
  stop();                             // Send the stop signal to the TM1651.
  start();
  writeByte(STARTADDR);               // Start at the first digit address.
  if(useDec) {
    writeByte(charTable[(number / 100) % 10]);
    writeByte(charTable[(number /  10) % 10]);
    writeByte(charTable[ number        % 10]);
  }
  else {
    writeByte(charTable[(number / 256) % 16]);
    writeByte(charTable[(number /  16) % 16]);
    writeByte(charTable[ number        % 16]);
  }
  stop();
}
// Turn ON/OFF the decimal points.
void TM1651::displayDP(bool status) {
  uint8_t segData;
  segData = status ? DP_ON : DP_OFF;
  start();                            // Send the start signal to the TM1651.
  writeByte(ADDR_FIXED);              // Cmd to set a specific address.
  stop();                             // Send the stop signal to the TM1651.
  start();
  writeByte(STARTADDR + 0x03);        // Start address +03 controls the decimal points.
  writeByte(segData);                 // Write the data to turn the decimal points ON/OFF.
  stop();      
}
// Clear the TM1651 display - fill with zeros.
void TM1651::displayClear(void) {
  uint8_t digit;
  for(digit = 0; digit <= 2; digit++){
    displayChar(digit, 0x00, true);   // Write a zero (all segments OFF) to each digit.
  }
  displayDP(false);
}
// Set the brightness and turn the TM1651 display ON.
void TM1651::displaySet(uint8_t brightness) {
  if(brightness > BRIGHTEST) {
    brightness = BRIGHTEST;
  }
  cmdDispCtrl = 0x88 + brightness;    // 88 + 0 to 7 brightness, 88 = display ON.
  start();                            // Send the start signal to the TM1651.
  writeByte(cmdDispCtrl);             // Set the brightness and turn the display ON.
  stop();                             // Send the stop signal to the TM1651.
}
// Turn the TM1651 display OFF.
void TM1651::displayOff(void) {
  cmdDispCtrl = 0x80;                 // 80 = display OFF.
  start();                            // Send the start signal to the TM1651.
  writeByte(cmdDispCtrl);             // Turn the display OFF.
  stop();                             // Send the stop signal to the TM1651.
}

/***************************/
/* Private Class Functions */
/***************************/
// Write a byte of data to the TM1651.
int16_t TM1651::writeByte(uint8_t data) {
  uint8_t bit, ack;
  // Send 8 bits of data.
  for(bit = 0; bit < 8; bit++) {
    digitalWrite(_clkPin, LOW);
    if(data & 0x01) {                 // LSB first.
      digitalWrite(_dataPin, HIGH);
    }
    else {
      digitalWrite(_dataPin, LOW);
    }
    data >>= 1;
    digitalWrite(_clkPin, HIGH);
  }
  // Wait for the ACK.
  digitalWrite(_clkPin, LOW);
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_clkPin, HIGH);
  pinMode(_dataPin, INPUT);
  bitDelay(); 
  ack = digitalRead(_dataPin);
  if(ack == 0) {  
    pinMode(_dataPin, OUTPUT);
    digitalWrite(_dataPin, LOW);
  }
  bitDelay();
  pinMode(_dataPin, OUTPUT);
  bitDelay();
  return(ack);
}
// Send start signal to the TM1651.
void TM1651::start(void) {
  digitalWrite(_clkPin, HIGH);
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clkPin, LOW);
}
//Send end signal to the TM1651.
void TM1651::stop(void) {
  digitalWrite(_clkPin, LOW);
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clkPin, HIGH);
  digitalWrite(_dataPin, HIGH);
}
// Wait a bit...
void TM1651::bitDelay(void) {
  delayMicroseconds(50);
}

// EOF