#include "timer.h"
#include "lvgl.h"

/********************/
#include "timer.h"
#include "led.h"
#include "stepper_motor.h"
#include "usart.h"
#include "protocol.h"
#include "radar.h"
/********************/
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//��ʱ���ж���������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
/******************************************************************************************/
/* ��ʱ�����þ�� ���� */

/* ͨ�ö�ʱ��PWM */
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef GTIM3_Handler;           /* ��ʱ��x��� */
TIM_HandleTypeDef GTIM4_Handler;           /* ��ʱ��x��� */
TIM_HandleTypeDef GTIM8_Handler;           /* ��ʱ��x��� */
TIM_HandleTypeDef GTIM5_Handler;      //��ʱ����� 
TIM_OC_InitTypeDef GTIMx_CHy_Handler;   /* ��ʱ��x���y��� */ 
volatile uint8_t g_run_flag_1 = 0, g_run_flag_2 = 0;
/******************************************************************************************/

TIM_HandleTypeDef TIM6_Handler;      //��ʱ����� 
volatile uint32_t time_ms = 0;


//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
void TIM6_Init(u16 arr,u16 psc)
{  
    TIM6_Handler.Instance=TIM6;                          //ͨ�ö�ʱ��6
    TIM6_Handler.Init.Prescaler=psc;                     //��Ƶ
    TIM6_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM6_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM6_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM6_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM6_Handler); //ʹ�ܶ�ʱ��6�Ͷ�ʱ��6�����жϣ�TIM_IT_UPDATE    
}

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��1��2��3��4 PWM�����ʼ��
 * @note
 *              ͨ�ö�ʱ����ʱ������APB1, ��PCLK1 = 54MHz, ��������PPRE1=4, ���
 *              ͨ�ö�ʱ��ʱ�� = 54MHz*2=108MHz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM1_Init(void)
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
//  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
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

