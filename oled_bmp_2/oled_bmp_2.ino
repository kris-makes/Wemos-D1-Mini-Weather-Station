#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include "logo_xbm.h"
float temp;
float pres;
Adafruit_BMP280 sensor_bmp;

//Definicoes do display Oled
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


void setup()
{
  Serial.begin(9600);
  Serial.println("BMP280 teszt");

  //Verifica a conexão do sensor BMP280
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

void loop()
{
  //Chama a rotina de desenho na tela
  draw();
  Serial.println(sensor_bmp.readTemperature());
  delay(2000);
}

void draw()
{
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
