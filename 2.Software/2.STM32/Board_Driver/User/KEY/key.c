#include "key.h"  
#include "delay.h"
/**
  * @brief  ���ð����õ���I/O�ڣ�ս��ѧϰ�����ʹ�ã�STM32�������ͨ�����ɾ��
  * @param  ��
  * @retval ��
  */
#if 0                         
void Key_GPIO_Config(void)     //PE2,3,4
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*���������˿ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(KEY0_GPIO_CLK|KEY1_GPIO_CLK|KEY2_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = KEY0_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //IPU->��������
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	
}


//���������ʹ��
#elif 1

/**
  * @brief  STM32�������ð����õ���I/O��
  * @param  ��
  * @retval ��
  */
void Key_GPIO_Config(void)     
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*���������˿ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK|KEY3_GPIO_CLK|KEY4_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //��������        IPU->��������     
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
 * ��������Key_Scan
 * ����  ������Ƿ��а�������
 * ����  ��GPIOx��x ������ A��B��C��D���� E
 *		     GPIO_Pin������ȡ�Ķ˿�λ 	
 * ���  ��KEY_OFF(���°���)��KEY_ON��û���°�����     //���°���������
 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_OFF )  
	{	 
		/*�ȴ������ͷ� */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_OFF);   
		return 	KEY_OFF;	 
	}
	else
		return KEY_ON;
}


#if 1
//������ʹ��
/*��������Ϊ�ߵ�ƽ*/
/*--------MY  KEY_SCAN-----------*/
uint8_t key_scan(void)
{
	uint8_t key1_state=0,key2_state=0,key3_state=0,key4_state=0;
	uint8_t key_val=0;
	
	key1_state = GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN);      //KEY1
	key2_state = GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN);      //KEY2
	key3_state = GPIO_ReadInputDataBit(KEY3_GPIO_PORT,KEY3_GPIO_PIN);      //KEY3
	key4_state = GPIO_ReadInputDataBit(KEY4_GPIO_PORT,KEY4_GPIO_PIN);      //KEY4
	
	if(key1_state == KEY_OFF)          //KEY_ON = 1 ��ʾ�а�������
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
//ս��V2����ʹ��

/*--------MY  KEY_SCAN-----------*/
uint8_t key_scan(void)
{
	uint8_t key1_state=0,key2_state=0,key3_state=0,key4_state=0;
	uint8_t key_val=0;
	
	key1_state = GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN);      //KEY1
	key2_state = GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN);      //KEY2
	//key3_state = GPIO_ReadInputDataBit(KEY3_GPIO_PORT,KEY3_GPIO_PIN);      //KEY3
	//key4_state = GPIO_ReadInputDataBit(KEY4_GPIO_PORT,KEY4_GPIO_PIN);      //KEY4
	
	
	if(key1_state == KEY_OFF)          //KEY_ON = 1 ��ʾ�а�������
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