void TIM8_PWM_Init(uint16_t arr, uint16_t psc)
{
    TIM8_CLK_ENABLE();                             /* TIM8 ʱ��ʹ�� */
    
    GTIM8_Handler.Instance = GTIM_TIM8;                    /* ��ʱ��x */
    GTIM8_Handler.Init.Prescaler = psc;                        /* ��ʱ����Ƶ */
    GTIM8_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       /* ���ϼ���ģʽ */
    GTIM8_Handler.Init.Period = arr;                           /* �Զ���װ��ֵ */
    GTIM8_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;   /* ��Ƶ���� */
	TIM8->CR1 |= TIM_CR1_ARPE; /* ʹ�ܶ�ʱ��ARR�Ĵ�����Ԥװ�� */
//    GTIM8_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /*ʹ��TIMx_ARR���л���*/
//    GTIMx_Handler.Init.RepetitionCounter = 0;                  /* ��ʼʱ������*/�ڸ߼���ʱ����ʹ��
    HAL_TIM_PWM_Init(&GTIM8_Handler);                          /* ��ʼ��PWM */
	
    
    GTIMx_CHy_Handler.OCMode = TIM_OCMODE_PWM1;             /* ģʽѡ��PWM1 */
    GTIMx_CHy_Handler.Pulse = (arr+1)/2;						/*ռ�ձ�Ϊ50%*/
    GTIMx_CHy_Handler.OCPolarity = TIM_OCPOLARITY_HIGH;    		/*����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ*/
    GTIMx_CHy_Handler.OCNPolarity = TIM_OCNPOLARITY_HIGH;		/*���û���ͨ������ļ���*/
    GTIMx_CHy_Handler.OCFastMode = TIM_OCFAST_DISABLE;			/*����ģʽ����*/
    GTIMx_CHy_Handler.OCIdleState = TIM_OCIDLESTATE_RESET; 		/*���е�ƽ*/
    GTIMx_CHy_Handler.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(&GTIM8_Handler, &GTIMx_CHy_Handler, GTIM_TIM8_PWM_CH3); /* ����TIM4ͨ��3 */
	HAL_TIM_PWM_ConfigChannel(&GTIM8_Handler, &GTIMx_CHy_Handler, GTIM_TIM8_PWM_CH4); /* ����TIM4ͨ��4 */
	
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH2); /* ����TIMxͨ��y */   
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH3); /* ����TIMxͨ��y */
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH4); /* ����TIMxͨ��y */
  
    
}
 
 
 
 
void GTIM_TIM3_PWM_Init(uint16_t arr, uint16_t psc)
{
    TIM3_CLK_ENABLE();                             /* TIM3 ʱ��ʹ�� */
    
    GTIM3_Handler.Instance = GTIM_TIM3;                    /* ��ʱ��x */
    GTIM3_Handler.Init.Prescaler = psc;                        /* ��ʱ����Ƶ */
    GTIM3_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       /* ���ϼ���ģʽ */
    GTIM3_Handler.Init.Period = arr;                           /* �Զ���װ��ֵ */
    GTIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;   /* ��Ƶ���� */
//    GTIM3_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /*ʹ��TIMx_ARR���л���*/
//    GTIMx_Handler.Init.RepetitionCounter = 0;                  /* ��ʼʱ������*/�ڸ߼���ʱ����ʹ��
    HAL_TIM_PWM_Init(&GTIM3_Handler);                          /* ��ʼ��PWM */
	
    
    GTIMx_CHy_Handler.OCMode = TIM_OCMODE_PWM1;             /* ģʽѡ��PWM1 */
    GTIMx_CHy_Handler.Pulse = (arr+1)/2;						/*ռ�ձ�Ϊ50%*/
    GTIMx_CHy_Handler.OCPolarity = TIM_OCPOLARITY_HIGH;    		/*����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ*/
    GTIMx_CHy_Handler.OCNPolarity = TIM_OCNPOLARITY_HIGH;		/*���û���ͨ������ļ���*/
    GTIMx_CHy_Handler.OCFastMode = TIM_OCFAST_DISABLE;			/*����ģʽ����*/
    GTIMx_CHy_Handler.OCIdleState = TIM_OCIDLESTATE_RESET; 		/*���е�ƽ*/
    GTIMx_CHy_Handler.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(&GTIM3_Handler, &GTIMx_CHy_Handler, GTIM_TIM3_PWM_CH1); /* ����TIM4ͨ��1 */
	
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH2); /* ����TIMxͨ��y */   
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH3); /* ����TIMxͨ��y */
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH4); /* ����TIMxͨ��y */
  
    
}


void GTIM_TIM4_PWM_Init(uint16_t arr, uint16_t psc)
{
    TIM4_CLK_ENABLE();                             /* TIM4 ʱ��ʹ�� */
    
    GTIM4_Handler.Instance = GTIM_TIM4;                    /* ��ʱ��x */
    GTIM4_Handler.Init.Prescaler = psc;                        /* ��ʱ����Ƶ */
    GTIM4_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       /* ���ϼ���ģʽ */
    GTIM4_Handler.Init.Period = arr;                           /* �Զ���װ��ֵ */
    GTIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;   /* ��Ƶ���� */
//    GTIM4_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /*ʹ��TIMx_ARR���л���*/
//    GTIMx_Handler.Init.RepetitionCounter = 0;                  /* ��ʼʱ������*/�ڸ߼���ʱ����ʹ��
    HAL_TIM_PWM_Init(&GTIM4_Handler);                          /* ��ʼ��PWM */
	
    
    GTIMx_CHy_Handler.OCMode = TIM_OCMODE_PWM1;             /* ģʽѡ��PWM1 */
    GTIMx_CHy_Handler.Pulse = (arr+1)/2;						/*ռ�ձ�Ϊ50%*/
    GTIMx_CHy_Handler.OCPolarity = TIM_OCPOLARITY_HIGH;    		/*����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ*/
    GTIMx_CHy_Handler.OCNPolarity = TIM_OCNPOLARITY_HIGH;		/*���û���ͨ������ļ���*/
    GTIMx_CHy_Handler.OCFastMode = TIM_OCFAST_DISABLE;			/*����ģʽ����*/
    GTIMx_CHy_Handler.OCIdleState = TIM_OCIDLESTATE_RESET; 		/*���е�ƽ*/
    GTIMx_CHy_Handler.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(&GTIM4_Handler, &GTIMx_CHy_Handler, GTIM_TIM4_PWM_CH2); /* ����TIM4ͨ��1 */
	
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH2); /* ����TIMxͨ��y */   
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH3); /* ����TIMxͨ��y */
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH4); /* ����TIMxͨ��y */
  
    
}


