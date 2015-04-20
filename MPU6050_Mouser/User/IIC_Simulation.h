/***************************************************************************************
 *	File Name				:			IIC_Simulation.h
 *	CopyRight				:
 *	ModuleName			:	
 *
 *	CPU						:
 *	RTOS					:
 *
 *	Create Data				:	2015/03/11
 *	Author/Corportation		:	Ray
 *
 *	Abstract Description	:	this will be used for IIC_Simulation.c
 *
 *--------------------------------Revision History--------------------------------------
 *	No	version		Data			Revised By			Item			Description
 *	1		v1.0			2015/3/11 Ray												Create this file
 *
 ***************************************************************************************/


/**************************************************************
*	Multi-Include-Prevent Section
**************************************************************/
#ifndef __IIC_SIMULATION_H
#define __IIC_SIMULATION_H


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
#define IIC_GPIO_Group	GPIOB
#define IIC_APB_Periph 	RCC_APB2Periph_GPIOB
#define SCL	GPIO_Pin_6
#define SDA	GPIO_Pin_7

/**************************************************************
*	Struct Define Section
**************************************************************/
typedef enum{FALSE = 0,TRUE}Status;


/**************************************************************
*	Prototype Declare Section
**************************************************************/
extern void IIC_GPIO_Configuration();
extern void Single_WriteIIC(uint8_t REG_Address,uint8_t REG_data);
extern uint8_t Single_ReadIIC(uint8_t REG_Address);
/**************************************************************
*	End-Multi-Include-Prevent Section
**************************************************************/
#endif
