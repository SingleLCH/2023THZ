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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//定时器中断驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/11/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
/******************************************************************************************/
/* 定时器配置句柄 定义 */

/* 通用定时器PWM */
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef GTIM3_Handler;           /* 定时器x句柄 */
TIM_HandleTypeDef GTIM4_Handler;           /* 定时器x句柄 */
TIM_HandleTypeDef GTIM8_Handler;           /* 定时器x句柄 */
TIM_HandleTypeDef GTIM5_Handler;      //定时器句柄 
TIM_OC_InitTypeDef GTIMx_CHy_Handler;   /* 定时器x输出y句柄 */ 
volatile uint8_t g_run_flag_1 = 0, g_run_flag_2 = 0;
/******************************************************************************************/

TIM_HandleTypeDef TIM6_Handler;      //定时器句柄 
volatile uint32_t time_ms = 0;


//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
void TIM6_Init(u16 arr,u16 psc)
{  
    TIM6_Handler.Instance=TIM6;                          //通用定时器6
    TIM6_Handler.Init.Prescaler=psc;                     //分频
    TIM6_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM6_Handler.Init.Period=arr;                        //自动装载值
    TIM6_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM6_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM6_Handler); //使能定时器6和定时器6更新中断：TIM_IT_UPDATE    
}

/**
 * @brief       通用定时器TIMX 通道1、2、3、4 PWM输出初始化
 * @note
 *              通用定时器的时钟来自APB1, 而PCLK1 = 54MHz, 我们设置PPRE1=4, 因此
 *              通用定时器时钟 = 54MHz*2=108MHz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值
 * @param       psc: 时钟预分频数
 * @retval      无
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
    TIM8_CLK_ENABLE();                             /* TIM8 时钟使能 */
    
    GTIM8_Handler.Instance = GTIM_TIM8;                    /* 定时器x */
    GTIM8_Handler.Init.Prescaler = psc;                        /* 定时器分频 */
    GTIM8_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       /* 向上计数模式 */
    GTIM8_Handler.Init.Period = arr;                           /* 自动重装载值 */
    GTIM8_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;   /* 分频因子 */
	TIM8->CR1 |= TIM_CR1_ARPE; /* 使能定时器ARR寄存器的预装载 */
//    GTIM8_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /*使能TIMx_ARR进行缓冲*/
//    GTIMx_Handler.Init.RepetitionCounter = 0;                  /* 开始时不计数*/在高级定时器中使用
    HAL_TIM_PWM_Init(&GTIM8_Handler);                          /* 初始化PWM */
	
    
    GTIMx_CHy_Handler.OCMode = TIM_OCMODE_PWM1;             /* 模式选择PWM1 */
    GTIMx_CHy_Handler.Pulse = (arr+1)/2;						/*占空比为50%*/
    GTIMx_CHy_Handler.OCPolarity = TIM_OCPOLARITY_HIGH;    		/*当定时器计数值小于CCR1_Val时为高电平*/
    GTIMx_CHy_Handler.OCNPolarity = TIM_OCNPOLARITY_HIGH;		/*设置互补通道输出的极性*/
    GTIMx_CHy_Handler.OCFastMode = TIM_OCFAST_DISABLE;			/*快速模式设置*/
    GTIMx_CHy_Handler.OCIdleState = TIM_OCIDLESTATE_RESET; 		/*空闲电平*/
    GTIMx_CHy_Handler.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(&GTIM8_Handler, &GTIMx_CHy_Handler, GTIM_TIM8_PWM_CH3); /* 配置TIM4通道3 */
	HAL_TIM_PWM_ConfigChannel(&GTIM8_Handler, &GTIMx_CHy_Handler, GTIM_TIM8_PWM_CH4); /* 配置TIM4通道4 */
	
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH2); /* 配置TIMx通道y */   
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH3); /* 配置TIMx通道y */
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH4); /* 配置TIMx通道y */
  
    
}
 
 
 
 
void GTIM_TIM3_PWM_Init(uint16_t arr, uint16_t psc)
{
    TIM3_CLK_ENABLE();                             /* TIM3 时钟使能 */
    
    GTIM3_Handler.Instance = GTIM_TIM3;                    /* 定时器x */
    GTIM3_Handler.Init.Prescaler = psc;                        /* 定时器分频 */
    GTIM3_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       /* 向上计数模式 */
    GTIM3_Handler.Init.Period = arr;                           /* 自动重装载值 */
    GTIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;   /* 分频因子 */
//    GTIM3_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /*使能TIMx_ARR进行缓冲*/
//    GTIMx_Handler.Init.RepetitionCounter = 0;                  /* 开始时不计数*/在高级定时器中使用
    HAL_TIM_PWM_Init(&GTIM3_Handler);                          /* 初始化PWM */
	
    
    GTIMx_CHy_Handler.OCMode = TIM_OCMODE_PWM1;             /* 模式选择PWM1 */
    GTIMx_CHy_Handler.Pulse = (arr+1)/2;						/*占空比为50%*/
    GTIMx_CHy_Handler.OCPolarity = TIM_OCPOLARITY_HIGH;    		/*当定时器计数值小于CCR1_Val时为高电平*/
    GTIMx_CHy_Handler.OCNPolarity = TIM_OCNPOLARITY_HIGH;		/*设置互补通道输出的极性*/
    GTIMx_CHy_Handler.OCFastMode = TIM_OCFAST_DISABLE;			/*快速模式设置*/
    GTIMx_CHy_Handler.OCIdleState = TIM_OCIDLESTATE_RESET; 		/*空闲电平*/
    GTIMx_CHy_Handler.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(&GTIM3_Handler, &GTIMx_CHy_Handler, GTIM_TIM3_PWM_CH1); /* 配置TIM4通道1 */
	
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH2); /* 配置TIMx通道y */   
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH3); /* 配置TIMx通道y */
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH4); /* 配置TIMx通道y */
  
    
}


