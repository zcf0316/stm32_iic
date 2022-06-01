#ifndef IIC_H
#define IIC_H
#include "stm32f4xx.h"    

#define SCL_OUT_H (GPIOB->ODR |= (1<<6))
#define SCL_OUT_L (GPIOB->ODR &= ~(1<<6))

#define SDA_OUT_H (GPIOB->ODR |= (1<<7))
#define SDA_OUT_L (GPIOB->ODR &= ~(1<<7))

#define SDA_IN_H (GPIOB->IDR & (1<<7))

//void iic_io_init(void);//io初始化
//void iic_start_init(void);//起始位
//void iic_stop_init(void);//停止位
//void iic_ack_send(uint8_t ack);//回复ack信号
//uint8_t iic_ack_init(void);//检测ACK信号
//void iic_send_init(uint8_t date);//发送一个字符
//uint8_t iic_rec_init(void);//接收一个字符

void iic_io_init(void);
void iic_start_init(void);
void iic_stop_init(void);
void iic_ack_send(uint8_t ack);
uint8_t iic_ack_init(void);
void iic_send_init(uint8_t date);
uint8_t iic_rec_init(void);

#endif


//IIC1_SCL/PB6
//IIC1_SDA/PB7

