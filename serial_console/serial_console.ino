/*
 * Пример использования MicroRL в Arduino
 * File: "serial_console.ino"
 */

//-----------------------------------------------------------------------------
#include "global.h"
#include "config.h"
#include "cli.h"
//-----------------------------------------------------------------------------
// функция вызываемая тикером
void ticker_callback() {
  Led.tick();
}
//-----------------------------------------------------------------------------
void setup() {
  // setup blink LED
  Led.begin(LED, LED_INVERT, LED_BLINK_ON, LED_BLINK_OFF);

  // setup UART
  Serial.begin(BAUDRATE);
  Serial.println("\r\n\r\nHello ESP8266!\r\n");

  // init CLI (MicroRL)
  cli_init();

  Serial.flush();

  // setup ticker
  Ticker.begin(ticker_callback, TICKER_MS, true, millis());
}
//-----------------------------------------------------------------------------
void loop() {
  Ticker.yield(millis());
  cli_loop();
}
//-----------------------------------------------------------------------------

