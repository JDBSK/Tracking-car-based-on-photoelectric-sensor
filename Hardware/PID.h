#ifndef __PID_H
#define __PID_H

extern float setspeed;
extern float Err;


typedef struct 
{
	float Target_val;   //目标值  对应编码器
	float Err;          /*第 k 次偏差 */
	float LastErr;     /* Error[-1],第 k-1 次偏差 */
	float PrevErr;    /* Error[-2],第 k-2 次偏差 */
	float Kp,Ki,Kd;     //比例、积分、微分系数
	float Sum_err;     //积分值
	float Output_val;   	  //输出值
}PID_type;

extern PID_type L_pid,R_pid;


PID_type PID_Init(PID_type pidstruct);
PID_type PID_control(PID_type pidstruct,int16_t speed);
PID_type PID_SetVal(PID_type pidstruct,float Kp,float Ki,float Kd);

#endif
