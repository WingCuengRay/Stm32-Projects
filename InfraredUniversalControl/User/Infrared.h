/***************************************************************************************
 *	File Name				:		Infrared.h
 *	CopyRight				:
 *	ModuleName			:		学习型红外遥控模块初始化
 *
 *	CPU						:
 *	RTOS					:
 *
 *	Create Data				:	2015/03/11
 *	Author/Corportation		:	Ray
 *
 *	Abstract Description	:	this will be used for 
 *
 *--------------------------------Revision History--------------------------------------
 *	No	version		Data			Revised By			Item			Description
 *	1		v1.0			2015/4/21	Ray												Create this file
 *
 ***************************************************************************************/


/**************************************************************
*	Multi-Include-Prevent Section
**************************************************************/
#ifndef __INFRARED_H
#define __INFRARED_H


/**************************************************************
*	Debug switch Section
**************************************************************/



/**************************************************************
*	Include File Section
**************************************************************/
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"


/**************************************************************
*	Macro Define Section
**************************************************************/
#define INFRARED_GPIOx	GPIOA
#define INFRARED_GPIO_Pinx	GPIO_Pin_1
#define INFRARED_RCC_GPIOx	RCC_APB2Periph_GPIOA
#define INFRARED_EXTI_GPIOx	GPIO_PortSourceGPIOA
#define INFRARED_EXTI_Line	GPIO_PinSource1

#define IR_RDATA PAin(1)			//红外数据输入脚

#define INFRARED_RECEIVE
#define INFRARED_SEND


/**************************************************************
*	Struct Define Section
**************************************************************/
#define MAX_PULSE_LEN 300



/**************************************************************
*	Prototype Declare Section
**************************************************************/
extern u8 Flag_LearnState;
extern u16 PulseTab[MAX_PULSE_LEN];

//Functions for Receive
void Infrared_GPIO_Configuration();
void Infrared_EXTI_Configuration();

//Functions for Send
void TIM2_PWM_Init(u16 arr,u16 psc);
void Infrared_Send();
/**************************************************************
*	End-Multi-Include-Prevent Section
**************************************************************/
#endif