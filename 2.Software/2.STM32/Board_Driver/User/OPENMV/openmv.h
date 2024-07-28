#ifndef __OPENMV_
#define __OPENMV_
#include "sys.h"

extern int Openmv_X;          /*OPENMV X 轴反馈坐标*/
extern int Openmv_Y;          /*OPENMV X 轴反馈坐标*/
extern int Face_Inf;

extern int openmv[5];
extern int face_information[5];

void Openmv_data(void);
void Openmv_Recive(u8 q[]);
void face_inf_Rec(u8 q[]);
void Face_data(void);

extern u8 q[100], hh, tt, maxn;

#endif
