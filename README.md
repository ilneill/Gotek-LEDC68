# Gotek-LEDC68
Using an Old Gotek LEDC68 (TM1651 based) 3-Digit LED Board

![](Gotek%20LEDC68%20Front.jpg)

_**LEDC68 Rear**_

![](Gotek%20LEDC68%20Rear.jpg)

Having recently, like many, upgraded my Gotek drive to use the FlashFloppy firmware, I then went on to do a few hardware mods - including replacing the stock 3-digit LED screen (LEDC68) with a superior OLED screen.

Not wanting to just throw the LEDC68 away, I decided to see if I could do anything with it using my trusty Arduino Mega 2560 R3.

Here is the result... a small library of functions and some demo code to show it in action!

+ **TM1651.ino**: Demo sketch showing the library functions at work.
+ **TM1651.h**: Header file for the function library.
+ **TM1651.cpp**: The library of TM1651 functions.

Now to be fair, there is not a lot that you can do with a 3-digit LED display, but it was fun to work it all out.

## Important information
The LEDC68 uses a TM1651 chip to address the digits and drive the segments. The TM1651 uses a 2-wire serial interface (clock and data), but despite apearances, it is **not** I2C.

## The Screen on my Mega and Working
![](LEDC68_Working_on_a_Mega.jpg)

## References
Built on work by Derek Cooper. Thank you for the jump start!
+ https://www.instructables.com/Re-use-LEDC86-Old-Gotek-Display/

_**Enjoy!**_
