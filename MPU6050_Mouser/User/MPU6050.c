/***************************************************************************************
 *	FileName					:		MPU6050.c
 *	CopyRight					:
 *	ModuleName				:	
 *
 *	CPU							:
 *	RTOS						:
 *
 *	Create Data					:	2015/03/30
 *	Author/Corportation			:	Ray
 *
 *	Abstract Description		:	
 *
 *--------------------------------Revision History--------------------------------------
 *	No	version		Data			Revised By			Item			Description
 *	1		v1.0			2015/3/30	Ray												Create this file
 *
 ***************************************************************************************/



/**************************************************************
*	Debug switch Section
**************************************************************/


/**************************************************************
*	Include File Section
**************************************************************/


/**************************************************************
*	Macro Define Section
**************************************************************/
# include	"stm32f10x.h"
# include "MPU6050.h"
# include "IIC_Simulation.h"

# define CHECK_CNT 2
/**************************************************************
*	Struct Define Section
**************************************************************/


/**************************************************************
*	Prototype Declare Section
**************************************************************/
static void delay_nus(u16 time);

/**************************************************************
*	Global Variable Declare Section
**************************************************************/
AccelRevise AccelReviseType;
Accel AccelType = {0};
Velocity VelocityType = {0};
Displacement DpmType = {0};
int16_t countX,countY,countZ;

/**************************************************************
*	File Static Variable Define Section
**************************************************************/


/**************************************************************
*	Function Define Section
**************************************************************/
/**
 *  @name					void InitMPU6050()；			
 *	@description	初始化MPU6050
 *	@param			
 *	@return		
 *  @notice
 */
void InitMPU6050()
{
	Single_WriteIIC(PWR_MGMT_1,0X00);
	Single_WriteIIC(SMPLRT_DIV,0X07);
	Single_WriteIIC(CONFIG,0x06);
	Single_WriteIIC(GYRO_CONFIG,0X18);		//FS_SEL[1:0] = 11	 0001 1000B	
																				//Full-Scale Range:+500%s,Sensitivity Scale Factor:16.4LSB/(%s)
	Single_WriteIIC(ACCEL_CONFIG,0X01);		//AFS_SEL[1:0] = 00	 0000 0001B
																				//Full-scale Range:+2g;Sensitivity Scale Factor:16384LSB/g
	return;
}




/**
 *  @name					pAccelRevise Self_Revise(pAccelRevise p)
 *	@description	加速度计自校正程序，用于消除初始漂移。
 *	@param				p	----	pAccelRevise,指向加速度偏移的结构体
 *	@return		
 *  @notice
 */
void Self_Revise(pAccelRevise p)
{
	uint16_t i;
	int32_t AX = 0,AY = 0,AZ = 0;
	
	for(i=0; i<0x0400; i++)
	{
		AX += (int16_t)GetData(ACCEL_XOUT_H);
		AY += (int16_t)GetData(ACCEL_YOUT_H);
		AZ += (int16_t)GetData(ACCEL_ZOUT_H);
	}	
	
	p->OffsetX = AX >> 10;
	p->OffsetY = AY >> 10;
	p->OffsetZ = AZ >> 10;
	
	return;
}



/**	
 *  @name					uint16_t GetData(uint8_t REG_Address);
 *	@description	读出X,Y,Z轴的角速度或角速度数据
 *	@param			
 *	@return		
 *  @notice
 */
uint16_t GetData(uint8_t REG_Address)
{
	uint16_t dat;
	
	dat = Single_ReadIIC(REG_Address);
	dat <<= 8;
	dat |= Single_ReadIIC(REG_Address+1);
	
	return dat;
}




/**
 *  @name					void GetAccel(pAccelRevise p);	
 *	@description	读取加速度角速度数据
 *	@param				
 *	@return		
 *  @notice
 */
void GetAccel()
{
	int32_t AX = 0,AY = 0,AZ = 0;
	uint16_t i;
	
	for(i=0; i<0x10; i++)
	{
		AX += (int16_t)GetData(ACCEL_XOUT_H);
		AY += (int16_t)GetData(ACCEL_YOUT_H);
		AZ += (int16_t)GetData(ACCEL_ZOUT_H);
	}
	
	AccelType.AccelX[1] = (AX >> 4) - AccelReviseType.OffsetX;
	AccelType.AccelY[1] = (AY >> 4) - AccelReviseType.OffsetY;
	AccelType.AccelZ[1] = (AZ >> 4) - AccelReviseType.OffsetZ;
	
	if(AccelType.AccelX[1] >= -200 && AccelType.AccelX[1] <= 200)			//机械过滤窗口
		AccelType.AccelX[1] = 0;
	if(AccelType.AccelY[1] >= -200 && AccelType.AccelY[1] <= 200)
		AccelType.AccelY[1] = 0;
	if(AccelType.AccelZ[1] >= -200 && AccelType.AccelZ[1] <= 200)
		AccelType.AccelZ[1] = 0;
}



/**
 *  @name						void MovementEndCheck()
 *	@description		检查移动是否停止，因为积分有误差，所以速度是否为0不能仅根据计算获得，还需检测加速度的状态。
										当加速度持续为0时，则表示静止，置速度为0
 *	@param			
 *	@return		
 *  @notice
 */
void MovementEndCheck()
{
	if(AccelType.AccelX[1] == 0)
		countX++;
	else
		countX = 0;
	if(countX >= CHECK_CNT)
		VelocityType.VelocityX[1] = VelocityType.VelocityX[0] = 0;
	
	if(AccelType.AccelY[1] == 0)
		countY++;
	else
		countY = 0;
	if(countY >= CHECK_CNT)
		VelocityType.VelocityY[1] = VelocityType.VelocityY[0] = 0;
	
	if(AccelType.AccelZ[1] == 0)
		countZ++;
	else
		countZ = 0;
	if(countZ >= CHECK_CNT)
		VelocityType.VelocityZ[1] = VelocityType.VelocityZ[0] = 0;
	
}


static void delay_nus(u16 time)
{    
		u16 i=0;  
		while(time--)
    {
      i=10;  //自己定义
			while(i--) ;    
	}
}

/**
 *  @name
 *	@description
 *	@param			
 *	@return		
 *  @notice
 */
