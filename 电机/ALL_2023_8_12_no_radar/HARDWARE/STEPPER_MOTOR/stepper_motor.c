
#include "stepper_motor.h"
#include "timer.h"
#include "HANA_math.h"
/***************************************/
#include "led.h"
/**************************************/

STEPPER_MOTOR g_stepper_x = {0};
STEPPER_MOTOR g_stepper_y = {0};

void switch_init(void)
{
	SWITCH1_GPIO_CLK_ENABLE();
	SWITCH2_GPIO_CLK_ENABLE();
	SWITCH3_GPIO_CLK_ENABLE();
	SWITCH4_GPIO_CLK_ENABLE();
	SWITCH5_GPIO_CLK_ENABLE();
	SWITCH6_GPIO_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_Initure;
	
	GPIO_Initure.Pin=SWITCH1_GPIO_PIN;       
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_NOPULL;       	//无上下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(SWITCH1_GPIO_PORT,&GPIO_Initure);
	
	GPIO_Initure.Pin=SWITCH2_GPIO_PIN;       
    HAL_GPIO_Init(SWITCH2_GPIO_PORT,&GPIO_Initure);
	
	GPIO_Initure.Pin=SWITCH3_GPIO_PIN;       
    HAL_GPIO_Init(SWITCH3_GPIO_PORT,&GPIO_Initure);
	
	GPIO_Initure.Pin=SWITCH4_GPIO_PIN;       
    HAL_GPIO_Init(SWITCH4_GPIO_PORT,&GPIO_Initure);
	
	GPIO_Initure.Pin=SWITCH5_GPIO_PIN;       
    HAL_GPIO_Init(SWITCH5_GPIO_PORT,&GPIO_Initure);
	
	GPIO_Initure.Pin=SWITCH6_GPIO_PIN;       
    HAL_GPIO_Init(SWITCH6_GPIO_PORT,&GPIO_Initure);

}



/**
 * @brief       初始化步进电机相关IO口, 并使能时钟
 * @param       arr: 自动重装值
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void stepper_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    STEPPER_DIR1_GPIO_CLK_ENABLE();                                 /* DIR1时钟使能 */
    STEPPER_DIR2_GPIO_CLK_ENABLE();                                 /* DIR2时钟使能 */

    STEPPER_ENA2_GPIO_CLK_ENABLE();                                  /* EN1时钟使能 */
    STEPPER_ENA2_GPIO_CLK_ENABLE();                                  /* EN2时钟使能 */

    GPIO_InitStructure.Pin = STEPPER_DIR1_GPIO_PIN;                   /* DIR1引脚 */
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;                    /* 推挽输出 */
    GPIO_InitStructure.Pull = GPIO_PULLDOWN;                          /* 下拉 */
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;                   /* 低速 */
    HAL_GPIO_Init(STEPPER_DIR1_GPIO_PORT, &GPIO_InitStructure);       /* 初始化DIR1引脚 */

    GPIO_InitStructure.Pin = STEPPER_DIR2_GPIO_PIN;                   /* DIR2引脚 */
	HAL_GPIO_Init(STEPPER_DIR2_GPIO_PORT, &GPIO_InitStructure);       /* 初始化DIR2引脚 */
	
    /*   脱机引脚初始化   */
    
    GPIO_InitStructure.Pin = STEPPER_ENA1_GPIO_PIN;                    /* EN1引脚 */
    HAL_GPIO_Init(STEPPER_ENA1_GPIO_PORT, &GPIO_InitStructure);        /* 初始化EN1引脚 */
    
    GPIO_InitStructure.Pin = STEPPER_ENA2_GPIO_PIN;                    /* EN2引脚 */
    HAL_GPIO_Init(STEPPER_ENA2_GPIO_PORT, &GPIO_InitStructure);        /* 初始化EN2引脚 */
	
	switch_init();
    
//    GTIM_TIM3_PWM_Init(arr, psc);                                /* 初始化PUL引脚，以及脉冲模式等 */
//	GTIM_TIM4_PWM_Init(arr, psc);                                /* 初始化PUL引脚，以及脉冲模式等 */
}

/**
 * @brief       开启步进电机
 * @param       motor_num: 步进电机接口序号
 * @param       dir      : 步进电机旋转方向
 * @retval      无
 */
