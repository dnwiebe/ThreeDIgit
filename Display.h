#ifndef Display_h
#define Display_h

#include <Arduino.h>
#include "NumberKeeper.h"

class Display {
private:
  byte latchPin;
  byte clockPin;
  byte dataPin;
  const NumberKeeper& keeper;  

public:
  Display (byte latchPin, byte clockPin, byte dataPin, const NumberKeeper& keeper);
  void setup ();
  void update (unsigned long now);
  void post ();
};

#endif

