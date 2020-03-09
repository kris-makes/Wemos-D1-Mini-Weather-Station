#include <U8g2lib.h>
#include <Wire.h>
#include "logo_xbm.h"

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  u8g2.begin();
  u8g2.firstPage();
  do {
    u8g2.setDrawColor(1);
    u8g2.setBitmapMode(1);
    //u8g2.drawRFrame(0, 0, 128, 64, 10); //x,y,w,h,r
    //u8g2.drawRFrame(1, 1, 126, 62, 8); //x,y,w,h,r
    u8g2.drawXBM( 0, 0, logo_width, logo_height, logo_bits);
  } while ( u8g2.nextPage() );
  delay(10000);
  u8g2.begin();
}

void loop() {}
