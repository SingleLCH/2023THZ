
#ifndef __STEPPER_MOTOR_H
#define __STEPPER_MOTOR_H

#include "sys.h"
/******************************************************************************************/
/* 步进电机参数相关宏 */
#define X_PULSE_REV       (5000.0)             	/* 每圈脉冲数 */
#define Y_PULSE_REV       (2000.0)             	/* 每圈脉冲数 */
#define X_STEP_ANGLE  	(360.0/X_PULSE_REV)               	/* 步进角(360/PULSE_REV) */
#define Y_STEP_ANGLE  	(360.0*0.4/Y_PULSE_REV)   /* 步进角(360*减速比/PULSE_REV) */
#define X_STEP_MM		(80.0/X_PULSE_REV) 		/* 80mm导程/细分数 */
#define Y_STEP_MM		(80.0*0.4/Y_PULSE_REV) 		/* 80mm导程*0.4减速比/细分数 */
#define X_MAX			(500)
#define Y_MAX			(500)
#define X_MIN			(0)
#define Y_MIN			(0)
//#define SCAN_DIVISION			(100)

typedef struct
{
    volatile int angle ;                             /* 设置需要旋转的角度 */
    volatile uint8_t dir;                            /* 方向 */
    volatile uint8_t en;                             /* 使能 */
    volatile uint32_t pulse_count;          /* 脉冲个数记录 */
    volatile int add_pulse_count;           /* 脉冲个数累计 */  
	volatile double pos_mm;					/* 位移坐标mm单位 */
} STEPPER_MOTOR;

extern  STEPPER_MOTOR g_stepper_x;
extern  STEPPER_MOTOR g_stepper_y;
enum dir
{
    CCW = 0,                                /* 逆时针旋转 */
    CW ,                                    /* 顺时针旋转 */
};  

/******************************************************************************************/
/* 步进电机引脚定义*/
/*	
	PUL1	---->	PA6
	PUL2	---->	PB7
	DIR1	---->	PC6
	DIR2	---->	PB8
	ENA1	---->	PC7
	ENA2	---->	PB9
	
*/
#define STEPPER_MOTOR_1       1             /* 步进电机接口序号 */
#define STEPPER_MOTOR_2       2
/*     步进电机方向引脚定义     */

#define STEPPER_DIR1_GPIO_PIN                  GPIO_PIN_6
#define STEPPER_DIR1_GPIO_PORT                 GPIOC
#define STEPPER_DIR1_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC口时钟使能 */

#define STEPPER_DIR2_GPIO_PIN                  GPIO_PIN_13			//8
#define STEPPER_DIR2_GPIO_PORT                 GPIOB
#define STEPPER_DIR2_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    /* PB口时钟使能 */

/*     步进电机脱机引脚定义     */

#define STEPPER_ENA1_GPIO_PIN                   GPIO_PIN_7
#define STEPPER_ENA1_GPIO_PORT                  GPIOC
#define STEPPER_ENA1_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC口时钟使能 */

#define STEPPER_ENA2_GPIO_PIN                   GPIO_PIN_15		//9
#define STEPPER_ENA2_GPIO_PORT                  GPIOB
#define STEPPER_ENA2_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    /* PB口时钟使能 */

/*     步进电机警报引脚定义     */
#define STEPPER_ALM1_GPIO_PIN                   GPIO_PIN_8
#define STEPPER_ALM1_GPIO_PORT                  GPIOH
#define STEPPER_ALM1_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0)    /* PH口时钟使能 */

#define STEPPER_ALM2_GPIO_PIN                   GPIO_PIN_3
#define STEPPER_ALM2_GPIO_PORT                  GPIOD
#define STEPPER_ALM2_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)    /* PD口时钟使能 */

/* 直线导轨光电开关引脚定义 */
#define SWITCH1_GPIO_PIN                   GPIO_PIN_10
#define SWITCH1_GPIO_PORT                  GPIOG
#define SWITCH1_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)    /* PG口时钟使能 */

#define SWITCH2_GPIO_PIN                   GPIO_PIN_12
#define SWITCH2_GPIO_PORT                  GPIOG
#define SWITCH2_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)    /* PG口时钟使能 */

#define SWITCH3_GPIO_PIN                   GPIO_PIN_13
#define SWITCH3_GPIO_PORT                  GPIOG
#define SWITCH3_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)    /* PG口时钟使能 */

#define SWITCH4_GPIO_PIN                   GPIO_PIN_14
#define SWITCH4_GPIO_PORT                  GPIOG
#define SWITCH4_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)    /* PG口时钟使能 */

#define SWITCH5_GPIO_PIN                   GPIO_PIN_2
#define SWITCH5_GPIO_PORT                  GPIOE
#define SWITCH5_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)    /* PG口时钟使能 */

#define SWITCH6_GPIO_PIN                   GPIO_PIN_3
#define SWITCH6_GPIO_PORT                  GPIOE
#define SWITCH6_GPIO_CLK_ENABLE()          do{  __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)    /* PG口时钟使能 */

/*----------------------- 方向引脚控制 -----------------------------------*/
/* 由于我们使用的是共阳极解法，并且硬件对电平做了取反操作，所以当 x = 1 有效，x = 0时无效*/  
#define ST1_DIR(x)    do{ x ? \
                              HAL_GPIO_WritePin(STEPPER_DIR1_GPIO_PORT, STEPPER_DIR1_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(STEPPER_DIR1_GPIO_PORT, STEPPER_DIR1_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)  

#define ST2_DIR(x)    do{ x ? \
                              HAL_GPIO_WritePin(STEPPER_DIR2_GPIO_PORT, STEPPER_DIR2_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(STEPPER_DIR2_GPIO_PORT, STEPPER_DIR2_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)  


/*----------------------- 脱机引脚控制 -----------------------------------*/
/* 由于我们使用的是共阳极解法，并且硬件对电平做了取反操作，所以当 x = 1 有效，x = 0时无效*/                          
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
/* 外部接口函数*/
void switch_init(void);
void stepper_init(void);                          						/* 步进电机接口初始化 */
void stepper_star(uint8_t motor_num, uint8_t dir);                      /* 开启步进电机 */
void stepper_stop(uint8_t motor_num);                                   /* 关闭步进电机 */      
void stepper_set_angle(uint16_t angle, uint8_t dir,uint8_t motor_num);  /* 将角度转换成脉冲个数 */
int stepper_set_pos(float x_aim, float y_aim);							/* 发送目标位置 */
#endif