void GTIM_TIM4_PWM_Init(uint16_t arr, uint16_t psc)
{
    TIM4_CLK_ENABLE();                             /* TIM4 时钟使能 */
    
    GTIM4_Handler.Instance = GTIM_TIM4;                    /* 定时器x */
    GTIM4_Handler.Init.Prescaler = psc;                        /* 定时器分频 */
    GTIM4_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       /* 向上计数模式 */
    GTIM4_Handler.Init.Period = arr;                           /* 自动重装载值 */
    GTIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;   /* 分频因子 */
//    GTIM4_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /*使能TIMx_ARR进行缓冲*/
//    GTIMx_Handler.Init.RepetitionCounter = 0;                  /* 开始时不计数*/在高级定时器中使用
    HAL_TIM_PWM_Init(&GTIM4_Handler);                          /* 初始化PWM */
	
    
    GTIMx_CHy_Handler.OCMode = TIM_OCMODE_PWM1;             /* 模式选择PWM1 */
    GTIMx_CHy_Handler.Pulse = (arr+1)/2;						/*占空比为50%*/
    GTIMx_CHy_Handler.OCPolarity = TIM_OCPOLARITY_HIGH;    		/*当定时器计数值小于CCR1_Val时为高电平*/
    GTIMx_CHy_Handler.OCNPolarity = TIM_OCNPOLARITY_HIGH;		/*设置互补通道输出的极性*/
    GTIMx_CHy_Handler.OCFastMode = TIM_OCFAST_DISABLE;			/*快速模式设置*/
    GTIMx_CHy_Handler.OCIdleState = TIM_OCIDLESTATE_RESET; 		/*空闲电平*/
    GTIMx_CHy_Handler.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(&GTIM4_Handler, &GTIMx_CHy_Handler, GTIM_TIM4_PWM_CH2); /* 配置TIM4通道1 */
	
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH2); /* 配置TIMx通道y */   
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH3); /* 配置TIMx通道y */
//    HAL_TIM_PWM_ConfigChannel(&g_gtimx_handle, &g_gtimx_oc_chy_handle, GTIM_TIMX_PWM_CH4); /* 配置TIMx通道y */
  
    
}


//通用定时器5中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器5!(定时器5挂在APB1上，时钟为HCLK/2)
void TIM5_Init(uint16_t arr,uint16_t psc)
{  
    GTIM5_Handler.Instance=TIM5;                          //通用定时器5
    GTIM5_Handler.Init.Prescaler=psc;                     //分频
    GTIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    GTIM5_Handler.Init.Period=arr;                        //自动装载值
    GTIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&GTIM5_Handler);
    
    HAL_TIM_Base_Start_IT(&GTIM5_Handler); //使能定时器5和定时器5更新中断：TIM_IT_UPDATE    
}

