#include "Keypad.h"

Keypad::Keypad (const Debounce& upButton, const Debounce& downButton, NumberKeeper& keeper):
  upButton (upButton),
  downButton (downButton),
  keeper (keeper)
{
  this->upWasDown = upButton.isDown ();
  this->downWasDown = downButton.isDown ();
}

bool wasJustPressed (const Debounce& button, bool &prevState) {
  bool retval = button.isDown () && !prevState;
  prevState = button.isDown ();
  return retval;
}

void handlePress (bool justPressed, const Debounce& otherButton, NumberKeeper& keeper, int delta) {
  if (otherButton.isDown ()) {
    keeper.increaseBy (0 - keeper.value ());
  }
  else {
    keeper.increaseBy (delta);
  }
}

void Keypad::update (unsigned long now) {
  bool upPressed = wasJustPressed (upButton, upWasDown);
  bool downPressed = wasJustPressed (downButton, downWasDown);
  if (!upPressed && !downPressed) {return;}
  handlePress (upPressed, downButton, keeper, 1);
  handlePress (downPressed, upButton, keeper, -1);
}

