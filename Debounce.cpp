#include <Arduino.h>
#include "Debounce.h"
#include "Utils.h"

#define DEBOUNCE_DELAY 20

Debounce::Debounce (byte pin) {
  this->pin = pin;
  this->prevActualState = false;
  this->officialState = false;
  this->delaying = false;
  this->delayStart = 0L;
}

void Debounce::setup() {
  pinMode (pin, INPUT);
}

void Debounce::update (unsigned long now) {
  if (delaying) {
    unsigned long interval = calculateInterval (delayStart, now);
    if (interval < DEBOUNCE_DELAY) {return;}
    bool currActualState = (digitalRead (pin) == HIGH);
    prevActualState = currActualState;
    officialState = currActualState;
    delaying = false;
    return;
  }
  bool currActualState = (digitalRead (pin) == HIGH);
  if (currActualState == prevActualState) {return;}
  prevActualState = currActualState;
  delaying = true;
  delayStart = now;
}

bool Debounce::isDown () const {
  return officialState;
}