/**
 * @brief       定时器底层驱动，时钟使能，引脚配置
                此函数会被HAL_TIM_PWM_Init()调用
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	

    if (htim->Instance == GTIM_TIM3)
    {
        GPIO_InitTypeDef GPIO_InitStruct;
        TIM3_CLK_ENABLE();                             				/* 开启通道y的GPIO时钟 */
        GTIM_TIM3_PWM_CH1_GPIO_CLK_ENABLE();                        /* PWM通道输出IO时钟使能 */

        GPIO_InitStruct.Pin = GTIM_TIM3_PWM_CH1_GPIO_PIN;          /* 通道y的GPIO口 */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                    /* 复用推挽输出 */
        GPIO_InitStruct.Pull = GPIO_PULLUP;                        /* 上拉 */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;     				/* 端口复用 */
        HAL_GPIO_Init(GTIM_TIM3_PWM_CH1_GPIO_PORT, &GPIO_InitStruct);
        
//        GPIO_InitStruct.Pin = GTIM_TIMX_PWM_CH2_GPIO_PIN;          /* 通道y的GPIO口 */
//        HAL_GPIO_Init(GTIM_TIMX_PWM_CH2_GPIO_PORT, &GPIO_InitStruct);
        
        HAL_NVIC_SetPriority(GTIM_TIM3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(GTIM_TIM3_IRQn);
    }
    if (htim->Instance == GTIM_TIM4)
    {
        GPIO_InitTypeDef GPIO_InitStruct;
        TIM4_CLK_ENABLE();                             				/* 开启通道y的GPIO时钟 */
        GTIM_TIM4_PWM_CH2_GPIO_CLK_ENABLE();                        /* PWM通道输出IO时钟使能 */

        GPIO_InitStruct.Pin = GTIM_TIM4_PWM_CH2_GPIO_PIN;          /* 通道y的GPIO口 */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                    /* 复用推挽输出 */
        GPIO_InitStruct.Pull = GPIO_PULLUP;                        /* 上拉 */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;     				/* 端口复用 */
        HAL_GPIO_Init(GTIM_TIM4_PWM_CH2_GPIO_PORT, &GPIO_InitStruct);
        
//        GPIO_InitStruct.Pin = GTIM_TIMX_PWM_CH2_GPIO_PIN;          /* 通道y的GPIO口 */
//        HAL_GPIO_Init(GTIM_TIMX_PWM_CH2_GPIO_PORT, &GPIO_InitStruct);
        
        HAL_NVIC_SetPriority(GTIM_TIM4_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(GTIM_TIM4_IRQn);
    }
	if (htim->Instance == GTIM_TIM8)
    {
        GPIO_InitTypeDef GPIO_InitStruct;
        TIM8_CLK_ENABLE();                             				/* 开启通道y的GPIO时钟 */
        GTIM_TIM8_PWM_CH3_GPIO_CLK_ENABLE();                        /* PWM通道输出IO时钟使能 */
		GTIM_TIM8_PWM_CH4_GPIO_CLK_ENABLE();                        /* PWM通道输出IO时钟使能 */

        GPIO_InitStruct.Pin = GTIM_TIM8_PWM_CH3_GPIO_PIN;          /* 通道y的GPIO口 */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                    /* 复用推挽输出 */
        GPIO_InitStruct.Pull = GPIO_PULLUP;                        /* 上拉 */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;     				/* 端口复用 */
        HAL_GPIO_Init(GTIM_TIM8_PWM_CH3_GPIO_PORT, &GPIO_InitStruct);
        
		GPIO_InitStruct.Pin = GTIM_TIM8_PWM_CH4_GPIO_PIN;          /* 通道y的GPIO口 */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                    /* 复用推挽输出 */
        GPIO_InitStruct.Pull = GPIO_PULLUP;                        /* 上拉 */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;     				/* 端口复用 */
        HAL_GPIO_Init(GTIM_TIM8_PWM_CH4_GPIO_PORT, &GPIO_InitStruct);
//        GPIO_InitStruct.Pin = GTIM_TIMX_PWM_CH2_GPIO_PIN;          /* 通道y的GPIO口 */
//        HAL_GPIO_Init(GTIM_TIMX_PWM_CH2_GPIO_PORT, &GPIO_InitStruct);
        
        HAL_NVIC_SetPriority(GTIM_TIM8_IRQn, 1, 1);
        HAL_NVIC_EnableIRQ(GTIM_TIM8_IRQn);
    }

}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
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
		__HAL_RCC_TIM5_CLK_ENABLE();            //使能TIM5时钟
		HAL_NVIC_SetPriority(TIM5_IRQn,1,1);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM5_IRQn);          //开启ITM5中断   
	}  
	
	if(htim->Instance==TIM6)
	{
		__HAL_RCC_TIM6_CLK_ENABLE();            //使能TIM6时钟
		HAL_NVIC_SetPriority(TIM6_DAC_IRQn,0,0);    //设置中断优先级，抢占优先级0，子优先级0
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);          //开启ITM6中断   
	}  
}

