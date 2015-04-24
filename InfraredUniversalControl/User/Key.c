/***************************************************************************************
 *	FileName					:		Key.c
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
 *	1		v1.0			2015/04/21	Ray												Create this file
 *
 ***************************************************************************************/



/**************************************************************
*	Debug switch Section
**************************************************************/


/**************************************************************
*	Include File Section
**************************************************************/
#include "Key.h"


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
 *  @name						void Key_GPIO_Configuration()
 *	@description		按键0设置，PA13
 *	@param			
 *	@return		
 *  @notice
 */
void Key_GPIO_Configuration()
{
	GPIO_InitTypeDef GPIO_InitType;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitType.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitType.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitType);	
	
	GPIO_InitType.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitType);	
}



u8 Is_Press_Key()
{
	u8 tmp = 0;
	
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0) == 0)
	{
				tmp = 1;
				while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0) == 0)			//等待释放
						;
	}
	else if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13) == 0)
	{
				tmp = 2;
				while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13) == 0)			//等待释放
						;
	}
	
	return tmp;
}
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
