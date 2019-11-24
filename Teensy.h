#ifndef BOARD_H
#define BOARD_H

// Teensy 3.2 config
#ifdef __MK20DX256__

#define TEENSY32
// Load an array with the Teensy 3.2 I/O numbers that correspond to FPC pins 1 thru 34.
int fpc_map[] = {23, 0, 22, 1, 21, 2, 20, 3, 19, 4, 18, 5, 17, 6, 24, 7, 25, 8, 33, 9, 26, 10, 27, 11, 28, 12, 32, 31, 30, 29, 16, 15, 14, 13};
// Maximum FPC pin number that can be soldered to this microcontroller
#define HW_MAX_PIN 34

#endif

// Teensy LC config
#ifdef __MKL26Z64__

#define TEENSYLC
// Load an array with the Teensy LC I/O numbers that correspond to FPC pins 1 thru 26.
int fpc_map[] = {23, 0, 22, 1, 24, 2, 21, 3, 25, 4, 20, 5, 19, 6, 18, 7, 17, 8, 16, 9, 15, 10, 14, 11, 26, 12};
// Maximum FPC pin number that can be soldered to this microcontroller
#define HW_MAX_PIN 26

#endif

#endif
