# Mini időjárás állomás
Kompakt kijelző és mérőállomás internet alapú idő megjelenítéssel

## Működés
A rendszer egy [WEMOS D1 mini](https://3dwarehouse.sketchup.com/model/b3c9daa8-2c69-4b6d-b22c-631123883484/Wemos-D1-mini-V210-ESP8266?hl=hu) alapon fut, erre van kötve egy 128x64-es [OLED kijelzőre](http://www.lcdwiki.com/res/MC130GX_VX/1.3inch_IIC_OLED_Module_MC130GX&MC130VX_User_Manual_EN.pdf) I2C-vel. A lekért adatok vízszintesen görgetve jelennek meg a képernyőn az [u8g2 könyvtár](https://github.com/olikraus/u8g2) használatával.
Videó róla [itt](https://youtu.be/lskLDnISdC4).

#### Időjárás állomás
Egy I2C-vel bekötött [BMP280-as szenzor](https://www.pcb-hero.com/products/hw-611-bmp280-3-3-digital-barometric-pressure-altitude-sensor) felel hőmérséklet, és a légnyomás méréséért, utóbbiból tengerfeletti magasságot számolva.
#### Idő
A mikrokontroller wifin keresztül csatlakozik egy [NTP szerverre](https://www.ntppool.org/hu/), ahonnan lekéri az aktuális időt és a hét napját.

## Kialakítás
A kontrollerhez tartozik egy saját készítésű shield, amire a kijelző és a BMP280-as van bekötve. A teljes egység egy saját tervezésű 3D tervezett házban van rögzítve.

#### Képek:
![Image](https://github.com/kris-makes/Wemos-D1-Mini-Weather-Station/blob/master/kepek/img1.jpg?raw=true)
![Image](https://github.com/kris-makes/Wemos-D1-Mini-Weather-Station/blob/master/kepek/img2.jpg?raw=true)

## Felhasznált könyvtárak
**Megjelenítés:** [U8g2](https://github.com/olikraus/u8g2)
**NTP szerver:** [NTPClient](https://github.com/arduino-libraries/NTPClient)
**NTP kliens kezelése:** [WifiUDP](https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/WiFiUdp.h)
**Wifis kompatibilitás:** [Esp8266Wifi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi)
**I2C kezelés:** [Wire](https://github.com/esp8266/Arduino/blob/master/libraries/Wire/Wire.h)
**BMP280**: [Adafruit Sensor](https://github.com/adafruit/Adafruit_Sensor) és [Adafruit BMP280](https://github.com/adafruit/Adafruit_BMP280_Library)
