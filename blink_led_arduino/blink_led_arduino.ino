/*
 * Еще один "Blink LED" для Arduino с использованием классов (ABlink, ATicker)
 */

#include "ABlink.h"
#include "ATicker.h"

#define BAUDRATE 115200 // скорость UART бит/с

#define LED LED_BUILTIN // голубой светодиод на плате подключен к GPIO2 и GND
#define LED_INVERT true // признак инверсии сигнала управления светодиодом

#define LED_BLINK_ON  25 // время включения светодиода [мс]
#define LED_BLINK_OFF 50 // время выключения светодиода [мс]

ABlink  led;    // компонент для моргания светодиодом
ATicker ticker; // тикер с периодом 1 с

// функция вызываемая с периодом 1 с
void ticker_callback() {
  led.blink(2); // blink LED 3 times
  Serial.print("millis() = ");
  Serial.println(millis(), DEC);
}

void setup() {
  // setup UART
  Serial.begin(BAUDRATE);
  Serial.println("\nHello Arduino!\n");
  Serial.flush();

  // setup blink LED
  led.begin(LED, LED_INVERT, LED_BLINK_ON, LED_BLINK_OFF);

  // setup ticker
  ticker.begin(ticker_callback, 1000, true, millis());
}

void loop() {
  unsigned long t = millis();
  led.yield(t);
  ticker.yield(t);
}

