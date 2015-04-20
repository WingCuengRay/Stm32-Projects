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
#include "MPU6050.h"

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


extern unsigned long g_timecount;
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	g_timecount++;
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
// 		VelocityType.VelocityX[1] = VelocityType.VelocityX[0] + AccelType.AccelX[0] + 
// 																((AccelType.AccelX[1] - AccelType.AccelX[0]) >> 1);
// 		VelocityType.VelocityY[1] = VelocityType.VelocityY[0] + AccelType.AccelY[0] + 
// 																((AccelType.AccelY[1] - AccelType.AccelY[0]) >> 1);
// 		VelocityType.VelocityZ[1] = VelocityType.VelocityZ[0] + AccelType.AccelZ[0] + 
// 																((AccelType.AccelZ[1] - AccelType.AccelZ[0]) >> 1);
		
		VelocityType.VelocityX[1] = VelocityType.VelocityX[0] + AccelType.AccelX[0] + 
																(((AccelType.AccelX[1] - AccelType.AccelX[0])>>1) >> 5);		//Éèdt=0.03125s,¼´2^-5
		DpmType.DpmX[1] = DpmType.DpmX[0] + VelocityType.VelocityX[0] + 
											(((VelocityType.VelocityX[1]-VelocityType.VelocityX[0])>>1) >> 5);
		VelocityType.VelocityY[1] = VelocityType.VelocityY[0] + AccelType.AccelY[0] + 
																(((AccelType.AccelY[1] - AccelType.AccelY[0]) >> 1) >> 5);
		DpmType.DpmY[1] = DpmType.DpmY[0] + VelocityType.VelocityY[0] + 
											(((VelocityType.VelocityY[1]-VelocityType.VelocityY[0])>>1) >> 5);
		VelocityType.VelocityZ[1] = VelocityType.VelocityZ[0] + AccelType.AccelZ[0] + 
																(((AccelType.AccelZ[1] - AccelType.AccelZ[0]) >> 1) >> 5);
		
		
		AccelType.AccelX[0] = AccelType.AccelX[1];
		AccelType.AccelY[0] = AccelType.AccelY[1];
		AccelType.AccelZ[0] = AccelType.AccelZ[1];
		VelocityType.VelocityX[0] = VelocityType.VelocityX[1];
		VelocityType.VelocityY[0] = VelocityType.VelocityY[1];
		VelocityType.VelocityZ[0] = VelocityType.VelocityZ[1];
		DpmType.DpmX[0] = DpmType.DpmX[1];
		DpmType.DpmY[0] = DpmType.DpmY[1];
		
		
// 		AccelType.AccelX[0] = AccelType.AccelX[1];
// 		AccelType.AccelY[0] = AccelType.AccelY[1];
// 		AccelType.AccelZ[0] = AccelType.AccelZ[1];
	}	
}

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
