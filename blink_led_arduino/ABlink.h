/*
 * Simple Arduino blink LED implementration
 * File: "ABlink.h"
 */

#pragma once
#ifndef ABLINK_H
#define ABLINK_H
//-----------------------------------------------------------------------------
#include <Arduino.h>
//-----------------------------------------------------------------------------
#define ABLINK_PIN    LED_BUILTIN
#define ABLINK_INVERT false
#define ABLINK_ON     50  // LED on time [ms]
#define ABLINK_OFF    100 // LED off time [ms]
//-----------------------------------------------------------------------------
class ABlink {
private:
  int _pin;
  uint8_t _neg;   // 0-positive (HIGH=on), 1-negative (LOW=on)
  uint8_t _state; // 1-on, 0-off
  uint8_t _run;   // 0-initial, 1-run blink
  unsigned _cnt, _t, _on, _off;

public:
  // call from setup()
  void begin(int      pin=ABLINK_PIN,    // GPIO pin number
             bool  invert=ABLINK_INVERT, // negative conect
             unsigned  on=ABLINK_ON,     // on time [ms]
             unsigned off=ABLINK_OFF) {  // off time [ms]
    _pin   = pin;
    _neg   = invert ? 1 : 0;
    _state = _run = 0;
    _cnt   = _t   = 0;
    _on    = on;
    _off   = off;
    pinMode(_pin, OUTPUT);
    digitalWrite(pin, _neg); // LED off by default
  }

  // set LED state (ON/OFF)
  void set(uint8_t val) { // on/off LED: 0-off, 1-on
    digitalWrite(_pin, (_state = val) ^ _neg);
  }

  void on()  { set(1); } // set LED on
  void off() { set(0); } // set LED off

  // get LED state
  uint8_t get() const { return _state; } // return: 0-off 1-on
  
  // blink LED some times (0 - stop blink)
  void blink(unsigned cnt=1) { (_cnt = cnt) || (_run = 0); }

  // periodic call from main loop (t = millis())
  void yield(unsigned long t) {
    if (_cnt) {
      if (!_run) {
        _run = 1;
        on();
        _t = t + _on;
      } else {
        long dt = (long) (_t - t);
        if (dt <= 0) {
          if (_state) {
            off();
            _t = t + _off;
          } else {
            if (--_cnt) {
              on();
              _t = t + _on;
            } else {
              _run = 0;
            }
          }
        }
      }
    }
  }
};
//-----------------------------------------------------------------------------
#endif // ABLINK_H

/*** end of "ABlink.h" file ***/
