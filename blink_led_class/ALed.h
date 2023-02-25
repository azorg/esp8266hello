/*
 * Simple blink LED class for Arduino projects
 * File: "ALed.h"
 */

#pragma once
#ifndef ALED_H
#define ALED_H

#include <Arduino.h>

#define ALED_PIN       2
#define ALED_ON_TICKS  50
#define ALED_OFF_TICKS 100

class ALed {
private:
  int pin;
  uint8_t neg;   // 0-positive (HIGH=on), 1-negative (LOW=on)
  uint8_t state; // 1-on, 0-off
  uint32_t blink_on, blink_off, blink_cnt, blink_times;

public:
  // call from setup()
  void begin(int pin=ALED_PIN,                   // GPIO pin number
             bool negative=false,                // negative conect      
             uint32_t on_ticks=ALED_ON_TICKS,    // on time in ticks
             uint32_t off_ticks=ALED_OFF_TICKS); // off time in ticks
  
  // get LED state
  uint8_t get() const { return state; } // return: 0-off 1-on
  
  // set LED state (ON/OFF)
  void set(uint8_t val) // on/off LED: 0-off, 1-on
  {
    digitalWrite(pin, (state = val) ^ neg);
  }

  void on()  { set(1); } // set LED on
  void off() { set(0); } // set LED off

  // blink LED some times (0 - stop blink)
  void blink(uint32_t times=1) { blink_times = times; }

  // piriodic call by timer
  void tick();
};

#endif // ALED_H

/*** end of "ALed.h" file ***/

