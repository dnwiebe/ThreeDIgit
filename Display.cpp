#include <Arduino.h>
#include "Display.h"

#define UL 0x01
#define TP 0x02
#define UR 0x04
#define LL 0x08
#define CT 0x10
#define LR 0x20
#define BT 0x40

#define SEG(D, S) (S << (8 * D))

#define V0 (UL | TP | UR | LL      | LR | BT)
#define V1 (          UR           | LR     )
#define V2 (     TP | UR | LL | CT      | BT)
#define V3 (     TP | UR      | CT | LR | BT)
#define V4 (UL      | UR      | CT | LR     )
#define V5 (UL | TP           | CT | LR | BT)
#define V6 (UL | TP      | LL | CT | LR | BT)
#define V7 (     TP | UR           | LR     )
#define V8 (UL | TP | UR | LL | CT | LL | BT)
#define V9 (UL | TP | UR | LL | CT      | BT)

#define DIG(D, N) (V##N << (8 * D))

const uint32_t D0[] = {DIG(0,0), DIG(0,1), DIG(0,2), DIG(0,3), DIG(0,4), DIG(0,5), DIG(0,6), DIG(0,7), DIG(0,8), DIG(0,9)};
const uint32_t D1[] = {DIG(1,0), DIG(1,1), DIG(1,2), DIG(1,3), DIG(1,4), DIG(1,5), DIG(1,6), DIG(1,7), DIG(1,8), DIG(1,9)};
const uint32_t D2[] = {DIG(2,0), DIG(2,1), DIG(2,2), DIG(2,3), DIG(2,4), DIG(2,5), DIG(2,6), DIG(2,7), DIG(2,8), DIG(2,9)};

const uint32_t POST_SEQUENCE[] = {
  SEG(0,UL), SEG(0,TP), SEG(0,UR), SEG(0,CT), SEG(0,LL), SEG(0,BT), SEG(0,LR),
  SEG(1,UL), SEG(1,TP), SEG(1,UR), SEG(1,CT), SEG(1,LL), SEG(1,BT), SEG(1,LR),
  SEG(2,UL), SEG(2,TP), SEG(2,UR), SEG(2,CT), SEG(2,LL), SEG(2,BT), SEG(2,LR),
};

Display::Display (byte latchPin, byte clockPin, byte dataPin, const NumberKeeper& keeper):
  keeper (keeper)
{
  this->latchPin = latchPin;
  this->clockPin = clockPin;
  this->dataPin = dataPin;
}

void displayFrame (uint32_t pattern, byte latchPin, byte clockPin, byte dataPin) {
  digitalWrite (latchPin, LOW);
  digitalWrite (clockPin, LOW);
  digitalWrite (dataPin, LOW);
  for (byte i = 0; i < 24; i++) {
    boolean active = (pattern & 1) != 0;
    pattern >>= 1;
    digitalWrite (dataPin, active ? HIGH : LOW);
    digitalWrite (clockPin, HIGH);
    delayMicroseconds (1);
    digitalWrite (clockPin, LOW);
  }
  digitalWrite (latchPin, HIGH);
  digitalWrite (dataPin, LOW);
}

void Display::setup() {
  pinMode (latchPin, OUTPUT);
  pinMode (clockPin, OUTPUT);
  pinMode (dataPin, OUTPUT);
}

void Display::update (unsigned long now) {
  signed int number = keeper.value ();
  byte d0 = (byte)(number % 10);
  byte d1 = (byte)((number / 10) % 10);
  byte d2 = (byte)((number / 100) % 10);
  unsigned long d0pat = D0[d0];
  unsigned long d1pat = (d1 == 0) ? 0L : D1[d1];
  unsigned long d2pat = (d2 == 0) ? 0L : D2[d2];
  displayFrame (d0pat | d1pat | d2pat, latchPin, clockPin, dataPin);
}

void Display::post () {
  for (byte i = 0; i < 21; i++) {
    unsigned long pattern = POST_SEQUENCE[i];
    displayFrame (pattern, latchPin, clockPin, dataPin);
    delay (50);
  }
}

