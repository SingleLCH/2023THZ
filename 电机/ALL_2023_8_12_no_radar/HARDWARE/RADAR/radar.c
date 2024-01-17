#include "radar.h"
#include "math.h"
#include "sys.h"
#include "led.h"

#include "stepper_motor.h"

char radar_chripCFG[]="sensorStop flushCfg dfeDataOutputMode 1 channelCfg 15 5 0 adcCfg 2 1 adcbufCfg -1 0 1 1 1 profileCfg 0 77 287 7 200 0 0 20 1 384 2000 0 0 30 chirpCfg 0 0 0 0 0 0 0 1 chirpCfg 1 1 0 0 0 0 0 4 frameCfg 0 1 16 0 * 1 0 lowPower 0 0 guiMonitor -1 1 1 0 0 0 1 cfarCfg -1 0 2 8 4 3 0 15 1 cfarCfg -1 1 0 4 2 3 1 15 1 multiObjBeamForming -1 1 0.5 clutterRemoval -1 0 calibDcRangeSig -1 0 -5 8 256 extendedMaxVelocity -1 0 lvdsStreamCfg -1 0 0 0 compRangeBiasAndRxChanPhase 0.0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 measureRangeBiasAndRxChanPhase 0 1.5 0.2 CQRxSatMonitor 0 3 19 125 0 CQSigImgMonitor 0 127 6 analogMonitor 0 0 aoaFovCfg -1 -90 90 -90 90 cfarFovCfg -1 0 0 12.00 cfarFovCfg -1 1 -1 1.00 calibData 0 0 0 sensorStart        ";
const uint8_t radar_startComd[]="sensorStart 0\n";
const uint8_t radar_stopComd[]="sensorStop\n";
int radar_rxDataFlag=0;
int radar_rxDataCnt=0;
uint8_t radar_rxDataBuf[4000];	//store the data received every frame
uint8_t radar_rxDataTemp[8];	//data in processing
uint8_t radar_rxDataBufTemp[1];
/*----------------------------------------------------------------------------*/
/*--These global variables are results from every frame precessed.------------*/
/*----------------------------------------------------------------------------*/
float radar_pointDetacted[20][6]; //point detacted of x, y, z, v, snr, noise
float radar_rangeProfile[512];    //range profile in dB
int radar_temperature[10];      //temperature of Rx0, Rx1, Rx2, Rx3, Tx0, Tx1, Tx2, Pm, Dig0, Dig1 in Deg C
int radar_timeInfo[8];          //time information of InterFrameProcessingTime(usec), TransmitOutputTime(usec), InterFrameProcessingMargin(usec),
                                //RunningTime(day), RunningTime(h), RunningTime(min), RunningTime(s), RunningTime(ms)
								
