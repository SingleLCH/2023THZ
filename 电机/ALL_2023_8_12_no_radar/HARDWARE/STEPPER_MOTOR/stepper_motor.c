
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
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_NOPULL;       	//��������
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
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
 * @brief       ��ʼ������������IO��, ��ʹ��ʱ��
 * @param       arr: �Զ���װֵ
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void stepper_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    STEPPER_DIR1_GPIO_CLK_ENABLE();                                 /* DIR1ʱ��ʹ�� */
    STEPPER_DIR2_GPIO_CLK_ENABLE();                                 /* DIR2ʱ��ʹ�� */

    STEPPER_ENA2_GPIO_CLK_ENABLE();                                  /* EN1ʱ��ʹ�� */
    STEPPER_ENA2_GPIO_CLK_ENABLE();                                  /* EN2ʱ��ʹ�� */

    GPIO_InitStructure.Pin = STEPPER_DIR1_GPIO_PIN;                   /* DIR1���� */
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;                    /* ������� */
    GPIO_InitStructure.Pull = GPIO_PULLDOWN;                          /* ���� */
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;                   /* ���� */
    HAL_GPIO_Init(STEPPER_DIR1_GPIO_PORT, &GPIO_InitStructure);       /* ��ʼ��DIR1���� */

    GPIO_InitStructure.Pin = STEPPER_DIR2_GPIO_PIN;                   /* DIR2���� */
	HAL_GPIO_Init(STEPPER_DIR2_GPIO_PORT, &GPIO_InitStructure);       /* ��ʼ��DIR2���� */
	
    /*   �ѻ����ų�ʼ��   */
    
    GPIO_InitStructure.Pin = STEPPER_ENA1_GPIO_PIN;                    /* EN1���� */
    HAL_GPIO_Init(STEPPER_ENA1_GPIO_PORT, &GPIO_InitStructure);        /* ��ʼ��EN1���� */
    
    GPIO_InitStructure.Pin = STEPPER_ENA2_GPIO_PIN;                    /* EN2���� */
    HAL_GPIO_Init(STEPPER_ENA2_GPIO_PORT, &GPIO_InitStructure);        /* ��ʼ��EN2���� */
	
	switch_init();
    
//    GTIM_TIM3_PWM_Init(arr, psc);                                /* ��ʼ��PUL���ţ��Լ�����ģʽ�� */
//	GTIM_TIM4_PWM_Init(arr, psc);                                /* ��ʼ��PUL���ţ��Լ�����ģʽ�� */
}

/**
 * @brief       �����������
 * @param       motor_num: ��������ӿ����
 * @param       dir      : ���������ת����
 * @retval      ��
 */
void stepper_star(uint8_t motor_num, uint8_t dir)
{
//	HAL_TIM_PWM_Start_IT(&g_gtimx_handle);//ʹ����OC�ж�
    switch(motor_num)
    {
        case STEPPER_MOTOR_1 :
        {
			HAL_TIM_Base_Start_IT(&GTIM3_Handler);
            ST1_DIR(dir);
            HAL_TIM_PWM_Start(&GTIM3_Handler, GTIM_TIM3_PWM_CH1);     /* ������ӦPWMͨ�� */
            break;
        }
        case STEPPER_MOTOR_2 :
        {
			HAL_TIM_Base_Start_IT(&GTIM4_Handler);
            ST2_DIR(dir);
            HAL_TIM_PWM_Start(&GTIM4_Handler, GTIM_TIM4_PWM_CH2);     /* ������ӦPWMͨ�� */
            break;
        }
        default : break;
    }
}

/**
 * @brief       �رղ������
 * @param       motor_num: ��������ӿ����
 * @retval      ��
 */
void stepper_stop(uint8_t motor_num)
{
    switch(motor_num)
    {
        case STEPPER_MOTOR_1 :
        {
			HAL_TIM_Base_Stop_IT(&GTIM3_Handler);
            HAL_TIM_PWM_Stop(&GTIM3_Handler, GTIM_TIM3_PWM_CH1);     /* �رն�ӦPWMͨ�� */
            break;
        }
        case STEPPER_MOTOR_2 :
        {
			HAL_TIM_Base_Stop_IT(&GTIM4_Handler);
            HAL_TIM_PWM_Stop(&GTIM4_Handler, GTIM_TIM4_PWM_CH2);     /* �رն�ӦPWMͨ�� */
            break;
        }
        default : break;
    }
}

/**
 * @brief       ����Ҫת���ĽǶ�ת����������
 * @param       angle    : ��Ҫת���ĽǶ�ֵ
 * @param       dir      : ��ת����
 * @param       motor_num: ��������ӿ����
 * @retval      ��
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
		/****����X����***/
		LIMIT(x_aim,X_MIN,X_MAX);//��x�޷�
		pulse_temp = (int32_t)((x_aim-g_stepper_x.pos_mm) / X_STEP_MM);
		
		/**�жϷ���**/
		if((x_aim-g_stepper_x.pos_mm)>=0)
		{
			g_stepper_x.pulse_count = pulse_temp;
			g_stepper_x.dir = CW;//˳ʱ��
		}
		else
		{
			g_stepper_x.pulse_count = -pulse_temp;
			g_stepper_x.dir = CCW;//��ʱ��
		}
		/* ���� */
		if(g_stepper_x.pulse_count <= 10) 
		{
			stepper_stop(1);
		}
		else stepper_star(1,g_stepper_x.dir);
		
		/****����Y����***/
		LIMIT(y_aim,Y_MIN,Y_MAX);//��y�޷�
		pulse_temp = (int32_t)((y_aim-g_stepper_y.pos_mm) / Y_STEP_MM);
		/**�жϷ���**/
		if((y_aim-g_stepper_y.pos_mm)>=0)
		{
			g_stepper_y.pulse_count = pulse_temp;
			g_stepper_y.dir = CW;//˳ʱ��
		}
		else
		{
			g_stepper_y.pulse_count = -pulse_temp;
			g_stepper_y.dir = CCW;//��ʱ��
		}
		/* ���� */
		if(g_stepper_y.pulse_count <= 10) //�㻹���˵����Ҫ�Ҹ�0�����͸ҳ�bug������Ϊʲô�أ�1��������û��Ҫ�������PWM
		{
			stepper_stop(2);
		}
		else stepper_star(2,g_stepper_y.dir);
		return 1;
	
//	}
//	else
//		return 0;

}
