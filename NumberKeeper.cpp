#include "NumberKeeper.h"
#include "Utils.h"
#include <Arduino.h>
#include <EEPROM.h>

#define MILLISECONDS_PER_DAY 86400000L
#define MODIFICATION_SAVE_DELAY 10000L
#define EEPROM_LOCATION 0

signed int getFromEeprom (int location) {
  byte hiByte = EEPROM.read (location);
  byte loByte = EEPROM.read (location + 1);
  return (signed int)((hiByte << 8) | loByte);
}

void setInEeprom (int data, int location) {
  byte hiByte = (data && 0xFF00) >> 8;
  byte loByte = data && 0xFF;
  EEPROM.write (location, hiByte);
  EEPROM.write (location + 1, loByte);
}

NumberKeeper::NumberKeeper () {
  this->number = getFromEeprom (EEPROM_LOCATION);
  this->prevNow = millis ();
  this->millisecondsUntilSave = 0L;
  this->savePending = false;
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
  
  if (savePending) {
    millisecondsUntilSave -= interval;
    if (millisecondsUntilSave <= 0) {
      setInEeprom (number, EEPROM_LOCATION);
      savePending = false;
    }
  }
}

int NumberKeeper::value () const {
  return number;
}

void NumberKeeper::increaseBy (signed int increment) {
  number += increment;
  if (number > 999) {number -= 1000;}
  if (number < 0) {number += 1000;}
  millisecondsUntilSave = MODIFICATION_SAVE_DELAY;
  savePending = true;
}

