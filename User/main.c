#include "stm32f10x.h" 
#include <string.h>
#include "OLED.h"
#include "Timer.h"
#include "Motor.h"
#include "MotorRun.h"
#include "Encoder.h"
#include "PID.h"
//#include "Delay.h"
#include "Sensor.h"
#include "serial.h"
#include "SteerPID.h"


int16_t Speed;
int16_t L_counter_speed,R_counter_speed;
uint16_t L_TIM2_CCR,R_TIM2_CCR;   //PWM比较值

extern uint8_t Car_mode;
extern PID_type L_pid,R_pid,SteerPID;
extern float setspeed;//pid.c

float L_wheel_speed,R_wheel_speed;
char sentence[30];
uint8_t time_count=0;//计量次数 time_count循环显示一次oled
Cardir_type Car_dir=Front; //车辆运行方向默认为前进



uint8_t* Line_Direction;//车道线数组
		
float drive_distance ; //平均转速
float distance;//小车距离

uint8_t counter;
int main(void)
{
	Timer_Init();
	Motor_Init();
	Encoder_Init();
	Serial_Init();
	OLED_Init();
	Sensor_Init();

	PID_Init(L_pid);PID_Init(R_pid);	
	Speed_Control(0,0);
	
	//速度环pid设置参数
		L_pid = PID_SetVal(L_pid,45,1.5,0); // 45 1.5 18
		R_pid = PID_SetVal(R_pid,40,2,0); // 40 2 18
		SteerPID = PID_SetVal(SteerPID,3.91,0,0.945);// 3.9 1.02 /  3.9 0.92 18速 /  3.91 0.93 19速
	
//		L_PWM_SetCompare(200);
//		R_PWM_SetCompare(200);
	while (1)
	{		
		//获取传感器数据
		Line_Direction = read_sensor();
		if(setspeed>8)  //需要设定速度大于阈值
		{
		//转向环pid控制
			SteerPID= SteerPID_control(SteerPID, Line_Direction);
			L_pid.Target_val = setspeed - SteerPID.Output_val;
			R_pid.Target_val = setspeed + SteerPID.Output_val;
		}
		//速度环pid控制
		if(Car_mode==0)
		{
			L_pid=PID_control(L_pid,L_counter_speed);
			R_pid=PID_control(R_pid,R_counter_speed);
			Speed_Control(L_pid.Output_val,R_pid.Output_val);//进行两轮占空比设置
		}
		
		
			sprintf(sentence,"%f,%f\n",drive_distance,distance); //蓝牙输出速度和里程
			Serial_SendString(sentence);

		
//		if(time_count++>=5)                                   
//		{
//			OLED_ShowNum(4 , 1, L_pid.Output_val, 4);          //显示编码器计数值10ms
//			OLED_ShowNum(4 , 6, R_pid.Output_val, 4);
//		}

			if(Car_mode == 1)
				{
					switch(Car_dir){
					
						case Front:
							Speed_Control(100,100);
							break;
						
						case Left_Turn:
							Speed_Control(-100,100);
							break;
						
						case Stop :
							Speed_Control(0,0);		
							break;
						
						case Right_Turn:
							Speed_Control(100,-100);
							break;
						
						case Back:
							Speed_Control(-100,- 100);
					}
				}
	}
}


 
//速度获取 读取编码器
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		L_counter_speed = TIM_GetCounter(TIM1);//每隔10ms获取速度 编码器
		
		R_counter_speed = TIM_GetCounter(TIM4);//获取右轮速度
		
		drive_distance += (L_counter_speed +	R_counter_speed)/2;
		
	//drive_distance = 0.051772261 * drive_distance;
		drive_distance = 0.056772261 * drive_distance;
		
		distance = distance + drive_distance * 0.01 ;
		//耗时较长不在此用
//			sprintf(sentence,"%d,%d,%f\n",L_counter_speed,R_counter_speed,setspeed);
//			Serial_SendString(sentence);
//			Serial_SendByte(0x0a);
		
//    uint8_t tempData[12];   //定义的传输Buffer，一个int占4个字节
//		float tempFloat[3]={
//			(float)L_counter_speed,
//			(float)R_counter_speed,
//			(float)setspeed,
//			};
//    memcpy(tempData, (uint8_t *)tempFloat, sizeof(tempFloat));  //通过拷贝把数据重新整理
//		for (uint8_t i=0;i<12;i++)
//			Serial_SendByte(tempData[i]);
//			Serial_SendByte(0x00);
//			Serial_SendByte(0x00);
//			Serial_SendByte(0x80);
//			Serial_SendByte(0x7f);

		
		TIM_SetCounter(TIM4, 0);
		TIM_SetCounter(TIM1, 0);
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
