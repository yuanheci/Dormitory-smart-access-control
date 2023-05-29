/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "led.h"
#include "GeneralTim.h" 
#include "openmv.h"
#include "usart1.h"
#include "lcd.h"
#include "as608.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

uint8_t LCD_xy_display_flag=0;

extern uint16_t time_count;
uint8_t steer_control_flag = 0;

uint16_t AS608_count = 0;
uint8_t as608_move_count = 0;

extern uint8_t as_flag;    //��־λ   ��1��ʾ����K4����ָ��ģ��   ��һ��ʱ��Ҫ��ʧ��ʾ
extern uint8_t as608_move_flag;       //�����Ƴ�ָ��һ��ʱ���̵����رյı�־λ

extern uint8_t open_door_flag;    //���ű�־λ

uint16_t open_time_count = 0;   //���ڿ��ż�ʱ

uint8_t clear_face_display_flag = 0;

extern uint8_t face_rec_fail_flag;

uint8_t face_rec_count = 0;

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�   ,20ms��һ��
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		steer_control_flag = 1;
		implement();       //���ж��PID����
		time_count++;

		if(time_count>=50)   //1s          //����밴�����й�������->�ж����������
		{
			time_count=0;
			//LED_TOGGLE;
		}
		
		if(open_door_flag)
		{	
			RELAY8_ON;	                     //���ż̵���		
			open_time_count++;
			if(open_time_count<=150)   //3s
			{
				TIM_SetCompare3(TIM3,50);    //��ת����
			}
			else if(open_time_count<=200)     //���ź�ͣת1s
			{
				TIM_SetCompare3(TIM3,150);    //���ֹͣ 
			}
			else if(open_time_count<=350)     //��ת3s����
			{
				TIM_SetCompare3(TIM3,250);    //��ת  
			}	
			else
			{
				TIM_SetCompare3(TIM3,150);   //���ֹͣ
				open_time_count = 0;       //��ռ�ʱ
				open_door_flag = 0;        //��־λ��0
				
				clear_face_display_flag = 1;	

				RELAY8_OFF;               //�ر��ż̵���
			}
			
		}
		
		if(face_rec_fail_flag)
		{
			face_rec_count++;
			if(face_rec_count >= 150)   //3s
			{
				face_rec_count = 0;
				face_rec_fail_flag = 0;
				clear_face_display_flag = 1;   //ִ������
			}
		}
		
	
		LCD_xy_display_flag=1;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־ 
	}
}


extern vu16 USART3_RX_STA;

void TIM2_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		USART3_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIM2�����жϱ�־    
		TIM_Cmd(TIM2, DISABLE);  //�ر�TIM2      ->�˴����رն�ʱ������뿨��״̬  ->  ??
	}	    
}




//����OEPNMVͨ��
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	int Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)  //��һֱ����
	{
		Res = USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������

		face_inf_Rec(Res);        //��������ʶ������
		Openmv_Recive(Res);	      //������������
				
		if(openmv[2]!=0 && openmv[3]!=0)        //ֻ���ܲ�����0����Ϣ
			Openmv_data();
		if(face_information[2] != 0)		    //���������ж���Ϣ
			Face_data();

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);   //��� USART1 ���жϴ�����λ	 		 
  } 
} 


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
