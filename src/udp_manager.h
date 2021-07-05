#ifndef udp_manager_h         
#define udp_manager_h


#include "Arduino.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include "setting.h"


class udp_m{
    public:
        WiFiUDP udp;
        char udp_tx[50];
        char log[50];

    udp_m(){ }
    
    void debug_mode(boolean d){
        
    }

    void init( int port ){
        udp.begin( port );
        localUdpPort = port; //대입
        before_millis = millis();
    }

    boolean available(){
        boolean rt = rx_en;
        rx_en = false;
        return rt;
    }

    String read_string(){
        return udp_packet_read;
    }

    
    void read_cach_dell(){
        for(int i=0; i<255; i++){
            udp_packet_read[i] = 0;
        }
    }

    void send( String id , int player_number ){
        if( WiFi.status() == WL_CONNECTED ){ 
            String send = id + "," + player_number + ",";
            for(int i=0; i<3; i++){
                udp.beginPacket("192.168.0.255", localUdpPort); //접속해라
                udp.printf( send.c_str() );
                udp.endPacket();
                delay(5);  
            }   
        }else{
            Serial.println("UDP PLAYER SEND ERROR...");
        }  
    }

    void send_score(int _sc){
        if( WiFi.status() == WL_CONNECTED ){ 
                udp.beginPacket("192.168.0.255", localUdpPort); 
                sprintf(udp_tx, "#SPCC,%d,%d," , DEVICE_ID, _sc);
                //#SPCC,ID, 값
                udp.printf( udp_tx );
                udp.endPacket();
        }
    }

    void send_score(int _sc , String log_s){
            if( WiFi.status() == WL_CONNECTED ){ 
                    udp.beginPacket("192.168.0.255", localUdpPort); 

                    strcpy(log,log_s.c_str());
                    sprintf(udp_tx, "#SPCC,%d,%d,%s" , DEVICE_ID, _sc , log );
                    //#SPCC,ID, 값
                    udp.printf( udp_tx );
                    udp.endPacket();
            }
        }



    void send( String id ){
            if( WiFi.status() == WL_CONNECTED ){ 
                for(int i=0; i<3; i++){
                    udp.beginPacket("192.168.0.255", localUdpPort); //접속해라
                    udp.printf( id.c_str() );
                    udp.endPacket();
                    delay(5);  
                }   
            }else{
                Serial.println("UDP PLAYER SEND ERROR...");
            }  
        }

        //------------------



    void recive(){
        int packet = udp.parsePacket(); //데이터 받음
            if(packet){ //수신 받은거가 있으면
                int len = udp.read( udp_packet_read , 255 );
                if(len > 0){
                  
                    rx_en = true;
                    //Serial.println("recive ok");
                }
            }
    }

    private:
        int localUdpPort  = 0;
        long before_millis = 0;
        char udp_packet_read[255];

        boolean rx_en = false;


};

#endif
