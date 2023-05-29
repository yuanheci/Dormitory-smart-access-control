#include "led.h"


void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;                 //PC13
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	LED_ON;
}


//¼ÌµçÆ÷²âÊÔPB8-------------ÃÅ¼ÌµçÆ÷
void RELAYPB8_GPIO_Config(void)        //PB8
{
	GPIO_InitTypeDef RELAY_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RELAY_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	RELAY_InitStruct.GPIO_Pin = GPIO_Pin_8;
	RELAY_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &RELAY_InitStruct);
	
	//GPIO_SetBits(GPIOB,GPIO_Pin_8);     //¿ªÆô
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
}

//¼ÌµçÆ÷²âÊÔPB9--------------Ö¸ÎÆ¼ÌµçÆ÷
void RELAYPB9_GPIO_Config(void)        //PB9
{
	GPIO_InitTypeDef RELAY_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RELAY_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	RELAY_InitStruct.GPIO_Pin = GPIO_Pin_9;
	RELAY_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &RELAY_InitStruct);
	
	//GPIO_SetBits(GPIOB,GPIO_Pin_9);		//kaiqi
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
}


//·äÃùÆ÷²âÊÔ
void BEEP_GPIO_Config(void)        //PA1
{
	GPIO_InitTypeDef RELAY_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RELAY_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	RELAY_InitStruct.GPIO_Pin = GPIO_Pin_1;
	RELAY_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &RELAY_InitStruct);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	//GPIO_SetBits(GPIOA,GPIO_Pin_1);
}
