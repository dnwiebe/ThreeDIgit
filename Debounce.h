#ifndef Debounce_h
#define Debounce_h

#include <Arduino.h>

class Debounce {
private:
  byte pin;
  bool prevActualState;
  bool officialState;
  bool delaying;
  unsigned long delayStart;

public:
  Debounce (byte pin);
  void setup ();  
  void update (unsigned long now);
  bool isDown () const;
};

#endif