struct radar_packHeader radar_packData;
								
								
/*----------------------------------------------------------------------------*/
/*--These functions are used to unpack and process datapack-------------------*/
/*----------------------------------------------------------------------------*/
int radar_getUint32(uint8_t data[4]){
	return(data[0]+data[1]*256+data[2]*65536+data[3]*16777216);
}
int radar_getUint16(uint8_t data[2]){
	return(data[0]+data[1]*256);
}
float getFloat32(uint8_t data[]){
    unsigned long temp=0;
    temp|=data[3];
    temp=temp << 8;
    temp |= data[2];
    temp = temp << 8;
    temp |= data[1];
    temp = temp << 8;
    temp |= data[0];
    float float_num = *(float*)&temp;
    return float_num;
}
void radar_getTime(uint8_t data[4]){
    int time=radar_getUint32(data);
    radar_timeInfo[3]=time/86400000; time%=86400000; 	//d
    radar_timeInfo[4]=time/3600000; time%=3600000; 		//h
    radar_timeInfo[5]=time/60000; time%=60000; 			//m
    radar_timeInfo[6]=time/1000; time%=1000; 			//s
    radar_timeInfo[7]=time; 							//ms
}
void radar_rxDataBufCut(int x){
    memcpy(radar_rxDataTemp,radar_rxDataBuf,x);
    radar_rxDataCnt-=x;
    for(int i=0;i<radar_rxDataCnt;i++){
        radar_rxDataBuf[i]=radar_rxDataBuf[i+x];
    }
}
int radar_checkMagicPattern(){
	int found=1;
	uint8_t MagicPattern[8]={2,1,4,3,6,5,8,7};
	for(int i=0;i<8;i++){
		if(radar_rxDataBuf[i]!=MagicPattern[i]) found=0;
	}
    radar_rxDataBufCut(8);
	return (found);
}
void radar_dataUnpack(void){	//main data unpack function
    int radar_tlvType;
    int radar_tlvLength;
    if(radar_checkMagicPattern()){
        radar_rxDataBufCut(4); //skip firmwareVersion
        radar_rxDataBufCut(4); radar_packData.totalPacketNumBytes   =radar_getUint32(radar_rxDataTemp);
        radar_rxDataBufCut(4); //skip platform
        radar_rxDataBufCut(4); radar_packData.frameNumber           =radar_getUint32(radar_rxDataTemp);
        radar_rxDataBufCut(4); radar_packData.timeCpuCycles         =radar_getUint32(radar_rxDataTemp);
        radar_rxDataBufCut(4); radar_packData.numDetObj             =radar_getUint32(radar_rxDataTemp);
        radar_rxDataBufCut(4); radar_packData.numTlv                =radar_getUint32(radar_rxDataTemp);
        radar_rxDataBufCut(4); //skip subFrameNumber
        for(int i=0;i<radar_packData.numTlv;i++){
            radar_rxDataBufCut(4); radar_tlvType    =radar_getUint32(radar_rxDataTemp);
            radar_rxDataBufCut(4); radar_tlvLength  =radar_getUint32(radar_rxDataTemp);
            if(radar_tlvType == 1){ //point detacted in x,y,z,v
                for(int j=0;j<radar_tlvLength/16;j++){
                    radar_rxDataBufCut(4); radar_pointDetacted[j][0]=getFloat32(radar_rxDataTemp); //x
                    radar_rxDataBufCut(4); radar_pointDetacted[j][1]=getFloat32(radar_rxDataTemp); //y
                    radar_rxDataBufCut(4); radar_pointDetacted[j][2]=getFloat32(radar_rxDataTemp); //z
                    radar_rxDataBufCut(4); radar_pointDetacted[j][3]=getFloat32(radar_rxDataTemp); //v
                }
            }
            else if(radar_tlvType == 7){ //point detacted in snr,noise
                for(int j=0;j<radar_tlvLength/4;j++){
                    radar_rxDataBufCut(2); radar_pointDetacted[j][4]=radar_getUint16(radar_rxDataTemp); //snr
                    radar_rxDataBufCut(2); radar_pointDetacted[j][5]=radar_getUint16(radar_rxDataTemp); //noise
                }
            }
            else if(radar_tlvType == 2){ //range profile
                for(int j=0;j<radar_tlvLength/2;j++){
                    radar_rxDataBufCut(2); radar_rangeProfile[j]=radar_getUint16(radar_rxDataTemp); //in dB
                }
            }
            else if(radar_tlvType == 6){ //time information
                for(int j=0;j<3;j++){
                    radar_rxDataBufCut(4); radar_timeInfo[j]=radar_getUint32(radar_rxDataTemp);
                }
                radar_rxDataBufCut(12); //jump 12 useless bytes
            }
            else if(radar_tlvType == 9){ //temperature of Rx0,Rx1,Rx2,Rx3,Tx0,Tx1,Tx2,Pm,Dig0,Dig1 in Deg C
                radar_rxDataBufCut(4);
                radar_rxDataBufCut(4);
                radar_getTime(radar_rxDataTemp);
                for(int j=0;j<10;j++){
                    radar_rxDataBufCut(2); radar_temperature[j]=radar_getUint16(radar_rxDataTemp);
                }
            }
        }
    }
    for(int i=0;i<radar_rxDataCnt;i++) radar_rxDataBuf[i] = 0;
    radar_rxDataCnt=0;
    radar_rxDataFlag=0;
}	

