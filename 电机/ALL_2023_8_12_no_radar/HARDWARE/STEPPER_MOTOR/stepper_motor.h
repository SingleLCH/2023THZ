
#ifndef __STEPPER_MOTOR_H
#define __STEPPER_MOTOR_H

#include "sys.h"
/******************************************************************************************/
/* �������������غ� */
#define X_PULSE_REV       (5000.0)             	/* ÿȦ������ */
#define Y_PULSE_REV       (2000.0)             	/* ÿȦ������ */
#define X_STEP_ANGLE  	(360.0/X_PULSE_REV)               	/* ������(360/PULSE_REV) */
#define Y_STEP_ANGLE  	(360.0*0.4/Y_PULSE_REV)   /* ������(360*���ٱ�/PULSE_REV) */
#define X_STEP_MM		(80.0/X_PULSE_REV) 		/* 80mm����/ϸ���� */
#define Y_STEP_MM		(80.0*0.4/Y_PULSE_REV) 		/* 80mm����*0.4���ٱ�/ϸ���� */
#define X_MAX			(500)
#define Y_MAX			(500)
#define X_MIN			(0)
#define Y_MIN			(0)
//#define SCAN_DIVISION			(100)

typedef struct
{
    volatile int angle ;                             /* ������Ҫ��ת�ĽǶ� */
    volatile uint8_t dir;                            /* ���� */
    volatile uint8_t en;                             /* ʹ�� */
    volatile uint32_t pulse_count;          /* ���������¼ */
    volatile int add_pulse_count;           /* ��������ۼ� */  
	volatile double pos_mm;					/* λ������mm��λ */
} STEPPER_MOTOR;

extern  STEPPER_MOTOR g_stepper_x;
extern  STEPPER_MOTOR g_stepper_y;
enum dir
{
    CCW = 0,                                /* ��ʱ����ת */
    CW ,                                    /* ˳ʱ����ת */
};  

/******************************************************************************************/
/* ����������Ŷ���*/
/*	
	PUL1	---->	PA6
	PUL2	---->	PB7
	DIR1	---->	PC6
	DIR2	---->	PB8
	ENA1	---->	PC7
	ENA2	---->	PB9
	
*/
#define STEPPER_MOTOR_1       1             /* ��������ӿ���� */
#define STEPPER_MOTOR_2       2
/*     ��������������Ŷ���     */

#define STEPPER_DIR1_GPIO_PIN                  GPIO_PIN_6
#define STEPPER_DIR1_GPIO_PORT                 GPIOC
#define STEPPER_DIR1_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC��ʱ��ʹ�� */

#define STEPPER_DIR2_GPIO_PIN                  GPIO_PIN_13			//8
#define STEPPER_DIR2_GPIO_PORT                 GPIOB
#define STEPPER_DIR2_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    /* PB��ʱ��ʹ�� */

/*     ��������ѻ����Ŷ���     */

#define STEPPER_ENA1_GPIO_PIN                   GPIO_PIN_7
#define STEPPER_ENA1_GPIO_PORT                  GPIOC
#define STEPPER_ENA1_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC��ʱ��ʹ�� */

#define STEPPER_ENA2_GPIO_PIN                   GPIO_PIN_15		//9
#define STEPPER_ENA2_GPIO_PORT                  GPIOB
#define STEPPER_ENA2_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    /* PB��ʱ��ʹ�� */

/*     ��������������Ŷ���     */
#define STEPPER_ALM1_GPIO_PIN                   GPIO_PIN_8
#define STEPPER_ALM1_GPIO_PORT                  GPIOH
#define STEPPER_ALM1_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0)    /* PH��ʱ��ʹ�� */

#define STEPPER_ALM2_GPIO_PIN                   GPIO_PIN_3
#define STEPPER_ALM2_GPIO_PORT                  GPIOD
#define STEPPER_ALM2_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)    /* PD��ʱ��ʹ�� */

