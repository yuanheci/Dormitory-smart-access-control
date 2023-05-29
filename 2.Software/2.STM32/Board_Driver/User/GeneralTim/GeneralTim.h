#ifndef __GENERALTIME_H
#define __GENERALTIME_H

#include "stm32f10x.h"

/************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
 

//TIM2----与指纹模块AS608通信  
#define            AS608_TIM                     TIM2
#define            AS608_TIM_Period              999                 //一开始是9 
#define            AS608_TIM_Prescaler           71               //时钟周期10us  

//TIM3----PWM输出  
#define            GENERAL_TIM                   TIM3
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd    //周期20ms, 频率50HZ
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM3
#define            GENERAL_TIM_Period            1999                 //一开始是9       T = (1999+1)*10us = 20ms
#define            GENERAL_TIM_Prescaler         719                  //时钟周期10us  
                                                                      //72 000 000/720 = 100 000Hz -> T = 10us

/*---------------------------------------------------------------------------------------------------*/
// TIM3 输出比较通道1
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH1_PORT          GPIOA
#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_6

// TIM3 输出比较通道2
#define            GENERAL_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH2_PORT          GPIOA
#define            GENERAL_TIM_CH2_PIN           GPIO_Pin_7

// TIM3 输出比较通道3
#define            GENERAL_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL_TIM_CH3_PORT          GPIOB
#define            GENERAL_TIM_CH3_PIN           GPIO_Pin_0

// TIM3 输出比较通道4
#define            GENERAL_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL_TIM_CH4_PORT          GPIOB
#define            GENERAL_TIM_CH4_PIN           GPIO_Pin_1
          

#define            GENERAL_TIM2_IRQ               TIM2_IRQn
#define            GENERAL_TIM3_IRQ               TIM3_IRQn


/**************************函数声明********************************/

void GENERAL_TIM_Init(void);
void GENERAL_TIM_NVIC_Config(void);

void servo_init(void);
void servo1_angle(uint16_t angle1);
void servo2_angle(uint16_t angle2);
void servo3_angle(uint16_t angle3);
void servo_debug(void);
void delay(uint32_t time);

extern void implement(void);           //PID控制


#endif	/* __GENERALTIME_H */


