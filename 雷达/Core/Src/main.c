/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
#ifdef __GNUC__ //printf remap
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart7;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_UART7_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_UART5_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*----------------------------------------------------------------------------*/
/*--printf() remap------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart3,(uint8_t *)&ch,1,0xFFFF);//阻塞方式打印,串口1
  return ch;
}
/*----------------------------------------------------------------------------*/
/*--Some global variables used in function.-----------------------------------*/
/*----------------------------------------------------------------------------*/
char radar_chripCFG[]="sensorStop flushCfg dfeDataOutputMode 1 channelCfg 15 5 0 adcCfg 2 1 adcbufCfg -1 0 1 1 1 profileCfg 0 77 429 7 57.14 0 0 70 1 256 5209 0 0 30 chirpCfg 0 0 0 0 0 0 0 1 chirpCfg 1 1 0 0 0 0 0 4 frameCfg 0 1 16 0 100 1 0 lowPower 0 0 guiMonitor -1 1 1 0 0 0 1 cfarCfg -1 0 2 8 4 3 0 15 1 cfarCfg -1 1 0 4 2 3 1 15 1 multiObjBeamForming -1 1 0.5 clutterRemoval -1 0 calibDcRangeSig -1 0 -5 8 256 extendedMaxVelocity -1 0 lvdsStreamCfg -1 0 0 0 compRangeBiasAndRxChanPhase 0.0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 measureRangeBiasAndRxChanPhase 0 1.5 0.2 CQRxSatMonitor 0 3 5 121 0 CQSigImgMonitor 0 127 4 analogMonitor 0 0 aoaFovCfg -1 -90 90 -90 90 cfarFovCfg -1 0 0 8.92 cfarFovCfg -1 1 -1 1.00 calibData 0 0 0 sensorStart";
const uint8_t radar_startComd[]="sensorStart 0\n";
const uint8_t radar_stopComd[]="sensorStop\n";
int radar_rxDataFlag=0;
int radar_rxDataCnt=0;
uint8_t radar_rxDataBuf[2000];	//store the data received every frame
uint8_t radar_rxDataTemp[8];	//data in processing
uint8_t radar_rxDataBufTemp[1];
/*----------------------------------------------------------------------------*/
/*--These global variables are results from every frame precessed.------------*/
/*----------------------------------------------------------------------------*/
int radar_pointDetacted[20][6]; //point detacted of x, y, z, v, snr, noise
int radar_rangeProfile[512];    //range profile in dB
int radar_temperature[10];      //temperature of Rx0, Rx1, Rx2, Rx3, Tx0, Tx1, Tx2, Pm, Dig0, Dig1 in Deg C
int radar_timeInfo[8];          //time information of InterFrameProcessingTime(usec), TransmitOutputTime(usec), InterFrameProcessingMargin(usec),
                                //RunningTime(day), RunningTime(h), RunningTime(min), RunningTime(s), RunningTime(ms)