//ͨ�ö�ʱ��5�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��5!(��ʱ��5����APB1�ϣ�ʱ��ΪHCLK/2)
void TIM5_Init(uint16_t arr,uint16_t psc)
{  
    GTIM5_Handler.Instance=TIM5;                          //ͨ�ö�ʱ��5
    GTIM5_Handler.Init.Prescaler=psc;                     //��Ƶ
    GTIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    GTIM5_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    GTIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&GTIM5_Handler);
    
    HAL_TIM_Base_Start_IT(&GTIM5_Handler); //ʹ�ܶ�ʱ��5�Ͷ�ʱ��5�����жϣ�TIM_IT_UPDATE    
}

/**
 * @brief       ��ʱ���ײ�������ʱ��ʹ�ܣ���������
                �˺����ᱻHAL_TIM_PWM_Init()����
 * @param       htim:��ʱ�����
 * @retval      ��
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	

    if (htim->Instance == GTIM_TIM3)
    {
        GPIO_InitTypeDef GPIO_InitStruct;
        TIM3_CLK_ENABLE();                             				/* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM3_PWM_CH1_GPIO_CLK_ENABLE();                        /* PWMͨ�����IOʱ��ʹ�� */

        GPIO_InitStruct.Pin = GTIM_TIM3_PWM_CH1_GPIO_PIN;          /* ͨ��y��GPIO�� */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                    /* ����������� */
        GPIO_InitStruct.Pull = GPIO_PULLUP;                        /* ���� */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;     				/* �˿ڸ��� */
        HAL_GPIO_Init(GTIM_TIM3_PWM_CH1_GPIO_PORT, &GPIO_InitStruct);
        
