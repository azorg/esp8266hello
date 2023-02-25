/*
 * Простой "Blink LED" для ESP8266 с использованием таймера (Arduino Ticker'а)
 */

#include <Ticker.h>

#define LED LED_BUILTIN // голубой светодиод на плате подключен к GPIO2 и GND
#define LED_INVERT 1    // признак инверсии сигнала управления светодиодом {1|0}

#define BAUDRATE 115200 // скорость UART бит/с

#define TICKER_MS 10 // перид Тикера [ms]
#define TICKER_HZ (1000/TICKER_MS) // частота Тикера [Гц]

#define LED_ON  10 // время включения светодиода в тиках 
#define LED_OFF 90 // время выключения светодиода в тиках

Ticker ticker;

volatile uint8_t  ticker_flag = 0; // флаг от тикера
volatile uint32_t ticker_cnt  = 0; // счетчик тиков

uint8_t  led = LOW;   // состояние светодиода  
uint32_t led_cnt = 0; // счетчик в тиках таймера состояния светодиода

void ticker_callback() {
  ticker_flag = 1;
  ticker_cnt++;
}
void setup() {
  // set LED pin mode to output
  pinMode(LED, OUTPUT);
  digitalWrite(LED, (led = LOW) ^ LED_INVERT);

  // setup Ticker (FIXME: возможно это как "Из пушки по воробьям!")
  //ticker.attach_ms_scheduled(TICKER_MS, ticker_callback);
  ticker.attach_ms(TICKER_MS, ticker_callback);

  Serial.begin(BAUDRATE);

  Serial.println("\nHello ESP8266!");
  Serial.flush();
}

// функция вызываемая с частотой тиков из контекста главного цикла
// для управления миганием светодиода
void led_callback()
{
  if (led_cnt == 0)
  {
    if (led == LOW)
    {
      led = HIGH;
      led_cnt = LED_ON;
    }
    else // led == HIGH
    {
      led = LOW;
      led_cnt = LED_OFF;
    }
    digitalWrite(LED, led ^ LED_INVERT);
  }
  else
    led_cnt--;
}

// функция вызываемая из контектса главного цикла с периодом 1 сек
void on_second()
{
  Serial.printf("ticker_cnt = %u\n", ticker_cnt);
  Serial.flush();
}

uint32_t t0 = 0, t1;

void loop() {
  for (;;) {
    if (ticker_flag) {
      ticker_flag = 0;
      led_callback();      
    }

    for (t1 = ticker_cnt; t1 - t0 >= TICKER_HZ; t0 += TICKER_HZ) {
      on_second();
    }

    // TODO: тут хорошо бы перевести SoC в легкий сон до следующего прерывания таймера
    yield();
  }
}

