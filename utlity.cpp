#include <Arduino.h>
#include "utility.h"

// Set a pin mode and state
void set_pin(int pin, int mode, int state) {
  pinMode(pin, mode);
  digitalWrite(pin, state);
}
