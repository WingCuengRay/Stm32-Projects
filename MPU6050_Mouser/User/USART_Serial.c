/***************************************************************************************
 *	FileName					:		USART_Serial.c			
 *	CopyRight					:
 *	ModuleName				:		Ray
 *
 *	CPU							:			STM32F103ZE
 *	RTOS						:
 *
 *	Create Data					:	2015/03/11
 *	Author/Corportation			:	Ray
 *
 *	Abstract Description		:	
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
# include "USART_Serial.h"

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
 *  @name					void USART_GPIO_Configuration()
 *	@description	USART的GPIO口设置，由宏定义TXD,RXD,USART_GPIO_Group确定。
 *	@param			
 *	@return		
 *  @notice				STM32的USART能映射的GPIO口似乎是固定的,若是如此，则宏定义不能随便改
 */
void USART_GPIO_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(USART_APB_Periph,ENABLE);
	/* 设置USART的TXD脚为第二功能推挽输出 */
	GPIO_InitStructure.GPIO_Pin = TXD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_GPIO_GROUP,&GPIO_InitStructure);
	
	/* 设置USART的RXD脚为浮空输入脚 */\
	GPIO_InitStructure.GPIO_Pin = RXD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_GPIO_GROUP,&GPIO_InitStructure);
}


/**
 *  @name					void USART_Configuration()
 *	@description	USART的设置，波特率115200
 *	@param			
 *	@return		
 *  @notice 
 */
void USART_Configuration()
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}




/**************************实现函数********************************************
*函数原型:		void UART1_Put_Char(unsigned char DataToSend)
*功　　能:		RS232发送一个字节
输入参数：
		unsigned char DataToSend   要发送的字节数据
输出参数：没有	
*******************************************************************************/
static void UART1_Put_Char(unsigned char DataToSend)
{
	//将要发送的字节写到UART1的发送缓冲区
	USART_SendData(USART1, (unsigned char) DataToSend);
	//等待发送完成
  	while (!(USART1->SR & USART_FLAG_TXE));
}

/**************************实现函数********************************************
*函数原型:		void UART1_ReportIMU(int16_t yaw,int16_t pitch,int16_t roll
				,int16_t alt,int16_t tempr,int16_t press)
*功　　能:		向上位机发送经过解算后的姿态数据
输入参数：
		int16_t yaw 经过解算后的航向角度。单位为0.1度 0 -> 3600  对应 0 -> 360.0度
		int16_t pitch 解算得到的俯仰角度，单位 0.1度。-900 - 900 对应 -90.0 -> 90.0 度
		int16_t roll  解算后得到的横滚角度，单位0.1度。 -1800 -> 1800 对应 -180.0  ->  180.0度
		int16_t alt   气压高度。 单位0.1米。  范围一个整型变量
		int16_t tempr 温度 。 单位0.1摄氏度   范围：直到你的电路板不能正常工作
		int16_t press 气压压力。单位10Pa  一个大气压强在101300pa 这个已经超过一个整型的范围。需要除以10再发给上位机
		int16_t IMUpersec  姿态解算速率。运算IMUpersec每秒。
输出参数：没有	
*******************************************************************************/
void UART1_ReportIMU(int16_t yaw,int16_t pitch,int16_t roll
,int16_t alt,int16_t tempr,int16_t press,int16_t IMUpersec)
{
 	unsigned int temp=0xaF+2+2;
	char ctemp;
	UART1_Put_Char(0xa5);
	UART1_Put_Char(0x5a);
	UART1_Put_Char(14+2);
	UART1_Put_Char(0xA1);

	if(yaw<0)yaw=32768-yaw;
	ctemp=yaw>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=yaw;
	UART1_Put_Char(ctemp);
	temp+=ctemp;

	if(pitch<0)pitch=32768-pitch;
	ctemp=pitch>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=pitch;
	UART1_Put_Char(ctemp);
	temp+=ctemp;

	if(roll<0)roll=32768-roll;
	ctemp=roll>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=roll;
	UART1_Put_Char(ctemp);
	temp+=ctemp;

	if(alt<0)alt=32768-alt;
	ctemp=alt>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=alt;
	UART1_Put_Char(ctemp);
	temp+=ctemp;

	if(tempr<0)tempr=32768-tempr;
	ctemp=tempr>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=tempr;
	UART1_Put_Char(ctemp);
	temp+=ctemp;

	if(press<0)press=32768-press;
	ctemp=press>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=press;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	
	ctemp=IMUpersec>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=IMUpersec;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	
	UART1_Put_Char(temp%256);
	UART1_Put_Char(0xaa);
}