void stepper_star(uint8_t motor_num, uint8_t dir)
{
//	HAL_TIM_PWM_Start_IT(&g_gtimx_handle);//使能了OC中断
    switch(motor_num)
    {
        case STEPPER_MOTOR_1 :
        {
			HAL_TIM_Base_Start_IT(&GTIM3_Handler);
            ST1_DIR(dir);
            HAL_TIM_PWM_Start(&GTIM3_Handler, GTIM_TIM3_PWM_CH1);     /* 开启对应PWM通道 */
            break;
        }
        case STEPPER_MOTOR_2 :
        {
			HAL_TIM_Base_Start_IT(&GTIM4_Handler);
            ST2_DIR(dir);
            HAL_TIM_PWM_Start(&GTIM4_Handler, GTIM_TIM4_PWM_CH2);     /* 开启对应PWM通道 */
            break;
        }
        default : break;
    }
}

/**
 * @brief       关闭步进电机
 * @param       motor_num: 步进电机接口序号
 * @retval      无
 */
void stepper_stop(uint8_t motor_num)
{
    switch(motor_num)
    {
        case STEPPER_MOTOR_1 :
        {
			HAL_TIM_Base_Stop_IT(&GTIM3_Handler);
            HAL_TIM_PWM_Stop(&GTIM3_Handler, GTIM_TIM3_PWM_CH1);     /* 关闭对应PWM通道 */
            break;
        }
        case STEPPER_MOTOR_2 :
        {
			HAL_TIM_Base_Stop_IT(&GTIM4_Handler);
            HAL_TIM_PWM_Stop(&GTIM4_Handler, GTIM_TIM4_PWM_CH2);     /* 关闭对应PWM通道 */
            break;
        }
        default : break;
    }
}

/**
 * @brief       将需要转动的角度转换成脉冲数
 * @param       angle    : 需要转动的角度值
 * @param       dir      : 旋转方向
 * @param       motor_num: 步进电机接口序号
 * @retval      无
 */
void stepper_set_angle(uint16_t angle, uint8_t dir,uint8_t motor_num)
{

		if(motor_num ==1)
		{
			g_stepper_x.pulse_count = angle / X_STEP_ANGLE;
			if(g_stepper_x.pulse_count == 0) 
			{
				stepper_stop(motor_num);
			}
			else stepper_star(motor_num,dir);
		}
		else if(motor_num ==2)
		{
			g_stepper_y.pulse_count = angle / Y_STEP_ANGLE;
			if(g_stepper_y.pulse_count == 0) 
			{
				stepper_stop(motor_num);
			}
			else stepper_star(motor_num,dir);
		}

	
}

int stepper_set_pos(float x_aim, float y_aim)
{
//	if(g_run_flag_1 == 0&&g_run_flag_2 == 0)
//	{
		int32_t pulse_temp = 0;
		/****处理X坐标***/
		LIMIT(x_aim,X_MIN,X_MAX);//对x限幅
		pulse_temp = (int32_t)((x_aim-g_stepper_x.pos_mm) / X_STEP_MM);
		
		/**判断方向**/
		if((x_aim-g_stepper_x.pos_mm)>=0)
		{
			g_stepper_x.pulse_count = pulse_temp;
			g_stepper_x.dir = CW;//顺时针
		}
		else
		{
			g_stepper_x.pulse_count = -pulse_temp;
			g_stepper_x.dir = CCW;//逆时针
		}
		/* 启动 */
		if(g_stepper_x.pulse_count <= 10) 
		{
			stepper_stop(1);
		}
		else stepper_star(1,g_stepper_x.dir);
		
		/****处理Y坐标***/
		LIMIT(y_aim,Y_MIN,Y_MAX);//对y限幅
		pulse_temp = (int32_t)((y_aim-g_stepper_y.pos_mm) / Y_STEP_MM);
		/**判断方向**/
		if((y_aim-g_stepper_y.pos_mm)>=0)
		{
			g_stepper_y.pulse_count = pulse_temp;
			g_stepper_y.dir = CW;//顺时针
		}
		else
		{
			g_stepper_y.pulse_count = -pulse_temp;
			g_stepper_y.dir = CCW;//逆时针
		}
		/* 启动 */
		if(g_stepper_y.pulse_count <= 10) //你还真别说，你要敢给0，他就敢出bug，至于为什么呢？1个脉冲真没必要启动这个PWM
		{
			stepper_stop(2);
		}
		else stepper_star(2,g_stepper_y.dir);
		return 1;
	
//	}
//	else
//		return 0;

}
