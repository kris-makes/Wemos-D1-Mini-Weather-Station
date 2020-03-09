#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "logo_xbm.h"

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int ora;
int perc;
int mp;
char time_output[10];

const char *ssid     = "wifi";
const char *password = "biztosnem";

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
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

  delay(1000);
}
