#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "lcd.h"
#include "ltdc.h"
#include "sdram.h"
#include "touch.h"
#include "lv_port_indev_template.h"
#include "lv_port_disp_template.h"
#include "../../lvgl.h"
#include "stepper_motor.h"
#include "radar.h"
#include "pcf8574.h"


#define scr_act_width()     300
#define scr_act_height()    25


static lv_obj_t* win;
static lv_obj_t* btn;
static lv_obj_t* label_name;
static lv_obj_t* Login; //按钮
static lv_obj_t* login; //窗口
static lv_obj_t* Main;
LV_IMG_DECLARE(THZ);
LV_IMG_DECLARE(ll);
LV_IMG_DECLARE(qrcode);
LV_IMG_DECLARE(water);
LV_IMG_DECLARE(Ceramic);
LV_IMG_DECLARE(metal);

uint8_t key,t;
uint8_t i=0;
uint8_t in=0;
uint8_t keyup_first_flag = 1;
uint8_t motor_index =1;
volatile double x_now = 0,y_now = 0;//当前出发位置
volatile double x_aim = 0,y_aim = 0;//当前目标位置
volatile int8_t scan_dir = 1;//1表示从小到大，-1表示从大到小
volatile int8_t scan_state = -3;

//state = -2为扫描开始前状态
//state = 9为扫描结束后状态					
uint8_t m = 0, w = 0, c = 0;
/************************
			state状态转移图
		①
		↑
		⑧/⑨
		↑
		⑦←←←←⑥←←←←⑤
						   ↑
						   ④/⑨
						   ↑
		①→→→→②→→→→③
		↑
		⑧
		↑
		⑦←←←←⑥←←←←⑤
						   ↑
						   ④
						   ↑
  -1→0→①→→→→②→→→→③
	
***************************/
void motor_scan(float x_min,float y_min ,float x_max,float y_max, volatile float division)
{
	static uint8_t count = 0;
	static uint8_t beep_times = 0;
	
	switch(scan_state)
	{
		case -2:
		{
			/* 更新出发位置 */
			x_now = g_stepper_x.pos_mm;  // 更新出发位置
			y_now = g_stepper_y.pos_mm;  // 更新出发位置
		}
		break;
		case -1:
		{
			/* 计算新的目标位置 */
			x_aim = x_min;
			y_aim = y_min;
			/* 前往新的目标位置 */
			if(stepper_set_pos(x_aim,y_aim))
				scan_state = 0; //状态转移 -1-->0
		}break;
		case 0:
		{
			if(g_run_flag_1 ==1||g_run_flag_2 ==1)
				break;
			else if(g_run_flag_1 == 0&&g_run_flag_2 == 0)
			{
				printf("a");
				/* 已到达目标位置，更新出发位置 */
				x_now = g_stepper_x.pos_mm;  // 更新出发位置
				y_now = g_stepper_y.pos_mm;  // 更新出发位置
				scan_state = 1;  // 状态转移 0-->1
			}
		}break;
		case 1:
		{
			/* 计算新的目标位置 */
			x_aim = x_max;
			y_aim = y_now;
			/* 前往新的目标位置 */
//			HAL_UART_Receive_IT(&huart5,radar_rxDataBufTemp,1);				//Begin to receive data
			
			if(stepper_set_pos(x_aim,y_aim))	
				scan_state = 2; //状态转移 1-->2
		}break;
		case 2:
		{
			if(g_run_flag_1 ==1||g_run_flag_2 ==1)
				break;
			else if(g_run_flag_1 == 0&&g_run_flag_2 == 0)
			{
				/* 已到达目标位置，更新出发位置 */
				x_now = g_stepper_x.pos_mm;  // 更新出发位置
				y_now = g_stepper_y.pos_mm;  // 更新出发位置
				scan_state = 3; //状态转移 2-->3
			}
		}break;
		case 3:
		{
			/* 计算新的目标位置 */
			x_aim = x_now;
			y_aim = y_now + division;//换行
			/* 前往新的目标位置 */
			if(y_now>y_max)
			{
				time_ms = 0;
				scan_state = 9;//状态转移 3-->9
			}
			else
			{
				if(stepper_set_pos(x_aim,y_aim))		
					scan_state = 4; //状态转移 3-->4
			}
		}break;
		case 4:
		{
			if(g_run_flag_1 ==1||g_run_flag_2 ==1)
				break;
			else if(g_run_flag_1 == 0&&g_run_flag_2 == 0)
			{
				/* 已到达目标位置，更新出发位置 */
				x_now = g_stepper_x.pos_mm;  // 更新出发位置
				y_now = g_stepper_y.pos_mm;  // 更新出发位置
				scan_state = 5; //状态转移 4-->5
			}
			
		}break;
		case 5:
		{
			/* 计算新的目标位置 */
			x_aim = x_min;
			y_aim = y_now;
			/* 前往新的目标位置 */
			if(stepper_set_pos(x_aim,y_aim))		
				scan_state = 6; //状态转移 5-->6
		}break;
		case 6:
		{
			if(g_run_flag_1 ==1||g_run_flag_2 ==1)
				break;
			else if(g_run_flag_1 == 0&&g_run_flag_2 == 0)
			{
				/* 已到达目标位置，更新出发位置 */
				x_now = g_stepper_x.pos_mm;  // 更新出发位置
				y_now = g_stepper_y.pos_mm;  // 更新出发位置
				scan_state = 7; //状态转移 6-->7
			}
			
		}break;
		case 7:
		{
			/* 计算新的目标位置 */
			x_aim = x_now;
			y_aim = y_now + division;//换行
			/* 前往新的目标位置 */
			if(y_now>y_max)
			{
				time_ms = 0;
				scan_state = 9;//状态转移 7-->9
			}
			else
			{
				if(stepper_set_pos(x_aim,y_aim))	
					scan_state = 8; //状态转移 7-->8
			}
		}break;
		case 8:
		{
			if(g_run_flag_1 ==1||g_run_flag_2 ==1)
				break;
			else if(g_run_flag_1 == 0&&g_run_flag_2 == 0)
			{
				/* 已到达目标位置，更新出发位置 */
				x_now = g_stepper_x.pos_mm;  // 更新出发位置
				y_now = g_stepper_y.pos_mm;  // 更新出发位置
				scan_state = 1; //状态转移 8-->1
			}
			
		}break;
		case 9:
		{
			i = 1;
			
//			__HAL_UART_DISABLE_IT(&huart5, UART_IT_RXNE);
			
//			delay_ms(1000);
//			printf("a");
//			delay_ms(10);
//			printf("a");
//			delay_ms(10);
//			printf("a");
			if(time_ms >=2000)
			scan_state = 10; //状态转移 9-->10
		}break;
		case 10:
		{
			/* 扫描完成 */
			if(count == 12)
			{
				PCF8574_WriteBit(BEEP_IO, 1);	//控制蜂鸣器	
			}
			else if(count == 50)
			{
				PCF8574_WriteBit(BEEP_IO, 0);	//控制蜂鸣器	
				count = 0;
				beep_times++;
				printf("a\r\n");
			}
			if(beep_times==20)
			{
				scan_state = 11; //状态转移 10-->11
				printf("a");
			}
			count++;

		}break;
		case 11:
		{
			
		}break;
		default:break;

	}
}

