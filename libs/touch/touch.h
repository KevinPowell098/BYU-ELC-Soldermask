#include "lcd/lcd.h"
#include "menu/vars.h"

#define MAX_BOXES 100

extern Box boxes[MAX_BOXES];
extern uint16_t boxCount;

void AddBoxToArray(Box b);
void AddBoxToArray(Box b, void (*func)(), int16_t page);

bool selectElement(int16_t x, int16_t y);
bool isElementTouched();