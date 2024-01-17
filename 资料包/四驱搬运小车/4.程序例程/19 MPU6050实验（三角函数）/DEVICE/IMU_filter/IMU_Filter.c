



#include "mpu_i2c.h"
#include "math.h"
#include "imu_filter.h"

float Angle_ax=0,Angle_ay=0,Angle_az=0;  	//�ɼ��ٶȼ���ļ��ٶ�(������)
float Angle_gy=0,Angle_gx=0,Angle_gz=0;  	//�ɽ��ٶȼ���Ľ�����(�Ƕ���)
short int g_x=0,g_y=0,g_z=0;            				//�����ǽ�������							
float Yaw=0.00;
float Roll=0,Pitch=0;

	
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_initialize(void)
*��������:	    ��ʼ�� 	MPU6050 �Խ������״̬��
*******************************************************************************/
void MPU6050_initialize(void)
{
	MPU_I2C_Init_IO();

	RCC->APB2ENR|=1<<4;     //ʹ��PORTCʱ��
	RCC->APB2ENR|=1<<0;     //��������ʱ��		  
	GPIOC->CRH&=0XF0FFFFFF; //PC14���ó�����	  
	GPIOC->CRH|=0X08000000;
// 	Ex_NVIC_Config(GPIO_C,14,FTIR); 			//�½��ش���
// 	MY_NVIC_Init(0,0,EXTI15_10_IRQn,2);    //��ռ2�������ȼ�2����2
	MPU_Single_Write(PWR_MGMT_1, 		0x00,	MPU6050_DEVADDR);	//�������״̬
	MPU_Single_Write(SMPLRT_DIV, 		0x07,	MPU6050_DEVADDR);  //������125hz
	MPU_Single_Write(CONFIG, 				0x1B,	MPU6050_DEVADDR); //42HZ�˲� ��ʱA4.8ms G8.3ms  �˴�ȡֵӦ�൱ע�⣬��ʱ��ϵͳ�������Ϊ��
	MPU_Single_Write(GYRO_CONFIG, 	0x18,	MPU6050_DEVADDR); //������2000��/S 65.5LSB/g
	MPU_Single_Write(ACCEL_CONFIG, 	0x00,	MPU6050_DEVADDR);//���ٶ�+-2g  8192LSB/g
	
}






//**************************************
//�ϳ�����
//**************************************
short int GetData(unsigned char REG_Address)
{
	short int Data_H,Data_L;
	Data_H=MPU_Single_Read(REG_Address,MPU6050_DEVADDR);
	Data_L=MPU_Single_Read(REG_Address+1,MPU6050_DEVADDR);
	return (Data_H<<8)+Data_L;   //�ϳ�����
}

void IMU_Filter(void)
{
	Angle_ay= GetData(ACCEL_YOUT_H);	//��ȡ6050����
	Angle_ax= GetData(ACCEL_XOUT_H);		   
	Angle_az= GetData(ACCEL_ZOUT_H);	     
	Angle_gx = GetData(GYRO_XOUT_H)-g_x;
	Angle_gy = GetData(GYRO_YOUT_H)-g_y;
	Angle_gz = GetData(GYRO_ZOUT_H)-g_z;	
	
	Angle_ax=(Angle_ax)/8192;  //���ٶȴ���
	Angle_az=(Angle_az)/8192;  //���ٶ����� +-4g/S
	Angle_ay=(Angle_ay)/8192;	//ת����ϵ8192LSB/g
	Angle_gx=(Angle_gx)/16.4;   //�����Ǵ���
	Angle_gy=(Angle_gy)/16.4;   //���������� +-500��/S
	Angle_gz=(Angle_gz)/16.4;   //ת����ϵ65.5LSB/��
	
//********************���Ǻ���ֱ�ӽ����Թ��Ƚ���Ԫ�����㾫׼��********************
	Roll=atan(Angle_ax/sqrt(Angle_ay*Angle_ay+Angle_az*Angle_az))*57.2957795f; //�����������180/PI Ŀ���ǻ���ת�Ƕ�
  Pitch=atan(Angle_ay/sqrt(Angle_ax*Angle_ax+Angle_az*Angle_az))*57.2957795f;
}


