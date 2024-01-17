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
static lv_obj_t* Login; //��ť
static lv_obj_t* login; //����
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
volatile double x_now = 0,y_now = 0;//��ǰ����λ��
volatile double x_aim = 0,y_aim = 0;//��ǰĿ��λ��
volatile int8_t scan_dir = 1;//1��ʾ��С����-1��ʾ�Ӵ�С
volatile int8_t scan_state = -3;

//state = -2Ϊɨ�迪ʼǰ״̬
//state = 9Ϊɨ�������״̬					
uint8_t m = 0, w = 0, c = 0;
/************************
			state״̬ת��ͼ
		��
		��
		��/��
		��
		�ߡ��������ޡ���������
						   ��
						   ��/��
						   ��
		�١��������ڡ���������
		��
		��
		��
		�ߡ��������ޡ���������
						   ��
						   ��
						   ��
  -1��0���١��������ڡ���������
	
***************************/
void motor_scan(float x_min,float y_min ,float x_max,float y_max, volatile float division)
{
	static uint8_t count = 0;
	static uint8_t beep_times = 0;
	
	switch(scan_state)
	{
		case -2:
		{
			/* ���³���λ�� */
			x_now = g_stepper_x.pos_mm;  // ���³���λ��
			y_now = g_stepper_y.pos_mm;  // ���³���λ��
		}
		break;
		case -1:
		{
			/* �����µ�Ŀ��λ�� */
			x_aim = x_min;
			y_aim = y_min;
			/* ǰ���µ�Ŀ��λ�� */
			if(stepper_set_pos(x_aim,y_aim))
				scan_state = 0; //״̬ת�� -1-->0
		}break;
		case 0:
		{
			if(g_run_flag_1 ==1||g_run_flag_2 ==1)
				break;
			else if(g_run_flag_1 == 0&&g_run_flag_2 == 0)
			{
				printf("a");
				/* �ѵ���Ŀ��λ�ã����³���λ�� */
				x_now = g_stepper_x.pos_mm;  // ���³���λ��
				y_now = g_stepper_y.pos_mm;  // ���³���λ��
				scan_state = 1;  // ״̬ת�� 0-->1
			}
		}break;
		case 1:
		{
			/* �����µ�Ŀ��λ�� */
			x_aim = x_max;
			y_aim = y_now;
			/* ǰ���µ�Ŀ��λ�� */
//			HAL_UART_Receive_IT(&huart5,radar_rxDataBufTemp,1);				//Begin to receive data
			
			if(stepper_set_pos(x_aim,y_aim))	
				scan_state = 2; //״̬ת�� 1-->2
		}break;
		case 2:
		{
			if(g_run_flag_1 ==1||g_run_flag_2 ==1)
				break;
			else if(g_run_flag_1 == 0&&g_run_flag_2 == 0)
			{
				/* �ѵ���Ŀ��λ�ã����³���λ�� */
				x_now = g_stepper_x.pos_mm;  // ���³���λ��
				y_now = g_stepper_y.pos_mm;  // ���³���λ��
				scan_state = 3; //״̬ת�� 2-->3
			}
		}break;
		case 3:
		{
			/* �����µ�Ŀ��λ�� */
			x_aim = x_now;
			y_aim = y_now + division;//����
			/* ǰ���µ�Ŀ��λ�� */
			if(y_now>y_max)
			{
				time_ms = 0;
				scan_state = 9;//״̬ת�� 3-->9
			}
			else
			{
				if(stepper_set_pos(x_aim,y_aim))		
					scan_state = 4; //״̬ת�� 3-->4
			}
		}break;
		case 4:
		{
			if(g_run_flag_1 ==1||g_run_flag_2 ==1)
				break;
			else if(g_run_flag_1 == 0&&g_run_flag_2 == 0)
			{
				/* �ѵ���Ŀ��λ�ã����³���λ�� */
				x_now = g_stepper_x.pos_mm;  // ���³���λ��
				y_now = g_stepper_y.pos_mm;  // ���³���λ��
				scan_state = 5; //״̬ת�� 4-->5
			}
			
		}break;
		case 5:
		{
			/* �����µ�Ŀ��λ�� */
			x_aim = x_min;
			y_aim = y_now;
			/* ǰ���µ�Ŀ��λ�� */
			if(stepper_set_pos(x_aim,y_aim))		
				scan_state = 6; //״̬ת�� 5-->6
		}break;
		case 6:
		{
			if(g_run_flag_1 ==1||g_run_flag_2 ==1)
				break;
			else if(g_run_flag_1 == 0&&g_run_flag_2 == 0)
			{
				/* �ѵ���Ŀ��λ�ã����³���λ�� */
				x_now = g_stepper_x.pos_mm;  // ���³���λ��
				y_now = g_stepper_y.pos_mm;  // ���³���λ��
				scan_state = 7; //״̬ת�� 6-->7
			}
			
		}break;
		case 7:
		{
			/* �����µ�Ŀ��λ�� */
			x_aim = x_now;
			y_aim = y_now + division;//����
			/* ǰ���µ�Ŀ��λ�� */
			if(y_now>y_max)
			{
				time_ms = 0;
				scan_state = 9;//״̬ת�� 7-->9
			}
			else
			{
				if(stepper_set_pos(x_aim,y_aim))	
					scan_state = 8; //״̬ת�� 7-->8
			}
		}break;
		case 8:
		{
			if(g_run_flag_1 ==1||g_run_flag_2 ==1)
				break;
			else if(g_run_flag_1 == 0&&g_run_flag_2 == 0)
			{
				/* �ѵ���Ŀ��λ�ã����³���λ�� */
				x_now = g_stepper_x.pos_mm;  // ���³���λ��
				y_now = g_stepper_y.pos_mm;  // ���³���λ��
				scan_state = 1; //״̬ת�� 8-->1
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
			scan_state = 10; //״̬ת�� 9-->10
		}break;
		case 10:
		{
			/* ɨ����� */
			if(count == 12)
			{
				PCF8574_WriteBit(BEEP_IO, 1);	//���Ʒ�����	
			}
			else if(count == 50)
			{
				PCF8574_WriteBit(BEEP_IO, 0);	//���Ʒ�����	
				count = 0;
				beep_times++;
				printf("a\r\n");
			}
			if(beep_times==20)
			{
				scan_state = 11; //״̬ת�� 10-->11
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

//ע:����littleVGL��,��ò�Ҫ����-O���Ż�,����Ϊ-O0���Ż��Ϳ�����

#define TEST_NUM		1   //1,2,3�ֱ��Ӧ3������



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
	
	Cache_Enable();                 //��L1-Cache
	HAL_Init();				        			//��ʼ��HAL��
	Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
	delay_init(216);                //��ʱ��ʼ��
	usart_init(115200);		        //���ڳ�ʼ��
	uart2_init(9600);
	LED_Init();                     //��ʼ��LED
	KEY_Init();                     //��ʼ������
	SDRAM_Init();                   //��ʼ��SDRAM
	TIM6_Init(10-1,9000-1);
//	TIM5_Init(500-1,1080-1);
	PCF8574_Init();		//��ʱ����ʼ��(1ms�ж�),���ڸ�lvgl�ṩ1ms����������
                 //LCD��ʼ��
				    			//��������ʼ�� 
  	TIM8_PWM_Init(2000 - 1, 2160 - 1);                                /* ��ʼ��PUL���ţ��Լ�����ģʽ�� */
	/*108 000 000/1080 =100 000�� 100 000 / 2 000 = 50*/
	/*	
CCRx	50	100	150	200	250
180��	-90	-45	0	45	90
	*/
	HAL_TIM_Base_Start_IT(&GTIM8_Handler);

	
	stepper_init(); 
//	GTIM_TIM3_PWM_Init(267 - 1, 108 - 1);            /* 108Mhz,    ��ʼ��PUL���ţ��Լ�����ģʽ�� */
//	GTIM_TIM4_PWM_Init(1000 - 1, 108 - 1);           /* ��ʼ��PUL���ţ��Լ�����ģʽ�� */
	GTIM_TIM3_PWM_Init(200 - 1, 108 - 1);            /* 108Mhz,    ��ʼ��PUL���ţ��Լ�����ģʽ�� */
	GTIM_TIM4_PWM_Init(1000 - 1, 108 - 1);           /* ��ʼ��PUL���ţ��Լ�����ģʽ�� */
//	RADAR_GPIO_Init();
//	MX_TIM1_Init();
//	MX_UART7_Init();
	MX_USART3_UART_Init();
//	MX_UART5_Init();
	lv_init();											//lvglϵͳ��ʼ��
	lv_port_disp_init();						//lvgl��ʾ�ӿڳ�ʼ��,����lv_init()�ĺ���
	lv_port_indev_init();						//lvgl����ӿڳ�ʼ��,����lv_init()�ĺ���

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
		/* �������� */
		{
			key = KEY_Scan(0);

			if(key == WKUP_PRES)                           /* ����KEYUP������� */
			{  
				if(keyup_first_flag == 1)
				{
					scan_state=-1;
					PCF8574_WriteBit(BEEP_IO,0);	//���Ʒ�����	
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
				PCF8574_WriteBit(BEEP_IO,1);	//���Ʒ�����	
				ST1_EN(0);//ʹ��
			}
			else
			{
				PCF8574_WriteBit(BEEP_IO,0);	//���Ʒ�����	
				ST1_EN(1);//ʧ��
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
		
//		TIM_SetTIM8Compare3(70-15);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//		TIM_SetTIM8Compare4(235+15);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//		HAL_TIM_PWM_Start(&GTIM8_Handler, GTIM_TIM8_PWM_CH3);     /* ������ӦPWMͨ�� */
//		HAL_TIM_PWM_Start(&GTIM8_Handler, GTIM_TIM8_PWM_CH4);     /* ������ӦPWMͨ�� */
//		
		
	   if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			printf("\r\n�����͵ĳ���Ϊ:%d\r\n",len);
//			printf("\r\n�����͵���ϢΪ:\r\n");
//			HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)USART_RX_BUF,len,1000);	//���ͽ��յ�������
//			while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
//			printf("\r\n...\r\n...\r\n");//���뻻��
			USART_RX_STA=0;
			if(len == 3&&strncmp((const char*)USART_RX_BUF, "GTC", 3) == 0&&wait_GTC == 0)
			{
//				printf("\ryes!!\r\n");
				PCF8574_WriteBit(BEEP_IO,0);	//���Ʒ�����	
				scan_state=-1;
				wait_GTC = 1;
			}
			if(len == 1&&strncmp((const char*)USART_RX_BUF, "m", 1) == 0)
			{
//				printf("\ryes!!\r\n");
				PCF8574_WriteBit(BEEP_IO,0);	//���Ʒ�����	
				delay_ms(100);
				PCF8574_WriteBit(BEEP_IO, 1);	//���Ʒ�����	
//				delay_ms(100);
//				PCF8574_WriteBit(BEEP_IO,0);	//���Ʒ�����	
//				delay_ms(100);
//				PCF8574_WriteBit(BEEP_IO, 1);	//���Ʒ�����	
//				delay_ms(100);
				m = 1;
//				scan_state=-1;
//				wait_GTC = 1;
				
			}
			if(len == 1&&strncmp((const char*)USART_RX_BUF, "w", 1) == 0)
			{
//				printf("\ryes!!\r\n");

				PCF8574_WriteBit(BEEP_IO,0);	//���Ʒ�����	
				delay_ms(300);
				PCF8574_WriteBit(BEEP_IO, 1);	//���Ʒ�����	
//				delay_ms(300);
//				PCF8574_WriteBit(BEEP_IO,0);	//���Ʒ�����	
//				delay_ms(300);
//				PCF8574_WriteBit(BEEP_IO, 1);	//���Ʒ�����	
//				delay_ms(300);
//				PCF8574_WriteBit(BEEP_IO,0);	//���Ʒ�����	
//				delay_ms(300);
//				PCF8574_WriteBit(BEEP_IO, 1);	//���Ʒ�����	
//				delay_ms(300);
//				scan_state=-1;
//				wait_GTC = 1;
				
				w = 1;
				
			}
			if(len == 1&&strncmp((const char*)USART_RX_BUF, "c", 1) == 0)
			{
//				printf("\ryes!!\r\n");
				PCF8574_WriteBit(BEEP_IO,0);	//���Ʒ�����	
				delay_ms(10);
				PCF8574_WriteBit(BEEP_IO,1);	//���Ʒ�����
				delay_ms(10);
				PCF8574_WriteBit(BEEP_IO,0);	//���Ʒ�����	
				delay_ms(10);
				PCF8574_WriteBit(BEEP_IO,1);	//���Ʒ�����
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
////				printf("\r\nALIENTEK STM32F7������ ����ʵ��\r\n");
////				printf("����ԭ��@ALIENTEK\r\n\r\n\r\n");
////			}
////			if(times%200==0)printf("����������,�Իس�������\r\n");  
////			if(times%30==0)LED0_TOGGLE();//��˸LED,��ʾϵͳ��������.
//			delay_ms(4);   
//		printf("b1 2 50");
//		}
	}
}
