#ifndef CONFIG_H
#define CONFIG_H

/** scanner config **/

// Wait for the host to connect to the Teensy as a keyboard. If 2 pins are
// shorted, you want the host to be ready to receive the pin numbers.
#define INITIAL_DELAY 5000

// Maximum time for pins to be joined before it's considered a short
#define MAX_HOLD_TIME 1000

// the first pin to be tested on the FPC connector (usually pin 1)
#define MIN_PIN 1


// the keyboard FPC connector pin count. If set to 34, unsolder the LED or
// the code won't work
#define MAX_PIN 33

// If your keyboard has diodes, you must pay attention to the order of the
// two pins that are reported by the Teensy. The code performs a bottom up
// test first, followed by a top down test so that one of the two tests
// will forward bias the diode. The first pin reported over USB is the
// cathode side and the second pin is the anode side. The diode direction
// must be taken into account when programming the TMK or Teensyduino
// keyboard routine.

// If this line is commented out, the scanner will only scan bottom-up
// #define DIODES

// If you're on a Mac, uncomment this line
// #define MAC

/** end scanner config */

/** keyboard config */

// Microcontroller's built-in LED shows Caps-Lock state; comment out to
// disable this Default is LED_BUILTIN, which is whatever the pin is for
// the on-board LED (usually 13)
#define CAPS_LED LED_BUILTIN

// Uncomment to use Keyboard.set_media() instead of consuming a
// normal key slot (experimental)
// #define USE_SET_MEDIA

/** end keyboard config */

#endif
