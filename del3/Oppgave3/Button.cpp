#include "Arduino.h"
#include "Button.h"

Button::Button(int pin) {
  pinMode (pin, INPUT);
  _btnPin = pin;
}

bool Button::state() {
      
  bool _reading = digitalRead(_btnPin);
  if (_reading != _lastState)
  {
    _lastDebounce = millis();
    _lastState = _reading;
  }
    
  if ((millis() - _lastDebounce) > _debounceDelay)
  {
      
    _lastTrueState = _buttonState;
    _buttonState = _reading;
  }
  return _buttonState;
}

bool Button::onRelease() {
  
  if (!state() && _lastTrueState)
  {
    return true;
  }
  return false;
}


bool Button::onPress() {
    bool btnstate = state();
  if ( btnstate && !_lastTrueState) {
    return true;
  }
  return false;
}

bool Button::on() {

  if (state()) {
    return true;
  }
  return false;
}

bool Button::off() {
  if (!state()) {
    return true;
  }
  return false;
}


