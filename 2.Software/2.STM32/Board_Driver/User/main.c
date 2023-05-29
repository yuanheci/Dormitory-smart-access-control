#include "stm32f10x.h"   
#include "led.h"
#include "delay.h"
#include "key.h" 
#include "lcd_init.h"
#include "lcd.h"
#include "openmv.h"
#include "usart1.h"      //----OPENMV-通信
#include "usart3.h" 	 //----AS608-通信
#include "GeneralTim.h" 
#include "pid.h"
#include "as608.h"
#include "timer.h"   


/*--------------openmv-PID-部分------------*/
extern PID pid;
extern uint8_t flag;
//int out_x=2110,out_y=1500;           //X轴中值为1500，但是实际舵机要2110才是合适位置，Y轴中值为1500;
int out_x=2110,out_y=1000;        //新机械结构的中值
/*-----------------------------------------*/


extern uint8_t LCD_xy_display_flag;     //改变LCD上显示的坐标值标志位
extern u8 key_num;
extern uint8_t steer_control_flag;       //舵机控制标志位

uint16_t time_count=0;
uint8_t open_door_flag=0;        //开门标志位，在指纹识别通过后置1

int error_X=0,error_Y=0;
int fushu_flag=0;

uint8_t key_val;

uint8_t as608_move_flag;

uint8_t face_rec_fail_flag = 0;           //人脸识别失败标志位

extern int face_information[5];

extern uint8_t clear_face_display_flag;


void implement()
{
	int ccrx,ccry;
	
	if(steer_control_flag)
	{
		steer_control_flag = 0;
		ccrx = PID_Level(Openmv_X);
		ccry = PID_vertical(Openmv_Y);
		
		out_x = 2110 - ccrx;            //out值为实际输出量
		out_y = 1000 + ccry;
		
		if(out_x>=2500)out_x=2500;        //脉冲值
		else if(out_x<=500) out_x=500;
		if(out_y>=2500)out_y=2500;
		else if(out_y<=500) out_y=500;
		
		TIM_SetCompare1(TIM3,out_x/10);        //要除以十，问题可能在这！！！！（待测试）
		TIM_SetCompare2(TIM3,out_y/10);
	}
}



