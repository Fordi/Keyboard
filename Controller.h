#ifndef CONTROLLER_H
#define CONTROLLER_H

void controller_init(int rows, int cols, int* row_pins, int* col_pins);
void controller_loop(int rows, int cols, int* normal, int* modifier, int* media, int* row_pins, int* col_pins);

#endif