/*----------------------------------------------------------------------------*/
/*--This functions is used to send configuration cfg.-------------------------*/
/*----------------------------------------------------------------------------*/
void str_replace(char *str_src, int n, char * str_copy)
{
	int lenofstr;
	char *tmp;
	lenofstr=strlen(str_copy);
	if(lenofstr < n){
		tmp=str_src+n;
		while(*tmp){
			*(tmp-(n-lenofstr)) = *tmp;
			tmp++;
		}
		*(tmp-(n-lenofstr)) = *tmp;
	}
	else if(lenofstr > n){
		tmp = str_src;
		while(*tmp) tmp++;
		while( tmp>=(str_src+n) ){
			*(tmp+(lenofstr-n)) = *tmp;
			tmp--;
		}
	}
	strncpy(str_src, str_copy, lenofstr);
}

void radar_cfgTransmit(int freq){
//	if(freq>30) freq=30;
	if(freq<=0) freq=1;
	char strFind[10] = "*";
	char strCopy[10];
	char *p;
	float period=1000.0/freq;
	sprintf(strCopy,"%.3f\n",period);
	strCopy[strlen(strCopy)-1]='\0';
	p=strstr(radar_chripCFG, strFind);
	while(p){
		str_replace(p,strlen(strFind),strCopy);
		p=p+strlen(strCopy);
		p=strstr(p,strFind);
	}
	int radar_flagL=0;
	uint8_t radar_commandBuf[80];
	for(int radar_flagH=0;radar_flagH<sizeof(radar_chripCFG);radar_flagH++){
	    if((radar_chripCFG[radar_flagH]==32&&radar_chripCFG[radar_flagH+1]>=65)||radar_flagH==sizeof(radar_chripCFG)-1){
	        for(int i=0;i<radar_flagH-radar_flagL;i++){
	            radar_commandBuf[i]=radar_chripCFG[radar_flagL+i];
	        }
	        radar_commandBuf[radar_flagH-radar_flagL]=10;
	        HAL_UART_Transmit(&huart7,radar_commandBuf,radar_flagH-radar_flagL+1,0xffff);
	        radar_flagL=radar_flagH+1;
	        HAL_Delay(10);
	    }
	}
}
/*----------------------------------------------------------------------------*/
/*--These functions are used to start or stop radar.--------------------------*/
/*----------------------------------------------------------------------------*/
void radarStart(void){
	HAL_UART_Transmit(&huart7,(uint8_t *)radar_startComd,sizeof(radar_startComd)-1,0xffff);
}
void radarStop(void){
	HAL_UART_Transmit(&huart7,(uint8_t *)radar_stopComd,sizeof(radar_stopComd)-1,0xffff);
}

