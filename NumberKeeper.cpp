#include "NumberKeeper.h"
#include "Utils.h"
#include <Arduino.h>

#define MILLISECONDS_PER_DAY 86400000L

NumberKeeper::NumberKeeper () {
  this->number = 0;
  this->prevNow = millis ();
  this->millisecondsRemainingInDay = MILLISECONDS_PER_DAY;
}

void NumberKeeper::update (unsigned long now) {
  unsigned long interval = calculateInterval (prevNow, now);
  prevNow = now;
  millisecondsRemainingInDay -= interval;
  if (millisecondsRemainingInDay <= 0) {
    increaseBy (1);
    millisecondsRemainingInDay = MILLISECONDS_PER_DAY;
  }
}

int NumberKeeper::value () const {
  return number;
}

void NumberKeeper::increaseBy (signed int increment) {
  number += increment;
  if (number > 999) {number -= 1000;}
  if (number < 0) {number += 1000;}
}

