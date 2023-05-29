#include "pid.h"

PID pid;

//void PID_Init()
//{
//	pid.X_Kp = 0.2;
//	pid.X_Ki = 0.006;
//	pid.X_Kd = 0;
//	pid.X_err = 0;
//	pid.X_err_sum = 0;
//	pid.X_err_last = 0;
//	
//	pid.Y_Kp=0.2;
//	pid.Y_Ki=0.004;
//	pid.Y_Kd=0;
//	pid.Y_err=0;
//	pid.Y_err_sum=0;
//	pid.Y_err_last=0;
//}

void PID_Init()
{
	pid.X_Kp = 1.0;
	pid.X_Ki = 0;
	pid.X_Kd = 0;
	pid.X_err = 0;
	pid.X_err_sum = 0;
	pid.X_err_last = 0;
	
	pid.Y_Kp = 1.0;
	pid.Y_Ki=0;
	pid.Y_Kd=0;
	pid.Y_err=0;
	pid.Y_err_sum=0;
	pid.Y_err_last=0;
}

//X-103--------Y-82

//水平方向
int PID_Level(int x)
{
	int out;                //输入量是坐标
	
	pid.X_err = x - 115;    //当前误差     期望值：127
	pid.X_err_sum += pid.X_err;
	out = pid.X_Kp*(pid.X_err)
		+ pid.X_Ki*(pid.X_err_sum)
		+ pid.X_Kd*(pid.X_err - pid.X_err_last);
	pid.X_err_last = pid.X_err;
	
	return out;
}

//垂直方向
int PID_vertical(int y)
{
	int out;
		
	pid.Y_err = y - 100;       //120
	pid.Y_err_sum += pid.Y_err;
	out = pid.Y_Kp*(pid.Y_err)
		+ pid.Y_Ki*(pid.Y_err_sum)
		+ pid.Y_Kd*(pid.Y_err - pid.Y_err_last);
	pid.Y_err_last = pid.Y_err;
	
	return out;
}
