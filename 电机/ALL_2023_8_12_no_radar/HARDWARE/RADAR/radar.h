#include "stdint.h"
#include "string.h"
#include "usart.h"

#ifndef __RADAR_H
#define __RADAR_H

#define radarRST_GPIO_Port 		GPIOB
#define radarRST_Pin 			GPIO_PIN_14

//extern const uint8_t radar_stopComd[];
extern uint8_t radar_rxDataBuf[4000];	//store the data received every frame
extern int radar_rxDataFlag;
extern uint8_t radar_rxDataBufTemp[1];
extern int radar_rxDataCnt;
extern struct radar_packHeader radar_packData;
struct radar_packHeader {
    int firmwareVersion;
    int totalPacketNumBytes;
    int platform;
    int frameNumber;
    int timeCpuCycles;
    int numDetObj;
    int numTlv;
    int subFrameNumber;
};

void RADAR_GPIO_Init(void);
void radar_dataUnpack(void);
void radar_cfgTransmit(int freq);
void radar_debug(void);
void radarStart(void);
void radarStop(void);


#endif
