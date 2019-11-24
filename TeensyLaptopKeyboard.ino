#include "config.h"
#include "keymap.h"

#ifdef KEYMAP_H
  #include "Controller.h"
  #warning "keymap.h is populated; building in CONTROLLER mode.  Empty out keymap.h if you need to re-scan."
  void setup() {
    controller_setup();
  }
  void loop() {
    controller_loop();
  }
#else
  #include "Scanner.h"
  #warning "keymap.h is empty; building in SCANNER mode.  This is normal for a first-run."
  void setup() {
    scanner_init();
  }
  void loop() {
    scanner_loop();
  }
#endif
