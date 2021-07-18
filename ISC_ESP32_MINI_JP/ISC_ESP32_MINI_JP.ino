#include "WiFi.h"
#include "Arduino.h"
//------------------------
#include "setting.h"
#include "udp_manager.h"
#include "tft.h"
#include "pin.h"
#include "link.h"
//-----------------------


//-----------------------

int cnt;
char str[100];
udp_m udp1;
long millis_before = 0;

//----- 아래는 나의 id
boolean link_recive = false;
boolean link_udp = false;
int link_cnt = 0;

//-------- 아래는 udp 관련한거
String udp_rx_data = ""; 
String string_log = "startup";

char udp_rx_char[100];
int array[15];

int my_jump = 0;

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
    Serial.println("Connected to AP!");
}

void WiFiStationDisconnect(WiFiEvent_t event, WiFiEventInfo_t info){
    Serial.println("Connect fail.. reconnect..");
    WiFi.disconnect();
    WiFi.reconnect();  
}


void WiFiStationGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
    Serial.println(WiFi.localIP());
} 


void setup(){
  serial_init();
  pin_init();
  delay(2000);
  tft_init(); // tft lcd 활성화
  
  Serial.println("------------Program Start"); 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setAutoConnect(true); //자동 연결 설정

  WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED); //핸들러 설정
  WiFi.onEvent(WiFiStationDisconnect, SYSTEM_EVENT_STA_DISCONNECTED); //핸들러 설정
  WiFi.onEvent(WiFiStationGotIP, SYSTEM_EVENT_STA_GOT_IP);
  
  tft.fillScreen(ILI9341_BLACK); tft.setCursor(0, 50); 
  sprintf(str, "WIFI BOOT...."); tft.print(str);
  udp1.init( UDP_PORT );
    Serial.println("set udp port..");
  //WiFi.setHostname("ESP32_Moon");
  millis_before = millis();
  Serial.println("Program start....");
}

void loop(){
    if( millis_before + 1500 <= millis() ){
        display();
        display_udp_rx = "";
        millis_before = millis();
        Serial.println("update");
    }

    if( udp1.available() ){
        udp_rx_data = udp1.read_string();
        strcpy( udp_rx_char , udp_rx_data.c_str() );
        //------------------------------------------- 
        if( strstr(udp_rx_char , "#ISC") != NULL ){ //전송함
            if(UDP_READ_ENABLE){
                for(int i=0; i<5; i++){
                    SERIAL_ISC.print(udp_rx_data); 
                    Serial.print(udp_rx_data);
                    delay(10);
                }
            }
        }
        else if( strstr(udp_rx_char , "#SPGT") != NULL ){ 
            char *ptr = strtok(udp_rx_char , ",");
            int cnt = 0;

            while (ptr != NULL){
                array[ cnt ] = atoi(ptr);
                ptr = strtok(NULL, ",");
                cnt++;
            }

            if(array[1] == DEVICE_ID){
                Serial.println(udp_rx_data);
                display_udp_rx = udp_rx_data;

                if(my_jump == 0){
                    my_jump = array[2];
                }
                else if( !(my_jump == array[2]) ){
                    SERIAL_ISC.print("$JUMP"); //점프 명령어
                    Serial.println("ISC<- JUMP");
                    my_jump = array[2];
                }
                udp1.send_score(my_jump , string_log); 
                
            }
        
        
        }
        udp1.read_cach_dell(); //삭제
    }
    
    udp1.recive(); //loop문 허용함.
    command_recive(); //ATMEAG2560 과 통신함 + 처리

}


