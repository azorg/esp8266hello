/*
 * Simple blink LED class for Arduino projects
 * File: "ALed.cpp"
 */

#include "ALed.h"
  
// call from setup()
void ALed::begin(int pin, bool negative, uint32_t on_ticks, uint32_t off_ticks)
{
  this->pin = pin;
  neg       = negative ? 1 : 0;
  state     = 0;
  blink_on  = on_ticks;
  blink_off = off_ticks;
  blink_cnt = blink_times = 0;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, neg); // off by default
}

// piriodic call by timer
void ALed::tick()
{
  // blink LED by timer
  if (blink_times)
  {
    if (blink_cnt == 0)
    {
      blink_cnt = blink_on;
      on();
    }
    else // blink_cnt != 0
    {
      if (--blink_cnt == 0)
      {
        if (state)
        {
          blink_cnt = blink_off;
          off();
        }
        else
        {
          blink_times--;
        }
      }
    }
  }
}

/*** end of "ALed.cpp" file ***/

