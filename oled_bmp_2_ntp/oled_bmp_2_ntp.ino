#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include "logo_xbm.h"
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

float temp;
float pres;
float alt;
int i = 5;
int ora;
int perc;
int mp;
char time_output[10];

const char *ssid     = "wifi";
const char *password = "biztosnem";

const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

Adafruit_BMP280 sensor_bmp;

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


void setup()
{
  Serial.begin(115200);
  Serial.println("BMP280 teszt");
  WiFi.begin(ssid, password);
  u8g2.begin();
  while ( WiFi.status() != WL_CONNECTED ) {
    Serial.print ( "." );
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_t0_15_tf );
      u8g2.drawStr((128 - u8g2.getStrWidth("Connecting")) / 2, 13, "Connecting");
      u8g2.drawCircle(i, 45, 2);
      i = i + 2;
    } while ( u8g2.nextPage() );
    delay ( 500 );
  }

  timeClient.begin();

  if (!sensor_bmp.begin())
  {
    Serial.println("Az érzékelő nem található. Ellenőrizze a csatlakozásokat!");
    while (1);
  }
  u8g2.begin();
  u8g2.firstPage();
  do {
    u8g2.setDrawColor(1);
    u8g2.setBitmapMode(1);
    //u8g2.drawRFrame(0, 0, 128, 64, 10); //x,y,w,h,r
    //u8g2.drawRFrame(1, 1, 126, 62, 8); //x,y,w,h,r
    u8g2.drawXBM( 0, 0, logo_width, logo_height, logo_bits);
  } while ( u8g2.nextPage() );
  delay(3000);
  u8g2.begin();

}

void loop() {
  for (i = 0; i < 10; i++) {
    draw_sensor();
    delay(1000);
  }
  scroll_sensor();
  for (i = 0; i < 10; i++) {
    draw_time();
    delay(1000);
  }
  scroll_time();
}

void draw_sensor() {
  temp = (sensor_bmp.readTemperature() - 4.0);
  pres = (sensor_bmp.readPressure() / 100.0F  + 17);
  u8g2.firstPage();
  do {
    //Comandos graficos para o display devem ser colocados aqui
    u8g2.setFont(u8g2_font_t0_15_tf );
    //Mostra a temperatura
    u8g2.drawStr(5, 31, "Temp:      C");
    u8g2.drawCircle(89, 23, 2); //Grau
    u8g2.setCursor(51, 31);
    u8g2.print(temp, 1);

    u8g2.drawStr(5, 45, "Pres:       hPa");
    u8g2.setCursor(50, 45);
    u8g2.print(pres, 1);

    u8g2.drawStr(5, 59, "Alt :       m");
    u8g2.setCursor(50, 59);
    u8g2.print(sensor_bmp.readAltitude(1010));

    u8g2.drawStr((128 - u8g2.getStrWidth("Weather Station")) / 2, 13, "Weather Station");
    u8g2.drawRFrame(0, 16, 128, 48, 4);
    u8g2.drawRFrame(0, 0, 128, 16, 4);
  } while ( u8g2.nextPage() );
}

void scroll_sensor() {
  temp = (sensor_bmp.readTemperature() - 4.0);
  pres = (sensor_bmp.readPressure() / 100.0F  + 17);
  alt = (sensor_bmp.readAltitude(1010));
  timeClient.update();
  ora = timeClient.getHours();
  perc = timeClient.getMinutes();
  mp = timeClient.getSeconds();
  sprintf(time_output, "%02d:%02d:%02d", ora, perc, mp);

  for (i = 0; i < 128; i = i + 4) {
    u8g2.firstPage();
    do {
      //sensor out
      u8g2.setFont(u8g2_font_t0_15_tf );

      u8g2.drawStr(5 - i, 31, "Temp:      C");
      u8g2.drawCircle(89 - i, 23, 2); //Grau
      u8g2.setCursor(51 - i, 31);
      u8g2.print(temp, 1);

      u8g2.drawStr(5 - i, 45, "Pres:       hPa");
      u8g2.setCursor(50 - i, 45);
      u8g2.print(pres, 1);

      u8g2.drawStr(5 - i, 59, "Alt :       m");
      u8g2.setCursor(50 - i, 59);
      u8g2.print(alt, 1);

      u8g2.drawStr((128 - u8g2.getStrWidth("Weather Station")) / 2 - i, 13, "Weather Station");
      u8g2.drawRFrame(0 - i, 16, 128, 48, 4);
      u8g2.drawRFrame(0 - i, 0, 128, 16, 4);

      //time in
      u8g2.setFont(u8g2_font_maniac_tf);

      u8g2.setCursor((128 - u8g2.getStrWidth(time_output)) / 2 + 128 - i, 51);
      u8g2.print(time_output);

      u8g2.setFont(u8g2_font_t0_15_tf );
      u8g2.drawStr((128 - u8g2.getStrWidth(daysOfTheWeek[timeClient.getDay()])) / 2 + 128 - i, 13, daysOfTheWeek[timeClient.getDay()]);
      u8g2.drawRFrame(0 + 128 - i, 16, 128, 48, 4);
      u8g2.drawRFrame(0 + 128 - i, 0, 128, 16, 4);
    } while ( u8g2.nextPage() );
    delay(10);
  }
}

