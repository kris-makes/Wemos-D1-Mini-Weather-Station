// 1.1: added TH06 humidity reading
// nokia, rotated 180 > U8G2_R0=>U8G2_R2
//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include <Wire.h>     //I2C protocol
#include <U8g2lib.h>     //display handling
#include <Adafruit_Sensor.h>     //sensor handling
#include <Adafruit_BMP280.h>     //BMP280 sensor measuring
#include "logo_xbm.h"     //logo bitmap
#include <NTPClient.h>     //getting time from ntp server
#include <ESP8266WiFi.h>     //esp8266 handling
#include <WiFiUdp.h>     //ntp handling

//-----------------------------------------------------------------------------
// variables
//-----------------------------------------------------------------------------
float Temperature;
float Pressure;
int Humidity;
int i = 5;
int Hours;
int Minutes;
int Seconds;
const char *ssid     = "Digi-02265438";     //your ssid in ""
const char *password = "Panna001";     //your password in ""
const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char time_output[10];

//-----------------------------------------------------------------------------
// defines
//-----------------------------------------------------------------------------
Adafruit_BMP280 sensor_bmp;     //BMP280
//U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);     //display: SH1106; I2C; 128x64
U8G2_PCD8544_84X48_1_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/ 15, /* dc=*/ 2, /* reset=*/ 0);      // Nokia 5110 Display
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


//=============================================================================
// setup
//=============================================================================
void setup() {
  Serial.begin(115200);     //start serial
  WiFi.begin(ssid, password);     //start wifi
  u8g2.begin();     //start display
  while ( WiFi.status() != WL_CONNECTED ) {     //connect to wifi
    Serial.print ( "." );
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_sandyforest_tr );
      u8g2.drawStr((84 - u8g2.getStrWidth("Connecting")) / 2, 11, "Connecting");     //draw progress circles while connecting
      u8g2.drawCircle(i, 30, 2);
      i = i + 1;
    } while ( u8g2.nextPage() );
    delay ( 500 );
  }
  timeClient.begin();     //start ntp
  if (!sensor_bmp.begin())     //start bmp280
  {
    Serial.println("Az érzékelő nem található. Ellenőrizze a csatlakozásokat!");
    while (1);
  }
  u8g2.begin();
  u8g2.firstPage();
  do {
    u8g2.setDrawColor(1);
    u8g2.setBitmapMode(1);
    u8g2.drawXBM( 0, 0, logo_width, logo_height, logo_bits);     //draw logo
  } while ( u8g2.nextPage() );
  delay(5000);
  u8g2.begin();     //restart display
}

//=============================================================================
// main loop
//=============================================================================
void loop() {
  for (i = 0; i < 10; i++) {     //update measurements 10 times
    draw_sensor();
    delay(1000);
  }
  scroll_sensor();     //fade in clock display
  for (i = 0; i < 10; i++) {     //update time 10 times
    draw_time();
    delay(1000);
  }
  scroll_time();     //fade in measurements display
}


//=============================================================================
// Function declarations
//=============================================================================
//-----------------------------------------------------------------------------
// getting humidity
//-----------------------------------------------------------------------------
int humidity() {
  float h = 0;
  int r;
  Wire.beginTransmission(0x40);
  Wire.write(0xE5);
  Wire.endTransmission();
  Wire.requestFrom(0x40, 2);
  if (Wire.available() > 0) {
    r = Wire.read();
    r = r << 8;
    r = r | Wire.read();
  }
  return h = (125.0 * r) / 65536.0 - 6.0;
}

//-----------------------------------------------------------------------------
// drawing sensor data
//-----------------------------------------------------------------------------
void draw_sensor() {
  Temperature = (sensor_bmp.readTemperature() - 4.0);
  Pressure = (sensor_bmp.readPressure() / 100.0F  + 17);
  Humidity = (humidity());
  u8g2.firstPage();
  do {
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_6x12_mf  );
    u8g2.drawStr(6, 25, "Temp:      C");     //draw temperature
    u8g2.drawCircle(69, 18, 1);
    u8g2.setCursor(39, 25);
    u8g2.print(Temperature, 1);
    u8g2.drawStr(6, 35, "Pres:");     //draw pressure
    u8g2.setCursor(39, 35);
    u8g2.print(Pressure, 1);
    u8g2.drawStr(6, 45, "Hum :   %");     //draw humidity
    u8g2.setCursor(39, 45);
    u8g2.print(Humidity, 1);
    u8g2.drawRBox(0, 0, 84, 15, 4);
    u8g2.setDrawColor(0);
    u8g2.setFont(u8g2_font_7x14B_mf  );
    u8g2.drawStr((84 - u8g2.getStrWidth("Weather")) / 2, 13, "Weather"); //draw frame and title
    u8g2.setDrawColor(1);
    u8g2.drawRFrame(0, 15, 84, 33, 4);
  } while ( u8g2.nextPage() );
}

