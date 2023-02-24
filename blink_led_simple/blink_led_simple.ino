/*
 * Безобразно простой "Blink LED" для ESP8266
 */

#define LED LED_BUILTIN // голубой светодиод на плате подключен к GPIO2 и GND
                        // красный светодиод горит при включении питания

#define BAUDRATE 115200 // скорость UART бит/с

void setup() {
  // put your setup code here, to run once:

  // set pin mode to output
  pinMode(LED, OUTPUT);

  Serial.begin(BAUDRATE);
  Serial.println("\nHello ESP8266!\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, HIGH);
  delay(100);

  digitalWrite(LED, LOW);
  delay(900);
}