//        GPIO_InitStruct.Pin = GTIM_TIMX_PWM_CH2_GPIO_PIN;          /* ͨ��y��GPIO�� */
//        HAL_GPIO_Init(GTIM_TIMX_PWM_CH2_GPIO_PORT, &GPIO_InitStruct);
        
        HAL_NVIC_SetPriority(GTIM_TIM3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(GTIM_TIM3_IRQn);
    }
    if (htim->Instance == GTIM_TIM4)
    {
        GPIO_InitTypeDef GPIO_InitStruct;
        TIM4_CLK_ENABLE();                             				/* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM4_PWM_CH2_GPIO_CLK_ENABLE();                        /* PWMͨ�����IOʱ��ʹ�� */

        GPIO_InitStruct.Pin = GTIM_TIM4_PWM_CH2_GPIO_PIN;          /* ͨ��y��GPIO�� */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                    /* ����������� */
        GPIO_InitStruct.Pull = GPIO_PULLUP;                        /* ���� */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;     				/* �˿ڸ��� */
        HAL_GPIO_Init(GTIM_TIM4_PWM_CH2_GPIO_PORT, &GPIO_InitStruct);
        
//        GPIO_InitStruct.Pin = GTIM_TIMX_PWM_CH2_GPIO_PIN;          /* ͨ��y��GPIO�� */
//        HAL_GPIO_Init(GTIM_TIMX_PWM_CH2_GPIO_PORT, &GPIO_InitStruct);
        
        HAL_NVIC_SetPriority(GTIM_TIM4_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(GTIM_TIM4_IRQn);
    }
	if (htim->Instance == GTIM_TIM8)
    {
        GPIO_InitTypeDef GPIO_InitStruct;
        TIM8_CLK_ENABLE();                             				/* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM8_PWM_CH3_GPIO_CLK_ENABLE();                        /* PWMͨ�����IOʱ��ʹ�� */
		GTIM_TIM8_PWM_CH4_GPIO_CLK_ENABLE();                        /* PWMͨ�����IOʱ��ʹ�� */

        GPIO_InitStruct.Pin = GTIM_TIM8_PWM_CH3_GPIO_PIN;          /* ͨ��y��GPIO�� */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                    /* ����������� */
        GPIO_InitStruct.Pull = GPIO_PULLUP;                        /* ���� */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;     				/* �˿ڸ��� */
        HAL_GPIO_Init(GTIM_TIM8_PWM_CH3_GPIO_PORT, &GPIO_InitStruct);
        
		GPIO_InitStruct.Pin = GTIM_TIM8_PWM_CH4_GPIO_PIN;          /* ͨ��y��GPIO�� */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                    /* ����������� */
        GPIO_InitStruct.Pull = GPIO_PULLUP;                        /* ���� */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;     				/* �˿ڸ��� */
        HAL_GPIO_Init(GTIM_TIM8_PWM_CH4_GPIO_PORT, &GPIO_InitStruct);
//        GPIO_InitStruct.Pin = GTIM_TIMX_PWM_CH2_GPIO_PIN;          /* ͨ��y��GPIO�� */
//        HAL_GPIO_Init(GTIM_TIMX_PWM_CH2_GPIO_PORT, &GPIO_InitStruct);
        
        HAL_NVIC_SetPriority(GTIM_TIM8_IRQn, 1, 1);
        HAL_NVIC_EnableIRQ(GTIM_TIM8_IRQn);
    }

}

//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM1)
	{
	/* USER CODE BEGIN TIM1_MspInit 0 */

	/* USER CODE END TIM1_MspInit 0 */
	/* Peripheral clock enable */
	__HAL_RCC_TIM1_CLK_ENABLE();
	/* TIM1 interrupt Init */
	HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	/* USER CODE BEGIN TIM1_MspInit 1 */

	/* USER CODE END TIM1_MspInit 1 */
	}
	if(htim->Instance==TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();            //ʹ��TIM5ʱ��
		HAL_NVIC_SetPriority(TIM5_IRQn,1,1);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM5_IRQn);          //����ITM5�ж�   
	}  
	
	if(htim->Instance==TIM6)
	{
		__HAL_RCC_TIM6_CLK_ENABLE();            //ʹ��TIM6ʱ��
		HAL_NVIC_SetPriority(TIM6_DAC_IRQn,0,0);    //�����ж����ȼ�����ռ���ȼ�0�������ȼ�0
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);          //����ITM6�ж�   
	}  
}

/**
 * @brief       ͨ�ö�ʱ�������жϷ�����
 * @param       ��
 * @retval      ��
 */
void GTIM_TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&GTIM3_Handler);            /* ��ʱ�����ô����� */
}

void GTIM_TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&GTIM4_Handler);            /* ��ʱ�����ô����� */
}

void GTIM_TIM8_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&GTIM8_Handler);            /* ��ʱ�����ô����� */
}

//��ʱ��5�жϷ�����
void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&GTIM5_Handler);
}

//��ʱ��6�жϷ�����
void TIM6_DAC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM6_Handler);
}

void TIM1_UP_TIM10_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}

