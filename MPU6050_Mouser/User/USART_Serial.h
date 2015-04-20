/***************************************************************************************
 *	File Name				:		USART_Serial.h
 *	CopyRight				:
 *	ModuleName			:		
 *
 *	CPU						:
 *	RTOS					:
 *
 *	Create Data				:	2015/03/11
 *	Author/Corportation		:	Ray
 *
 *	Abstract Description	:	this will be used for USART_Serial.c
 *
 *--------------------------------Revision History--------------------------------------
 *	No	version		Data			Revised By			Item			Description
 *	1		v1.0			2015/3/11	Ray												Create this file
 *
 ***************************************************************************************/


/**************************************************************
*	Multi-Include-Prevent Section
**************************************************************/
#ifndef __USART_SERIAL_H
#define __USART_SERIAL_H


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
#define USART_GPIO_GROUP	GPIOA
#define USART_APB_Periph	RCC_APB2Periph_GPIOA
#define TXD	GPIO_Pin_9
#define	RXD	GPIO_Pin_10



/**************************************************************
*	Struct Define Section
**************************************************************/



/**************************************************************
*	Prototype Declare Section
**************************************************************/
extern void USART_Configuration();
extern void USART_GPIO_Configuration();

void USART_SendByte(uint8_t ch);
void UART1_ReportMotion(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz,
					int16_t hx,int16_t hy,int16_t hz);
void Wait_Respond(u32 timeOut);	



/**************************************************************
*	End-Multi-Include-Prevent Section
**************************************************************/
#endif