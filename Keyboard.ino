#include "config.h"
#include "keymap.h"

#ifdef KEYMAP_H
  #include "Controller.h"
  #warning "keymap.h is populated; building in CONTROLLER mode.  Empty out keymap.h if you need to re-scan."
  void setup() {
    controller_init(MATRIX_ROWS, MATRIX_COLS, (int*)Row_IO, (int*)Col_IO);
  }
  void loop() {
    controller_loop(MATRIX_ROWS, MATRIX_COLS, (int*)normal, (int*)modifier, (int*)media, (int*)Row_IO, (int*)Col_IO);
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
