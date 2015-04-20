/***************************************************************************************
 *	File Name				:		MPU6050.h	
 *	CopyRight				:
 *	ModuleName			:		
 *
 *	CPU						:
 *	RTOS					:
 *
 *	Create Data				:	2015/03/14
 *	Author/Corportation		:	Ray
 *
 *	Abstract Description	:	this will be used for 
 *
 *--------------------------------Revision History--------------------------------------
 *	No	version		Data			Revised By			Item			Description
 *	1		v1.0			2015/3/14	Ray												Create this file
 *
 ***************************************************************************************/


/**************************************************************
*	Multi-Include-Prevent Section
**************************************************************/
#ifndef __MPU6050_H
#define __MPU6050_H


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
#define	SMPLRT_DIV		0x19	//
#define	CONFIG			0x1A	
#define	GYRO_CONFIG		0x1B	
#define	ACCEL_CONFIG	0x1C	
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//????,???:0x00(????)
#define SlaveAddress	0xD0

#define CNT 1

/**************************************************************
*	Struct Define Section
**************************************************************/
typedef struct Acceleration
{
	int32_t AccelX[2];
	int32_t AccelY[2];
	int32_t AccelZ[2];
}Accel,*pAccel;

typedef struct Velocity
{
	int32_t VelocityX[2];
	int32_t VelocityY[2];
	int32_t VelocityZ[2];
}Velocity,*pVelocity;

typedef struct Displacement
{
	int32_t DpmX[2];
	int32_t DpmY[2];
	int32_t DpmZ[2];
}Displacement,*pDisplayment;

typedef struct Acceleration_Adjust
{
	int16_t OffsetX;
	int16_t OffsetY;
	int16_t OffsetZ;
}AccelRevise,*pAccelRevise;


/**************************************************************
*	Prototype Declare Section
**************************************************************/
extern uint16_t GetData(uint8_t REG_Address);
extern void InitMPU6050();
void GetAccel();
void Self_Revise(pAccelRevise p);
void MovementEndCheck();

extern int16_t ax,ay,az,gx,gy,gz;
extern Accel AccelType;
extern AccelRevise AccelReviseType;
extern Velocity VelocityType;
extern Displacement DpmType;

/**************************************************************
*	End-Multi-Include-Prevent Section
**************************************************************/
#endif