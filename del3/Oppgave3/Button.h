#ifndef Button_h
#define Button_h

#include <inttypes.h>
#include "Arduino.h"

class Button{
private:
  uint8_t _btnPin;
  uint8_t _lastState = false;
  uint8_t _buttonState;
  uint8_t _lastTrueState;
  
  long _lastDebounce = 0;
  long _debounceDelay = 20; //ms

  public:
  Button(int pin);

  bool state();
  
  bool onRelease();
  bool onPress();
  bool on();
  bool off();
};


#endif /* Button_h*/

