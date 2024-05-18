#include "stm32f10x.h"
#include "PID.h"
#include "Motor.h"
#include "OlED.h"


PID_type SteerPID;
int8_t sensor_pos[7]={7,4,2,0,-2,-4,-7};//err

/**
  * @Brief	转向环pid控制
  * @Param  pidstruct 转向环pid结构体  directtion 传感器数组显示当前位置  
  * @Retval Δ编码器值
  */
PID_type SteerPID_control(PID_type pidstruct,uint8_t* Line_Direction)
{
	uint8_t Line_pos;
	for(uint8_t i=0;i<7;i++)
	{
		if(Line_Direction[i] == 1)//识别到线
		{
			Line_pos = i;//当前线的位置（传感器获取）
			break;
		}
		
		if(i==6 && Line_pos==0)//没有扫到线就停车
		{
			Line_pos=3;//保持中线
			Motor_SetDir(Stop);
		}

	}
	
	pidstruct.Err = sensor_pos[Line_pos];   //获取黑线与中线差距  target默认为中线0，表示两轮不差速
	pidstruct.Sum_err += pidstruct.Err;		
	
	pidstruct.Output_val = pidstruct.Kp *  pidstruct.Err +
						   pidstruct.Ki *  pidstruct.Sum_err +
						   pidstruct.Kd * (pidstruct.Err - pidstruct.LastErr); //编码器计量值 （10ms）
	
//	OLED_ShowSignedNum(2,1,pidstruct.Output_val,2);//显示Δ编码器
	
		//限制在电机运行转速范围内
	if(pidstruct.Output_val > 40)
        pidstruct.Output_val = 40;
    if(pidstruct.Output_val < -40)
        pidstruct.Output_val = -40;

		
    pidstruct.PrevErr = pidstruct.LastErr;  //把上一次误差赋值给上上次误差
    pidstruct.LastErr = pidstruct.Err;			//把最新误差赋值给上次误差
	pidstruct.Sum_err += pidstruct.Err;
		
    return pidstruct;					//返回PWM新的占空比值
}

