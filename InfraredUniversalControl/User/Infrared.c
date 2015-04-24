/***************************************************************************************
 *	FileName					:		Infrared.c
 *	CopyRight					:
 *	ModuleName				:	
 *
 *	CPU							:
 *	RTOS						:
 *
 *	Create Data					:	2015/04/21
 *	Author/Corportation			:	Ray
 *
 *	Abstract Description		:	
 *
 *--------------------------------Revision History--------------------------------------
 *	No	version		Data			Revised By			Item			Description
 *	1		v1.0			2015/4/21	Ray												Create this file
 *	2		v2.0			2015/4/23	Ray												Sucessfully control the air conditioner
 *	3		v2.1			2015/4/24 Ray												Packaging the Infrared Module
 *
 ***************************************************************************************/



/**************************************************************
*	Debug switch Section
**************************************************************/


/**************************************************************
*	Include File Section
**************************************************************/
#include "Infrared.h"


/**************************************************************
*	Macro Define Section
**************************************************************/

//debug调试宏定义，根据表达式a的真假执行has_bug或no_bug 
#define BUG_DETECT_PRINT(a,has_bug,no_bug) { if(a) \
printf("%s",has_bug); \
else \
printf("%s",no_bug);}


/**************************************************************
*	Struct Define Section
**************************************************************/


/**************************************************************
*	Prototype Declare Section
**************************************************************/


/**************************************************************
*	Global Variable Declare Section
**************************************************************/
u8 Flag_LearnState = 0;
u16 PulseTab[MAX_PULSE_LEN];


/**************************************************************
*	File Static Variable Define Section
**************************************************************/


/**************************************************************
*	Function Define Section
**************************************************************/

#ifdef INFRARED_RECEIVE
/**
 *  @name						void Infrared_GPIO_Configuration()
 *	@description		红外接收端GPIO口设置
 *	@param					PA.01 作外部中断接收口
 *	@return		
 *  @notice
 */
void Infrared_GPIO_Configuration()
{
	GPIO_InitTypeDef GPIO_InitType;
	
	RCC_APB2PeriphClockCmd(INFRARED_RCC_GPIOx,ENABLE);
	GPIO_InitType.GPIO_Mode = GPIO_Mode_IPU;			//上拉输入
	GPIO_InitType.GPIO_Pin = INFRARED_GPIO_Pinx;
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(INFRARED_GPIOx,&GPIO_InitType);
	GPIO_EXTILineConfig(INFRARED_EXTI_GPIOx,INFRARED_EXTI_Line);
}



/**
 *  @name						void Infrared_EXTI_Configuration()
 *	@description		红外线接受端外部中断初始化设置，设置为 线路1，使用PA.1作为外部中断的输入端
 *	@param					
 *	@return		
 *  @notice					若改变外部中断的输入端，此函数内的线路设置也需改变s
 */
void Infrared_EXTI_Configuration()
{
	EXTI_InitTypeDef EXTI_InitType;
	NVIC_InitTypeDef NVIC_InitType;
	
	EXTI_InitType.EXTI_Line = EXTI_Line1;
	EXTI_InitType.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitType.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitType.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitType);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);					//NVIC 中断设置
	NVIC_InitType.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitType.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitType.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitType.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitType);
}


/**
 *  @name					void EXTI1_IRQHandler()
 *	@description	外部中断1中断处理程序，用于采集红外波形
 *	@param			
 *	@return				全局变量Flag_LearnState可以用于返回是否有学习到波形
 *  @notice
 */
void EXTI1_IRQHandler()
{
	u16 pulseWidth = 0;
	u16 i = 0;
	
	Flag_LearnState = 0;
	//中断指示
	
	while(1)
	{
		if(IR_RDATA)						//有高脉冲出现，代表空闲信号
		{
			pulseWidth = 0;
			while(IR_RDATA)
			{
				pulseWidth++;
				delay_us(19);
				if(pulseWidth >= 2000)			// >40ms 则结束记录
					break;
			}
			
			if(pulseWidth<=15 || pulseWidth>=2000)		// >40ms || <300us 则结束记录
				break;
			PulseTab[i] = pulseWidth*20;
			i++;
		}		
		else										//载波信号,偶数位为低电平（载波），奇数位为高电平（空闲）
		{	
			pulseWidth = 0;
			while(IR_RDATA == 0)			
			{
				pulseWidth++;
				delay_us(19);
			}
			if(pulseWidth<=15 || pulseWidth>=2000) 		// >40ms || <300sus  则结束记录
				break;
			PulseTab[i] = pulseWidth*20;
			i++;		
		}
	}
	PulseTab[i++] = pulseWidth;
	PulseTab[i] = 0xffff;
	
	Flag_LearnState = 1;
	EXTI_ClearITPendingBit(EXTI_Line1);

	return;
}

#endif




#ifdef INFRARED_SEND
/**
 *  @name						void Infrared_Send()
 *	@description		红外发射，根据 PulseTab[]内的数据发波形
 *	@param			
 *	@return		
 *  @notice
 */
void Infrared_Send()
{
	u16 i;
	
	EXTI->IMR &= ~(0x00000002);			//关中断，避免发送的红外线被自己接受
	for(i=0; i<MAX_PULSE_LEN && PulseTab[i]!=0xffff; i++)
	{
		if(i%2 == 0)
		{
			TIM_Cmd(TIM2,ENABLE);
			delay_us(PulseTab[i]);
			TIM_Cmd(TIM2,DISABLE);
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
		}
		else
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
			delay_us(PulseTab[i]);
		}
	}
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	
	EXTI->IMR |= (0x00000002);		//开中断
}


/**
 *  @name					void TIM2_PWM_Init(u16 arr,u16 psc)
 *	@description	初始化定时器2的设置，将定时器2用于PWM调制，PWM输出口为 PA.0
 *	@param				arr --	u16,定时器重装值
									psc --	u16,定时器分频值							
 *	@return		
 *  @notice				PWM频率 = 72M/((arr+1)*(psc+1)),这里用作红外发射的载波，需要生成38kHz的方波，故取arr = 1895,psc = 0。
 */
void TIM2_PWM_Init(u16 arr,u16 psc)
{
	/* 初始化结构体定义 */
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef 	TIM_OCInitStructure;
	/* 使能相应端口的时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //使能GPIO外设时钟
	
	/* GPIOA.0初始化 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			// TIM2 CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// PA.0 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	/* TIM2 初始化*/
	TIM_TimeBaseInitStructure.TIM_Period = arr;	 //下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;	//作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;  //时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	/* 定时器TIM2 Ch1 PWM模式初始化 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //选择定时器模式:TIM PWM1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
	//TIM_OCInitStructure.TIM_Pulse = (arr+1)/2;	  //占空比 50%
	TIM_OCInitStructure.TIM_Pulse = (arr+1)/3;	  //占空比1:3
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

	/* 使能TIM2在CCR1上的预装载寄存器 */
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	/* 使能定时器 */
//	TIM_Cmd(TIM2,ENABLE);
}

#endif

	

/**
 *  @name
 *	@description
 *	@param			
 *	@return		
 *  @notice
 */
 

 

 
 
 #ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
          /* User can add his own implementation to report the file name and line number,
             ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
        
          while (1)
          {}
}
#endif
