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

extern uint8_t as_flag;    //标志位   置1表示按下K4保存指纹模板   过一段时间要消失显示
extern uint8_t as608_move_flag;       //用于移除指纹一段时间后继电器关闭的标志位

extern uint8_t open_door_flag;    //开门标志位

uint16_t open_time_count = 0;   //用于开门记时

uint8_t clear_face_display_flag = 0;

extern uint8_t face_rec_fail_flag;

uint8_t face_rec_count = 0;

//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断   ,20ms进一次
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		steer_control_flag = 1;
		implement();       //进行舵机PID控制
		time_count++;

		if(time_count>=50)   //1s          //舵机与按键并行工作测试->有抖动，待解决
		{
			time_count=0;
			//LED_TOGGLE;
		}
		
		if(open_door_flag)
		{	
			RELAY8_ON;	                     //打开门继电器		
			open_time_count++;
			if(open_time_count<=150)   //3s
			{
				TIM_SetCompare3(TIM3,50);    //正转开门
			}
			else if(open_time_count<=200)     //开门后停转1s
			{
				TIM_SetCompare3(TIM3,150);    //舵机停止 
			}
			else if(open_time_count<=350)     //反转3s关门
			{
				TIM_SetCompare3(TIM3,250);    //反转  
			}	
			else
			{
				TIM_SetCompare3(TIM3,150);   //舵机停止
				open_time_count = 0;       //清空记时
				open_door_flag = 0;        //标志位置0
				
				clear_face_display_flag = 1;	

				RELAY8_OFF;               //关闭门继电器
			}
			
		}
		
		if(face_rec_fail_flag)
		{
			face_rec_count++;
			if(face_rec_count >= 150)   //3s
			{
				face_rec_count = 0;
				face_rec_fail_flag = 0;
				clear_face_display_flag = 1;   //执行清屏
			}
		}
		
	
		LCD_xy_display_flag=1;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志 
	}
}


extern vu16 USART3_RX_STA;

void TIM2_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//是更新中断
	{	 			   
		USART3_RX_STA|=1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIM2更新中断标志    
		TIM_Cmd(TIM2, DISABLE);  //关闭TIM2      ->此处不关闭定时器会进入卡死状态  ->  ??
	}	    
}




//用于OEPNMV通信
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	int Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)  //会一直进入
	{
		Res = USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据

		face_inf_Rec(Res);        //接收人脸识别数据
		Openmv_Recive(Res);	      //接收坐标数据
				
		if(openmv[2]!=0 && openmv[3]!=0)        //只接受不等于0的信息
			Openmv_data();
		if(face_information[2] != 0)		    //接收人脸判断信息
			Face_data();

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);   //清除 USART1 的中断待处理位	 		 
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
