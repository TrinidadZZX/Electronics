#include "pid.h"


//typedef struct {
//    float kp;
//    float ki;
//    float kd;
//} P_I_D;

//void NewCtrl(void)   //对 UpdateCtrl 函数给出的控制量进行补偿
//{
//    height_new_ctrl[1]=height_new_ctrl[0];
//    pitch_new_ctrl[1]=pitch_new_ctrl[0];
//    roll_new_ctrl[1]=roll_new_ctrl[0];
//    yaw_new_ctrl[1]=yaw_new_ctrl[0];

//    height_new_ctrl[0]=height_ctrl;
//    pitch_new_ctrl[0]=pitch_y;
//    roll_new_ctrl[0]=roll_x;
//    yaw_new_ctrl[0]=0;
//}

//void pid(u16 x, u16 y){
//	
//	
//	pwmout = kp*error
//}
//	