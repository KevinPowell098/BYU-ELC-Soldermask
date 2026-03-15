#include "touch.h"

void AddBox(Box b) {
  if (boxCount < MAX_BOXES) {
    boxes[boxCount++] = b;
  } else {
    // print error
  }
}

void AddBox(Box b, void (*func)(), int16_t page) {
  if (boxCount < MAX_BOXES) {
    b.operation = func;
    b.page = page;
    boxes[boxCount++] = b;
  } else {
    // print error
  }
}