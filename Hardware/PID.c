#include "stm32f10x.h"                  // Device header
#include "PID.h"

float Err=0,LastErr=0,LastLastErr=0;
//float pwm_CCR=0,Add_CCR=0;                                //pwm新的PWM比较值，add，新的PWM占空比更改值
float setspeed=0;                              //设定编码器计量值


PID_type L_pid,R_pid;


PID_type PID_Init(PID_type pidstruct)
{
	pidstruct.Kp = 0;
	pidstruct.Ki = 0;
	pidstruct.Kd = 0;
	
	pidstruct.Target_val = 0;//编码器速度
	pidstruct.Err = 0;
	pidstruct.LastErr = 0;
	pidstruct.PrevErr = 0;
	pidstruct.Sum_err = 0;
	pidstruct.Output_val=0; //输出CRR 给定时器
	return pidstruct;
}


/**
  * @Brief	pid控制
  * @Param  speed 编码器10ms测量值 	pidstruct pid结构体
  * @Retval crr比较值（max 400-1）
  */
PID_type PID_control(PID_type pidstruct,int16_t speed)
{
	
  pidstruct.Err = pidstruct.Target_val - speed;   
	pidstruct.Sum_err += pidstruct.Err;	
	if(pidstruct.Sum_err > 799)pidstruct.Sum_err = 799;
	if(pidstruct.Sum_err < -799)	 pidstruct.Sum_err = -799;//0
	
    //Add_CCR = p*(Err-LastErr)+i*(Err)+d*(Err+LastLastErr-2*LastErr);
	
	
	pidstruct.Output_val = pidstruct.Kp * pidstruct.Err +
												 pidstruct.Ki * pidstruct.Sum_err +
												 pidstruct.Kd * (pidstruct.Err - pidstruct.LastErr); //占空比
	
	//if( 1 < pidstruct.Err || pidstruct.Err < -1 ) //限幅，微小变化不更改，减少抖动
	//{
	 //pwm_CCR+=Add_CCR; 增量式
		//pwm_CCR = pidstruct.Output_val;
	if(pidstruct.Output_val > 799)
        pidstruct.Output_val = 799;
    if(pidstruct.Output_val < -799)
        pidstruct.Output_val = -799;
	//}
	 

		
    pidstruct.PrevErr = pidstruct.LastErr;  //把上一次误差赋值给上上次误差
    pidstruct.LastErr = pidstruct.Err;			//把最新误差赋值给上次误差
		pidstruct.Sum_err += pidstruct.Err;
		
    return pidstruct;					//返回PWM新的占空比值
}

PID_type PID_SetVal(PID_type pidstruct,float Kp,float Ki,float Kd)
{
	pidstruct.Kp = Kp;
	pidstruct.Ki = Ki;
	pidstruct.Kd = Kd;
	
	return pidstruct;
}
