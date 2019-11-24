/*
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
// It will determine how a laptop keyboard matrix is wired using a Teensy 3.2 on an FPC daughterboard.
// Open an editor on a computer and load or create a file that lists every key
// on the laptop keyboard that will be tested. Connect the FPC cable of the test keyboard
// to the Teensy/FPC board. Connect a USB cable from the Teensy to the computer.
//
// Wait a few seconds for the computer to see the Teensy as a keyboard. If numbers are reported on the screen
// before any keys are pressed, these pin numbers are shorted together and must be fixed.
//
// Press each key one by one on the test keyboard as listed on the editor screen. When a key
// is pressed on the test keyboard, the program detects which two pins on the FPC connector
// were connected. Those two pin numbers are sent over USB (separated by a TAB) and displayed
// on the editor. After sending the numbers, a DOWN ARROW is sent over USB to prepare for
// the next key. Once all keys on the test keyboard have been pressed, the file in
// the editor can be saved to create a row-column matrix.
//
// Revision History
// Rev 1.00 [Frank Adams] - Nov 18, 2018
//    Original release
// Rev 2.00 [Bryan Elliott] - Nov 24, 2019
//    Merged Teensy 3.2 and Teensy LC variants
//    Made use of Teensy features to simplify code
//    Added short detection
//    Split user-config and board-config out into headers

// Board configuration
#include "Teensy.h"
#include "config.h"
#include "Scanner.h"

// This array will hold per-pin enabled / disabled state.  Pins will be disabled if a short is detected
// A pair of shorted pins is not necessarily indicative of a problem; some matrices have a pair of ground pins, for example.
int disabled[HW_MAX_PIN];

// This is used in the scanner to see how long we wait before a pair of shorted pins releases.
// If held past MAX_HOLD_TIME (in config.h), the pins are considered shorted, and they are
// disabled for future scans.  Resetting the microcontroller will undo this.
elapsedMillis hold_time = 0;

// Number of scans that have passed since last change in heartbeat
int scan_count = 0;

// State of the microcontroller's built-in LED; this should switch once every 10 scans.
boolean heartbeat_state = LOW;


// Set the mode and state of a pin.
void set_pin(int pin, int mode, int state) {
  pinMode(pin, mode);
  digitalWrite(pin, state);
}

// Tap a key
void tap(int key) {
  Keyboard.press(key);
  delay(20);
  Keyboard.release(key);
}

// OSX/Windows/Linux differences between keystrokes to go to end-of-line
void to_eol() {
  #ifndef MAC
    // Windows / Linux
    tap(KEY_END);
  #else
    // OS-X
    Keyboard.press(MODIFIERKEY_GUI);
    tap(KEY_RIGHT);
    Keyboard.release(MODIFIERKEY_GUI);
  #endif
}

// OSX/Windows/Linux differences between keystrokes to go to start-of-line
void to_sol() {
  #ifndef MAC
    // Windows / Linux
    tap(KEY_HOME);
  #else
    // OS-X
    Keyboard.press(MODIFIERKEY_GUI);
    tap(KEY_LEFT);
    Keyboard.release(MODIFIERKEY_GUI);
  #endif
}

// Initialize the pins
void init_pins() {
  // Set each pin to input_pullup/high, and enable it
  for (int pin = MIN_PIN; pin < MAX_PIN && pin < HW_MAX_PIN; pin++) {
    set_pin(pin, INPUT_PULLUP, HIGH);
    disabled[pin] = 0;
  }
}

// Identify the microcontroller
void identify(void) {
  //Send Home, Shift+End to select contents of current line
  to_sol();
  Keyboard.press(MODIFIERKEY_LEFT_SHIFT);
  to_eol();
  Keyboard.release(MODIFIERKEY_LEFT_SHIFT);
  tap(KEY_BACKSPACE);
  // Print the board type
#ifdef TEENSY32
  Keyboard.print("Teensy32");
#endif
#ifdef TEENSYLC
  Keyboard.print("TeensyLC");
#endif
  // Go to end of next line.
  tap(KEY_DOWN);
  to_eol();
}

// Scan the matrix by grounding each pin in turn and reading the others to find out which was pulled low.
void scan_matrix(int from, int to) {
  // Counting up?  Or down?
  int dir = to - from > 0 ? 1 : -1;
  for (
    int outer = from;
    dir == 1
      ? (outer < (to - 1))
      : (outer > (to - 1));
    outer += dir
  ) {
    // make the outer loop pin an output and send this pin low
    set_pin(fpc_map[outer], OUTPUT, LOW);
    if (disabled[outer] == 1) continue;
    for (
      int inner = outer + dir;
      dir == 1
        ? (inner < to)
        : (inner > to);
      inner += dir
    ) {
      if (disabled[inner] == 1) continue;
      delayMicroseconds(10);
      // check for connection between inner and outer pins
      if (!digitalRead(fpc_map[inner])) {
        tap(KEY_TAB);
        Keyboard.print(outer + 1);
        // Send outer loop pin number
        // type_num(inner + 1);
        tap(KEY_TAB);
        Keyboard.print(inner + 1);
        // Send inner loop pin number
        // type_num(outer + 1);
        // hang until key is released
        hold_time = 0;
        while(!digitalRead(fpc_map[inner])) {
          // Held for too long; consider these pins shorted.
          if (hold_time >= MAX_HOLD_TIME) {
            // Comment out the current line
            to_sol();
            Keyboard.print('# ');
            to_eol();
            tap(KEY_TAB);
            // Mark as shorted
            Keyboard.print("Shorted");
            // Disable scan on these pins
            disabled[inner] = 1;
            disabled[outer] = 1;
            break;
          }
        }
        // Go to end of next line
        tap(KEY_DOWN);
        to_eol();
      }
      // The outer-loop pin is shorted; stop scanning the inner pins.
      if (disabled[outer] == 1) break;
    }
    set_pin(fpc_map[outer], INPUT_PULLUP, HIGH);
  }
}

void scanner_init() {
  delay(INITIAL_DELAY);
  init_pins();
  identify();
}

void scanner_loop() {
  // ***********Bottom up Test************
  scan_matrix(MIN_PIN, min(MAX_PIN, HW_MAX_PIN));
  // *********Top down Test***********
  #ifdef DIODES
    scan_matrix(min(MAX_PIN, HW_MAX_PIN), MIN_PIN);
  #endif
  //
  // Blink the LED to show the Teensy is alive
  //
  if (scan_count == 10) {
    heartbeat_state = heartbeat_state == HIGH ? LOW : HIGH;
    scan_count = 0;
    set_pin(LED_BUILTIN, OUTPUT, heartbeat_state);
  } else {
    scan_count++;
  }
  delay(25);
}