/*----------------------------------------------------------------------------*/
/*--For dubug-----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
//void radar_debug(void){
//	//printf("totalPacketNumBytes = %d\n", radar_packData.totalPacketNumBytes);
//	//printf("frameNumber         = %d\n", radar_packData.frameNumber);
//	//printf("timeCpuCycles       = %d\n", radar_packData.timeCpuCycles);
//	//printf("numDetObj           = %d\n", radar_packData.numDetObj);
//	//printf("numTlv              = %d\n", radar_packData.numTlv);
//	//printf("subFrameNumber      = %d\n", radar_packData.subFrameNumber);
//	//printf("%d day %d : %d : %d . %d\n",radar_timeInfo[3],radar_timeInfo[4],radar_timeInfo[5],radar_timeInfo[6],radar_timeInfo[7]);
//	//printf("a %d %d %d\n", radar_packData.totalPacketNumBytes,radar_packData.frameNumber,radar_packData.numDetObj);
//	//printf("%d\r\n", radar_packData.numDetObj);
//	for(int i=0;i<radar_packData.numDetObj;i++){
//		if(radar_pointDetacted[i][1]>0.38&&radar_pointDetacted[i][1]<0.44&&radar_rangeProfile[(int)round(radar_pointDetacted[i][1]/0.02929688)]>=4400){ 
//			printf("b%.0f %.0f %.0f\r\n", g_stepper_x.pos_mm, g_stepper_y.pos_mm, radar_rangeProfile[(int)round(radar_pointDetacted[i][1]/0.02929688)]);
////			printf("%fb\r\n", radar_rangeProfile[(int)round(radar_pointDetacted[i][1]/0.02929688)]);
//		}
//	}
//}
void radar_debug(void){
//	printf("totalPacketNumBytes = %d\n", radar_packData.totalPacketNumBytes);
//	printf("frameNumber         = %d\n", radar_packData.frameNumber);
//	printf("timeCpuCycles       = %d\n", radar_packData.timeCpuCycles);
//	printf("numDetObj           = %d\n", radar_packData.numDetObj);
//	printf("numTlv              = %d\n", radar_packData.numTlv);
//	printf("subFrameNumber      = %d\n", radar_packData.subFrameNumber);
//	printf("%d day %d : %d : %d . %d\n",radar_timeInfo[3],radar_timeInfo[4],radar_timeInfo[5],radar_timeInfo[6],radar_timeInfo[7]);
//	printf("a %d %d %d\n", radar_packData.totalPacketNumBytes,radar_packData.frameNumber,radar_packData.numDetObj);
//	printf("%d\r\n", radar_packData.numDetObj);
//	
//	for(int i=0;i<radar_packData.numDetObj;i++){
//		if(radar_pointDetacted[i][1]>0.37&&radar_pointDetacted[i][1]<0.42&&radar_rangeProfile[(int)round(radar_pointDetacted[i][1]/0.02929688)]>=4500){ 
//			printf("b%.0f %.0f %.0f\r\n", g_stepper_x.pos_mm, g_stepper_y.pos_mm, radar_rangeProfile[(int)round(radar_pointDetacted[i][1]/0.02929688)]);
//		}
//	for(int i=25;i<=60;i=i+3){
//        printf("%.0f  ",radar_rangeProfile[(int)round(i/2.929688)]);
//	}
//	printf("***************************\r\n");
//    int z=maxFind();
//    float a=0.1;
//    float temp[7];
//    temp[0]=radar_rangeProfile[z-3];
//    temp[1]=radar_rangeProfile[z-2];
//    temp[2]=radar_rangeProfile[z-1];
//    temp[3]=radar_rangeProfile[z];
//    temp[4]=radar_rangeProfile[z+1];
//    temp[5]=radar_rangeProfile[z+2];
//    temp[6]=radar_rangeProfile[z+3];
//    radar_rangeProfile[2*z-3]-=temp[0]*a*0.25;
//    radar_rangeProfile[2*z-2]-=temp[1]*a*0.5;
//    radar_rangeProfile[2*z-1]-=temp[2]*a*0.8;
//    radar_rangeProfile[2*z]-=temp[3]*a;
//    radar_rangeProfile[2*z+1]-=temp[4]*a*0.8;
//    radar_rangeProfile[2*z+2]-=temp[5]*a*0.5;
//    radar_rangeProfile[2*z+3]-=temp[6]*a*0.25;
//	printf("b%.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f\r\n",
	printf("b%.0f %.0f %.0f\r\n",
	g_stepper_x.pos_mm,
	g_stepper_y.pos_mm,
//    radar_rangeProfile[8],
//    radar_rangeProfile[9],
//    radar_rangeProfile[10],
//    radar_rangeProfile[11],
//    radar_rangeProfile[12],
//    radar_rangeProfile[13],
//    radar_rangeProfile[14],
//    radar_rangeProfile[15],
//    radar_rangeProfile[16],
//    radar_rangeProfile[17],
//    radar_rangeProfile[18],
//    radar_rangeProfile[19],
//    radar_rangeProfile[20],
//    radar_rangeProfile[21],
//    radar_rangeProfile[22],
//    radar_rangeProfile[23],
//    radar_rangeProfile[24],
//    radar_rangeProfile[25]
    radar_rangeProfile[17]
	);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void RADAR_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(radarRST_GPIO_Port, radarRST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin : radarRST_Pin */
  GPIO_InitStruct.Pin = radarRST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(radarRST_GPIO_Port, &GPIO_InitStruct);

//  /*Configure GPIO pin : DS0_Pin */
//  GPIO_InitStruct.Pin = DS0_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(DS0_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

								
