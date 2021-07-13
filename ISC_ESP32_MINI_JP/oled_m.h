#ifndef oled_m_h         
#define oled_m_h

#include <Arduino.h>
#include "ssd1306.h"
#include "WiFi.h"
#include "setting.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);
boolean lcd_enable = false;
char str_led[50];
String display_udp_rx = "";

int score = 0;


void oled_init(){

    

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        lcd_enable = false;
    }else{
        lcd_enable = true;
        display.display();
        display.clearDisplay();
        display.setTextSize(1);      // Normal 1:1 pixel scale
        display.setTextColor(WHITE);        // Draw white text
        Serial.println("LCD enable..");
  

    }
}
//----------------
void oled_view(){
    if(lcd_enable == true){
        display.clearDisplay();
        
        display.setCursor(0,0); display.println(F("WIFI ID: "));
        display.setCursor(50,0); display.println(F(ssid));

        display.setCursor(0,10); display.println(F("WIFI Pw: "));
        display.setCursor(50,10); display.println(F(password));

        display.setCursor(0,20); display.println(F("IP: "));
        display.setCursor(50,20); display.println( WiFi.localIP() );

        display.setCursor(0,30); display.println(F("RSSI: "));
        display.setCursor(50,30); display.println( WiFi.RSSI() );

        display.setCursor(0,40); display.println(F("RECIVE: "));
        display.setCursor(50,40); display.println( display_udp_rx );

        display.setCursor(0,50); display.println(F("SCORE: "));
        display.setCursor(50,50); display.println( score );

        display_udp_rx = ""; //초기화
        display.display();
    }
}



/*
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
*/




#endif
