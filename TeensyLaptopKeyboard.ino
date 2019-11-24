#include "config.h"
#include "keymap.h"

#ifdef KEYMAP_H
  #include "Controller.h"
  void setup() {
    controller_setup();
  }
  void loop() {
    controller_loop();
  }
#else
  #include "Scanner.h"
  void setup() {
    scanner_setup();
  }
  void loop() {
    scanner_loop();
  }
#endif
