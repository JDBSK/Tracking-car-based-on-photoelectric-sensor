#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "PID.h"
#include <OLED.h>
#include "MotorRun.h"
#include "Motor.h"


//蓝牙通讯 使用hex传输数字（10进制）进行速度设置（10ms编码器读数）
//tim2 channel1 | channel2 接蓝牙 
//即pa2进行单片机发送  pa3进行接受

char Serial_RxPacket[100];				//"@MSG\r\n"
//uint8_t Serial_RxFlag;
uint8_t Car_mode=0; //自动模式0 手动模式1
uint8_t Car_manual_dir=0;
extern Cardir_type Car_dir;

extern float setspeed;//pid中

void Serial_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART2, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

void USART2_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART2);
		

		if (RxState == 0)
		{
			
			if(RxData == 'A')//自动模式 按键1
			{
				Serial_SendString("Auto mode!");
				Car_mode=0;//自动模式
				return;
				
			}
			if(RxData == 'B')//手动模式 按键2
			{
				Serial_SendString("Manual mode!");
				Car_mode=1;//手动模式
				return;
			}
			
				if(Car_mode == 0)
				{
					if(RxData == 0x0A )//是回车
					{
						memset(Serial_RxPacket,'\0',sizeof(Serial_RxPacket));
						pRxPacket=0;	
						return;
					}
					else 
					{
						Serial_RxPacket[pRxPacket] = RxData;
						//TIM_SetCompare1(TIM2, RxData);
						//TIM_SetCompare2(TIM2, RxData);
						
						//使用16进制输入
						setspeed = RxData % 16; // 个位数
						RxData /= 16;
						setspeed += (RxData % 16) *10; // 十位数
						RxData /= 16;
						setspeed += (RxData % 16) *100;// 百位数
						
						L_pid.Target_val=setspeed;
						R_pid.Target_val=setspeed;
						
						pRxPacket ++;
						return;
					}
				}
				
				if(Car_mode == 1)
				{
					switch(RxData){
					
						case 'G':
							Car_dir=Front;
							break;
						
						case 'H':
							Car_dir=Left_Turn;
							break;
						
						case 'I' :
							Car_dir=Stop;
							break;
						
						case 'J':
							Car_dir=Right_Turn;
							break;
						
						case 'K':
							Car_dir=Back;
							break;
					}
				}

				
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}
