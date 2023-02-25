/*
 * Простой "Blink LED" для ESP8266 с использованием классов (ALed, ATicker)
 */

#include "ALed.h"
#include "ATicker.h"

#define LED LED_BUILTIN // голубой светодиод на плате подключен к GPIO2 и GND
#define LED_INVERT true // признак инверсии сигнала управления светодиодом

#define BAUDRATE 115200 // скорость UART бит/с

#define LED_ON  5  // время включения светодиода в тиках по 10 мс
#define LED_OFF 10 // время выключения светодиода в тиках по 10 мс

ALed led; // компонент для моргания светодиодом

ATicker ticker_10ms; // тикер с периодом 10 мс
ATicker ticker_1sec; // тикер с периодом 1 с

// функция вызываемая с периодом 10 мс
void tick_10ms() {
  led.tick();
}

// функция вызываемая с периодом 1 с
void tick_1sec() {
  led.blink(3); // blink LED 3 times
  Serial.printf("millis() = %lu\n", millis());
  //Serial.flush();
}

void setup() {
  // setup blink LED
  led.begin(LED, LED_INVERT, LED_ON, LED_OFF);

  // setup UART
  Serial.begin(BAUDRATE);
  Serial.println("\nHello ESP8266!\n");
  Serial.flush();

  // setup tickers
  unsigned long t = millis();
  ticker_10ms.begin(tick_10ms,   10, true, t);
  ticker_1sec.begin(tick_1sec, 1000, true, t);
}

void loop() {
  unsigned long t = millis();
  ticker_10ms.yield(t);
  ticker_1sec.yield(t);

  // TODO: тут хорошо бы перевести SoC в легкий сон до следующего прерывания таймера
}