/* ֱ�ߵ����翪�����Ŷ��� */
#define SWITCH1_GPIO_PIN                   GPIO_PIN_10
#define SWITCH1_GPIO_PORT                  GPIOG
#define SWITCH1_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)    /* PG��ʱ��ʹ�� */

#define SWITCH2_GPIO_PIN                   GPIO_PIN_12
#define SWITCH2_GPIO_PORT                  GPIOG
#define SWITCH2_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)    /* PG��ʱ��ʹ�� */

#define SWITCH3_GPIO_PIN                   GPIO_PIN_13
#define SWITCH3_GPIO_PORT                  GPIOG
#define SWITCH3_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)    /* PG��ʱ��ʹ�� */

#define SWITCH4_GPIO_PIN                   GPIO_PIN_14
#define SWITCH4_GPIO_PORT                  GPIOG
#define SWITCH4_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)    /* PG��ʱ��ʹ�� */

#define SWITCH5_GPIO_PIN                   GPIO_PIN_2
#define SWITCH5_GPIO_PORT                  GPIOE
#define SWITCH5_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)    /* PG��ʱ��ʹ�� */

#define SWITCH6_GPIO_PIN                   GPIO_PIN_3
#define SWITCH6_GPIO_PORT                  GPIOE
#define SWITCH6_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)    /* PG��ʱ��ʹ�� */

/*----------------------- �������ſ��� -----------------------------------*/
/* ��������ʹ�õ��ǹ������ⷨ������Ӳ���Ե�ƽ����ȡ�����������Ե� x = 1 ��Ч��x = 0ʱ��Ч*/  
#define ST1_DIR(x)    do{ x ? \
                              HAL_GPIO_WritePin(STEPPER_DIR1_GPIO_PORT, STEPPER_DIR1_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(STEPPER_DIR1_GPIO_PORT, STEPPER_DIR1_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)  

#define ST2_DIR(x)    do{ x ? \
                              HAL_GPIO_WritePin(STEPPER_DIR2_GPIO_PORT, STEPPER_DIR2_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(STEPPER_DIR2_GPIO_PORT, STEPPER_DIR2_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)  


/*----------------------- �ѻ����ſ��� -----------------------------------*/
/* ��������ʹ�õ��ǹ������ⷨ������Ӳ���Ե�ƽ����ȡ�����������Ե� x = 1 ��Ч��x = 0ʱ��Ч*/                          
#define ST1_EN(x)      do{ x ? \
                          HAL_GPIO_WritePin(STEPPER_ENA1_GPIO_PORT, STEPPER_ENA1_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(STEPPER_ENA1_GPIO_PORT, STEPPER_ENA1_GPIO_PIN, GPIO_PIN_RESET); \
                        }while(0)    

#define ST2_EN(x)      do{ x ? \
                          HAL_GPIO_WritePin(STEPPER_ENA2_GPIO_PORT, STEPPER_ENA2_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(STEPPER_ENA2_GPIO_PORT, STEPPER_ENA2_GPIO_PIN, GPIO_PIN_RESET); \
                        }while(0)    
                           
#define SWITCH1  HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_10)
#define SWITCH2  HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_12) 
#define SWITCH3  HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_13) 
#define SWITCH4  HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_14)
#define SWITCH5  HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2) 
#define SWITCH6  HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)						
/******************************************************************************************/
/* �ⲿ�ӿں���*/
void switch_init(void);
void stepper_init(void);                          						/* ��������ӿڳ�ʼ�� */
void stepper_star(uint8_t motor_num, uint8_t dir);                      /* ����������� */
void stepper_stop(uint8_t motor_num);                                   /* �رղ������ */      
void stepper_set_angle(uint16_t angle, uint8_t dir,uint8_t motor_num);  /* ���Ƕ�ת����������� */
int stepper_set_pos(float x_aim, float y_aim);							/* ����Ŀ��λ�� */
#endif
