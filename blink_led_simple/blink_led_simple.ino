/*
 * Безобразно простой "Blink LED" для ESP8266
 */

#define LED LED_BUILTIN // голубой светодиод на плате подключен к GPIO2 и GND
#define LED_INVERT 1    // признак игферсии сишнала управления светодиодом {1|0}

#define BAUDRATE 115200 // скорость UART бит/с

void setup() {
  // set pin mode to output
  pinMode(LED, OUTPUT);

  Serial.begin(BAUDRATE);
  Serial.println("\nHello ESP8266!");
}

uint32_t cnt = 0;

void loop() {
  digitalWrite(LED, HIGH ^ LED_INVERT);
  delay(100);

  digitalWrite(LED, LOW ^ LED_INVERT);
  delay(900);

  Serial.printf("%d\n", cnt++);
}

