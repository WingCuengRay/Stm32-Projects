#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#include <inttypes.h>

#define I2C_WR	0		/* Ð´¿ØÖÆbit */
#define I2C_RD	1		/* ¶Á¿ØÖÆbit */


void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);
uint8_t i2c_ReceiveByte_WithACK(void);
uint8_t i2c_Write(uint8_t anbt_dev_addr, uint8_t anbt_reg_addr, uint8_t anbt_i2c_len, uint8_t *anbt_i2c_data_buf);
uint8_t i2c_Read(uint8_t anbt_dev_addr, uint8_t anbt_reg_addr, uint8_t anbt_i2c_len, uint8_t *anbt_i2c_data_buf);

#endif
