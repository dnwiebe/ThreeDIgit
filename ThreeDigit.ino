#include <avr/pgmspace.h>

#define LATCH 0
#define CLOCK 1
#define DATA 2

#define BUTTON 4

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

#define D0_0 D0UL | D0TP | D0UR | D0LL | D0LR | D0BT
#define D0_1 D0UR | D0LR
#define D0_2 D0TP | D0UR | D0CT | D0LL | D0BT
#define D0_3 D0TP | D0UR | D0CT | D0LR | D0BT
#define D0_4 D0UL | D0UR | D0CT | D0LR
#define D0_5 D0UR | D0TP | D0CT | D0LR | D0BT
#define D0_6 D0UR | D0TP | D0CT | D0LL | D0LR | D0BT
#define D0_7 D0TP | D0UR | D0LR
#define D0_8 D0UR | D0TP | D0UL | D0LL | D0CT | D0LL | D0BT
#define D0_9 D0UR | D0TP | D0UL | D0CT | D0LL | D0BT

#define D1_0 D1UL | D1TP | D1UR | D1LL | D1LR | D1BT
#define D1_1 D1UR | D1LR
#define D1_2 D1TP | D1UR | D1CT | D1LL | D1BT
#define D1_3 D1TP | D1UR | D1CT | D1LR | D1BT
#define D1_4 D1UL | D1UR | D1CT | D1LR
#define D1_5 D1UR | D1TP | D1CT | D1LR | D1BT
#define D1_6 D1UR | D1TP | D1CT | D1LL | D1LR | D1BT
#define D1_7 D1TP | D1UR | D1LR
#define D1_8 D1UR | D1TP | D1UL | D1LL | D1CT | D1LL | D1BT
#define D1_9 D1UR | D1TP | D1UL | D1CT | D1LL | D1BT

#define D2_0 D2UL | D2TP | D2UR | D2LL | D2LR | D2BT
#define D2_1 D2UR | D2LR
#define D2_2 D2TP | D2UR | D2CT | D2LL | D2BT
#define D2_3 D2TP | D2UR | D2CT | D2LR | D2BT
#define D2_4 D2UL | D2UR | D2CT | D2LR
#define D2_5 D2UR | D2TP | D2CT | D2LR | D2BT
#define D2_6 D2UR | D2TP | D2CT | D2LL | D2LR | D2BT
#define D2_7 D2TP | D2UR | D2LR
#define D2_8 D2UR | D2TP | D2UL | D2LL | D2CT | D2LL | D2BT
#define D2_9 D2UR | D2TP | D2UL | D2CT | D2LL | D2BT

const PROGMEM unsigned long D0[] = {D0_0, D0_1, D0_2, D0_3, D0_4, D0_5, D0_6, D0_7, D0_8, D0_9};
const PROGMEM unsigned long D1[] = {D1_0, D1_1, D1_2, D1_3, D1_4, D1_5, D1_6, D1_7, D1_8, D1_9};
const PROGMEM unsigned long D2[] = {D2_0, D2_1, D2_2, D2_3, D2_4, D2_5, D2_6, D2_7, D2_8, D2_9};

void setup() {
  pinMode (LATCH, OUTPUT);
  pinMode (CLOCK, OUTPUT);
  pinMode (DATA, OUTPUT);
  pinMode (BUTTON, INPUT);
  for (byte i = 0; i < 3; i++) {
    post ();
  }
}

unsigned long buttonDownAt = 0;
#define BUTTON_UP 0
#define BUTTON_DOWN 1
#define BUTTON_UNK -1
#define RESET_DELAY 2000
#define MILLISECONDS_PER_DAY 86400000
byte buttonState = BUTTON_UNK;
unsigned int displayedNumber = 0;

void loop() {
  debounceButton ();
  handleButton ();
  updateNumber ();
  showNumber (displayedNumber);
}

#define DEBOUNCE_DELAY 50
bool instantaneousButtonState = false;
unsigned long lastDebounceTime = 0L;

void debounceButton () {
  bool prevState = instantaneousButtonState;
  bool instantaneousButtonState = digitalRead (BUTTON);
  unsigned long now = millis ();
  if (prevState != instantaneousButtonState) {
    lastDebounceTime = now;
    buttonState = BUTTON_UNK;
    return;
  }
  if (lastDebounceTime == 0L) {return;}
  if ((now - lastDebounceTime) >= DEBOUNCE_DELAY) {
    lastDebounceTime = 0L;
    buttonState = instantaneousButtonState ? BUTTON_DOWN : BUTTON_UP;
    return;
  }
}

byte prevButtonState = BUTTON_UP;

void handleButton () {
  if (buttonState == prevButtonState) {return;}
  if (buttonState == BUTTON_UNK) {return;}
  unsigned long now = millis ();
  prevButtonState = buttonState;
  if (buttonState == BUTTON_DOWN) {
    buttonDownAt = now;
    return;
  }
  if (now - buttonDownAt > RESET_DELAY) {
    displayedNumber = 0;
  }
  else {
    displayedNumber += 1;
  }
}

unsigned long millisecondsLeftInDay = MILLISECONDS_PER_DAY;
unsigned long lastClock = millis ();

void updateNumber () {
  unsigned long now = millis ();
  unsigned long interval = (now >= lastClock) ? now - lastClock : 0xFFFFFFF - lastClock + now;
  lastClock = now;
  if (interval >= millisecondsLeftInDay) {
    millisecondsLeftInDay = MILLISECONDS_PER_DAY - (interval - millisecondsLeftInDay);
    displayedNumber += 1;
  }
  else {
    millisecondsLeftInDay -= interval;
  }
}

void showNumber (int number) {
  byte d0 = (byte)(number % 10);
  byte d1 = (byte)((number / 10) % 10);
  byte d2 = (byte)((number / 100) % 10);
  unsigned long d0pat = pgm_read_dword_near (D0 + d0);
  unsigned long d1pat = (d1 == 0) ? 0L : pgm_read_dword_near (D1 + d1);
  unsigned long d2pat = (d2 == 0) ? 0L : pgm_read_dword_near (D2 + d2);
  displayFrame (d0pat | d1pat | d2pat, LATCH, CLOCK, DATA);
}

const PROGMEM uint32_t POST_SEQUENCE[] = {
  D0UL, D0TP, D0UR, D0CT, D0LL, D0BT, D0LR,
  D1UL, D1TP, D1UR, D1CT, D1LL, D1BT, D1LR,
  D2UL, D2TP, D2UR, D2CT, D2LL, D2BT, D2LR
};

void post () {
  for (byte i = 0; i < 21; i++) {
    unsigned long pattern = pgm_read_dword_near (POST_SEQUENCE + i);
    displayFrame (pattern, LATCH, CLOCK, DATA);
    delay (50);
  }
}

void displayFrame (uint32_t pattern, byte latch, byte clock, byte data) {
  digitalWrite (latch, LOW);
  digitalWrite (clock, LOW);
  digitalWrite (data, LOW);
  for (byte i = 0; i < 21; i++) {
    boolean active = (pattern & 1) != 0;
    pattern >>= 1;
    digitalWrite (data, active ? HIGH : LOW);
    digitalWrite (clock, HIGH);
    delayMicroseconds (1);
    digitalWrite (clock, LOW);
  }
  digitalWrite (latch, HIGH);
  digitalWrite (data, LOW);
}