//-----------------------------------------------------------------------------
// scrolling sensor data
//-----------------------------------------------------------------------------
void scroll_sensor() {
  Temperature = (sensor_bmp.readTemperature() - 4.0);
  Pressure = (sensor_bmp.readPressure() / 100.0F  + 17);
  Humidity = (humidity());
  timeClient.update();
  Hours = timeClient.getHours();
  Minutes = timeClient.getMinutes();
  Seconds = timeClient.getSeconds();
  sprintf(time_output, "%02d-%02d", Hours, Minutes);  // :%02d  , Seconds

  for (i = 0; i < 84; i = i + 2) {
    u8g2.firstPage();
    do {
      u8g2.setDrawColor(1);
      //fade sensor out
      u8g2.setFont(u8g2_font_6x12_mf  );
      u8g2.drawStr(6 - i, 25, "Temp:      C");     //draw temperature
      u8g2.drawCircle(69 - i, 18, 1);
      u8g2.setCursor(39 - i, 25);
      u8g2.print(Temperature, 1);
      u8g2.drawStr(6 - i, 35, "Pres:");     //draw pressure
      u8g2.setCursor(39 - i, 35);
      u8g2.print(Pressure, 1);
      u8g2.drawStr(6 - i, 45, "Hum :   %");     //draw humidity
      u8g2.setCursor(39 - i, 45);
      u8g2.print(Humidity, 1);
      u8g2.drawRBox(0, 0, 84, 15, 4);
      u8g2.setDrawColor(0);
      u8g2.setFont(u8g2_font_7x14B_mf  );
      u8g2.drawStr((84 - u8g2.getStrWidth("Weather")) / 2 - i, 13, "Weather");
      u8g2.setDrawColor(1);
      u8g2.drawRFrame(0, 15, 84, 33, 4);

      //fade time in
      u8g2.setFontMode(1);
      u8g2.setFont(u8g2_font_logisoso20_tn);
      u8g2.setCursor(8 + 84 - i, 42);    //draw time
      u8g2.print(time_output);
      u8g2.setFont(u8g2_font_7x14B_mf  );
      u8g2.drawRBox(0, 0, 84, 15, 4);
      u8g2.setDrawColor(0);
      u8g2.drawStr((84 - u8g2.getStrWidth(daysOfTheWeek[timeClient.getDay()])) / 2 + 84 - i, 13, daysOfTheWeek[timeClient.getDay()]); //draw frame and day
      u8g2.setDrawColor(1);
      u8g2.drawRFrame(0, 15, 84, 33, 4);
    } while ( u8g2.nextPage() );
    delay(20);
  }
}

//-----------------------------------------------------------------------------
// drawing time data
//-----------------------------------------------------------------------------
void draw_time() {
  timeClient.update();
  Hours = timeClient.getHours();
  Minutes = timeClient.getMinutes();
  Seconds = timeClient.getSeconds();
  sprintf(time_output, "%02d-%02d", Hours, Minutes);  // :%02d  , Seconds

  u8g2.firstPage();
  do {
    u8g2.setFontMode(1);
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_trixel_square_tn);
    u8g2.setCursor(38, 28);
    u8g2.print(String (Seconds));
    u8g2.setFont(u8g2_font_logisoso20_tn);
    u8g2.setCursor(8, 42);    //draw time
    u8g2.print(time_output);
    u8g2.setFont(u8g2_font_7x14B_mf  );
    u8g2.drawRBox(0, 0, 84, 15, 4);
    u8g2.setDrawColor(0);
    u8g2.drawStr((84 - u8g2.getStrWidth(daysOfTheWeek[timeClient.getDay()])) / 2, 13, daysOfTheWeek[timeClient.getDay()]); //draw frame and day
    u8g2.setDrawColor(1);
    u8g2.drawRFrame(0, 15, 84, 33, 4);
  } while ( u8g2.nextPage() );
}

//-----------------------------------------------------------------------------
// scrolling time data
//-----------------------------------------------------------------------------
void scroll_time() {
  Temperature = (sensor_bmp.readTemperature() - 4.0);
  Pressure = (sensor_bmp.readPressure() / 100.0F  + 17);
  Humidity = (humidity());
  timeClient.update();
  Hours = timeClient.getHours();
  Minutes = timeClient.getMinutes();
  Seconds = timeClient.getSeconds();
  sprintf(time_output, "%02d-%02d", Hours, Minutes);  // :%02d  , Seconds

  for (i = 0; i < 84; i = i + 2) {
    u8g2.firstPage();
    do {
      u8g2.setDrawColor(1);
      //fade time out
      u8g2.setFontMode(1);
      u8g2.setFont(u8g2_font_logisoso20_tn);
      u8g2.setCursor(8 - i, 42);    //draw time
      u8g2.print(time_output);
      u8g2.setFont(u8g2_font_7x14B_mf  );
      u8g2.drawRBox(0, 0, 84, 15, 4);
      u8g2.setDrawColor(0);
      u8g2.drawStr((84 - u8g2.getStrWidth(daysOfTheWeek[timeClient.getDay()])) / 2 - i, 13, daysOfTheWeek[timeClient.getDay()]); //draw frame and day
      u8g2.setDrawColor(1);
      u8g2.drawRFrame(0, 15, 84, 33, 4);

      //sensor in
      u8g2.setFont(u8g2_font_6x12_mf  );
      u8g2.drawStr(6 + 84 - i, 25, "Temp:      C");     //draw temperature
      u8g2.drawCircle(69 + 84 - i, 18, 1);
      u8g2.setCursor(39 + 84 - i, 25);
      u8g2.print(Temperature, 1);
      u8g2.drawStr(6 + 84 - i, 35, "Pres:");     //draw pressure
      u8g2.setCursor(39 + 84 - i, 35);
      u8g2.print(Pressure, 1);
      u8g2.drawStr(6 + 84 - i, 45, "Hum :   %");     //draw humidity
      u8g2.setCursor(39 + 84 - i, 45);
      u8g2.print(Humidity, 1);
      u8g2.drawRBox(0, 0, 84, 15, 4);
      u8g2.setDrawColor(0);
      u8g2.setFont(u8g2_font_7x14B_mf  );
      u8g2.drawStr((84 - u8g2.getStrWidth("Weather")) / 2 + 84 - i, 13, "Weather");
      u8g2.setDrawColor(1);
      u8g2.drawRFrame(0, 15, 84, 33, 4);
    } while ( u8g2.nextPage() );
    delay(20);
  }
}
