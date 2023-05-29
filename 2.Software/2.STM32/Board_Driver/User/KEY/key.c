#include "key.h"  
#include "delay.h"
/**
  * @brief  配置按键用到的I/O口，战舰学习板测试使用，STM32主板测试通过后可删除
  * @param  无
  * @retval 无
  */
#if 0                         
void Key_GPIO_Config(void)     //PE2,3,4
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口的时钟*/
	RCC_APB2PeriphClockCmd(KEY0_GPIO_CLK|KEY1_GPIO_CLK|KEY2_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = KEY0_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //IPU->上拉输入
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	
}


//大创设计主板使用
#elif 1

/**
  * @brief  STM32主板配置按键用到的I/O口
  * @param  无
  * @retval 无
  */
void Key_GPIO_Config(void)     
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口的时钟*/
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK|KEY3_GPIO_CLK|KEY4_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //上拉输入        IPU->上拉输入     
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);	
}
#endif


 /*
 * 函数名：Key_Scan
 * 描述  ：检测是否有按键按下
 * 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
 *		     GPIO_Pin：待读取的端口位 	
 * 输出  ：KEY_OFF(按下按键)、KEY_ON（没按下按键）     //按下按键被拉低
 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_OFF )  
	{	 
		/*等待按键释放 */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_OFF);   
		return 	KEY_OFF;	 
	}
	else
		return KEY_ON;
}


#if 1
//大创主板使用
/*按键按下为高电平*/
/*--------MY  KEY_SCAN-----------*/
uint8_t key_scan(void)
{
	uint8_t key1_state=0,key2_state=0,key3_state=0,key4_state=0;
	uint8_t key_val=0;
	
	key1_state = GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN);      //KEY1
	key2_state = GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN);      //KEY2
	key3_state = GPIO_ReadInputDataBit(KEY3_GPIO_PORT,KEY3_GPIO_PIN);      //KEY3
	key4_state = GPIO_ReadInputDataBit(KEY4_GPIO_PORT,KEY4_GPIO_PIN);      //KEY4
	
	if(key1_state == KEY_OFF)          //KEY_ON = 1 表示有按键按下
	{
		delay_ms(10);
		if(key1_state == KEY_OFF)
		{
			key_val = 1;
			while(!GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN));
		}	
	}
	else if(key2_state == KEY_OFF)
	{
		delay_ms(10);
		if(key2_state == KEY_OFF)
		{
			key_val = 2;
			while(!GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN));
		}
	}
	else if(key3_state == KEY_OFF)
	{
		delay_ms(10);
		if(key3_state == KEY_OFF)
		{
			key_val = 3;
			while(!GPIO_ReadInputDataBit(KEY3_GPIO_PORT,KEY3_GPIO_PIN));
		}
	}
	else if(key4_state == KEY_OFF)
	{
		delay_ms(10);
		if(key4_state == KEY_OFF)
		{
			key_val = 4;
			while(!GPIO_ReadInputDataBit(KEY4_GPIO_PORT,KEY4_GPIO_PIN));
		}
	}
	
	return key_val;
}




#elif 0
//战舰V2测试使用

/*--------MY  KEY_SCAN-----------*/
uint8_t key_scan(void)
{
	uint8_t key1_state=0,key2_state=0,key3_state=0,key4_state=0;
	uint8_t key_val=0;
	
	key1_state = GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN);      //KEY1
	key2_state = GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN);      //KEY2
	//key3_state = GPIO_ReadInputDataBit(KEY3_GPIO_PORT,KEY3_GPIO_PIN);      //KEY3
	//key4_state = GPIO_ReadInputDataBit(KEY4_GPIO_PORT,KEY4_GPIO_PIN);      //KEY4
	
	
	if(key1_state == KEY_OFF)          //KEY_ON = 1 表示有按键按下
	{
		delay_ms(10);
		if(key1_state == KEY_OFF)
		{
			key_val = 1;
			while(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == RESET);
		}	
	}
	else if(key2_state == KEY_OFF)
	{
		delay_ms(10);
		if(key2_state == KEY_OFF)
		{
			key_val = 2;
			while(GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == RESET);
		}
	}
//	else if(key3_state == KEY_ON)
//	{
//		delay_ms(10);
//		if(key3_state == KEY_ON)
//		{
//			key_val = 3;
//			while(GPIO_ReadInputDataBit(KEY3_GPIO_PORT,KEY3_GPIO_PIN));
//		}
//	}
//	else if(key4_state == KEY_ON)
//	{
//		delay_ms(10);
//		if(key4_state == KEY_ON)
//		{
//			key_val = 4;
//			while(GPIO_ReadInputDataBit(KEY4_GPIO_PORT,KEY4_GPIO_PIN));
//		}
//	}
	
	return key_val;
}

#endif
/*********************************************END OF FILE**********************/
