/***************************************************************************************
 *	FileName					:	
 *	CopyRight					:
 *	ModuleName				:	
 *
 *	CPU							:
 *	RTOS						:
 *
 *	Create Data							:	2015/01/31
 *	Author/Corportation			:	Ray
 *
 *	Abstract Description		:	
 *
 *--------------------------------Revision History--------------------------------------
 *	No	version		Data			Revised By			Item			Description
 *	1		v1.0								Ray												Create this file
 *	2   v2.0			2015/4/13 Ray												添加了DMP模块采集角度
 *
 ***************************************************************************************/



/**************************************************************
*	Debug switch Section
**************************************************************/


/**************************************************************
*	Include File Section
**************************************************************/
#include "stm32f10x.h"
#include "IIC_Simulation.h"
#include "USART_Serial.h"
#include "stdio.h"
#include "MPU6050.h"
#include "math.h"

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "i2c.h"
#include "clock.h"
#include "delay.h"

/**************************************************************
*	Macro Define Section
**************************************************************/
#define PI 3.14
#define FRAME_DATA_CNT 0x01
#define FRAME_SUM_LEN (4+FRAME_DATA_CNT*4) 


#define BUG_DETECT_PRINT(a,has_bug,no_bug) { if(a) \
printf("%s",has_bug); \
else \
printf("%s",no_bug);}

#define DEFAULT_MPU_HZ  (100)


/**************************************************************
*	Struct Define Section
**************************************************************/
typedef struct
{
	uint8_t PressKey;
	uint8_t OffsetX;
	uint8_t OffsetY;
	uint8_t OffsetWheel;
	uint8_t Check;
}Mouse,*pMouse;


/**************************************************************
*	Prototype Declare Section
**************************************************************/
extern void NVIC_Configuration();
extern void TIM2_Configuration();

void USART_SendByte(uint8_t ch);


void DMP_Init(signed char gyro_orientation[],struct int_param_s *int_param);
void acc_filter(int16_t accel[3],int16_t acc_ave[3]);
void acc_convert(float accel_res[3],int16_t accel[3],float q[4]);

void USART_Send_Frame(pMouse p);
void Get_Frame(pMouse p);
void Start_Singal_Check();


/**************************************************************
*	Global Variable Declare Section
**************************************************************/
int16_t ax,ay,az,gx,gy,gz;
float angleAx,gyroGy;
extern float angle,angle_dot;


/**************************************************************
*	File Static Variable Define Section
**************************************************************/


