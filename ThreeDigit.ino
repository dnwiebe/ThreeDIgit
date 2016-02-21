#include <EEPROM.h>
#include "NumberKeeper.h"
#include "Display.h"
#include "Debounce.h"
#include "Keypad.h"

#define LATCH 0
#define CLOCK 1
#define DATA 2

#define DOWN_BUTTON 4
#define UP_BUTTON 5

NumberKeeper keeper;
Debounce upButton (UP_BUTTON);
Debounce downButton (DOWN_BUTTON);
Keypad keypad (upButton, downButton, keeper);
Display display (LATCH, CLOCK, DATA, keeper);

void setup() {
  display.setup ();
  upButton.setup ();
  downButton.setup ();

  display.post ();
  display.post ();
  display.post ();
}

void loop() {
  unsigned long now = millis ();
  keeper.update (now);
  upButton.update (now);
  downButton.update (now);
  keypad.update (now);
  display.update (now);
}