struct radar_packHeader {
    int firmwareVersion;
    int totalPacketNumBytes;
    int platform;
    int frameNumber;
    int timeCpuCycles;
    int numDetObj;
    int numTlv;
    int subFrameNumber;
} radar_packData;
/*----------------------------------------------------------------------------*/
/*--These functions is used to receive data packets of unknown length---------*/
/*--using the serial port. Two bytes separated by more than 10ms will be------*/
/*--considered as two packets.------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim==(&htim1)){
    	HAL_GPIO_TogglePin(DS0_GPIO_Port,DS0_Pin);//
	    radar_rxDataFlag=1;
	    HAL_TIM_Base_Stop_IT(&htim1);
    }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==UART5){
		__HAL_TIM_SET_COUNTER(&htim1,0);
		if(0 == radar_rxDataCnt){
		    __HAL_TIM_CLEAR_FLAG(&htim1,TIM_FLAG_UPDATE);
		    HAL_TIM_Base_Start_IT(&htim1);
		}
		radar_rxDataBuf[radar_rxDataCnt]=radar_rxDataBufTemp[0];
		radar_rxDataCnt++;
		HAL_UART_Receive_IT(&huart5,(uint8_t *)radar_rxDataBufTemp,1);
	}
}
/*----------------------------------------------------------------------------*/
/*--These functions are used to unpack and process datapack-------------------*/
/*----------------------------------------------------------------------------*/
int radar_getUint32(uint8_t data[4]){
	return(data[0]+data[1]*256+data[2]*65536+data[3]*16777216);
}
int radar_getUint16(uint8_t data[2]){
	return(data[0]+data[1]*256);
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
void radar_dataUnpack(){	//main data unpack function
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
                    radar_rxDataBufCut(4); radar_pointDetacted[j][0]=radar_getUint32(radar_rxDataTemp); //x
                    radar_rxDataBufCut(4); radar_pointDetacted[j][1]=radar_getUint32(radar_rxDataTemp); //y
                    radar_rxDataBufCut(4); radar_pointDetacted[j][2]=radar_getUint32(radar_rxDataTemp); //z
                    radar_rxDataBufCut(4); radar_pointDetacted[j][3]=radar_getUint32(radar_rxDataTemp); //v
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
void radar_cfgTransmit(){
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
void radarStart(){
	HAL_UART_Transmit(&huart7,radar_startComd,sizeof(radar_startComd)-1,0xffff);
}
void radarStop(){
	HAL_UART_Transmit(&huart7,radar_stopComd,sizeof(radar_stopComd)-1,0xffff);
}
/*----------------------------------------------------------------------------*/
/*--For dubug-----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void radar_debug(){
	printf("totalPacketNumBytes = %d\n", radar_packData.totalPacketNumBytes);
	printf("frameNumber         = %d\n", radar_packData.frameNumber);
	printf("timeCpuCycles       = %d\n", radar_packData.timeCpuCycles);
	printf("numDetObj           = %d\n", radar_packData.numDetObj);
	printf("numTlv              = %d\n", radar_packData.numTlv);
	printf("subFrameNumber      = %d\n", radar_packData.subFrameNumber);
	printf("%d day %d : %d : %d . %d\n",radar_timeInfo[3],radar_timeInfo[4],radar_timeInfo[5],radar_timeInfo[6],radar_timeInfo[7]);
}
/*----------------------------------------------------------------------------*/

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_UART7_Init();
  MX_USART3_UART_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */

  HAL_GPIO_WritePin(radarRST_GPIO_Port, radarRST_Pin, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(radarRST_GPIO_Port, radarRST_Pin, GPIO_PIN_SET);
  HAL_Delay(500);														//radar initialization

  radar_cfgTransmit();													//configuration cfg transmission done
  HAL_UART_Receive_IT(&huart5,radar_rxDataBufTemp,2000);				//Begin to receive data

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(radar_rxDataFlag){
	  	  radar_dataUnpack();
	  	  radar_debug();
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 9999;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 107;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 921600;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
  huart5.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  huart5.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief UART7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART7_Init(void)
{

  /* USER CODE BEGIN UART7_Init 0 */

  /* USER CODE END UART7_Init 0 */

  /* USER CODE BEGIN UART7_Init 1 */

  /* USER CODE END UART7_Init 1 */
  huart7.Instance = UART7;
  huart7.Init.BaudRate = 115200;
  huart7.Init.WordLength = UART_WORDLENGTH_8B;
  huart7.Init.StopBits = UART_STOPBITS_1;
  huart7.Init.Parity = UART_PARITY_NONE;
  huart7.Init.Mode = UART_MODE_TX;
  huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart7.Init.OverSampling = UART_OVERSAMPLING_16;
  huart7.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart7.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart7) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART7_Init 2 */

  /* USER CODE END UART7_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 921600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
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
  HAL_GPIO_WritePin(DS0_GPIO_Port, DS0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : radarRST_Pin */
  GPIO_InitStruct.Pin = radarRST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(radarRST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DS0_Pin */
  GPIO_InitStruct.Pin = DS0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DS0_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
