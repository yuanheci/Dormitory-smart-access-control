#include "stm32f10x.h"   
#include "led.h"
#include "delay.h"
#include "key.h" 
#include "lcd_init.h"
#include "lcd.h"
#include "openmv.h"
#include "usart1.h"      //----OPENMV-ͨ��
#include "usart3.h" 	 //----AS608-ͨ��
#include "GeneralTim.h" 
#include "pid.h"
#include "as608.h"
#include "timer.h"   


/*--------------openmv-PID-����------------*/
extern PID pid;
extern uint8_t flag;
//int out_x=2110,out_y=1500;           //X����ֵΪ1500������ʵ�ʶ��Ҫ2110���Ǻ���λ�ã�Y����ֵΪ1500;
int out_x=2110,out_y=1000;        //�»�е�ṹ����ֵ
/*-----------------------------------------*/


extern uint8_t LCD_xy_display_flag;     //�ı�LCD����ʾ������ֵ��־λ
extern u8 key_num;
extern uint8_t steer_control_flag;       //������Ʊ�־λ

uint16_t time_count=0;
uint8_t open_door_flag=0;        //���ű�־λ����ָ��ʶ��ͨ������1

int error_X=0,error_Y=0;
int fushu_flag=0;

uint8_t key_val;

uint8_t as608_move_flag;

uint8_t face_rec_fail_flag = 0;           //����ʶ��ʧ�ܱ�־λ

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
		
		out_x = 2110 - ccrx;            //outֵΪʵ�������
		out_y = 1000 + ccry;
		
		if(out_x>=2500)out_x=2500;        //����ֵ
		else if(out_x<=500) out_x=500;
		if(out_y>=2500)out_y=2500;
		else if(out_y<=500) out_y=500;
		
		TIM_SetCompare1(TIM3,out_x/10);        //Ҫ����ʮ������������⣡�������������ԣ�
		TIM_SetCompare2(TIM3,out_y/10);
	}
}