void draw_time() {
  timeClient.update();
  ora = timeClient.getHours();
  perc = timeClient.getMinutes();
  mp = timeClient.getSeconds();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(ora);
  Serial.print(":");
  Serial.print(perc);
  Serial.print(":");
  Serial.println(mp);
  //Serial.println(timeClient.getFormattedTime());
  sprintf(time_output, "%02d:%02d:%02d", ora, perc, mp);
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_maniac_tf);

    u8g2.setCursor((128 - u8g2.getStrWidth(time_output)) / 2, 51);
    u8g2.print(time_output);

    u8g2.setFont(u8g2_font_t0_15_tf );
    u8g2.drawStr((128 - u8g2.getStrWidth(daysOfTheWeek[timeClient.getDay()])) / 2, 13, daysOfTheWeek[timeClient.getDay()]);
    u8g2.drawRFrame(0, 16, 128, 48, 4);
    u8g2.drawRFrame(0, 0, 128, 16, 4);
  } while ( u8g2.nextPage() );
}

void scroll_time() {
  temp = (sensor_bmp.readTemperature() - 4.0);
  pres = (sensor_bmp.readPressure() / 100.0F  + 17);
  alt = (sensor_bmp.readAltitude(1010));
  timeClient.update();
  ora = timeClient.getHours();
  perc = timeClient.getMinutes();
  mp = timeClient.getSeconds();
  sprintf(time_output, "%02d:%02d:%02d", ora, perc, mp);

  for (i = 0; i < 128; i = i + 4) {
    u8g2.firstPage();
    do {

      //time out
      u8g2.setFont(u8g2_font_maniac_tf);

      u8g2.setCursor((128 - u8g2.getStrWidth(time_output)) / 2 - i, 51);
      u8g2.print(time_output);

      u8g2.setFont(u8g2_font_t0_15_tf );
      u8g2.drawStr((128 - u8g2.getStrWidth(daysOfTheWeek[timeClient.getDay()])) / 2 - i, 13, daysOfTheWeek[timeClient.getDay()]);
      u8g2.drawRFrame(0 - i, 16, 128, 48, 4);
      u8g2.drawRFrame(0 - i, 0, 128, 16, 4);


      //sensor in
      u8g2.setFont(u8g2_font_t0_15_tf );

      u8g2.drawStr(5 + 128 - i, 31, "Temp:      C");
      u8g2.drawCircle(89 + 128 - i, 23, 2); //Grau
      u8g2.setCursor(51 + 128 - i, 31);
      u8g2.print(temp, 1);

      u8g2.drawStr(5 + 128 - i, 45, "Pres:       hPa");
      u8g2.setCursor(50 + 128 - i, 45);
      u8g2.print(pres, 1);

      u8g2.drawStr(5 + 128 - i, 59, "Alt :       m");
      u8g2.setCursor(50 + 128 - i, 59);
      u8g2.print(alt, 1);

      u8g2.drawStr((128 - u8g2.getStrWidth("Weather Station")) / 2 + 128 - i, 13, "Weather Station");
      u8g2.drawRFrame(0 + 128 - i, 16, 128, 48, 4);
      u8g2.drawRFrame(0 + 128 - i, 0, 128, 16, 4);
    } while ( u8g2.nextPage() );
    delay(10);
  }
}
