#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//��ʱ����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ��3PWM��� 
extern volatile uint32_t time_ms;

/******************************************************************************************/
/* �߼���ʱ�� ���� */

 /* TIMX PWM ���� 
 * Ĭ��ʹ�õ���TIM8_CH1.
 * ע��: ͨ���޸��⼸���궨��, ����֧��TIM1/TIM8��ʱ��
 */
#define GTIM_TIM3_PWM_CH1_GPIO_PORT            GPIOA
#define GTIM_TIM3_PWM_CH1_GPIO_PIN             GPIO_PIN_6
#define GTIM_TIM3_PWM_CH1_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)     /* PA��ʱ��ʹ�� */

#define GTIM_TIM4_PWM_CH2_GPIO_PORT            GPIOB
#define GTIM_TIM4_PWM_CH2_GPIO_PIN             GPIO_PIN_7
#define GTIM_TIM4_PWM_CH2_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)     /* PB��ʱ��ʹ�� */

#define GTIM_TIM8_PWM_CH3_GPIO_PORT            GPIOC
#define GTIM_TIM8_PWM_CH3_GPIO_PIN             GPIO_PIN_8
#define GTIM_TIM8_PWM_CH3_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)     /* PC��ʱ��ʹ�� */

#define GTIM_TIM8_PWM_CH4_GPIO_PORT            GPIOC
#define GTIM_TIM8_PWM_CH4_GPIO_PIN             GPIO_PIN_9
#define GTIM_TIM8_PWM_CH4_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)     /* PC��ʱ��ʹ�� */

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
#define GTIM_TIM3_PWM_CH1                      TIM_CHANNEL_1                                    /* ͨ��1 */
#define GTIM_TIM4_PWM_CH2                      TIM_CHANNEL_2                                    /* ͨ��2 */
#define GTIM_TIM8_PWM_CH3                      TIM_CHANNEL_3                                    /* ͨ��3 */
#define GTIM_TIM8_PWM_CH4                      TIM_CHANNEL_4                                    /* ͨ��4 */
//#define GTIM_TIMX_PWM_CH3                      TIM_CHANNEL_3                                    /* ͨ��3 */
//#define GTIM_TIMX_PWM_CH4                      TIM_CHANNEL_4                                    /* ͨ��4 */

#define TIM3_CLK_ENABLE()         do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)       /* TIM3 ʱ��ʹ�� */
#define TIM4_CLK_ENABLE()         do{ __HAL_RCC_TIM4_CLK_ENABLE(); }while(0)       /* TIM4 ʱ��ʹ�� */
#define TIM8_CLK_ENABLE()         do{ __HAL_RCC_TIM8_CLK_ENABLE(); }while(0)       /* TIM8 ʱ��ʹ�� */

extern volatile uint8_t g_run_flag_1, g_run_flag_2;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef GTIM3_Handler;                                                        /* ��ʱ��x��� */
extern TIM_HandleTypeDef GTIM4_Handler;                                                        /* ��ʱ��x��� */
extern TIM_HandleTypeDef GTIM5_Handler;      //��ʱ��3PWM��� 
extern TIM_HandleTypeDef GTIM8_Handler;      //��ʱ��3PWM��� 


/******************************************************************************************/
void MX_TIM1_Init(void);
void GTIM_TIM3_PWM_Init(uint16_t arr, uint16_t psc);                                         /* ͨ�ö�ʱ��3 PWM��ʼ������ */
void GTIM_TIM4_PWM_Init(uint16_t arr, uint16_t psc);                                         /* ͨ�ö�ʱ��4 PWM��ʼ������ */
void TIM5_Init(uint16_t arr,uint16_t psc);    //��ʱ����ʼ��

void TIM6_Init(u16 arr,u16 psc);    //��ʱ����ʼ��
void TIM8_PWM_Init(uint16_t arr, uint16_t psc);                                         /* ͨ�ö�ʱ��8 PWM��ʼ������ */
void TIM_SetTIM8Compare3(uint32_t compare);
void TIM_SetTIM8Compare4(uint32_t compare);
#endif

