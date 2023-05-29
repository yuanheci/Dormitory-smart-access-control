#ifndef __KEY_H
#define	__KEY_H

#include "stm32f10x.h"

//ս��ѧϰ�����ʹ��
#if 0
//  ���Ŷ���
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
//STM32����ʹ��
//  ���Ŷ���
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

 /** �������±��ú�
	*  ��������Ϊ�ߵ�ƽ������ KEY_ON=1�� KEY_OFF=0
	*  ����������Ϊ�͵�ƽ���Ѻ����ó�KEY_ON=0 ��KEY_OFF=1 ����
	*/
#define KEY_ON	1
#define KEY_OFF	0

void Key_GPIO_Config(void);
uint8_t key_scan(void);


//uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif /* __KEY_H */

