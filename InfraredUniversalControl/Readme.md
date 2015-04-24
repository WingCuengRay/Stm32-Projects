# Infrared Universal Control #

what is it?
---------

令红外线模块具有学习的功能，该模块能够学习各种遥控器的编码并完全复制，以实现万能遥控器的功能。

The Lastest Version
---------

目前为v2.1版本，能在github上找到这个开源代码[https://github.com/WingCuengRay/Stm32-Projects/tree/master/InfraredUniversalControl](https://github.com/WingCuengRay/Stm32-Projects/tree/master/InfraredUniversalControl)

Documenation
---------

该模块需要用到SysTick 的精确延迟，所以还需另行加入 sys.c 和 sys.h


Installing
---------

...


Licensing
---------

...


Function explanation
---------
###**Infrared For Recieve**
*******************

 *  @name				void Infrared_GPIO_Configuration()
 *	@description		红外接收端GPIO口设置
 *	@param				PA.01 作外部中断接收口
 *	@return		
 *  @notice 

*******************

 *  @name				void Infrared_EXTI_Configuration()
 *	@description		红外线接受端外部中断初始化设置，设置为 线路1，使用PA.1作为外部中断的输入端
 *	@param					
 *	@return		
 *  @notice				若改变外部中断的输入端，此函数内的线路设置也需改变
*******************

 *  @name				void EXTI1_IRQHandler()
 *	@description		外部中断1中断处理程序，用于采集红外波形
 *	@param			
 *	@return				全局变量Flag_LearnState可以用于返回是否有学习到波形
 *  @notice

*******************

###**Infrared For Send**
****************

 *  @name				void TIM2_PWM_Init(u16 arr,u16 psc)
 *	@description		初始化定时器2的设置，将定时器2用于PWM调制，PWM输出口为 PA.0
 *	@param				arr --	u16,定时器重装值
						psc --	u16,定时器分频值							
 *	@return		
 *  @notice				PWM频率 = 72M/((arr+1)*(psc+1)),这里用作红外发射的载波，需要生成38kHz的方波，故取arr = 1895,psc = 0。


*******************

 *  @name				void Infrared_Send()
 *	@description		红外发射，根据 PulseTab[]内的数据发波形
 *	@param			
 *	@return		
 *  @notice

*******************

Example
---------
    int main(void)
    {
    	SystemInit();
    	delay_init(72);
    	TIM2_PWM_Init(1895,0);
    	Infrared_GPIO_Configuration();
    	Infrared_EXTI_Configuration();
    	
    	Key_GPIO_Configuration();
    	
    	while(1)
    	{
    		if(Is_Press_Key() == 1)
    			Infrared_Send();
    	}
    
    	return 0;
    }




