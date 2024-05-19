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

#ifndef TM1651_H
  #define TM1651_H
  #include <Arduino.h>

  // Command and address definitions for the TM1651.
  #define ADDR_AUTO       0x40
  #define ADDR_FIXED      0x44
  #define STARTADDR       0xc0 

  // Definitions for the decimal point, it's not well implemented in hardware and is of little use.
  #define DP_OFF          0x00
  #define DP_ON           0x08                          // Controlled via start address +03, segment d.

  // Definitions for the 7 segment display brightness.
  #define BRIGHT_DARKEST  0
  #define BRIGHT_TYPICAL  2
  #define BRIGHTEST       7

  class TM1651 {
    public:
      TM1651(uint8_t = 2, uint8_t = 3);                 // TM1651 Class instantiation.
      uint8_t cmdDispCtrl;                              // The current display control command.
      uint8_t charTableSize;                            // The size of the defined character code table.
      void displayInit(uint8_t = BRIGHT_TYPICAL);       // Initialise the display defaults.
      void displayChar(uint8_t, uint8_t, bool = false); // Display a character in a specific digit.
      void displayHexByte(uint8_t);                     // Display a hex byte 0x00 - 0xFF.
      void displayInteger(uint16_t, bool = true);       // Display a decimal integer between 0 - 999, or a hex integer between 0 - 0xFFF.
      void displayDP(bool = false);                     // Turn ON/OFF the decimal points.
      void displayClear(void);                          // Clear the TM1651 display - fill with zeros.
      void displaySet(uint8_t = BRIGHT_TYPICAL);        // Set the brightness and turn the TM1651 display ON.
      void displayOff(void);                            // Turn the TM1651 display OFF.
    private:
      uint8_t _clkPin;                                  // The current TM1651 clock pin.
      uint8_t _dataPin;                                 // The current TM1651 data pin.
      int16_t writeByte(uint8_t);                       // Write a byte of data to the TM1651.
      void start(void);                                 // Send start signal to the TM1651.
      void stop(void);                                  // Send stop signal to the TM1651.
      void bitDelay(void);                              // Wait a bit...
  };
#endif

// EOF