#include "mpu_i2c.h"
#include "math.h"
#include "imu_filter.h"

float Angle_ax=0,Angle_ay=0,Angle_az=0;  	//由加速度计算的加速度(弧度制)
float Angle_gy=0,Angle_gx=0,Angle_gz=0;  	//由角速度计算的角速率(角度制)
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


#define pi 3.14159265f                           
#define Kp 10.0f                        
#define Ki 0.008f                         
#define halfT 0.004f        


float q0=1,q1=0,q2=0,q3=0;   
float exInt=0,eyInt=0,ezInt=0;  

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	float norm;
  float vx, vy, vz;
  float ex, ey, ez;

  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
  float q0q3 = q0*q3;
  float q1q1 = q1*q1;
  float q1q2 = q1*q2;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;

  norm = sqrt(ax*ax + ay*ay + az*az);      
  ax = ax /norm;
  ay = ay / norm;
  az = az / norm;
           
  vx = 2*(q1q3 - q0q2);												
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;

  ex = (ay*vz - az*vy) ;                           					
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;

  exInt = exInt + ex * Ki;								 
  eyInt = eyInt + ey * Ki;
  ezInt = ezInt + ez * Ki;

  gx = gx + Kp*ex + exInt;					   							
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt;				   							
					   
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;

  Roll	=	asin(2*(q0*q2-q1*q3 ))* 57.2957795f; // 俯仰
  Pitch	=	asin(2*(q0*q1+q2*q3 ))* 57.2957795f; // 横滚
}
void IMU_Filter(void)
{
	Angle_ay = (float)GetData(ACCEL_YOUT_H);	//读取6050数据
	Angle_ax = (float)GetData(ACCEL_XOUT_H);		   
	Angle_az = (float)GetData(ACCEL_ZOUT_H);	     
	Angle_gx = (float)GetData(GYRO_XOUT_H);
	Angle_gy = (float)GetData(GYRO_YOUT_H);
	Angle_gz = (float)GetData(GYRO_ZOUT_H);

	Angle_ax=(Angle_ax)/8192;  //加速度处理
	Angle_az=(Angle_az)/8192;  //加速度量程 +-4g/S
	Angle_ay=(Angle_ay)/8192;	//转换关系8192LSB/g
	Angle_gx=(Angle_gx)/16.4;   //陀螺仪处理
	Angle_gy=(Angle_gy)/16.4;   //陀螺仪量程 +-500度/S
	Angle_gz=(Angle_gz)/16.4;   //转换关系65.5LSB/度
	
	IMUupdate(Angle_gx*0.0609756,Angle_gy*0.0609756,Angle_gz*0.0609756,Angle_ax,Angle_ay,Angle_az);
}


