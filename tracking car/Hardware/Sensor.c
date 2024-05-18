#include "stm32f10x.h"                  // Device header

//pa4567  pb034 光电传感器进行读数，有黑线就返回1

void Sensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	 

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

uint8_t* read_sensor(void)//红外传感器识别到黑线返回数字信号低电平0，未识别到黑线返回高电平1
{
	static uint8_t sensor[7];
	/*将位置从最左按顺序到最右的传感器返回的数字信号依次存入数组sensor[0至8]里*/
	sensor[0]=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4);//最左的传感器
	sensor[1]=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5);
	sensor[2]=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
	sensor[3]=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7);//中央的传感器
	sensor[4]=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
	sensor[5]=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
	sensor[6]=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
	return sensor;
}

