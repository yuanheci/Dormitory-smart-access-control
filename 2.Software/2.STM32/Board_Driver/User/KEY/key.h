#ifndef __KEY_H
#define	__KEY_H

#include "stm32f10x.h"

//战舰学习板测试使用
#if 0
//  引脚定义
#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOE         //KEY2
#define    KEY2_GPIO_PORT    GPIOE			   
#define    KEY2_GPIO_PIN	 GPIO_Pin_2

#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOE         //KEY1
#define    KEY1_GPIO_PORT    GPIOE		   
#define    KEY1_GPIO_PIN	 GPIO_Pin_3

#define    KEY0_GPIO_CLK     RCC_APB2Periph_GPIOE         //KEY0
#define    KEY0_GPIO_PORT    GPIOE		   
#define    KEY0_GPIO_PIN	 GPIO_Pin_4


#elif 1
//STM32主板使用
//  引脚定义
#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOA         //KEY1 -- PA8
#define    KEY1_GPIO_PORT    GPIOA			   
#define    KEY1_GPIO_PIN	 GPIO_Pin_8

#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOB         //KEY2 -- PB5
#define    KEY2_GPIO_PORT    GPIOB		   
#define    KEY2_GPIO_PIN	 GPIO_Pin_5

#define    KEY3_GPIO_CLK     RCC_APB2Periph_GPIOB         //KEY3 -- PB6
#define    KEY3_GPIO_PORT    GPIOB		   
#define    KEY3_GPIO_PIN	 GPIO_Pin_6

#define    KEY4_GPIO_CLK     RCC_APB2Periph_GPIOB         //KEY4 -- PB7
#define    KEY4_GPIO_PORT    GPIOB		   
#define    KEY4_GPIO_PIN	 GPIO_Pin_7

#endif

 /** 按键按下标置宏
	*  按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
	*  若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
	*/
#define KEY_ON	1
#define KEY_OFF	0

void Key_GPIO_Config(void);
uint8_t key_scan(void);


//uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif /* __KEY_H */

