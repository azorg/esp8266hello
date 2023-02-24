Порядок установки Arduino IDE для разработки для ES8266
=======================================================

# 1. Установить Arduino IDE (https://www.arduino.cc/)
Я установил версию 2.0.3.
Просто сохранил "arduino-ide_2.0.3_Linux_64bit.AppImage" в каталог ~/Arduino.
Потом установил права на запуск `chmod a+x ~/Arduino/arduino-<TAB><ENTER>"

Запуск: ~/Arduino/arduino-[TAB][ENTER]" или иначе через файловый менеджер, "ярлык" и т.п.
При первом запуске попросили согласиться с условиями (но ктож их читает?).

В XFCE я сделал ссылку на рабочий стол для запуска Arduino IDE.

# 2. Установка библиотек и инструментов ESP8266 для Arduino IDE
https://github.com/arduino/Arduino/wiki/Unofficial-list-of-3rd-party-boards-support-urls

Можно скачать полное руководство в PDF формате (см. каталог doc в данном проекте).
Официальный сайт по ESP8266: https://www.espressif.com/

## 2.1 Запустить Arduino IDE и открыть настройки (File->Preferences)

## 2.2 Добавить в поле Additional Board Manager URLs следующую строку (Stable):
```
https://arduino.esp8266.com/stable/package_esp8266com_index.json
```
Нажать OK $-)

## 2.3 Открыть Tools->Board->Board Manager, задать в поиске ESP8266 и нажать INSTALL
(это не быстро, нужен хороший Интернет)

## 2.4 Необходимо настроить права доступа для вашего пользователя к последовательным устройствам (/dev/ttyUSB*)
На Debian 11 я добавил своего пользователя user в группу dialout так (это не единственный способ):
```
sudo adduser $USER dialout
```
Теперь после выхода/входа в систему я могу обращаться
в /dev/ttyUSB* и запускать minicom/picocom от пользователя без повышенных
привилегий суперпользователя root.

Обязательно выйти из сеанса пользователя и снова войти!

## 2.5 Дополнительные пакеты, которые нужно (желательно, рекомендовано) установить в Debian-11:
```
sudo apt install git git-doc gitk qgit git-gui meld #<- git "must have"
sudo apt install minicom screen picocom

```
### 2.5.1 Желательно установить esptool из pip, а не пакетов Debian (в пакетах "плохая" версия)
```
sudo apt install python3-pip
sudo pip install esptool
``` 
Должен автоматически установиться pyserial и еще кое-что...

esptool - замечательная программа для загрузки кода во FLASH память ESP32.

* esptool в пакетах Debian весрии v2.8

* esptool.py из pip (сегодня 2023.02.17) - v4.5

* esptool в Arduino установился версии - v4.2.1

Устанавливать esptool в Debian так НЕ НАДО `sudo apt install esptool`
Если установили для опытов, то удалить так: `sudo apt purge esptool`

esptool.py (v4.5) может пригодится, но Arduino использует свою версию,
которая установилась у меня в ~/.arduino15/packages/esp32/tools/esptool_py

Пользу от установленного esptool.py можно извлечь сразу и посмотреть тип вашего чипа:
```
esptool.py -p /dev/ttyUSB0 chip_id
```

# 3. Первое подключение и загрузка загрузчика во FLASH

 * удерживая кнопку "BOOT" кратковременно нажать кнопку "EN" (reset)
Можно проконтролировать, что ESP8266 перешел в режим загрузчика:
```
picocom -b 115200 /dev/ttyUSB0

rst:0x1 (POWERON_RESET),boot:0x3 (DOWNLOAD_BOOT(UART0/UART1/SDIO_REI_REO_V2))
waiting for download
```
Не забыть закрыть picocom (Ctrl + A + X)

 * Выбрать Плату: Tools->Board -> esp8266 -> "..."

 * Выбрать Порт: Tools->Port -> /dev/ttyUSB0 (может быть USB1 и т.д.)

 * В меню Tools->Programmer выбрать esptool (тот самый, который установился сам)

 * Выбрать Tools->Burn Bootloader
Если нигде не ошиблись, то все должно пройти без ошибок. Нет - нужно разбираться.

# 4. Загрузите первый скетч (blink_led_simple)

 * File -> Open ... 

 * Sketch -> Upload

 * Tools -> Serial Monitor, задать скорость обмена 115200 бод

 * Убедится, что сообщение "Hello ESP8266" выводится, с светодиод моргает!

# 5. Поздравьте себя с успешной установкой Arduino IDE для ESP8266!
 