//注:对于littleVGL库,最好不要采用-O的优化,保持为-O0不优化就可以了

#define TEST_NUM		1   //1,2,3分别对应3个例程



void btn_event_cb(lv_event_t *e){
	
	lv_event_code_t code = lv_event_get_code(e);
	if(code == LV_EVENT_CLICKED){
	 
		lv_obj_add_flag(win, LV_OBJ_FLAG_HIDDEN);
		
	}
	
}

void event_cb(lv_event_t *e){
	
	win = lv_win_create(lv_scr_act(),30);
	lv_obj_t*title = lv_win_add_title(win,"Prograss");
	lv_obj_set_size(win,300,200);
	lv_obj_set_x(win,10);
	lv_obj_set_y(win,20);
	lv_obj_set_style_border_width(win, 1, LV_STATE_DEFAULT);
	lv_obj_t*btn_close = lv_win_add_btn(win,LV_SYMBOL_CLOSE,30);
	lv_obj_add_event_cb(btn_close,btn_event_cb,LV_EVENT_CLICKED,NULL);
	 
	lv_obj_t* content = lv_win_get_content(win);
	lv_obj_t* bar = lv_bar_create(content);
	lv_obj_set_align(bar,LV_ALIGN_CENTER);
	lv_obj_set_size(bar, 200, 20);
	lv_obj_set_style_anim_time(bar,255000,LV_STATE_DEFAULT);
	lv_bar_set_value(bar,100,LV_ANIM_ON);
	
	if(in==1){
	lv_bar_set_start_value(bar,90,LV_ANIM_ON);
	lv_obj_set_style_anim_time(bar,200,LV_STATE_DEFAULT);
	lv_bar_set_value(bar,100,LV_ANIM_ON);
	}
	
	scan_state = -1;


}

