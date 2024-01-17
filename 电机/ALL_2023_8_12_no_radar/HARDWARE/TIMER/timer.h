#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//定时器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/11/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
extern TIM_HandleTypeDef TIM3_Handler;      //定时器3PWM句柄 
extern volatile uint32_t time_ms;

/******************************************************************************************/
/* 高级定时器 定义 */

 /* TIMX PWM 定义 
 * 默认使用的是TIM8_CH1.
 * 注意: 通过修改这几个宏定义, 可以支持TIM1/TIM8定时器
 */
#define GTIM_TIM3_PWM_CH1_GPIO_PORT            GPIOA
#define GTIM_TIM3_PWM_CH1_GPIO_PIN             GPIO_PIN_6
#define GTIM_TIM3_PWM_CH1_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)     /* PA口时钟使能 */

#define GTIM_TIM4_PWM_CH2_GPIO_PORT            GPIOB
#define GTIM_TIM4_PWM_CH2_GPIO_PIN             GPIO_PIN_7
#define GTIM_TIM4_PWM_CH2_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)     /* PB口时钟使能 */

#define GTIM_TIM8_PWM_CH3_GPIO_PORT            GPIOC
#define GTIM_TIM8_PWM_CH3_GPIO_PIN             GPIO_PIN_8
#define GTIM_TIM8_PWM_CH3_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)     /* PC口时钟使能 */

#define GTIM_TIM8_PWM_CH4_GPIO_PORT            GPIOC
#define GTIM_TIM8_PWM_CH4_GPIO_PIN             GPIO_PIN_9
#define GTIM_TIM8_PWM_CH4_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)     /* PC口时钟使能 */

#define GTIM_TIMX_PWM_CHY_GPIO_AF              GPIO_AF3_TIM8

#define GTIM_TIM3                         		TIM3
#define GTIM_TIM4                         		TIM4
#define GTIM_TIM8                         		TIM8
#define GTIM_TIM3_IRQn                      	TIM3_IRQn
#define GTIM_TIM3_IRQHandler                	TIM3_IRQHandler
#define GTIM_TIM4_IRQn                      	TIM4_IRQn
#define GTIM_TIM4_IRQHandler                	TIM4_IRQHandler
#define GTIM_TIM8_IRQn                      	TIM8_CC_IRQn
#define GTIM_TIM8_IRQHandler                	TIM8_CC_IRQHandler
#define GTIM_TIM3_PWM_CH1                      TIM_CHANNEL_1                                    /* 通道1 */
#define GTIM_TIM4_PWM_CH2                      TIM_CHANNEL_2                                    /* 通道2 */
#define GTIM_TIM8_PWM_CH3                      TIM_CHANNEL_3                                    /* 通道3 */
#define GTIM_TIM8_PWM_CH4                      TIM_CHANNEL_4                                    /* 通道4 */
//#define GTIM_TIMX_PWM_CH3                      TIM_CHANNEL_3                                    /* 通道3 */
//#define GTIM_TIMX_PWM_CH4                      TIM_CHANNEL_4                                    /* 通道4 */

#define TIM3_CLK_ENABLE()         do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)       /* TIM3 时钟使能 */
#define TIM4_CLK_ENABLE()         do{ __HAL_RCC_TIM4_CLK_ENABLE(); }while(0)       /* TIM4 时钟使能 */
#define TIM8_CLK_ENABLE()         do{ __HAL_RCC_TIM8_CLK_ENABLE(); }while(0)       /* TIM8 时钟使能 */

extern volatile uint8_t g_run_flag_1, g_run_flag_2;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef GTIM3_Handler;                                                        /* 定时器x句柄 */
extern TIM_HandleTypeDef GTIM4_Handler;                                                        /* 定时器x句柄 */
extern TIM_HandleTypeDef GTIM5_Handler;      //定时器3PWM句柄 
extern TIM_HandleTypeDef GTIM8_Handler;      //定时器3PWM句柄 


/******************************************************************************************/
void MX_TIM1_Init(void);
void GTIM_TIM3_PWM_Init(uint16_t arr, uint16_t psc);                                         /* 通用定时器3 PWM初始化函数 */
void GTIM_TIM4_PWM_Init(uint16_t arr, uint16_t psc);                                         /* 通用定时器4 PWM初始化函数 */
void TIM5_Init(uint16_t arr,uint16_t psc);    //定时器初始化

void TIM6_Init(u16 arr,u16 psc);    //定时器初始化
void TIM8_PWM_Init(uint16_t arr, uint16_t psc);                                         /* 通用定时器8 PWM初始化函数 */
void TIM_SetTIM8Compare3(uint32_t compare);
void TIM_SetTIM8Compare4(uint32_t compare);
#endif

