



#include "mpu_i2c.h"
#include "math.h"
#include "imu_filter.h"

float Angle_ax=0,Angle_ay=0,Angle_az=0;  	//由加速度计算的加速度(弧度制)
float Angle_gy=0,Angle_gx=0,Angle_gz=0;  	//由角速度计算的角速率(角度制)
short int g_x=0,g_y=0,g_z=0;            	//陀螺仪矫正参数
float Yaw=0.00;
float Roll=0,Pitch=0;
float Accel_Angle;

// float K1 =0.02; 

	
/**************************实现函数********************************************
*函数原型:		void MPU6050_initialize(void)
*功　　能:	    初始化 	MPU6050 以进入可用状态。
*******************************************************************************/
void MPU6050_initialize(void)
{
	MPU_I2C_Init_IO();

	RCC->APB2ENR|=1<<4;     //使能PORTC时钟
	RCC->APB2ENR|=1<<0;     //开启辅助时钟		  
	GPIOC->CRH&=0XF0FFFFFF; //PC14设置成输入	  
	GPIOC->CRH|=0X08000000;
// 	Ex_NVIC_Config(GPIO_C,14,FTIR); 			//下降沿触发
// 	MY_NVIC_Init(0,0,EXTI15_10_IRQn,2);    //抢占2，子优先级2，组2
	
	MPU_Single_Write(PWR_MGMT_1, 		0x00,	MPU6050_DEVADDR);		//解除休眠状态
	MPU_Single_Write(SMPLRT_DIV, 		0x07,	MPU6050_DEVADDR);  	//陀螺仪125hz
	MPU_Single_Write(CONFIG, 				0x1B,	MPU6050_DEVADDR);   //21HZ滤波 延时A8.5ms G8.3ms  此处取值应相当注意，延时与系统周期相近为宜
	MPU_Single_Write(GYRO_CONFIG, 	0x18,	MPU6050_DEVADDR); 	//陀螺仪2000度/S 65.5LSB/g
	MPU_Single_Write(ACCEL_CONFIG, 	0x00,	MPU6050_DEVADDR);		//加速度+-2g  8192LSB/g
}
//**************************************
//合成数据
//**************************************
short int GetData(unsigned char REG_Address)
{
	short int Data_H,Data_L;
	Data_H=MPU_Single_Read(REG_Address,MPU6050_DEVADDR);
	Data_L=MPU_Single_Read(REG_Address+1,MPU6050_DEVADDR);
	return (Data_H<<8)+Data_L;   //合成数据
}
void IMU_Filter(void)
{
	Angle_ay= GetData(ACCEL_YOUT_H);	//读取6050数据
	Angle_ax= GetData(ACCEL_XOUT_H);		   
	Angle_az= GetData(ACCEL_ZOUT_H);	     
	Angle_gx = GetData(GYRO_XOUT_H)-g_x;
	Angle_gy = GetData(GYRO_YOUT_H)-g_y;
	Angle_gz = GetData(GYRO_ZOUT_H)-g_z;

// 	Accel_Angle=atan2(Angle_ay,Angle_az)*57.2957623f;          //计算倾角	
// 	Angle_gx=Angle_gx/16.4;                                    //陀螺仪量程转换	

	Roll 	= 0.02 * atan2(Angle_ay,Angle_az)*57.2957623f + (1-0.02) * (Roll + Angle_gx/16.4 * 0.005);
	Pitch = 0.02 * atan2(Angle_ax,Angle_az)*57.2957623f + (1-0.02) * (Pitch - Angle_gy/16.4 * 0.005);
}


