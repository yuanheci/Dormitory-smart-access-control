#ifndef __GENERALTIME_H
#define __GENERALTIME_H

#include "stm32f10x.h"

/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
 

//TIM2----��ָ��ģ��AS608ͨ��  
#define            AS608_TIM                     TIM2
#define            AS608_TIM_Period              999                 //һ��ʼ��9 
#define            AS608_TIM_Prescaler           71               //ʱ������10us  

//TIM3----PWM���  
#define            GENERAL_TIM                   TIM3
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd    //����20ms, Ƶ��50HZ
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM3
#define            GENERAL_TIM_Period            1999                 //һ��ʼ��9       T = (1999+1)*10us = 20ms
#define            GENERAL_TIM_Prescaler         719                  //ʱ������10us  
                                                                      //72 000 000/720 = 100 000Hz -> T = 10us

/*---------------------------------------------------------------------------------------------------*/
// TIM3 ����Ƚ�ͨ��1
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH1_PORT          GPIOA
#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_6

// TIM3 ����Ƚ�ͨ��2
#define            GENERAL_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH2_PORT          GPIOA
#define            GENERAL_TIM_CH2_PIN           GPIO_Pin_7

// TIM3 ����Ƚ�ͨ��3
#define            GENERAL_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL_TIM_CH3_PORT          GPIOB
#define            GENERAL_TIM_CH3_PIN           GPIO_Pin_0

// TIM3 ����Ƚ�ͨ��4
#define            GENERAL_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL_TIM_CH4_PORT          GPIOB
#define            GENERAL_TIM_CH4_PIN           GPIO_Pin_1
          

#define            GENERAL_TIM2_IRQ               TIM2_IRQn
#define            GENERAL_TIM3_IRQ               TIM3_IRQn


/**************************��������********************************/

void GENERAL_TIM_Init(void);
void GENERAL_TIM_NVIC_Config(void);

void servo_init(void);
void servo1_angle(uint16_t angle1);
void servo2_angle(uint16_t angle2);
void servo3_angle(uint16_t angle3);
void servo_debug(void);
void delay(uint32_t time);

extern void implement(void);           //PID����


#endif	/* __GENERALTIME_H */


