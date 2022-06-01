#include "stm32f4xx.h"                  // Device header
#include "iic.h"
#include "timer.h"


#define SCL_OUT_H (GPIOB->ODR |= (1<<6))
#define SCL_OUT_L (GPIOB->ODR &= ~(1<<6))

#define SDA_OUT_H (GPIOB->ODR |= (1<<7))
#define SDA_OUT_L (GPIOB->ODR &= ~(1<<7))

#define SDA_IN_H (GPIOB->IDR & (1<<7))


/**************************************
*函数名		：void iic_io_init(void);
*函数功能	：IIC 模拟端口初始化   端口拉高
*函数参数	：无
*函数返回值	：无
*函数描述	：IIC1_SCL/PB6
			  IIC1_SDA/PB7
**************************************/
void iic_io_init(void)
{
	RCC->AHB1ENR |= (1<<1);
	
	GPIOB->MODER &= ~(3<<2*6|3<<2*7);
	GPIOB->MODER |= (1<<2*6|1<<2*7);
	
	GPIOB->OTYPER &= ~(1<<6);
	GPIOB->OTYPER |= (1<<7);
	
	GPIOB->OSPEEDR &= ~(3<<2*6);
	
	GPIOB->PUPDR &= ~(3<<2*6|3<<2*7);

	GPIOB->ODR |= (1<<6|1<<7);
}


/**************************************
*函数名		：void iic_start_init(void);
*函数功能	：IIC 起始位
*函数参数	：无
*函数返回值	：无
*函数描述	：IIC1_SCL/PB6
			  IIC1_SDA/PB7
**************************************/
void iic_start_init(void)
{
	SCL_OUT_L;
	SDA_OUT_H;//确保数据线更改，时钟线为低，产生条件
	
	SCL_OUT_H;
	tim6_ns(5);
	
	SDA_OUT_L;
	tim6_ns(5);//产生起始信号
	
	SCL_OUT_L;//安全拉低时钟线

}


/**************************************
*函数名		：void iic_stop_init(void);
*函数功能	：IIC 停止位
*函数参数	：无
*函数返回值	：无
*函数描述	：IIC1_SCL/PB6
			  IIC1_SDA/PB7
**************************************/
void iic_stop_init(void)
{
	SCL_OUT_L;
	SDA_OUT_L;//产生条件
	
	SCL_OUT_H;
	tim6_ns(5);
	
	SDA_OUT_H;//双线拉高结束
	tim6_ns(5);

}

/**************************************
*函数名		：void iic_ack_init(uint8_t ack);
*函数功能  :iic通信发送应答/不应答信号
*函数参数  :u8 ack
*函数返回值:无
*函数描述  :参数传入0,表示发送应答
            参数传入1,表示发送不应答 
**************************************/ 
void iic_ack_send(uint8_t ack)
{
	SCL_OUT_L;
	
	if(ack==0)
		SDA_OUT_L;
	else if(ack==1)
		SDA_OUT_H;

	tim6_ns(5);
	
	SCL_OUT_H;
	
	tim6_ns(5);	
	SCL_OUT_L;


}

/**************************************
*函数名		：uint8_t iic_ack_init(void);
*函数功能	：IIC 检测应答/不应答
*函数参数	：无
*函数返回值	：uint8_t
*函数描述	：返回值是0 表示检测到应答信号
            返回值是1 表示检测到不应答信号
**************************************/ 
uint8_t iic_ack_init(void)
{
	uint8_t ack;
	
	SDA_OUT_H;//拉高   切断输出路
	
	SCL_OUT_L;//拉低   从机发送信号
	
	tim6_ns(5);
	
	SCL_OUT_H;//主机读数据
	
	tim6_ns(5);
	
	if(SDA_IN_H==0)
		ack=0;//返回值是0 表示检测到应答信号
	else if(SDA_IN_H!=0)
		ack=1;//返回值是1 表示检测到不应答信号
	
	SCL_OUT_L;
	
	return ack;
}

/***********************************************
*函数名    :void iic_send_init(uint8_t date);
*函数功能  :iic通信发送一个字节函数
*函数参数  :uint8_t data
*函数返回值:无 
*函数描述  :
************************************************/
void iic_send_init(uint8_t date)
{
	uint8_t i;
	
	for(i=0;i<8;i++)
	{
		SCL_OUT_L;
		
		tim6_ns(5);
		
		if(date & 0x80)
		{
			SDA_OUT_H;
		}
		else
		{
			SDA_OUT_L;
		}
		
		tim6_ns(5);
		
		SCL_OUT_H;//帮助从机拉高数据线
		
		tim6_ns(5);	
		
		date=date<<1;
	}
	SCL_OUT_L;
}

/***********************************************
*函数名    :uint8_t iic_rec_init(void);
*函数功能  :iic通信接收一个字节函数
*函数参数  :无
*函数返回值:uint8_t data 
*函数描述  :
************************************************/
uint8_t iic_rec_init(void)
{
	uint8_t i;
	
	uint8_t date;
	
	SDA_OUT_H;
	
	for(i=0;i<8;i++)
	{
		SCL_OUT_L;
		
		tim6_ns(5);
		
		SCL_OUT_H;
		
		tim6_ns(5);
		
		date=date<<1;
		
		if(SDA_IN_H)
			
			date=date | 0x01;
	}
	
	SCL_OUT_L;

	return date;
}


