#ifndef tft_h         
#define tft_h

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#include "setting.h"

#define _cs   15  // goes to TFT CS
#define _dc   26 // goes to TFT DC
#define _mosi 23  // goes to TFT MOSI
#define _sclk 18  // goes to TFT SCK/CLK
#define _rst  4   // goes to TFT RESET
#define _miso     // Not connected    

Adafruit_ILI9341 tft = Adafruit_ILI9341(_cs, _dc, _rst);

String display_udp_rx;

void tft_init(){
     tft.begin();

     tft.setRotation(TFT_LCD_SET_ROTATION);
     tft.fillScreen(ILI9341_BLACK); 
     tft.setTextSize(3);

     tft.setTextColor(ILI9341_WHITE);  
     tft.setTextSize(2);
     
}
    //tft.setCursor(0, 0);
    //tft.setTextColor(ILI9341_WHITE);  
    //tft.setTextSize(1);
    //tft.print(); tft.setCursor(0, 0);
    

 

#endif