int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M。
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	LED_GPIO_Config();
	RELAYPB8_GPIO_Config();
	RELAYPB9_GPIO_Config();
	Key_GPIO_Config();
	BEEP_GPIO_Config();
	delay_init();
	//--------------OPENMV通信-------------------------
	USART1_Config(); 
	//--------------AS608通信--------------------------
	PS_StaGPIO_Init();
	usart3_init(57600);        //用于与AS608通信
	
	//定时器配置，PWM输出设置
	GENERAL_TIM_NVIC_Config();
	GENERAL_TIM_Init();        /* 定时器TIM2和TIM3初始化 */ 
	
	//TIM_SetCompare3(TIM3,150);        //250--反转最快（逆时针）        50--正转最快（顺时针）         150--停止
	
	
	//云台控制
	PID_Init();	
	
	LCD_Init();	
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	
	LCD_ShowString(0,0,"X-",RED,WHITE,16,0);
	LCD_ShowChinese(16,0,"坐标",RED,WHITE,16,0);
	LCD_ShowString(48,0,":",RED,WHITE,16,0);
	
	LCD_ShowString(0,32,"Y-",RED,WHITE,16,0);
	LCD_ShowChinese(16,32,"坐标",RED,WHITE,16,0);
	LCD_ShowString(48,32,":",RED,WHITE,16,0);
	
	LCD_ShowString(0,64,"X-ERROR:",RED,WHITE,16,0);
	LCD_ShowString(0,96,"Y-ERROR:",RED,WHITE,16,0);
	
	LCD_ShowIntNum(64,0,100,3,RED,WHITE,16);
	LCD_ShowIntNum(64,32,100,3,RED,WHITE,16);
	LCD_ShowIntNum(64,64,0,3,RED,WHITE,16);
	
	LCD_ShowIntNum(64,96,0,3,RED,WHITE,16);
	
	//人脸识别信息
	LCD_ShowChinese(0,128,"人脸识别结果",RED,WHITE,16,0);
	LCD_ShowString(96,128,":",RED,WHITE,16,0);
	//指纹识别结果
	LCD_ShowChinese(0,160,"指纹识别结果",RED,WHITE,16,0);
	LCD_ShowString(96,160,":",RED,WHITE,16,0);
	
	implement();

	while(1)
	{
		key_val = key_scan();
		if(key_val==1)         //开启一次人脸识别
		{
			LED_TOGGLE;
			//RELAY8_ON;
			LCD_ShowChinese(160,128,"识别中",RED,WHITE,16,0);
			delay_ms(10);
			Usart_SendByte(USART1,'1');
			delay_ms(10);
			key_val = 0;
		}
		if(key_val==2)         //人脸库采集
		{
			LED_TOGGLE;
			//RELAY8_OFF;
			delay_ms(10);
			Usart_SendByte(USART1,'2');
			delay_ms(10);
			key_val = 0;
		}
		if(key_val==3)
		{
			LED_TOGGLE;
			RELAY9_ON;	
			key_val = 0;
			Add_FR();	
			RELAY9_OFF;
			LCD_ShowString(112,160,"                                  ",RED,WHITE,16,0);			
		}
		if(key_val==4)
		{
			key_val = 0;
			LED_TOGGLE;
			//RELAY9_OFF;
			//BUZZER_TOGGLE;
			//press_FR();
			//Usart_SendByte(USART1,'1');
		}	
		
		/*------------------------------------------------指纹处理代码---------------------------------------------------*/
				
		//触摸端始终供电，主电源用继电器控制工作，当有触摸时，给电开启工作，移开后2s继电器失电
		if(PS_Sta)	 //检测PS_Sta状态，如果有手指按下
		{
			RELAY9_ON;      //打开指纹继电器，开启工作端电源
			LED_ON;
			LCD_ShowChinese(112,160,"请按手指",RED,WHITE,16,0);
			press_FR();//刷指纹	
		}
				
		/*------------------------------------------------摄像头处理代码---------------------------------------------------*/
		//openmv通信测试
		if(LCD_xy_display_flag)
		{
			LCD_xy_display_flag=0;
			
			error_X = Openmv_X-103;
			error_Y = Openmv_Y-82;
			if(error_X < 0)
			{
				error_X = -error_X;
				fushu_flag = 1;
			}
			else
			{
				fushu_flag = 0;
			}
			if(error_Y < 0)
			{
				error_Y = -error_Y;
				fushu_flag = 1;
			}
			else
			{
				fushu_flag = 0;
			}
			
			//数值显示
			LCD_ShowIntNum(64,0,Openmv_X,3,RED,WHITE,16);
			LCD_ShowIntNum(64,32,Openmv_Y,3,RED,WHITE,16);

			LCD_ShowIntNum(80,64,error_X,3,RED,WHITE,16);
			LCD_ShowIntNum(80,96,error_Y,3,RED,WHITE,16);
			
			if(fushu_flag == 1)
			{
				LCD_ShowString(72,64,"-",RED,WHITE,16,0);
				LCD_ShowString(72,96,"-",RED,WHITE,16,0);
			}
			else if(fushu_flag == 0)
			{
				LCD_ShowString(72,64,"+",RED,WHITE,16,0);
				LCD_ShowString(72,96,"+",RED,WHITE,16,0);
			}		
		}
		
		//LCD_ShowIntNum(0,112,Face_Inf,3,RED,WHITE,16);
		
		//-----------------------------识别成功--------------------------------
		if(Face_Inf == 200)         //Face_Inf  接收人脸识别信息显示对应名字   执行开门
		{
			LCD_ShowChinese(112,128,"任世豪",RED,WHITE,16,0);
			LCD_ShowString(160,128,"---",RED,WHITE,16,0);
			LCD_ShowChinese(184,128,"欢迎进入",RED,WHITE,16,0);
					
			open_door_flag = 1;
			Face_Inf = 0;
			face_information[2] = 0;        
		}	
		else if(Face_Inf == 201)
		{
			LCD_ShowChinese(112,128,"王旭",RED,WHITE,16,0);
			LCD_ShowString(160,128,"---",RED,WHITE,16,0);
			LCD_ShowChinese(184,128,"欢迎进入",RED,WHITE,16,0);
			
			open_door_flag = 1;
			Face_Inf = 0;
			face_information[2] = 0;
		}
		else if(Face_Inf == 202)
		{
			LCD_ShowChinese(112,128,"伍郭成",RED,WHITE,16,0);
			LCD_ShowString(160,128,"---",RED,WHITE,16,0);
			LCD_ShowChinese(184,128,"欢迎进入",RED,WHITE,16,0);
			
			open_door_flag = 1;
			Face_Inf = 0;
			face_information[2] = 0;
		}
		else if(Face_Inf == 203)
		{
			LCD_ShowChinese(112,128,"任世豪",RED,WHITE,16,0);
			LCD_ShowString(160,128,"---",RED,WHITE,16,0);
			LCD_ShowChinese(184,128,"欢迎进入",RED,WHITE,16,0);
			
			open_door_flag = 1;
			Face_Inf = 0;
			face_information[2] = 0;
		}
		else if(Face_Inf == 205)
		{
			LCD_ShowChinese(112,128,"新成员",RED,WHITE,16,0);
			LCD_ShowString(160,128,"---",RED,WHITE,16,0);
			LCD_ShowChinese(184,128,"欢迎进入",RED,WHITE,16,0);
			
			open_door_flag = 1;
			Face_Inf = 0;
			face_information[2] = 0;
		}
		//-----------------------------识别失败--------------------------------
		else if(Face_Inf == 204)
		{
			face_rec_fail_flag = 1;
			
			LCD_ShowChinese(112,128,"请重试",RED,WHITE,16,0);
			LCD_ShowString(160,128,"---",RED,WHITE,16,0);
			LCD_ShowChinese(184,128,"识别失败",RED,WHITE,16,0);
			
			Face_Inf = 0;
			face_information[2] = 0;
		}
		
		
		if(clear_face_display_flag)
		{
			clear_face_display_flag = 0;
			LCD_ShowString(112,128,"                  ",RED,WHITE,16,0);
			
			LCD_ShowString(112,160,"                          ",RED,WHITE,16,0);
		    LCD_ShowString(112,176,"                          ",RED,WHITE,16,0);
		    LCD_ShowString(112,192,"                          ",RED,WHITE,16,0);
		}
	}
}