/**************************************************************
*	Function Define Section
**************************************************************/
int main(void)
{	
	signed char gyro_orientation[9] = {1, 0, 0, 0,1, 0, 0, 0, 1};
	struct int_param_s int_param;
	long quat[4];
	short sensors = INV_XYZ_GYRO| INV_XYZ_ACCEL | INV_WXYZ_QUAT;
	unsigned char more;
	int16_t gyro[3], accel[3],acc_ave[3];
	unsigned long timestamp,time_pre;
	float accel_res[3],accel_final[3];
	float q[4];

	Mouse MouseType;
	int16_t accel_show[3];

	SystemInit();
	delay_init(72);
	clock_conf();
	IIC_GPIO_Configuration();
	USART_GPIO_Configuration();
	USART_Configuration();
	USART_ClearFlag(USART1,USART_FLAG_TC);
	InitMPU6050();
//	Self_Revise(&AccelReviseType);				//自校正初始化
//	NVIC_Configuration();
	DMP_Init(gyro_orientation,&int_param);

// 	TIM2_Configuration();
	
	
	dmp_read_fifo(gyro,accel,quat,&time_pre,&sensors,&more);	
	Start_Singal_Check();
	MouseType.PressKey = 0x00;
	MouseType.OffsetX = 0x00;
	MouseType.OffsetY = 0x00;
	MouseType.OffsetWheel = 0x00;
	while(1)
	{
		//MovementEndCheck();
		dmp_read_fifo(gyro,accel,quat,&timestamp,&sensors,&more);
		q[0]=quat[0] / 1073741824.0f;
    q[1]=quat[1] / 1073741824.0f;
    q[2]=quat[2] / 1073741824.0f;
    q[3]=quat[3] / 1073741824.0f;
		
		acc_filter(accel,acc_ave);
		acc_convert(accel_res,accel,q);
// 		accel_final[0] = accel_res[0]/16384.0*100;
// 		accel_final[1] = accel_res[1]/16384.0*100;
//     accel_final[2] = (accel_res[2]/16384.0-0.978833)*100;

// 	 accel_show [0] = accel_final[0];  
// 	 accel_show [1] = accel_final[1];  
// 	 accel_show [2] = accel_final[2]; 
// 		
		accel_show [0] = accel_res[0];  
		accel_show [1] = accel_res[1];  
		accel_show [2] = accel_res[2];  

		//UART1_ReportMotion(acc_ave[0],acc_ave[1],acc_ave[2],gyro[0],gyro[1],gyro[2],0,0,0);		
		//UART1_ReportMotion(accel_show[0],accel_show[1],accel_show[2],gyro[0],gyro[1],gyro[2],0,0,0);		
		
		
		
		if(gyro[0] >= 15000)
		{

			MouseType.PressKey = 0x01;
			Get_Frame(&MouseType);
			USART_Send_Frame(&MouseType);
			Wait_Respond(1000);
 //			delay_nms(1);
			
			MouseType.PressKey = 0x00;
			Get_Frame(&MouseType);
			USART_Send_Frame(&MouseType);
			Wait_Respond(1000);
		}
		else if (gyro[0] <= -15000)
		{	
			MouseType.PressKey = 0x02;
			Get_Frame(&MouseType);
			Get_Frame(&MouseType);
			USART_Send_Frame(&MouseType);
			Wait_Respond(1000);
// 			delay_nms(1);
			
			MouseType.PressKey = 0x00;
			Get_Frame(&MouseType);
			USART_Send_Frame(&MouseType);
			Wait_Respond(1000);
			
		}
	}


	return 0;
}







/**
 *  @name						void DMP_Init()
 *	@description		MPU6050 DMP模块初始化函数
 *	@param					gyro_orientation --	signed char []，代表三个坐标轴的方向
										int_param -- struct int_param_s *, DMP初始化结构体
 *	@return		
 *  @notice
 */
void DMP_Init(signed char gyro_orientation[],struct int_param_s *int_param)
{
	   ANBT_I2C_Configuration();		//IIC初始化
    //BUG_DETECT_PRINT(i2c_CheckDevice(0x68<<1),"\r\n 未检测到MPU6050 \r\n","\r\n 检测到MPU6050 \r\n");

    //BUG_DETECT_PRINT((result = DMP_MPU6050_DEV_CFG()),"\r\n MPU6050失败\r\n","\r\n MPU6050 \r\n");

    BUG_DETECT_PRINT(mpu_init(int_param),"\r\n MPU6050初始化失败\r\n","\r\n MPU6050初始化成功\r\n");

    BUG_DETECT_PRINT(mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL),"\r\n DMP设置传感器失败\r\n","\r\n DMP设置传感器成功\r\n");

    /* Push both gyro and accel data into the FIFO. */
    BUG_DETECT_PRINT(mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL),"\r\n 设置FIFO失败\r\n","\r\n 设置FIFO成功\r\n");

    BUG_DETECT_PRINT(mpu_set_sample_rate(DEFAULT_MPU_HZ),"\r\n 设置采样率失败\r\n","\r\n 设置采样率成功\r\n");

    BUG_DETECT_PRINT(dmp_load_motion_driver_firmware(),"\r\n 加载固件失败\r\n","\r\n 加载成功\r\n");

    BUG_DETECT_PRINT(dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)),"\r\n DMP设置初始方向失败\r\n","\r\n DMP设置初始方向成功\r\n");

    BUG_DETECT_PRINT(dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT  | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL),"\r\n DMP初始化特性失败\r\n","\r\n DMP初始化特性成功\r\n");

    BUG_DETECT_PRINT(dmp_set_fifo_rate(DEFAULT_MPU_HZ),"\r\n 设置FIFO输出速率失败\r\n","\r\n 设置FIFO输出速率成功\r\n");

    run_self_test();

    mpu_set_dmp_state(1);
}


