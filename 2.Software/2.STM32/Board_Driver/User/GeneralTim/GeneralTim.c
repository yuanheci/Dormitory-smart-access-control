#include "GeneralTim.h" 
#include "delay.h"


extern vu16 USART3_RX_STA; 

void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
	// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM2_IRQ;	
	// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM3_IRQ;	
	NVIC_Init(&NVIC_InitStructure);
}


static void GENERAL_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // ����Ƚ�ͨ��1 GPIO ��ʼ��
  RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH1_PORT, &GPIO_InitStructure);
	
  // ����Ƚ�ͨ��2 GPIO ��ʼ��
  RCC_APB2PeriphClockCmd(GENERAL_TIM_CH2_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH2_PORT, &GPIO_InitStructure);
	
  // ����Ƚ�ͨ��3 GPIO ��ʼ��
  RCC_APB2PeriphClockCmd(GENERAL_TIM_CH3_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH3_PORT, &GPIO_InitStructure);
	
  // ����Ƚ�ͨ��4 GPIO ��ʼ��
  RCC_APB2PeriphClockCmd(GENERAL_TIM_CH4_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH4_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStructure);
}


///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

static void GENERAL_TIM_Mode_Config(void)                            //TIM2,TIM3
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK,ENABLE);
	
/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	// �������ڣ���������Ϊ100K
	
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_Period;	  //2000������ֵΪһ������   T=10us*2000=20ms
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler;	     //�����10usһ������
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��  TIM3
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period=AS608_TIM_Period;	  //TIM2-999
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= AS608_TIM_Prescaler;   //71	
	// ��ʼ����ʱ��  TIM2
	TIM_TimeBaseInit(AS608_TIM, &TIM_TimeBaseStructure);
	
	// ����������жϱ�־λ
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	// �����������ж�
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

	/*--------------------����ȽϽṹ���ʼ��-------------------*/	
	// ռ�ձ�����
//	uint16_t CCR1_Val = 250;     //5
//	uint16_t CCR2_Val = 4;
//	uint16_t CCR3_Val = 3;
//	uint16_t CCR4_Val = 2;
	

	// ����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// ���ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// ���ͨ����ƽ��������	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	// ����Ƚ�ͨ�� 1
	//TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OC1Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// ����Ƚ�ͨ�� 2
	//TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
	TIM_OC2Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// ����Ƚ�ͨ�� 3
//	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
	TIM_OC3Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// ����Ƚ�ͨ�� 4
//	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
	TIM_OC4Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// ʹ�ܼ�����
	TIM_Cmd(AS608_TIM, ENABLE);
	TIM_Cmd(GENERAL_TIM, ENABLE);
}


void GENERAL_TIM_Init(void)      //TIM2,TIM3
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_Mode_Config();		
}


void servo_init(void)      //CCR�Ĵ�����ֵ->ռ�ձ�
{
  TIM_SetCompare1(TIM3,50);  //ʹ����ָ����м�λ��   //150
  TIM_SetCompare2(TIM3,50);  //ʹ����ָ����м�λ��
}


void delay(uint32_t time)
{
	uint32_t i;
	for(i=0;i<time;i++);
}

                                  //һ������ 10us
////0.5ms--0��  2.5ms--270��          <1> 0.5*1000/10=50������        <2> 2.5*1000us/10=250������
//void servo1_angle(uint16_t angle1)
//{
//  uint16_t pulse1;
//  
//  //��Զ����ת�Ƕ��޷�
//  if(angle1 <= 5)
//    angle1 = 5;                //20ms����  0.5ms--0��      2.5ms--270��                     200/270           
//  if(angle1 >= 265)            //angle: 0-270  ����ֵ50-250 (��ֵarr = 2000) y=kx+b  ->  k=(250-50)/(270-0) = 20/27 , b = 50
//    angle1 = 265;
//  //���Ƕ�ֵת��Ϊ����ֵ       //angle=0ʱ��pulse=50, duty_cycle(ռ�ձ�) = 50*10us = 0.5ms -> 0��
//  pulse1 = (uint16_t)(50 + angle1 * 200/270.0);  //��ת����ʽ�����pwm��arr��psc����������Ӧ�仯
//  TIM_SetCompare1(TIM3, pulse1);        //PWM3-CH1
//}


void servo1_angle(uint16_t angle1)
{
  uint16_t pulse1;
  
  //��Զ����ת�Ƕ��޷�
  if(angle1 <= 5)
    angle1 = 5;
  if(angle1 >= 175)            //angle: 0-180  ����ֵ50-250 (��ֵarr = 2000) y=kx+b  ->  k=(250-50)/(180-0) = 100/90 , b = 50
    angle1 = 175;
  //���Ƕ�ֵת��Ϊ����ֵ       //angle=0ʱ��pulse=50, duty_cycle(ռ�ձ�) = 50*10us = 0.5ms -> 0��
  pulse1 = (uint16_t)(50 + angle1 * 100/90.0);  //��ת����ʽ�����pwm��arr��psc����������Ӧ�仯
  TIM_SetCompare1(TIM3, pulse1);       //PWM3-CH2
}


//0.5ms--0��  2.5ms--180��          <1> 0.5*1000/10=50������        <2> 2.5*1000us/10=250������
void servo2_angle(uint16_t angle2)
{
  uint16_t pulse2;
  
  //��Զ����ת�Ƕ��޷�
  if(angle2 <= 5)
    angle2 = 5;
  if(angle2 >= 175)            //angle: 0-180  ����ֵ50-250 (��ֵarr = 2000) y=kx+b  ->  k=(250-50)/(180-0) = 100/90 , b = 50
    angle2 = 175;
  //���Ƕ�ֵת��Ϊ����ֵ       //angle=0ʱ��pulse=50, duty_cycle(ռ�ձ�) = 50*10us = 0.5ms -> 0��
  pulse2 = (uint16_t)(50 + angle2 * 100/90.0);  //��ת����ʽ�����pwm��arr��psc����������Ӧ�仯
  TIM_SetCompare2(TIM3, pulse2);       //PWM3-CH2
}

void servo3_angle(uint16_t angle3)
{
  uint16_t pulse3;
  
  //��Զ����ת�Ƕ��޷�
  if(angle3 <= 5)
    angle3 = 5;
  if(angle3 >= 175)            //angle: 0-180  ����ֵ50-250  y=kx+b   ->    k=(250-50)/(180-0) = 100/90 , b = 50
    angle3 = 175;
  //���Ƕ�ֵת��Ϊ����ֵ       //angle=0ʱ��pulse=50, duty_cycle(ռ�ձ�) = 50*10us = 0.5ms -> 0��
  pulse3 = (uint16_t)(50 + angle3 * 100/90.0);  //��ת����ʽ�����pwm��arr��psc����������Ӧ�仯
  TIM_SetCompare3(TIM3, pulse3);       //PWM3-CH2
}


void servo_debug(void)
{
  uint8_t i;
  for(i = 0; i < 10; i++)
  {
    delay_ms(1000);
    servo1_angle(45);
    delay_ms(1000);
    servo1_angle(90);
    delay_ms(1000);
    servo1_angle(135);
    delay_ms(1000);
    servo1_angle(90);
  }
}

/*********************************************END OF FILE**********************/