/**************************实现函数********************************************
*函数原型:		void UART1_ReportMotion(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz,
					int16_t hx,int16_t hy,int16_t hz)
*功　　能:		向上位机发送当前传感器的输出值
输入参数：
	int16_t ax  加速度 X轴ADC输出 范围 ：一个有符号整型
	int16_t ay  加速度 Y轴ADC输出 范围 ：一个有符号整型
	int16_t az  加速度 Z轴ADC输出 范围 ：一个有符号整型
	int16_t gx  陀螺仪 X轴ADC输出 范围 ：一个有符号整型
	int16_t gy  陀螺仪 Y轴ADC输出 范围 ：一个有符号整型
	int16_t gz  陀螺仪 Z轴ADC输出 范围 ：一个有符号整型
	int16_t hx  磁罗盘 X轴ADC输出 范围 ：一个有符号整型
	int16_t hy  磁罗盘 Y轴ADC输出 范围 ：一个有符号整型
	int16_t hz  磁罗盘 Z轴ADC输出 范围 ：一个有符号整型
	
输出参数：没有	
*******************************************************************************/
void UART1_ReportMotion(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz,
					int16_t hx,int16_t hy,int16_t hz)
{
 	unsigned int temp=0xaF+9;
	char ctemp;
	UART1_Put_Char(0xa5);
	UART1_Put_Char(0x5a);
	UART1_Put_Char(14+8);
	UART1_Put_Char(0xA2);

	if(ax<0)ax=32768-ax;
	ctemp=ax>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=ax;
	UART1_Put_Char(ctemp);
	temp+=ctemp;

	if(ay<0)ay=32768-ay;
	ctemp=ay>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=ay;
	UART1_Put_Char(ctemp);
	temp+=ctemp;

	if(az<0)az=32768-az;
	ctemp=az>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=az;
	UART1_Put_Char(ctemp);
	temp+=ctemp;

	if(gx<0)gx=32768-gx;
	ctemp=gx>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=gx;
	UART1_Put_Char(ctemp);
	temp+=ctemp;

	if(gy<0)gy=32768-gy;
	ctemp=gy>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=gy;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
//-------------------------
	if(gz<0)gz=32768-gz;
	ctemp=gz>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=gz;
	UART1_Put_Char(ctemp);
	temp+=ctemp;

	if(hx<0)hx=32768-hx;
	ctemp=hx>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=hx;
	UART1_Put_Char(ctemp);
	temp+=ctemp;

	if(hy<0)hy=32768-hy;
	ctemp=hy>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=hy;
	UART1_Put_Char(ctemp);
	temp+=ctemp;

	if(hz<0)hz=32768-hz;
	ctemp=hz>>8;
	UART1_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=hz;
	UART1_Put_Char(ctemp);
	temp+=ctemp;

	UART1_Put_Char(temp%256);
	UART1_Put_Char(0xaa);
}



int16_t readByte(void){
	//TODO 数据有效性检查
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET){
		return -1;
	}
	else{
		return (int)USART_ReceiveData(USART1);
	}
}


/**
 *  @name					void Wait_Respond(u32 timeOut)
 *	@description	发送完一个帧后等待应答。
 *	@param			
 *	@return		
 *  @notice
 */
void Wait_Respond(u32 timeOut)
{
	int temp;
	while(--timeOut){
		if(-1 != (temp = readByte())){
			if(0x88 == temp)
				break;
		}
		//TODO 测试
		#ifdef D_DISP_BASE
			//TODO 等待直至接收到应答
			timeOut++;
		#endif
	}
}


/**
 *  @name					void USART_SendByte(uint8_t ch)		
 *	@description	通过串口1发送一个字节的数据
 *	@param			
 *	@return		
 *  @notice
 */
void USART_SendByte(uint8_t ch)
{
		USART_SendData(USART1,ch);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET)
			;
}









/**
 *  @name
 *	@description
 *	@param			
 *	@return		
 *  @notice
 */
