#ifndef __LED_H_
#define __LED_H_


#include "stm32f10x.h"

void LED_GPIO_Config(void);
void RELAYPB8_GPIO_Config(void);
void RELAYPB9_GPIO_Config(void);
void BEEP_GPIO_Config(void);

//#define     LED_ON        GPIO_ResetBits(GPIOC,GPIO_Pin_13)
//#define     LED_OFF       GPIO_SetBits(GPIOC,GPIO_Pin_13)
//#define     LED_TOGGLE    GPIOC->ODR ^= GPIO_Pin_13

#define     LED_ON        GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define     LED_OFF       GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define     LED_TOGGLE    GPIOB->ODR ^= GPIO_Pin_12

//#define     LED_ON        GPIO_ResetBits(GPIOE,GPIO_Pin_5)
//#define     LED_OFF       GPIO_SetBits(GPIOE,GPIO_Pin_5)
//#define     LED_TOGGLE    GPIOE->ODR ^= GPIO_Pin_5


#define   RELAY8_ON		GPIO_SetBits(GPIOB,GPIO_Pin_8)       //门继电器
#define   RELAY8_OFF	GPIO_ResetBits(GPIOB,GPIO_Pin_8)

#define   RELAY9_ON		GPIO_SetBits(GPIOB,GPIO_Pin_9)       //指纹继电器
#define   RELAY9_OFF	GPIO_ResetBits(GPIOB,GPIO_Pin_9)

#define   BUZZER_TOGGLE     GPIOA->ODR ^= GPIO_Pin_1

#endif    /*__LED_H_*/

