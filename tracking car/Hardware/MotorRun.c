#include "stm32f10x.h"                  
#include "Motor.h"
#include "PWM.h"
/**
  * @Brief	本文件设置小车的行驶速度，同时调用motor中函数设置方向
  */
	
	
//小车调速函数
void Speed_Control(int16_t L_Compare,int16_t R_Compare)//目前是同时间
{
	if(L_Compare >= 0 && R_Compare >= 0){
		Motor_SetDir(Front);
	}
	else if(L_Compare < 0 && R_Compare < 0){
		Motor_SetDir(Back);
		R_Compare = -1 * R_Compare;
		L_Compare = -1 * L_Compare;
	}
	else if(L_Compare < 0 && R_Compare >= 0){
		Motor_SetDir(Left_Turn);
		L_Compare = -1 * L_Compare;
	}		
	else if(L_Compare >= 0 && R_Compare < 0){
		Motor_SetDir(Right_Turn);
		R_Compare = -1 * R_Compare;
	}
	else if(L_Compare ==0 && R_Compare ==0){
		Motor_SetDir(Stop);
	}
	
	
	
	L_PWM_SetCompare(L_Compare); 	//调速  TIM_SetCompare3(TIM2, Compare);
	R_PWM_SetCompare(R_Compare); 	
}


// 
////小车前进函数
//void run(int16_t L_Compare,int16_t R_Compare)
//{
//	Speed_Control(L_Compare,R_Compare);
//	Motor_SetDir(Front);
//}
// 
////小车后退函数
//void backrun(int16_t L_Compare,int16_t R_Compare)
//{
//	Speed_Control(L_Compare,R_Compare);
//	Motor_SetDir(Back);
//}
// 
////小车停车函数
//void stop(void)         		
//{
//	Speed_Control(0,0);
//	Motor_SetDir(Stop);   	
//}

////小车原地左旋
//void left_turn(uint16_t L_Compare,uint16_t R_Compare)
//{
//	Speed_Control(L_Compare,R_Compare);
//	Motor_SetDir(Left_Turn);
//}

////小车原地右旋
//void right_turn(uint16_t L_Compare,uint16_t R_Compare)
//{
//	Speed_Control(L_Compare,R_Compare);
//	Motor_SetDir(Right_Turn);
//}
