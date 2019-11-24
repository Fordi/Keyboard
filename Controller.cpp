/* Copyright 2018 Frank Adams
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

// This sketch implements a Laptop Keyboard Controller using a Teensy on
// a daughterboard.
// This routine uses the Teensyduino "Micro-Manager Method" to send Normal and Modifier
// keys over USB.
// Description of Teensyduino keyboard functions is at www.pjrc.com/teensy/td_keyboard.html
//
// Revision History
// Initial Release Nov 15, 2018
// Rev 1 July 16, 2019 - check if slots are full when detecting a key press
// Rev 2 November 24, 2019 [Bryan Elliott]
// - Switch to LED_BUILTIN for keyboard LED
// - pull non-generic code into a config file
// - Improve readbility of code

#include "config.h"
#include "keymap.h"

#ifdef KEYMAP_H

// Convenience constants
#define TRUE (1==1)
#define FALSE (1==0)

// give Fn key a code to track; this is never sent as HID
#define MODIFIERKEY_FN 0x8f

// holds the normal key values to be sent over USB.
int slots[6] = { 0, 0, 0, 0, 0, 0 };

// Stores states of existing keys, so we can track rising / falling edge
int old_key[MATRIX_ROWS][MATRIX_COLS];

// Holds the current modifier state
int modifier_state = 0;

// Load the key name into the first available slot
// If all slots are full, this will do effectively nothing.
void set_key(int key) {
  for (int index = 0; index < SLOT_COUNT; index++) {
    if (slots[index] == 0) {
      slots[index] = key;
      break;
    }
  }
}

// Clear the slot that contains the key name
// If the key isn't present, this will do nothing.
void reset_key(int key) {
  for (int index = 0; index < SLOT_COUNT; index++) {
    if (slot[index] == key) {
      slot[index] = 0;
      break;
    }
  }
}

// Load the modifier key name into the appropriate mod variable
void set_modifier(int m_key) {
  modifier_state = modifier_state | m_key;
  Keyboard.set_modifier(modifier_state);
}

// Load 0 into the appropriate mod variable
void reset_modifier(int m_key) {
  modifier_state = modifier_state & ~m_key;
  Keyboard.set_modifier(modifier_state);
}

// Function to send the normal keys in the 6 slots over usb
void send_keys() {
  Keyboard.set_key1(slots[0]);
  Keyboard.set_key2(slots[1]);
  Keyboard.set_key3(slots[2]);
  Keyboard.set_key4(slots[3]);
  Keyboard.set_key5(slots[4]);
  Keyboard.set_key6(slots[5]);
}

// Set a pin mode and state
void set_pin(int pin, int mode, int state) {
  pinMode(pin, mode);
  digitalWrite(pin, state);
}

//----------------------------------Setup-------------------------------------------
void keyboard_init() {
  for (int col = 0; col < MATRIX_COLS; col++) {
    // Set column pin to input_pullup / high
    set_pin(Col_IO[col], INPUT_PULLUP, HIGH);
  }

  for (int row = 0; row < MATRIX_ROWS; row++) {
    // Set row pin to high impedance
    set_pin(Row_IO[row], INPUT, HIGH);
    for (int row = 0; row < MATRIX_ROWS; row++) {
      // Initialize old_key
      old_key[row][col] = FALSE;
    }
  }

}

// Initialize Fn key to 0 = "not pressed"
boolean fn_pressed = FALSE;

// 8 bits sent from Pi to Teensy that give keyboard LED status. Caps lock is bit D1.
extern volatile uint8_t keyboard_leds;

//---------------------------------Main Loop---------------------------------------------
void keyboard_loop() {

// Scan keyboard matrix with an outer loop that drives each row low and an
// inner loop that reads every column (with pull ups). The routine looks
// at each key's present state (by reading the column input pin) and also
// the previous state from the last scan that was 30msec ago. The status
// of a key that was just pressed or just released is sent over USB and
// the state is saved in the old_key matrix.

  for (int x = 0; x < MATRIX_ROWS; x++) {
    // Activate Row by pulling it to ground
    set_pin(Row_IO[x], OUTPUT, LOW);
    // give the row time to go low and settle out
    delayMicroseconds(10);
    for (int y = 0; y < MATRIX_COLS; y++) {
      // Keys are active LOW
      boolean down = digitalRead(Col_IO[y]) == LOW;

      boolean just_pressed = down && old_key[x][y];
      boolean just_released = !down && old_key[x][y];

      // In whatever case, track the state of the matrix.
      if (just_pressed) {
        old_key[x][y] = TRUE;
      } else if (just_released) {
        old_key[x][y] = FALSE;
      }

      // # Handle modifiers
      if (modifier[x][y] != 0) {
        if (just_pressed) {
          // Handle Fn modifier key internally
          if (modifier[x][y] == MODIFIERKEY_FN) {
            fn_pressed = TRUE;
          } else {
            set_modifier(modifier[x][y]);
          }
        } else if (just_released) {
          // Handle Fn modifier key internally
          if (modifier[x][y] == MODIFIERKEY_FN) {
            fn_pressed = FALSE;
          } else {
            reset_modifier(modifier[x][y]);
          }
        }
      }
      // # end of modifiers

      else

      // # Fn-modified keys (media)
      if (fn_pressed) {
        if (media[x][y] != 0) {
          if (just_pressed) {
            #ifdef USE_SET_MEDIA
              Keyboard.set_media(media[x][y]);
            #else
              set_key(media[x][y]);
            #endif
          } else if (just_released) {
            #ifdef USE_SET_MEDIA
              Keyboard.set_media(0);
            #else
              reset_key(media[x][y]);
            #endif
          }
        }
      }
      // # end of FN-modified section

      else

      // # Normal keys
      if (normal[x][y] != 0) {
        if (just_pressed) {
          set_key(normal[x][y]);
        } else if (just_released) {
          reset_key(normal[x][y]);
        }
      }
      // # end normal keys

    }
    // De-activate Row (send it to hi-impedance)
    set_pin(Row_IO[x], INPUT, HIGH);
  }
  // Scan complete; send scanned state.
  Keyboard.send_now();

  // Switch on the LED on the Teensy for Caps Lock based on bit 1 in
  // the keyboard_leds variable controlled by the USB host computer
  #ifdef CAPS_LED
    boolean caps_on = (keyboard_leds & 2) == 2;
    if (caps_on) {
      // turn on the LED
      set_pin(CAPS_LED, OUTPUT, HIGH);
    } else {
      // turn off the LED
      set_pin(CAPS_LED, OUTPUT, LOW);
    }
  #endif

  delay(25);
}
#endif