//-----------------------
void command_recive(){ //arduino _> esp32 -> android tx 계열 
    if( SERIAL_ISC.available() > 0 ){
        String rx = SERIAL_ISC.readStringUntil('\n');
        strcpy( com.rx_char , rx.c_str() );

        Serial.print("ISC->");
        Serial.println(rx);
        if( strstr(com.rx_char , "$") != NULL ){ //에러 정보
                com.mode = 0; //변환 모드 끄기
                string_log = rx + ","; //값 넣기
        }
        
        if( strstr(com.rx_char , "#ESCMD") != NULL ){ //에러 정보
            com.mode = ISC_LOG;
        }
        else if( strstr(com.rx_char , "#ESPIN") != NULL ){
            com.mode = ISC_INPUT_LOG;
        }

        else if( strstr(com.rx_char , "#EWK") != NULL ){
            com.mode = ISC_WORK;
            /*이 모드만 사용*/
        }
        
        //--------------------------------------------------
        if( com.mode ){ //변환해주세요 하는 모드라면
            char *ptr = strtok(com.rx_char , ",");
            com.cnt = 0;
            while (ptr != NULL){
                com.array[ com.cnt ] = atoi(ptr);
                ptr = strtok(NULL, ",");
                com.cnt++;
            }
        }
        //----------------------------------------------
        
        switch( com.mode ){
            //Thread[2].3: d[1,5] -> c[1,5] //이런식으로
            case ISC_LOG:  
                sprintf(com.str, "T[%d].%d: d[%d,%d] -> c[%d.%d]", 
                    com.array[1], com.array[2], com.array[3] ,com.array[4], com.array[5], com.array[6]);
           
            break;

            case ISC_WORK: /*esp가 안드로이드로 보내는 코드*/
                    sprintf(com.str, "WORK: %d.%d", com.array[1], com.array[2] );
                   if( com.array[1] ==  0 ){ // 이게 안드로이드 장비 id 0, 1, 2 ...
                        for(int i=0; i<5; i++){ // 안드로이드로 나가는 UDP신호. 5~10회 정도 보내줘야 안정적.
                            udp1.send("#AND1", com.array[2]); // com.array는 건드릴 필요 없음.
                            delay(2);
                        }
                    }

                    if( com.array[1] ==  1 ){
                        for(int i=0; i<5; i++){
                            udp1.send("#AND2", com.array[2]);
                            delay(2);
                        }
                    }

                    if( com.array[1] ==  2 ){
                        for(int i=0; i<5; i++){
                            udp1.send("#AND3", com.array[2]);
                            delay(2);
                        }
                    }


                   
                   
                    if( com.array[1] ==  D_UDPPLAYER_DSP ){
                        for(int i=0; i<5; i++){
                            udp1.send("#DSP", com.array[2]);
                            delay(2);
                        }
                    }
                    
                    else if( com.array[1] ==  D_UDPPLAYER_UPT_AP ){
                      udp1.send("#UPT_AP" , com.array[2]);
                    }

                    
                    //---------------------------------------------------
                    else if( com.array[1] ==  D_UDPPALYER_AVP ){
                        if( com.array[2] == 8 ){
                            for(int i=0; i<10; i++){    
                                udp1.send("#AVP,OUT,");
                                delay(5);
                            }
                        
                        }else{
                            for(int i=0; i<10; i++){
                                udp1.send("#AVP", com.array[2]);
                                delay(5);
                            }
                        }
                        
                    } 
                    //----------------------------------------------------
                   

            break;

        }
        com.mode = 100; //강제 삭제
        SERIAL_DEBUG.println(com.str);
        
    }
}

//------------------

void display(){
    tft.fillScreen(ILI9341_BLACK); 
    tft.setTextColor(ILI9341_YELLOW);
    tft.setTextSize(2);
    tft.setCursor(35,0);
    tft.println("ISC Controller");
    tft.setTextColor(ILI9341_WHITE);

    if( WiFi.status() != WL_CONNECTED ){
            tft.drawRect(0, 20 , 240 , 40, ILI9341_RED);
            tft.setCursor(100, 30);
            tft.println("ON WIFI");
    }else{
        tft.drawRect(0, 20 , 240 , 40, ILI9341_WHITE);
        tft.setCursor(100, 30);
        tft.println("WIFI");
    }

    //---------------------------------------
    tft.drawRect(0, 62 , 240 , 82, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.setTextColor(ILI9341_YELLOW);
    
    tft.setCursor(10,70);
    tft.print( string_log );


    tft.setCursor(10,110);
    tft.print(display_udp_rx);

    tft.setTextSize(2);
    tft.setCursor(0,180);
    tft.print("UDP_POART: ");
    tft.print( UDP_PORT );

    tft.setCursor(0,200);
    tft.print("SSID: ");
    tft.print( ssid );

    tft.setCursor(0,220);
    tft.print("PSWD: ");
    tft.print( password );

    tft.setCursor(0,240);
    tft.print("IP: ");
    tft.print( WiFi.localIP() );

    tft.setCursor(0,260);
    tft.print("RSSI: ");
    tft.print( WiFi.RSSI() );

    tft.setCursor(0,280);
    tft.print("ROOM: ");
    tft.print( DEVICE_ID );
                  
}