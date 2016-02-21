#include <Arduino.h>
#include "Display.h"

#define D0UL 0x000001L
#define D0TP 0x000002L
#define D0UR 0x000004L
#define D0LL 0x000008L
#define D0CT 0x000010L
#define D0LR 0x000020L
#define D0BT 0x000040L

#define D1UL 0x000080L
#define D1TP 0x000100L
#define D1UR 0x000200L
#define D1LL 0x000400L
#define D1CT 0x000800L
#define D1LR 0x001000L
#define D1BT 0x002000L

#define D2UL 0x004000L
#define D2TP 0x008000L
#define D2UR 0x010000L
#define D2LL 0x020000L
#define D2CT 0x040000L
#define D2LR 0x080000L
#define D2BT 0x100000L

#define D0_0 D0UL | D0TP | D0UR | D0LL        | D0LR | D0BT
#define D0_1               D0UR               | D0LR
#define D0_2        D0TP | D0UR | D0LL | D0CT        | D0BT
#define D0_3        D0TP | D0UR        | D0CT | D0LR | D0BT
#define D0_4 D0UL        | D0UR        | D0CT | D0LR
#define D0_5        D0TP | D0UR        | D0CT | D0LR | D0BT
#define D0_6        D0TP | D0UR | D0LL | D0CT | D0LR | D0BT
#define D0_7        D0TP | D0UR               | D0LR
#define D0_8 D0UL | D0TP | D0UR | D0LL | D0CT | D0LL | D0BT
#define D0_9 D0UL | D0TP | D0UR | D0LL | D0CT        | D0BT

#define D1_0 D1UL | D1TP | D1UR | D1LL        | D1LR | D1BT
#define D1_1               D1UR               | D1LR
#define D1_2        D1TP | D1UR | D1LL | D1CT        | D1BT
#define D1_3        D1TP | D1UR        | D1CT | D1LR | D1BT
#define D1_4 D1UL        | D1UR        | D1CT | D1LR
#define D1_5        D1TP | D1UR        | D1CT | D1LR | D1BT
#define D1_6        D1TP | D1UR | D1LL | D1CT | D1LR | D1BT
#define D1_7        D1TP | D1UR               | D1LR
#define D1_8 D1UL | D1TP | D1UR | D1LL | D1CT | D1LL | D1BT
#define D1_9 D1UL | D1TP | D1UR | D1LL | D1CT        | D1BT

#define D2_0 D2UL | D2TP | D2UR | D2LL        | D2LR | D2BT
#define D2_1               D2UR               | D2LR
#define D2_2        D2TP | D2UR | D2LL | D2CT        | D2BT
#define D2_3        D2TP | D2UR        | D2CT | D2LR | D2BT
#define D2_4 D2UL        | D2UR        | D2CT | D2LR
#define D2_5        D2TP | D2UR        | D2CT | D2LR | D2BT
#define D2_6        D2TP | D2UR | D2LL | D2CT | D2LR | D2BT
#define D2_7        D2TP | D2UR               | D2LR
#define D2_8 D2UL | D2TP | D2UR | D2LL | D2CT | D2LL | D2BT
#define D2_9 D2UL | D2TP | D2UR | D2LL | D2CT        | D2BT

const unsigned long D0[] = {D0_0, D0_1, D0_2, D0_3, D0_4, D0_5, D0_6, D0_7, D0_8, D0_9};
const unsigned long D1[] = {D1_0, D1_1, D1_2, D1_3, D1_4, D1_5, D1_6, D1_7, D1_8, D1_9};
const unsigned long D2[] = {D2_0, D2_1, D2_2, D2_3, D2_4, D2_5, D2_6, D2_7, D2_8, D2_9};

const uint32_t POST_SEQUENCE[] = {
  D0UL, D0TP, D0UR, D0CT, D0LL, D0BT, D0LR,
  D1UL, D1TP, D1UR, D1CT, D1LL, D1BT, D1LR,
  D2UL, D2TP, D2UR, D2CT, D2LL, D2BT, D2LR
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
  for (byte i = 0; i < 21; i++) {
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

