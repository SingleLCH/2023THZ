#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "touch.h"
#include "lv_port_indev_template.h"
#include "lv_port_disp_template.h"
#include "../../lvgl.h"
#include "timer.h"
#include "pcf8574.h"
#include "dht11.h"



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
LV_IMG_DECLARE(MM);




void btn_event_cb(lv_event_t *e){
	
	lv_event_code_t code = lv_event_get_code(e);
	if(code == LV_EVENT_CLICKED){
	 
		lv_obj_add_flag(win, LV_OBJ_FLAG_HIDDEN);
		
	}
	
}



void event_cb(lv_event_t *e){
	
	int in=1;
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
	lv_obj_set_style_anim_time(bar,20000,LV_STATE_DEFAULT);
	lv_bar_set_value(bar,90,LV_ANIM_ON);
	
	if(in==1){
	lv_bar_set_start_value(bar,90,LV_ANIM_ON);
	lv_obj_set_style_anim_time(bar,200,LV_STATE_DEFAULT);
	lv_bar_set_value(bar,100,LV_ANIM_ON);
	}


}

void button_create(void){
	
	btn = lv_imgbtn_create(Main);
	lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_RELEASED, NULL, &MM, NULL);
	lv_obj_set_size(btn,81,81);
	lv_obj_set_x(btn,320);
	lv_obj_set_y(btn,90);
	lv_obj_add_event_cb(btn,event_cb,LV_EVENT_CLICKED,NULL);
	
	lv_obj_t*btn1= lv_btn_create(Main);
	lv_obj_set_size(btn1,120,60);
	lv_obj_set_x(btn1,320);
	lv_obj_set_y(btn1,200);
	lv_obj_add_event_cb(btn1,event_cb,LV_EVENT_CLICKED,NULL);
	
	lv_obj_t*lable1 = lv_label_create(btn1);
	lv_label_set_text(lable1, "Resert");
	lv_obj_center(lable1);
	lv_obj_set_style_text_font(lable1,&lv_font_montserrat_14,LV_STATE_DEFAULT);
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
		if(strcmp(txt,"admin")==0){
			
			lv_obj_add_event_cb(Login, Login_close,LV_EVENT_CLICKED,NULL);
			lv_scr_load(Main);
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
		if(strcmp(txt,"password")==0){
		
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
    Cache_Enable();                 //打开L1-Cache
    HAL_Init();				        //初始化HAL库
    Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
    delay_init(216);                //延时初始化
	uart_init(115200);		        //串口初始化
    LED_Init();                     //初始化LED
    KEY_Init();                     //初始化按键
    SDRAM_Init();                   //初始化SDRAM
	PCF8574_Init();
	DHT11_Init();
	TIM3_Init(10-1,9000-1);
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init(); 
	u8 temperature;  	    
	u8 humidity;  
	
	
	Main = lv_obj_create(NULL);
	

	button_create();
	lv_example_textarea();
	
	lv_obj_t* arc = lv_arc_create(Main);
	lv_arc_set_range(arc, 0, 50);
	lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
	lv_obj_set_style_bg_color(arc, lv_color_hex(0x656883), LV_PART_MAIN);
	lv_obj_set_x(arc,20);
	lv_obj_set_y(arc,50);
	lv_obj_set_size(arc, 100, 100);
	
	lv_obj_t* arc1 = lv_arc_create(Main);
	lv_arc_set_range(arc1, 0, 50);
	lv_obj_remove_style(arc1, NULL, LV_PART_KNOB); 
	lv_obj_set_x(arc1,150);
	lv_obj_set_y(arc1,50);
	lv_obj_set_size(arc1, 100, 100);
	
	while(1){
		delay_ms(5);
		PCF8574_ReadBit(BEEP_IO);
		DHT11_Read_Data(&temperature,&humidity);
		lv_timer_handler();
		lv_arc_set_value(arc, humidity);
		lv_arc_set_value(arc1,temperature); 		
		
	}

	
}