int main(void)
{
	// ���������ʱ��ϵͳ��ʱ���Ѿ������ó�72M��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	LED_GPIO_Config();
	RELAYPB8_GPIO_Config();
	RELAYPB9_GPIO_Config();
	Key_GPIO_Config();
	BEEP_GPIO_Config();
	delay_init();
	//--------------OPENMVͨ��-------------------------
	USART1_Config(); 
	//--------------AS608ͨ��--------------------------
	PS_StaGPIO_Init();
	usart3_init(57600);        //������AS608ͨ��
	
	//��ʱ�����ã�PWM�������
	GENERAL_TIM_NVIC_Config();
	GENERAL_TIM_Init();        /* ��ʱ��TIM2��TIM3��ʼ�� */ 
	
	//TIM_SetCompare3(TIM3,150);        //250--��ת��죨��ʱ�룩        50--��ת��죨˳ʱ�룩         150--ֹͣ
	
	
	//��̨����
	PID_Init();	
	
	LCD_Init();	
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	
	LCD_ShowString(0,0,"X-",RED,WHITE,16,0);
	LCD_ShowChinese(16,0,"����",RED,WHITE,16,0);
	LCD_ShowString(48,0,":",RED,WHITE,16,0);
	
	LCD_ShowString(0,32,"Y-",RED,WHITE,16,0);
	LCD_ShowChinese(16,32,"����",RED,WHITE,16,0);
	LCD_ShowString(48,32,":",RED,WHITE,16,0);
	
	LCD_ShowString(0,64,"X-ERROR:",RED,WHITE,16,0);
	LCD_ShowString(0,96,"Y-ERROR:",RED,WHITE,16,0);
	
	LCD_ShowIntNum(64,0,100,3,RED,WHITE,16);
	LCD_ShowIntNum(64,32,100,3,RED,WHITE,16);
	LCD_ShowIntNum(64,64,0,3,RED,WHITE,16);
	
	LCD_ShowIntNum(64,96,0,3,RED,WHITE,16);
	
	//����ʶ����Ϣ
	LCD_ShowChinese(0,128,"����ʶ����",RED,WHITE,16,0);
	LCD_ShowString(96,128,":",RED,WHITE,16,0);
	//ָ��ʶ����
	LCD_ShowChinese(0,160,"ָ��ʶ����",RED,WHITE,16,0);
	LCD_ShowString(96,160,":",RED,WHITE,16,0);
	
	implement();

	while(1)
	{
		key_val = key_scan();
		if(key_val==1)         //����һ������ʶ��
		{
			LED_TOGGLE;
			//RELAY8_ON;
			LCD_ShowChinese(160,128,"ʶ����",RED,WHITE,16,0);
			delay_ms(10);
			Usart_SendByte(USART1,'1');
			delay_ms(10);
			key_val = 0;
		}
		if(key_val==2)         //������ɼ�
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
		
		/*------------------------------------------------ָ�ƴ������---------------------------------------------------*/
				
		//������ʼ�չ��磬����Դ�ü̵������ƹ��������д���ʱ�����翪���������ƿ���2s�̵���ʧ��
		if(PS_Sta)	 //���PS_Sta״̬���������ָ����
		{
			RELAY9_ON;      //��ָ�Ƽ̵��������������˵�Դ
			LED_ON;
			LCD_ShowChinese(112,160,"�밴��ָ",RED,WHITE,16,0);
			press_FR();//ˢָ��	
		}
				
		/*------------------------------------------------����ͷ�������---------------------------------------------------*/
		//openmvͨ�Ų���
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
			
			//��ֵ��ʾ
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
		
		//-----------------------------ʶ��ɹ�--------------------------------
		if(Face_Inf == 200)         //Face_Inf  ��������ʶ����Ϣ��ʾ��Ӧ����   ִ�п���
		{
			LCD_ShowChinese(112,128,"������",RED,WHITE,16,0);
			LCD_ShowString(160,128,"---",RED,WHITE,16,0);
			LCD_ShowChinese(184,128,"��ӭ����",RED,WHITE,16,0);
					
			open_door_flag = 1;
			Face_Inf = 0;
			face_information[2] = 0;        
		}	
		else if(Face_Inf == 201)
		{
			LCD_ShowChinese(112,128,"����",RED,WHITE,16,0);
			LCD_ShowString(160,128,"---",RED,WHITE,16,0);
			LCD_ShowChinese(184,128,"��ӭ����",RED,WHITE,16,0);
			
			open_door_flag = 1;
			Face_Inf = 0;
			face_information[2] = 0;
		}
		else if(Face_Inf == 202)
		{
			LCD_ShowChinese(112,128,"�����",RED,WHITE,16,0);
			LCD_ShowString(160,128,"---",RED,WHITE,16,0);
			LCD_ShowChinese(184,128,"��ӭ����",RED,WHITE,16,0);
			
			open_door_flag = 1;
			Face_Inf = 0;
			face_information[2] = 0;
		}
		else if(Face_Inf == 203)
		{
			LCD_ShowChinese(112,128,"������",RED,WHITE,16,0);
			LCD_ShowString(160,128,"---",RED,WHITE,16,0);
			LCD_ShowChinese(184,128,"��ӭ����",RED,WHITE,16,0);
			
			open_door_flag = 1;
			Face_Inf = 0;
			face_information[2] = 0;
		}
		else if(Face_Inf == 205)
		{
			LCD_ShowChinese(112,128,"�³�Ա",RED,WHITE,16,0);
			LCD_ShowString(160,128,"---",RED,WHITE,16,0);
			LCD_ShowChinese(184,128,"��ӭ����",RED,WHITE,16,0);
			
			open_door_flag = 1;
			Face_Inf = 0;
			face_information[2] = 0;
		}
		//-----------------------------ʶ��ʧ��--------------------------------
		else if(Face_Inf == 204)
		{
			face_rec_fail_flag = 1;
			
			LCD_ShowChinese(112,128,"������",RED,WHITE,16,0);
			LCD_ShowString(160,128,"---",RED,WHITE,16,0);
			LCD_ShowChinese(184,128,"ʶ��ʧ��",RED,WHITE,16,0);
			
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


