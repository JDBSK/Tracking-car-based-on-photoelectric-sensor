#ifndef __STEERPID_H
#define __STEERPID_H

extern PID_type SteerPID;
PID_type SteerPID_control(PID_type pidstruct,uint8_t* Line_Direction);

#endif
