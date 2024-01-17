#include "mpu_i2c.h"
#include "math.h"
#include "imu_filter.h"

float Angle_ax=0,Angle_ay=0,Angle_az=0;  	//由加速度计算的加速度(弧度制)
float Angle_gy=0,Angle_gx=0,Angle_gz=0;  	//由角速度计算的角速率(角度制)
short int g_x=0,g_y=0,g_z=0;            	//陀螺仪矫正参数
float Yaw=0.00;
float Roll=0,Pitch=0;
float Accel_Angle;



	
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


double Angle=0,Angley=0;
float  dt=0.005;    //定时器周期
//******卡尔曼滤波参数-X轴*************************************
float  Q_angle=0.001;     //对加速度计的信任度
float  Q_gyro=0.003;      //对陀螺仪的信任度
float  R_angle=0.5;
char   C_0 = 1;
float  Q_bias, Angle_err;
float  PCt_0, PCt_1, E;
float  K_0, K_1, t_0, t_1;
float  Pdot[4] ={0,0,0,0};
float  PP[2][2] = { { 1, 0 },{ 0, 1 } }; 
//******卡尔曼滤波参数-Y轴*************************************
float  Q_biasy, Angle_erry;
float  PCt_0y, PCt_1y, Ey;
float  K_0y, K_1y, t_0y, t_1y;
float  Pdoty[4] ={0,0,0,0};
float  PPy[2][2] = { { 1, 0 },{ 0, 1 } };

void Kalman_Filter(float Accel,float Gyro)		
{
	Angle+=(Gyro - Q_bias) * dt; //陀螺仪积分角度（测量值-陀螺仪偏差)*dt  
//Angle相当于是系统的预测值
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // 先验估计误差协方差的微分
	Pdot[1]=- PP[1][1];
	Pdot[2]=- PP[1][1];
	Pdot[3]=  Q_gyro;
	PP[0][0] += Pdot[0] * dt;   // 先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
	Angle_err = Accel - Angle;//估计值与预测值之间的偏差
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	E = R_angle + C_0 * PCt_0;
	K_0 = PCt_0 / E;//卡尔曼增益1 用于计算最优估计值
	K_1 = PCt_1 / E;//卡尔曼增益2 用于计算最后估计的偏差
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];
	PP[0][0] -= K_0 * t_0;		 //更新协方差矩阵
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
	Angle	+= K_0 * Angle_err;//根据卡尔曼增益1算出最优角度
	Q_bias	+= K_1 * Angle_err;//根据卡尔曼增益2算出预测值偏差
	
// 	Angle+=(Gyro - Q_bias) * dt; //先验估计
// 	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

// 	Pdot[1]=-PP[1][1];
// 	Pdot[2]=-PP[1][1];
// 	Pdot[3]=Q_gyro;
// 	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
// 	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
// 	PP[1][0] += Pdot[2] * dt;
// 	PP[1][1] += Pdot[3] * dt;
// 		
// 	Angle_err = Accel - Angle;	//zk-先验估计
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

// 	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
// 	PP[0][1] -= K_0 * t_1;
// 	PP[1][0] -= K_1 * t_0;
// 	PP[1][1] -= K_1 * t_1;
// 		
// 	Angle	+= K_0 * Angle_err;	 //后验估计
// 	Q_bias	+= K_1 * Angle_err;	 //后验估计
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
	
	
// 	Angley+=(Gyroy - Q_biasy) * dt; //先验估计
// 	Pdoty[0]=Q_angle - PPy[0][1] - PPy[1][0]; // Pk-先验估计误差协方差的微分

// 	Pdoty[1]=-PPy[1][1];
// 	Pdoty[2]=-PPy[1][1];
// 	Pdoty[3]=Q_gyro;
// 	PPy[0][0] += Pdoty[0] * dt;   // Pk-先验估计误差协方差微分的积分
// 	PPy[0][1] += Pdoty[1] * dt;   // =先验估计误差协方差
// 	PPy[1][0] += Pdoty[2] * dt;
// 	PPy[1][1] += Pdoty[3] * dt;
// 		
// 	Angle_erry = Accely - Angley;	//zk-先验估计
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

// 	PPy[0][0] -= K_0y * t_0y;		 //后验估计误差协方差
// 	PPy[0][1] -= K_0y * t_1y;
// 	PPy[1][0] -= K_1y * t_0y;
// 	PPy[1][1] -= K_1y * t_1y;
// 		
// 	Angley	+= K_0y * Angle_erry;	 //后验估计
// 	Q_biasy	+= K_1y * Angle_erry;	 //后验估计
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

double AngleAx=0,AngleAy=0;               //初步处理的角度值
void IMU_Filter(void)
{
	Angle_ay= GetData(ACCEL_YOUT_H);	//读取6050数据
	Angle_ax= GetData(ACCEL_XOUT_H);		   
	Angle_az= GetData(ACCEL_ZOUT_H);	     
	Angle_gx = GetData(GYRO_XOUT_H)-g_x;
	Angle_gy = GetData(GYRO_YOUT_H)-g_y;
	Angle_gz = GetData(GYRO_ZOUT_H)-g_z;	
	
// 	Angle_ax=(Angle_ax)/8192;  //加速度处理
// 	Angle_az=(Angle_az)/8192;  //加速度量程 +-4g/S
// 	Angle_ay=(Angle_ay)/8192;	//转换关系8192LSB/g
	Angle_gx=(Angle_gx)/16.4;   //陀螺仪处理
	Angle_gy=(Angle_gy)/16.4;   //陀螺仪量程 +-500度/S
	Angle_gz=(Angle_gz)/16.4;   //转换关系65.5LSB/度
	
// //********************三角函数直接解算以供比较四元数解算精准度********************
// 	AngleAx=atan(Angle_ax/sqrt(Angle_ay*Angle_ay+Angle_az*Angle_az))*57.2957795f; //后面的数字是180/PI 目的是弧度转角度
//   AngleAy=atan(Angle_ay/sqrt(Angle_ax*Angle_ax+Angle_az*Angle_az))*57.2957795f;
// 	
// 	
  if(Angle_az<0){Angle_gy=-Angle_gy;}
	Kalman_Filter(atan2(Angle_ax,Angle_az)*57.2957795f,-Angle_gy);       //卡尔曼滤波

	
	if(Angle_az<0){Angle_gx=-Angle_gx;}
	Kalman_Filter_Y(atan2(Angle_ay,Angle_az)*57.2957795f,Angle_gx);      //卡尔曼滤波
	
	Roll=Angle;
	Pitch=Angley;
}