/**
 * @brief       通用定时器共用中断服务函数
 * @param       无
 * @retval      无
 */
void GTIM_TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&GTIM3_Handler);            /* 定时器共用处理函数 */
}

void GTIM_TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&GTIM4_Handler);            /* 定时器共用处理函数 */
}

void GTIM_TIM8_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&GTIM8_Handler);            /* 定时器共用处理函数 */
}

//定时器5中断服务函数
void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&GTIM5_Handler);
}

//定时器6中断服务函数
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

/* 中断回调函数 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM1){
    	HAL_GPIO_TogglePin(LED0_GPIO_PORT,LED0_GPIO_PIN);//
	    radar_rxDataFlag=1;
	    HAL_TIM_Base_Stop_IT(&htim1);
    }
    if(htim->Instance==TIM3)
    {
        g_run_flag_1 = 1;                             /* 标志位置一 */
        g_stepper_x.pulse_count--;                    /* 每一个完整的脉冲就-- */
        if(g_stepper_x.dir == CW)
        {
           g_stepper_x.add_pulse_count++;             /* 绝对位置++ */
			g_stepper_x.pos_mm += X_STEP_MM;
        }else if(g_stepper_x.dir == CCW)
        {
           g_stepper_x.add_pulse_count--;             /* 绝对位置-- */
			g_stepper_x.pos_mm -= X_STEP_MM;
        }
//		g_stepper_x.pos_mm = g_stepper_x.add_pulse_count*X_STEP_MM;
		
        if(g_stepper_x.pulse_count<=10)                /* 当脉冲数等于0的时候 代表需要发送的脉冲个数已完成，停止定时器输出 */
        {
//            printf("累计旋转的角度:%d\r\n",(int)(g_stepper_x.add_pulse_count*X_STEP_ANGLE));  /* 打印累计转动了多少角度 */
            stepper_stop(STEPPER_MOTOR_1);          /* 停止接口一输出 */
            g_run_flag_1 = 0;
        }
    }    
	if(htim->Instance==TIM4)
    {
        g_run_flag_2 = 1;                             /* 标志位置一 */
        g_stepper_y.pulse_count--;                    /* 每一个完整的脉冲就-- */
        if(g_stepper_y.dir == CW)
        {
           g_stepper_y.add_pulse_count++;             /* 绝对位置++ */
			g_stepper_y.pos_mm += Y_STEP_MM;
        }else if(g_stepper_y.dir == CCW)
        {
           g_stepper_y.add_pulse_count--;             /* 绝对位置-- */
			g_stepper_y.pos_mm -= Y_STEP_MM;
        }
//		g_stepper_y.pos_mm = g_stepper_y.add_pulse_count*Y_STEP_MM;
        if(g_stepper_y.pulse_count<=10)                /* 当脉冲数等于0的时候 代表需要发送的脉冲个数已完成，停止定时器输出 */
        {
//			printf("当前坐标:(%.2f, %.2f)\r\n",g_stepper_x.pos_mm,g_stepper_y.pos_mm);
//            printf("累计旋转的角度:%d\r\n",(int)(g_stepper_y.add_pulse_count*Y_STEP_ANGLE));  /* 打印累计转动了多少角度 */
            stepper_stop(2);          /* 停止接口一输出 */
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
        lv_tick_inc(1);//lvgl的1ms心跳
		time_ms++;
//		if(time_ms==)
    }
}

//设置TIM8通道1的占空比
//compare:比较值
void TIM_SetTIM8Compare3(uint32_t compare)
{
	TIM8->CCR3=compare; 
}

//设置TIM通道2的占空比
//compare:比较值
void TIM_SetTIM8Compare4(uint32_t compare)
{
	TIM8->CCR4=compare; 
}



