#ifndef setting_h         
#define setting_h

const char * ssid = "cinema";
const char * password = "123123123";
#define UDP_PORT 5959
#define WIFI_TIMEOUT 30 /*와이파이 연결 잘 안 되면 60까지 늘릴 수 있음 (시작 딜레이 초) */

#define DEVICE_ID 3
#define UDP_READ_ENABLE 1 /* 1로 바꿔야 신호가 오면 리시브가 됨 */

//#define SYNC_ROOM_ID 7 //타이머 방 번호
#define TFT_LCD_SET_ROTATION 4 // 회전각도 (2 또는 4 사용가능)


#endif
