#include "mpu_i2c.h"
#include "math.h"
#include "imu_filter.h"

float Angle_ax=0,Angle_ay=0,Angle_az=0;  	//�ɼ��ٶȼ���ļ��ٶ�(������)
float Angle_gy=0,Angle_gx=0,Angle_gz=0;  	//�ɽ��ٶȼ���Ľ�����(�Ƕ���)
short int g_x=0,g_y=0,g_z=0;            	//�����ǽ�������
float Yaw=0.00;
float Roll=0,Pitch=0;
float Accel_Angle;



	
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
	
	MPU_Single_Write(PWR_MGMT_1, 		0x00,	MPU6050_DEVADDR);		//�������״̬
	MPU_Single_Write(SMPLRT_DIV, 		0x07,	MPU6050_DEVADDR);  	//������125hz
	MPU_Single_Write(CONFIG, 				0x1B,	MPU6050_DEVADDR);   //21HZ�˲� ��ʱA8.5ms G8.3ms  �˴�ȡֵӦ�൱ע�⣬��ʱ��ϵͳ�������Ϊ��
	MPU_Single_Write(GYRO_CONFIG, 	0x18,	MPU6050_DEVADDR); 	//������2000��/S 65.5LSB/g
	MPU_Single_Write(ACCEL_CONFIG, 	0x00,	MPU6050_DEVADDR);		//���ٶ�+-2g  8192LSB/g
}


double Angle=0,Angley=0;
float  dt=0.005;    //��ʱ������
//******�������˲�����-X��*************************************
float  Q_angle=0.001;     //�Լ��ٶȼƵ����ζ�
float  Q_gyro=0.003;      //�������ǵ����ζ�
float  R_angle=0.5;
char   C_0 = 1;
float  Q_bias, Angle_err;
float  PCt_0, PCt_1, E;
float  K_0, K_1, t_0, t_1;
float  Pdot[4] ={0,0,0,0};
float  PP[2][2] = { { 1, 0 },{ 0, 1 } }; 
//******�������˲�����-Y��*************************************
float  Q_biasy, Angle_erry;
float  PCt_0y, PCt_1y, Ey;
float  K_0y, K_1y, t_0y, t_1y;
float  Pdoty[4] ={0,0,0,0};
float  PPy[2][2] = { { 1, 0 },{ 0, 1 } };

void Kalman_Filter(float Accel,float Gyro)		
{
	Angle+=(Gyro - Q_bias) * dt; //�����ǻ��ֽǶȣ�����ֵ-������ƫ��)*dt  
//Angle�൱����ϵͳ��Ԥ��ֵ
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // ����������Э�����΢��
	Pdot[1]=- PP[1][1];
	Pdot[2]=- PP[1][1];
	Pdot[3]=  Q_gyro;
	PP[0][0] += Pdot[0] * dt;   // ����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
	Angle_err = Accel - Angle;//����ֵ��Ԥ��ֵ֮���ƫ��
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	E = R_angle + C_0 * PCt_0;
	K_0 = PCt_0 / E;//����������1 ���ڼ������Ź���ֵ
	K_1 = PCt_1 / E;//����������2 ���ڼ��������Ƶ�ƫ��
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];
	PP[0][0] -= K_0 * t_0;		 //����Э�������
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
	Angle	+= K_0 * Angle_err;//���ݿ���������1������ŽǶ�
	Q_bias	+= K_1 * Angle_err;//���ݿ���������2���Ԥ��ֵƫ��
	
// 	Angle+=(Gyro - Q_bias) * dt; //�������
// 	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

// 	Pdot[1]=-PP[1][1];
// 	Pdot[2]=-PP[1][1];
// 	Pdot[3]=Q_gyro;
// 	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
// 	PP[0][1] += Pdot[1] * dt;   // =����������Э����
// 	PP[1][0] += Pdot[2] * dt;
// 	PP[1][1] += Pdot[3] * dt;
// 		
// 	Angle_err = Accel - Angle;	//zk-�������
// 	
// 	PCt_0 = C_0 * PP[0][0];
// 	PCt_1 = C_0 * PP[1][0];
// 	
// 	E = R_angle + C_0 * PCt_0;
// 	
// 	K_0 = PCt_0 / E;
// 	K_1 = PCt_1 / E;
// 	
// 	t_0 = PCt_0;
// 	t_1 = C_0 * PP[0][1];

