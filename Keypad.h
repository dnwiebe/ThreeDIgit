#ifndef Keypad_h
#define Keypad_h

#include "NumberKeeper.h"
#include "Debounce.h"

class Keypad {
private:
  const Debounce& upButton;
  bool upWasDown;
  const Debounce& downButton;
  bool downWasDown;
  NumberKeeper& keeper;
  
public:
  Keypad (const Debounce& upButton, const Debounce& downButton, NumberKeeper& keeper);
  void update (unsigned long now);
};

#endif

