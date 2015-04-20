/***************************************************************************************
 *	FileName					:		Ray		
 *	CopyRight					:
 *	ModuleName				:		IIC_Simulation.c
 *
 *	CPU							:			STM32F103ZE
 *	RTOS						:
 *
 *	Create Data					:	2015/03/11
 *	Author/Corportation			:	Ray
 *
 *	Abstract Description		:		模拟IIC
 *
 *--------------------------------Revision History--------------------------------------
 *	No	version		Data			Revised By			Item			Description
 *	1		v1.0			2015/3/11	Ray												Create this file
 *
 ***************************************************************************************/



/**************************************************************
*	Debug switch Section
**************************************************************/


/**************************************************************
*	Include File Section
**************************************************************/
# include "IIC_Simulation.h"

/**************************************************************
*	Macro Define Section
**************************************************************/
#define SlaveAddress	0xD0

/**************************************************************
*	Struct Define Section
**************************************************************/


/**************************************************************
*	Prototype Declare Section
**************************************************************/
static void delay_us(u16 time);
static void IIC_Start();
static void IIC_Stop();
static Status IIC_RecvACK();
static void IIC_SendByte(uint8_t dat);
static uint8_t IIC_RecvByte();

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
 *  @name						void IIC_Start();
 *	@description		发送IIC的开始信号
 *	@param			
 *	@return		
 *  @notice
 */
static void IIC_Start()
{
	GPIO_SetBits(IIC_GPIO_Group,SDA);
	GPIO_SetBits(IIC_GPIO_Group,SCL);
	delay_us(5);
	GPIO_ResetBits(IIC_GPIO_Group,SDA);	//在SCL为高电平期间，将SDA从1拉低至0即为开始信号 （下降沿）
	delay_us(5);
	GPIO_ResetBits(IIC_GPIO_Group,SCL);	//最后拉低时钟线，允许SDA线变化，用于后面的send或receive操作
					
	return;
}

/**
 *  @name					void IIC_Stop()
 *	@description	发送IIC的停止信号
 *	@param			
 *	@return		
 *  @notice
 */
static void IIC_Stop()
{
	GPIO_ResetBits(IIC_GPIO_Group,SDA);
	GPIO_SetBits(IIC_GPIO_Group,SCL);
	delay_us(5);	
	GPIO_SetBits(IIC_GPIO_Group,SDA);		//在SCL为高电平期间，将SDA从0拉高至1为停止信号	（上升沿）
	delay_us(5);
}

/**
 *  @name					Status IIC_RecvACK();
 *	@description	等待应答信号
 *	@param			
 *	@return		
 *  @notice
 */
static Status IIC_RecvACK()
{
	uint16_t i;
	
	GPIO_SetBits(IIC_GPIO_Group,SCL);
	delay_us(5);
	while(GPIO_ReadInputDataBit(IIC_GPIO_Group,SDA)==1 && i<0X2B0)		//等待应答
		i++;
	i = GPIO_ReadInputDataBit(IIC_GPIO_Group,SDA);
	GPIO_ResetBits(IIC_GPIO_Group,SCL);
	delay_us(5);	
	
	return (i?FALSE:TRUE);
}


/**
 *  @name					void IIC_SendByte(uint8_t dat)
 *	@description	通过IIC发送一个字节的数据
 *	@param			
 *	@return		
 *  @notice
 */
static void IIC_SendByte(uint8_t dat)
{
	uint8_t i,tmp;
	
	GPIO_ResetBits(IIC_GPIO_Group,SCL);
	for(i=0; i<8; i++)
	{
		tmp = dat&0x80;
		if(tmp)
			GPIO_SetBits(IIC_GPIO_Group,SDA);
		else
			GPIO_ResetBits(IIC_GPIO_Group,SDA);
		GPIO_SetBits(IIC_GPIO_Group,SCL);							//拉高时钟线
		delay_us(5);
		GPIO_ResetBits(IIC_GPIO_Group,SCL);						//拉低时钟线，重新允许数据变化
		delay_us(5);
		dat <<= 1;
	}	
	
	IIC_RecvACK();
}

/**
 *  @name						static uint8_t IIC_RecvByte()
 *	@description		从IIC总线中接收一个字节的数据
 *	@param			
 *	@return		
 *  @notice
 */
static uint8_t IIC_RecvByte()
{
	uint8_t i,dat = 0;
	
	GPIO_ResetBits(IIC_GPIO_Group,SCL);
	delay_us(5);
	GPIO_SetBits(IIC_GPIO_Group,SDA);
	delay_us(5);
	for(i=0; i<8; i++)
	{
		GPIO_SetBits(IIC_GPIO_Group,SCL);
		delay_us(5);
		dat = (dat<<1) | (uint8_t)GPIO_ReadInputDataBit(IIC_GPIO_Group,SDA);
		delay_us(5);
		GPIO_ResetBits(IIC_GPIO_Group,SCL);
		delay_us(5);
	}
	
	return dat;
}

/**
 *  @name					void Single_WriteIIC(uint8_t REG_Address,uint8_t REG_data);			
 *	@description	向IIC设备的指定地址中写入一个字节的数据	
 *	@param				REG_Address		-- uint8_t,要写入的(寄存器)地址
									REG_data			-- uint8_t,要写入的数据
 *	@return		
 *  @notice
 */
void Single_WriteIIC(uint8_t REG_Address,uint8_t REG_data)
{
	IIC_Start();
	IIC_SendByte(SlaveAddress);
	IIC_SendByte(REG_Address);
	IIC_SendByte(REG_data);	
	IIC_Stop();
}

/**
 *  @name					uint8_t Single_ReadIIC(uint8_t REG_Address)				
 *	@description	从IIC设备的指定地址中读取一个数据
 *	@param				REG_Address		-- uint8_t,要写入的(寄存器)地址
 *	@return				uint8_t		--  从IIC设备中接收的数据
 *  @notice
 */
uint8_t Single_ReadIIC(uint8_t REG_Address)
{
	uint8_t REG_data;

	IIC_Start();
	IIC_SendByte(SlaveAddress);
	IIC_SendByte(REG_Address);
	IIC_Start();
	IIC_SendByte(SlaveAddress+1);
	REG_data = IIC_RecvByte();
	IIC_Stop();
	
	return REG_data;
}


/**
 *  @name						void IIC_GPIO_Configuration()
 *	@description		GPIO口配置，SCL(GPIOB.6),SDA(GPIOB.7)开漏输出，用作模拟IIC
 *	@param			
 *	@return		
 *  @notice					宏定义SCL,SDA,IIC_GPIO_Group,IIC_APB_Periph在头文件中定义
 */
void IIC_GPIO_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(IIC_APB_Periph,ENABLE);	
	GPIO_InitStructure.GPIO_Pin = SCL | SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_GPIO_Group,&GPIO_InitStructure);

	return;
}

/**
 *  @name						void delay_nus(u16 time)			
 *	@description		微秒级延迟函数
 *	@param			
 *	@return		
 *  @notice
 */
static void delay_us(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=10;  	
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