// 	PP[0][0] -= K_0 * t_0;		 //����������Э����
// 	PP[0][1] -= K_0 * t_1;
// 	PP[1][0] -= K_1 * t_0;
// 	PP[1][1] -= K_1 * t_1;
// 		
// 	Angle	+= K_0 * Angle_err;	 //�������
// 	Q_bias	+= K_1 * Angle_err;	 //�������
}
void Kalman_Filter_Y(float Accely,float Gyroy)		
{
	Angley+=(Gyroy - Q_biasy) * dt; 
	Pdoty[0]=Q_angle - PPy[0][1] - PPy[1][0]; 
	Pdoty[1]=- PPy[1][1];
	Pdoty[2]=- PPy[1][1];
	Pdoty[3]=Q_gyro;
	PPy[0][0] += Pdoty[0] * dt;   
	PPy[0][1] += Pdoty[1] * dt;   
	PPy[1][0] += Pdoty[2] * dt;
	PPy[1][1] += Pdoty[3] * dt;
	Angle_erry = Accely - Angley;	
	PCt_0y = C_0 * PPy[0][0];
	PCt_1y = C_0 * PPy[1][0];
	Ey = R_angle + C_0 * PCt_0y;
	K_0y = PCt_0y / Ey;
	K_1y = PCt_1y / Ey;
	t_0y = PCt_0y;
	t_1y = C_0 * PPy[0][1];
	PPy[0][0] -= K_0y * t_0y;		 
	PPy[0][1] -= K_0y * t_1y;
	PPy[1][0] -= K_1y * t_0y;
	PPy[1][1] -= K_1y * t_1y;
	Angley	+= K_0y * Angle_erry;	 
	Q_biasy	+= K_1y * Angle_erry;	
	
	
// 	Angley+=(Gyroy - Q_biasy) * dt; //�������
// 	Pdoty[0]=Q_angle - PPy[0][1] - PPy[1][0]; // Pk-����������Э�����΢��

// 	Pdoty[1]=-PPy[1][1];
// 	Pdoty[2]=-PPy[1][1];
// 	Pdoty[3]=Q_gyro;
// 	PPy[0][0] += Pdoty[0] * dt;   // Pk-����������Э����΢�ֵĻ���
// 	PPy[0][1] += Pdoty[1] * dt;   // =����������Э����
// 	PPy[1][0] += Pdoty[2] * dt;
// 	PPy[1][1] += Pdoty[3] * dt;
// 		
// 	Angle_erry = Accely - Angley;	//zk-�������
// 	
// 	PCt_0y = C_0 * PPy[0][0];
// 	PCt_1y = C_0 * PPy[1][0];
// 	
// 	Ey = R_angle + C_0 * PCt_0y;
// 	
// 	K_0y = PCt_0y / Ey;
// 	K_1y = PCt_1y / Ey;
// 	
// 	t_0y = PCt_0y;
// 	t_1y = C_0 * PPy[0][1];

// 	PPy[0][0] -= K_0y * t_0y;		 //����������Э����
// 	PPy[0][1] -= K_0y * t_1y;
// 	PPy[1][0] -= K_1y * t_0y;
// 	PPy[1][1] -= K_1y * t_1y;
// 		
// 	Angley	+= K_0y * Angle_erry;	 //�������
// 	Q_biasy	+= K_1y * Angle_erry;	 //�������
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

double AngleAx=0,AngleAy=0;               //��������ĽǶ�ֵ
void IMU_Filter(void)
{
	Angle_ay= GetData(ACCEL_YOUT_H);	//��ȡ6050����
	Angle_ax= GetData(ACCEL_XOUT_H);		   
	Angle_az= GetData(ACCEL_ZOUT_H);	     
	Angle_gx = GetData(GYRO_XOUT_H)-g_x;
	Angle_gy = GetData(GYRO_YOUT_H)-g_y;
	Angle_gz = GetData(GYRO_ZOUT_H)-g_z;	
	
// 	Angle_ax=(Angle_ax)/8192;  //���ٶȴ���
// 	Angle_az=(Angle_az)/8192;  //���ٶ����� +-4g/S
// 	Angle_ay=(Angle_ay)/8192;	//ת����ϵ8192LSB/g
	Angle_gx=(Angle_gx)/16.4;   //�����Ǵ���
	Angle_gy=(Angle_gy)/16.4;   //���������� +-500��/S
	Angle_gz=(Angle_gz)/16.4;   //ת����ϵ65.5LSB/��
	
// //********************���Ǻ���ֱ�ӽ����Թ��Ƚ���Ԫ�����㾫׼��********************
// 	AngleAx=atan(Angle_ax/sqrt(Angle_ay*Angle_ay+Angle_az*Angle_az))*57.2957795f; //�����������180/PI Ŀ���ǻ���ת�Ƕ�
//   AngleAy=atan(Angle_ay/sqrt(Angle_ax*Angle_ax+Angle_az*Angle_az))*57.2957795f;
// 	
// 	
  if(Angle_az<0){Angle_gy=-Angle_gy;}
	Kalman_Filter(atan2(Angle_ax,Angle_az)*57.2957795f,-Angle_gy);       //�������˲�

	
	if(Angle_az<0){Angle_gx=-Angle_gx;}
	Kalman_Filter_Y(atan2(Angle_ay,Angle_az)*57.2957795f,Angle_gx);      //�������˲�
	
	Roll=Angle;
	Pitch=Angley;
}


