/***************************************************************************************
 *	FileName					:		TIM.c
 *	CopyRight					:
 *	ModuleName				:			
 *	
 *	CPU							:			STM32F103ZET
 *	RTOS						:
 *
 *	Create Data					:	2015/03/14
 *	Author/Corportation			:	Ray
 *
 *	Abstract Description		:	
 *
 *--------------------------------Revision History--------------------------------------
 *	No	version		Data			Revised By			Item			Description
 *	1		v1.0			2015/3/14	Ray												Create this file
 *
 ***************************************************************************************/



/**************************************************************
*	Debug switch Section
**************************************************************/


/**************************************************************
*	Include File Section
**************************************************************/
#include "stm32f10x.h"

/**************************************************************
*	Macro Define Section
**************************************************************/


/**************************************************************
*	Struct Define Section
**************************************************************/


/**************************************************************
*	Prototype Declare Section
**************************************************************/


/**************************************************************
*	Global Variable Declare Section
**************************************************************/


/**************************************************************
*	File Static Variable Define Section
**************************************************************/


/**************************************************************
*	Function Define Section
**************************************************************/
/**
 *  @name					void NVIC_Configuration();
 *	@description	开启TIM2中断
 *	@param			
 *	@return		
 *  @notice
 */
void NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/**
 *  @name						void TIM2_Configuration();
 *	@description		定时器2中断设置，72分频，计数值为60000
 *	@param			
 *	@return		
 *  @notice
 */
void TIM2_Configuration()
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_BaseInitStructure.TIM_Prescaler = 719;				//每10us定时器加1
	TIM_BaseInitStructure.TIM_Period = 3125;						//每31.25ms中断一次
	
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数模式
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2,&TIM_BaseInitStructure);
	TIM_Cmd(TIM2,ENABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	return;
}

/**
 *  @name
 *	@description
 *	@param			
 *	@return		
 *  @notice
 */