/* �жϻص����� */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM1){
    	HAL_GPIO_TogglePin(LED0_GPIO_PORT,LED0_GPIO_PIN);//
	    radar_rxDataFlag=1;
	    HAL_TIM_Base_Stop_IT(&htim1);
    }
    if(htim->Instance==TIM3)
    {
        g_run_flag_1 = 1;                             /* ��־λ��һ */
        g_stepper_x.pulse_count--;                    /* ÿһ�������������-- */
        if(g_stepper_x.dir == CW)
        {
           g_stepper_x.add_pulse_count++;             /* ����λ��++ */
			g_stepper_x.pos_mm += X_STEP_MM;
        }else if(g_stepper_x.dir == CCW)
        {
           g_stepper_x.add_pulse_count--;             /* ����λ��-- */
			g_stepper_x.pos_mm -= X_STEP_MM;
        }
//		g_stepper_x.pos_mm = g_stepper_x.add_pulse_count*X_STEP_MM;
		
        if(g_stepper_x.pulse_count<=10)                /* ������������0��ʱ�� ������Ҫ���͵������������ɣ�ֹͣ��ʱ����� */
        {
//            printf("�ۼ���ת�ĽǶ�:%d\r\n",(int)(g_stepper_x.add_pulse_count*X_STEP_ANGLE));  /* ��ӡ�ۼ�ת���˶��ٽǶ� */
            stepper_stop(STEPPER_MOTOR_1);          /* ֹͣ�ӿ�һ��� */
            g_run_flag_1 = 0;
        }
    }    
	if(htim->Instance==TIM4)
    {
        g_run_flag_2 = 1;                             /* ��־λ��һ */
        g_stepper_y.pulse_count--;                    /* ÿһ�������������-- */
        if(g_stepper_y.dir == CW)
        {
           g_stepper_y.add_pulse_count++;             /* ����λ��++ */
			g_stepper_y.pos_mm += Y_STEP_MM;
        }else if(g_stepper_y.dir == CCW)
        {
           g_stepper_y.add_pulse_count--;             /* ����λ��-- */
			g_stepper_y.pos_mm -= Y_STEP_MM;
        }
//		g_stepper_y.pos_mm = g_stepper_y.add_pulse_count*Y_STEP_MM;
        if(g_stepper_y.pulse_count<=10)                /* ������������0��ʱ�� ������Ҫ���͵������������ɣ�ֹͣ��ʱ����� */
        {
//			printf("��ǰ����:(%.2f, %.2f)\r\n",g_stepper_x.pos_mm,g_stepper_y.pos_mm);
//            printf("�ۼ���ת�ĽǶ�:%d\r\n",(int)(g_stepper_y.add_pulse_count*Y_STEP_ANGLE));  /* ��ӡ�ۼ�ת���˶��ٽǶ� */
            stepper_stop(2);          /* ֹͣ�ӿ�һ��� */
            g_run_flag_2 = 0;
        }
    }
	if(htim->Instance==TIM5)
    {
		
//				lv_timer_handler();
//		int32_t temp1 = (int32_t)g_stepper_x.pos_mm;
//		int32_t temp2 = (int32_t)g_stepper_y.pos_mm;
//		set_computer_value(SEND_FACT_CMD,CURVES_CH1,&temp1,1);
//		set_computer_value(SEND_TARGET_CMD,CURVES_CH1,&temp2,1);
//		LED1_TOGGLE();
//		LED1(0);
    }
//	if(htim==(&TIM6_Handler))
	if(htim->Instance==TIM6)
    {
        lv_tick_inc(1);//lvgl��1ms����
		time_ms++;
//		if(time_ms==)
    }
}

//����TIM8ͨ��1��ռ�ձ�
//compare:�Ƚ�ֵ
void TIM_SetTIM8Compare3(uint32_t compare)
{
	TIM8->CCR3=compare; 
}

//����TIMͨ��2��ռ�ձ�
//compare:�Ƚ�ֵ
void TIM_SetTIM8Compare4(uint32_t compare)
{
	TIM8->CCR4=compare; 
}



