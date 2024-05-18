#include "stm32f10x.h"             
#include "PWM.h"
#include "Motor.h"
#include "OLED.h"
/**
	* @Brief	本文件函数是用于设置电机转动的方向（l298n电调），而非设置速度（在MotorRun）
	* @pin	  pb8 pb9左轮方向控制  pa10 pa11右轮方向控制
  */
	
void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	PWM_Init();
}
 

//电机向前
void L_moto_go(void){
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}
void R_moto_go(void){
	GPIO_SetBits(GPIOA, GPIO_Pin_10);
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
}
 

//电机向后
void L_moto_back(void){
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
}
void R_moto_back(void){
	GPIO_ResetBits(GPIOA, GPIO_Pin_10);
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
}
 

//电机停止
void L_moto_Stop(void){
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
}
void R_moto_Stop(void){
	GPIO_SetBits(GPIOA, GPIO_Pin_10);
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
}

	
	//设置电机运行方向函数
void Motor_SetDir(int8_t Dir)												
{
	if (Dir ==Front){
		L_moto_go();
		R_moto_go();
	}
	if(Dir==Back){
		L_moto_back();
		R_moto_back();
	}
	if(Dir==Left_Turn){
		L_moto_back();
		R_moto_go();
	}
	if(Dir==Right_Turn){
		L_moto_go();
		R_moto_back();
	}
	if(Dir==Stop){
		L_moto_Stop();
		R_moto_Stop();
	}
}
