#ifndef Link_h         
#define Link_h

#include "setting.h"
#include "pin.h"

#define ISC_LOG 1
#define ISC_WORK 3 //일처맇 해라 그런거
#define ISC_INPUT_LOG 2
#define ISC_OTHER 0 

//-----------------

#define D_UDPPLAYER_DSP 1
#define D_UDPPALYER_AVP 2
#define D_UDPPLAYER_UPT_AP 3
#define D_TIMER 30
#define D_REQUEST 100

int rx_count = 0;

struct COMM_S{
    int mode;
    int array[50];
    int cnt;
    char rx_char[100];
    char str[100];

};

COMM_S com;






#endif