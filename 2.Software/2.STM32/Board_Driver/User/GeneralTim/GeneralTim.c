#include "GeneralTim.h" 
#include "delay.h"


extern vu16 USART3_RX_STA; 

void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
	// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM2_IRQ;	
	// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM3_IRQ;	
	NVIC_Init(&NVIC_InitStructure);
}


static void GENERAL_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 输出比较通道1 GPIO 初始化
  RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH1_PORT, &GPIO_InitStructure);
	
  // 输出比较通道2 GPIO 初始化
  RCC_APB2PeriphClockCmd(GENERAL_TIM_CH2_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH2_PORT, &GPIO_InitStructure);
	
  // 输出比较通道3 GPIO 初始化
  RCC_APB2PeriphClockCmd(GENERAL_TIM_CH3_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH3_PORT, &GPIO_InitStructure);
	
  // 输出比较通道4 GPIO 初始化
  RCC_APB2PeriphClockCmd(GENERAL_TIM_CH4_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH4_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStructure);
}


///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void GENERAL_TIM_Mode_Config(void)                            //TIM2,TIM3
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
  // 开启定时器时钟,即内部时钟CK_INT=72M
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK,ENABLE);
	
/*--------------------时基结构体初始化-------------------------*/
	// 配置周期，这里配置为100K
	
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_Period;	  //2000个脉冲值为一个周期   T=10us*2000=20ms
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler;	     //已配成10us一个脉冲
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器  TIM3
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period=AS608_TIM_Period;	  //TIM2-999
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= AS608_TIM_Prescaler;   //71	
	// 初始化定时器  TIM2
	TIM_TimeBaseInit(AS608_TIM, &TIM_TimeBaseStructure);
	
	// 清除计数器中断标志位
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	// 开启计数器中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

	/*--------------------输出比较结构体初始化-------------------*/	
	// 占空比配置
//	uint16_t CCR1_Val = 250;     //5
//	uint16_t CCR2_Val = 4;
//	uint16_t CCR3_Val = 3;
//	uint16_t CCR4_Val = 2;
	

	// 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 输出通道电平极性配置	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	// 输出比较通道 1
	//TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OC1Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// 输出比较通道 2
	//TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
	TIM_OC2Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// 输出比较通道 3
//	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
	TIM_OC3Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// 输出比较通道 4
//	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
	TIM_OC4Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// 使能计数器
	TIM_Cmd(AS608_TIM, ENABLE);
	TIM_Cmd(GENERAL_TIM, ENABLE);
}


void GENERAL_TIM_Init(void)      //TIM2,TIM3
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_Mode_Config();		
}


void servo_init(void)      //CCR寄存器的值->占空比
{
  TIM_SetCompare1(TIM3,50);  //使舵机恢复到中间位置   //150
  TIM_SetCompare2(TIM3,50);  //使舵机恢复到中间位置
}


void delay(uint32_t time)
{
	uint32_t i;
	for(i=0;i<time;i++);
}

                                  //一个脉冲 10us
////0.5ms--0°  2.5ms--270°          <1> 0.5*1000/10=50个脉冲        <2> 2.5*1000us/10=250个脉冲
//void servo1_angle(uint16_t angle1)
//{
//  uint16_t pulse1;
//  
//  //针对舵机可转角度限辐
//  if(angle1 <= 5)
//    angle1 = 5;                //20ms周期  0.5ms--0°      2.5ms--270°                     200/270           
//  if(angle1 >= 265)            //angle: 0-270  脉冲值50-250 (满值arr = 2000) y=kx+b  ->  k=(250-50)/(270-0) = 20/27 , b = 50
//    angle1 = 265;
//  //将角度值转换为脉冲值       //angle=0时，pulse=50, duty_cycle(占空比) = 50*10us = 0.5ms -> 0°
//  pulse1 = (uint16_t)(50 + angle1 * 200/270.0);  //此转换公式需根据pwm的arr及psc配置来做相应变化
//  TIM_SetCompare1(TIM3, pulse1);        //PWM3-CH1
//}


void servo1_angle(uint16_t angle1)
{
  uint16_t pulse1;
  
  //针对舵机可转角度限辐
  if(angle1 <= 5)
    angle1 = 5;
  if(angle1 >= 175)            //angle: 0-180  脉冲值50-250 (满值arr = 2000) y=kx+b  ->  k=(250-50)/(180-0) = 100/90 , b = 50
    angle1 = 175;
  //将角度值转换为脉冲值       //angle=0时，pulse=50, duty_cycle(占空比) = 50*10us = 0.5ms -> 0°
  pulse1 = (uint16_t)(50 + angle1 * 100/90.0);  //此转换公式需根据pwm的arr及psc配置来做相应变化
  TIM_SetCompare1(TIM3, pulse1);       //PWM3-CH2
}


//0.5ms--0°  2.5ms--180°          <1> 0.5*1000/10=50个脉冲        <2> 2.5*1000us/10=250个脉冲
void servo2_angle(uint16_t angle2)
{
  uint16_t pulse2;
  
  //针对舵机可转角度限辐
  if(angle2 <= 5)
    angle2 = 5;
  if(angle2 >= 175)            //angle: 0-180  脉冲值50-250 (满值arr = 2000) y=kx+b  ->  k=(250-50)/(180-0) = 100/90 , b = 50
    angle2 = 175;
  //将角度值转换为脉冲值       //angle=0时，pulse=50, duty_cycle(占空比) = 50*10us = 0.5ms -> 0°
  pulse2 = (uint16_t)(50 + angle2 * 100/90.0);  //此转换公式需根据pwm的arr及psc配置来做相应变化
  TIM_SetCompare2(TIM3, pulse2);       //PWM3-CH2
}

void servo3_angle(uint16_t angle3)
{
  uint16_t pulse3;
  
  //针对舵机可转角度限辐
  if(angle3 <= 5)
    angle3 = 5;
  if(angle3 >= 175)            //angle: 0-180  脉冲值50-250  y=kx+b   ->    k=(250-50)/(180-0) = 100/90 , b = 50
    angle3 = 175;
  //将角度值转换为脉冲值       //angle=0时，pulse=50, duty_cycle(占空比) = 50*10us = 0.5ms -> 0°
  pulse3 = (uint16_t)(50 + angle3 * 100/90.0);  //此转换公式需根据pwm的arr及psc配置来做相应变化
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