void event_picture(lv_event_t *e){
	
	win = lv_win_create(Main,30);
	lv_obj_t*title = lv_win_add_title(win,"PictureResult");
	lv_obj_set_size(win,300,200);
	lv_obj_set_x(win,10);
	lv_obj_set_y(win,20);
	lv_obj_set_style_border_width(win, 1, LV_STATE_DEFAULT);
	lv_obj_t*btn_close = lv_win_add_btn(win,LV_SYMBOL_CLOSE,30);
	lv_obj_add_event_cb(btn_close,btn_event_cb,LV_EVENT_CLICKED,NULL);
	

	lv_obj_t*res = lv_img_create(win);
//		m=1;
	if(m==1){
	lv_img_set_src(res, &metal);
		
	}
	if(c==1){
	lv_img_set_src(res, &Ceramic);
	}
	if(w==1){
	lv_img_set_src(res, &water);
	}
	

}


void button_create(void){ 
	
	lv_obj_t*btn= lv_btn_create(Main);
	lv_obj_set_size(btn,120,60);
	lv_obj_set_x(btn,320);
	lv_obj_set_y(btn,90);
	lv_obj_add_event_cb(btn,event_picture,LV_EVENT_CLICKED,NULL);
	
	lv_obj_t*lable5 = lv_label_create(btn);
	lv_label_set_text(lable5, "Result");
	lv_obj_center(lable5);
	lv_obj_set_style_text_font(lable5,&lv_font_montserrat_16,LV_STATE_DEFAULT);
	
	lv_obj_t*btn1= lv_btn_create(Main);
	lv_obj_set_size(btn1,120,60);
	lv_obj_set_x(btn1,320);
	lv_obj_set_y(btn1,160);
	lv_obj_add_event_cb(btn1,event_cb,LV_EVENT_CLICKED,NULL);
	
	lv_obj_t*lable1 = lv_label_create(btn1);
	lv_label_set_text(lable1, "Motor");
	lv_obj_center(lable1);
	lv_obj_set_style_text_font(lable1,&lv_font_montserrat_16,LV_STATE_DEFAULT);
	
	lv_obj_t*qr = lv_img_create(Main);
	lv_img_set_src(qr, &qrcode);
	lv_obj_set_x(qr,60);
	lv_obj_set_y(qr,40);
	
	lv_obj_t*lable3 = lv_label_create(Main);
	lv_label_set_text(lable3, "Wifi status:Ready");
	lv_obj_set_x(lable3,320);
	lv_obj_set_y(lable3,5);
	lv_obj_set_style_text_font(lable3,&lv_font_montserrat_16,LV_STATE_DEFAULT);
	
	lv_obj_t*lable4 = lv_label_create(Main);
	lv_label_set_text(lable4, "Rader status:Ready");
	lv_obj_set_x(lable4,315);
	lv_obj_set_y(lable4,30);
	lv_obj_set_style_text_font(lable4,&lv_font_montserrat_16,LV_STATE_DEFAULT);
	
	lv_obj_t*lable2 = lv_label_create(Main);
	lv_label_set_text(lable2, "Please Scan The QR");
	lv_obj_set_x(lable2,60);
	lv_obj_set_y(lable2,3);
	lv_obj_set_style_text_font(lable2,&lv_font_montserrat_14,LV_STATE_DEFAULT);
	

}

static void Login_close(lv_event_t* e){
	
	lv_event_code_t code = lv_event_get_code(e);
	if(code == LV_EVENT_CLICKED){
	 
		lv_obj_add_flag(login, LV_OBJ_FLAG_HIDDEN);
		
	}

}


static void event_textarea_name_cb(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* textarea_name = lv_event_get_target(e);
	lv_obj_t* keyboard = (lv_obj_t*)e->user_data;
	
	if (LV_EVENT_FOCUSED == code) {
		lv_keyboard_set_textarea(keyboard, textarea_name);
		lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
	}
 
	else if (LV_EVENT_DEFOCUSED == code) {
		const char* txt = lv_textarea_get_text(textarea_name);
		if(strcmp(txt,"aa")==0){
			
			
		}
	}
}