void acc_convert(float accel_res[3],int16_t accel[3],float q[4])
{
    accel_res[0]    = (q[0]*q[0] + q[1]*q[1] - q[2]*q[2] - q[3]*q[3])	*accel[0] + (2*q[1]*q[2]-2*q[0]*q[3])	                        *accel[1]	+(2*q[0]*q[2]+2*q[1]*q[3])                          *accel[2];
    accel_res[1]	= (2*q[0]*q[3]+2*q[1]*q[2])					        *accel[0] + (q[0]*q[0] - q[1]*q[1] + q[2]*q[2] - q[3]*q[3])	    *accel[1]   +(-2*q[0]*q[1]+2*q[2]*q[3])                         *accel[2];
    accel_res[2]	= (2*q[1]*q[3]-2*q[0]*q[2])							*accel[0] + (2*q[0]*q[1]+2*q[2]*q[3])                           *accel[1]	+(q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3])	*accel[2];
}


#define ACC_FILTER_MOUNT	1
int16_t	acc_xyz_data[3][ACC_FILTER_MOUNT] = {0};
int16_t	acc_data_index = 0;
int32_t	acc_data_sum[3] ={0};

/* 对原始数据加速度值进行滤波 */
void acc_filter(int16_t accel[3],int16_t acc_ave[3])
{

}




int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,(u8)ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET)	
		;
	
	return ch;
}


/**
*     @name               uint8 getCheckCode(uint8 *buf)
*     @description     生成4位数据的校验码
*     @param               buf【buffer】保存数据的内存空间的基地址
*     @return               校验码
*/
static uint8_t validateDatas(uint8_t *buf){
    
     uint8_t i, j;
     uint8_t temp      = 0x00;
     uint8_t result     = 0x00;
    
     for(i=0; i<4; i++){
         
          result = 0x00;
          for(j=0; j<8; j++){
               result = ((buf[i]<<j)&0x80)^result;
          }
          temp >>= 1;
          temp += result;
          temp >>= 1;
          temp += (~result)&0x80;
     }
    
     return temp;
}// */


/**
 *  @name					void Get_Frame(pMouse p)
 *	@description	得到帧里的具体数据，由用户自定义
 *	@param			
 *	@return		
 *  @notice
 */
void Get_Frame(pMouse p)
{
		uint8_t buf[4];
	
	//生成校验字节
		buf[0] = p->PressKey;
		buf[1] = p->OffsetX;
		buf[2] = p->OffsetY;
		buf[3] = p->OffsetWheel;
		p->Check = validateDatas(buf);
	
		return;
}


/**
 *  @name					void USART_Send_Frame(pMouse p)
 *	@description	发送一帧的数据，帧里的数据由结构体 Mouse定义
 *	@param				p  ----  pMouse，Mouse结构体的指针	
 *	@return		
 *  @notice
 */
void USART_Send_Frame(pMouse p)
{
		USART_SendByte(p->PressKey);
		USART_SendByte(p->OffsetX);
		USART_SendByte(p->OffsetY);
		USART_SendByte(p->OffsetWheel);
		USART_SendByte(p->Check);
		
		return;
}


/**
 *  @name							void Start_Singal_Check()
 *	@description			与USB握手的信号检测
 *	@param			
 *	@return		
 *  @notice
 */
void Start_Singal_Check()
{
	while(1)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET)
			;
		if(USART_ReceiveData(USART1) == 0x66)
		{
			USART_SendByte(0x77);
			break;
		}
	}
}





/**
 *  @name
 *	@description
 *	@param			
 *	@return		
 *  @notice
 */



