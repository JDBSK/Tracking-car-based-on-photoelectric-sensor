#ifndef __MOTOR_H__
#define __MOTOR_H__

typedef enum {
	Front,
	Left_Turn,
	Right_Turn,
	Back,
	Stop,
}Cardir_type;

void Motor_Init(void);
void L_moto_go(void);void R_moto_go(void);
void L_moto_back(void);void R_moto_back(void);
void L_moto_Stop(void);void R_moto_Stop(void);
void Motor_SetDir(int8_t Dir);

#endif