static void event_textarea_pass_cb(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* textarea_pass = lv_event_get_target(e);
	lv_obj_t* keyboard = (lv_obj_t*)e->user_data;
	
	if (LV_EVENT_FOCUSED == code) {
		lv_keyboard_set_textarea(keyboard, textarea_pass);
		lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
	
	}
	
 else if (LV_EVENT_DEFOCUSED == code) {
		const char* txt = lv_textarea_get_text(textarea_pass);
		if(strcmp(txt,"p")==0){
		
			lv_obj_add_event_cb(Login, Login_close,LV_EVENT_CLICKED,NULL);
			lv_scr_load(Main);
		}
	}
}




static void lv_example_textarea(void) {
	const lv_font_t* font;
	

	login = lv_obj_create(NULL);
	lv_obj_set_size(login,480,272);
	lv_obj_set_style_border_width(login, 1, LV_STATE_DEFAULT);
	lv_scr_load(login);
	

	
	font = &lv_font_montserrat_14;
	lv_obj_t* textarea_name = lv_textarea_create(login);
	lv_obj_set_width(textarea_name, scr_act_width() / 2);
	lv_obj_set_style_text_font(textarea_name, font, LV_PART_MAIN);
	lv_obj_align_to(textarea_name, login, LV_ALIGN_TOP_MID, 0, 5); 
	lv_obj_align(lv_textarea_get_label(textarea_name),LV_ALIGN_LEFT_MID, 0, 0); 
	lv_textarea_set_one_line(textarea_name, true);
	lv_textarea_set_max_length(textarea_name, 20);
	lv_textarea_set_text_selection(textarea_name, true);


	label_name = lv_label_create(login);
	lv_obj_set_width(label_name, font->line_height * 9);
	lv_obj_set_height(label_name, font->line_height + 10);
	lv_obj_align_to(label_name, textarea_name, LV_ALIGN_OUT_LEFT_BOTTOM, -10, 0);
	lv_obj_set_style_text_font(label_name, font, LV_PART_MAIN);
	lv_label_set_text(label_name, "User name");
	lv_obj_set_style_text_align(label_name, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);


	Login = lv_imgbtn_create(login);
	lv_imgbtn_set_src(Login, LV_IMGBTN_STATE_RELEASED, NULL, &ll, NULL);
	lv_obj_set_size(Login,61,61);
	lv_obj_align_to(Login ,login, LV_ALIGN_TOP_RIGHT  ,-25,10);


	lv_obj_t*lable = lv_label_create(login);
	lv_label_set_text(lable, "LogIn");
	lv_obj_align_to(lable ,login, LV_ALIGN_TOP_RIGHT  ,-35,70);
	lv_obj_set_style_text_font(lable,&lv_font_montserrat_12,LV_STATE_DEFAULT);


	lv_obj_t* textarea_pass = lv_textarea_create(login);
	lv_obj_set_width(textarea_pass, scr_act_width() / 2);
	lv_obj_set_height(textarea_pass, font->line_height << 1);
	lv_obj_set_style_text_font(textarea_pass, font, LV_PART_MAIN);
	lv_obj_align_to(textarea_pass, textarea_name, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
	lv_obj_align(lv_textarea_get_label(textarea_pass), LV_ALIGN_LEFT_MID, 0, 0);
	lv_textarea_set_one_line(textarea_pass, true);
	lv_textarea_set_password_mode(textarea_pass, true);
	lv_textarea_set_password_show_time(textarea_pass, 300);


	lv_textarea_set_max_length(textarea_pass, 20);
	lv_obj_t* label_pass = lv_label_create(login);
	lv_obj_set_width(label_pass, font->line_height * 8);
	lv_obj_set_height(label_pass, font->line_height + 10);
	lv_obj_align_to(label_pass, textarea_pass, LV_ALIGN_OUT_LEFT_BOTTOM,-10, 0);
	lv_obj_set_style_text_font(label_pass, font, LV_PART_MAIN);
	lv_label_set_text(label_pass, "Password" );
	lv_obj_set_style_text_align(label_pass, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);

	lv_obj_t*img = lv_img_create(login);
	lv_img_set_src(img,&THZ);
	lv_obj_align_to(img, login, LV_ALIGN_CENTER, 0, 20); 
	
	
	lv_obj_t*lab = lv_label_create(login);
	lv_label_set_text(lab, "THZCLOUDSYS");
	lv_obj_align_to(lab, login, LV_ALIGN_CENTER, -5, 80); 
	lv_obj_set_style_text_font(lab,&lv_font_montserrat_16,LV_STATE_DEFAULT);
	
	
	lv_obj_t* keyboard = lv_keyboard_create(login);
	lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
	

	lv_obj_add_event_cb(textarea_name, event_textarea_name_cb, LV_EVENT_ALL, keyboard);
	lv_obj_add_event_cb(textarea_pass, event_textarea_pass_cb, LV_EVENT_ALL, keyboard);
}

int main(void)
{
	u8 len;	
	u16 times=0; 
	int temp=250;
	bool wait_GTC =0;
	
	Cache_Enable();                 //打开L1-Cache
	HAL_Init();				        			//初始化HAL库
	Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
	delay_init(216);                //延时初始化
	usart_init(115200);		        //串口初始化
	uart2_init(9600);
	LED_Init();                     //初始化LED
	KEY_Init();                     //初始化按键
	SDRAM_Init();                   //初始化SDRAM
	TIM6_Init(10-1,9000-1);
//	TIM5_Init(500-1,1080-1);
	PCF8574_Init();		//定时器初始化(1ms中断),用于给lvgl提供1ms的心跳节拍
                 //LCD初始化
				    			//触摸屏初始化 
  	TIM8_PWM_Init(2000 - 1, 2160 - 1);                                /* 初始化PUL引脚，以及脉冲模式等 */
	/*108 000 000/1080 =100 000； 100 000 / 2 000 = 50*/
	/*	
CCRx	50	100	150	200	250
180度	-90	-45	0	45	90
	*/
	HAL_TIM_Base_Start_IT(&GTIM8_Handler);

	
	stepper_init(); 
//	GTIM_TIM3_PWM_Init(267 - 1, 108 - 1);            /* 108Mhz,    初始化PUL引脚，以及脉冲模式等 */
//	GTIM_TIM4_PWM_Init(1000 - 1, 108 - 1);           /* 初始化PUL引脚，以及脉冲模式等 */
	GTIM_TIM3_PWM_Init(200 - 1, 108 - 1);            /* 108Mhz,    初始化PUL引脚，以及脉冲模式等 */
	GTIM_TIM4_PWM_Init(1000 - 1, 108 - 1);           /* 初始化PUL引脚，以及脉冲模式等 */
//	RADAR_GPIO_Init();
//	MX_TIM1_Init();
//	MX_UART7_Init();
	MX_USART3_UART_Init();
//	MX_UART5_Init();
	lv_init();											//lvgl系统初始化
	lv_port_disp_init();						//lvgl显示接口初始化,放在lv_init()的后面
	lv_port_indev_init();						//lvgl输入接口初始化,放在lv_init()的后面

	Main = lv_obj_create(NULL);
	
//	HAL_GPIO_WritePin(radarRST_GPIO_Port, radarRST_Pin, GPIO_PIN_RESET);
//	HAL_Delay(10);
//	HAL_GPIO_WritePin(radarRST_GPIO_Port, radarRST_Pin, GPIO_PIN_SET);
//	HAL_Delay(500);														//radar initialization
	

//	radar_cfgTransmit(30);													//configuration cfg transmission done   1-30

	HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE2);	
	button_create();
	lv_example_textarea();
	

//	delay_ms(5000);
//	delay_ms(5000);
//	printf("a");
	
	while(1)
	{
		lv_timer_handler();
		LED1_Toggle;
		/* 按键控制 */
		{
			key = KEY_Scan(0);

			if(key == WKUP_PRES)                           /* 按下KEYUP开启电机 */
			{  
				if(keyup_first_flag == 1)
				{
					scan_state=-1;
					PCF8574_WriteBit(BEEP_IO,0);	//控制蜂鸣器	
				}              
			}
			if(key == 0)
			{
				keyup_first_flag = 1;
			}
		}
//		motor_scan(130,75 ,450,300, 4);//2
		motor_scan(130,75 ,450,300, 4);//2
		if(scan_state != 10)
		{
			if(SWITCH1&&SWITCH3)
			{
				PCF8574_WriteBit(BEEP_IO,1);	//控制蜂鸣器	
				ST1_EN(0);//使能
			}
			else
			{
				PCF8574_WriteBit(BEEP_IO,0);	//控制蜂鸣器	
				ST1_EN(1);//失能
			}
		}
//		if(radar_rxDataFlag){
//			radar_dataUnpack();
//			if(scan_state >=1&&scan_state <=8)
//			{
//				radar_debug();
////				delay_ms(3);   
//			}
//		}
		
//		TIM_SetTIM8Compare3(70-15);	//修改比较值，修改占空比
//		TIM_SetTIM8Compare4(235+15);	//修改比较值，修改占空比
//		HAL_TIM_PWM_Start(&GTIM8_Handler, GTIM_TIM8_PWM_CH3);     /* 开启对应PWM通道 */
//		HAL_TIM_PWM_Start(&GTIM8_Handler, GTIM_TIM8_PWM_CH4);     /* 开启对应PWM通道 */
//		
		
	   if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//			printf("\r\n您发送的长度为:%d\r\n",len);
//			printf("\r\n您发送的消息为:\r\n");
//			HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)USART_RX_BUF,len,1000);	//发送接收到的数据
//			while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);		//等待发送结束
//			printf("\r\n...\r\n...\r\n");//插入换行
			USART_RX_STA=0;
			if(len == 3&&strncmp((const char*)USART_RX_BUF, "GTC", 3) == 0&&wait_GTC == 0)
			{
//				printf("\ryes!!\r\n");
				PCF8574_WriteBit(BEEP_IO,0);	//控制蜂鸣器	
				scan_state=-1;
				wait_GTC = 1;
			}
			if(len == 1&&strncmp((const char*)USART_RX_BUF, "m", 1) == 0)
			{
//				printf("\ryes!!\r\n");
				PCF8574_WriteBit(BEEP_IO,0);	//控制蜂鸣器	
				delay_ms(100);
				PCF8574_WriteBit(BEEP_IO, 1);	//控制蜂鸣器	
//				delay_ms(100);
//				PCF8574_WriteBit(BEEP_IO,0);	//控制蜂鸣器	
//				delay_ms(100);
//				PCF8574_WriteBit(BEEP_IO, 1);	//控制蜂鸣器	
//				delay_ms(100);
				m = 1;
//				scan_state=-1;
//				wait_GTC = 1;
				
			}
			if(len == 1&&strncmp((const char*)USART_RX_BUF, "w", 1) == 0)
			{
//				printf("\ryes!!\r\n");

				PCF8574_WriteBit(BEEP_IO,0);	//控制蜂鸣器	
				delay_ms(300);
				PCF8574_WriteBit(BEEP_IO, 1);	//控制蜂鸣器	
//				delay_ms(300);
//				PCF8574_WriteBit(BEEP_IO,0);	//控制蜂鸣器	
//				delay_ms(300);
//				PCF8574_WriteBit(BEEP_IO, 1);	//控制蜂鸣器	
//				delay_ms(300);
//				PCF8574_WriteBit(BEEP_IO,0);	//控制蜂鸣器	
//				delay_ms(300);
//				PCF8574_WriteBit(BEEP_IO, 1);	//控制蜂鸣器	
//				delay_ms(300);
//				scan_state=-1;
//				wait_GTC = 1;
				
				w = 1;
				
			}
			if(len == 1&&strncmp((const char*)USART_RX_BUF, "c", 1) == 0)
			{
//				printf("\ryes!!\r\n");
				PCF8574_WriteBit(BEEP_IO,0);	//控制蜂鸣器	
				delay_ms(10);
				PCF8574_WriteBit(BEEP_IO,1);	//控制蜂鸣器
				delay_ms(10);
				PCF8574_WriteBit(BEEP_IO,0);	//控制蜂鸣器	
				delay_ms(10);
				PCF8574_WriteBit(BEEP_IO,1);	//控制蜂鸣器
//				scan_state=-1;
//				wait_GTC = 1;.
				
				c = 1;
				
			}
		}
//		else
//		{
//			times++;
////			if(times%5000==0)
////			{
////				printf("\r\nALIENTEK STM32F7开发板 串口实验\r\n");
////				printf("正点原子@ALIENTEK\r\n\r\n\r\n");
////			}
////			if(times%200==0)printf("请输入数据,以回车键结束\r\n");  
////			if(times%30==0)LED0_TOGGLE();//闪烁LED,提示系统正在运行.
//			delay_ms(4);   
//		printf("b1 2 50");
//		}
	}
}
