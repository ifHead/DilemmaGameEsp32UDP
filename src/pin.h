#ifndef pin_h         
#define pin_h

#include "Arduino.h"

#define SERIAL_ISC Serial2
#define SERIAL_DEBUG Serial

#define ISC_CONTROLL_PIN 27


void serial_init(){
    //SERIAL_ISC.begin(9600);
    SERIAL_ISC.begin(38400);
    
    SERIAL_DEBUG.begin(115200);
}

void pin_init(){
    pinMode(ISC_CONTROLL_PIN, OUTPUT);
    digitalWrite(ISC_CONTROLL_PIN, HIGH);

}


#endif