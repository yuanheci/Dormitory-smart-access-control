#include "openmv.h"

int Openmv_X=100; /*OPENMV X 轴反馈坐标*/
int Openmv_Y=100; /*OPENMV Y 轴反馈坐标*/

int Face_Inf = 100;

int openmv[5];//stm32接收数据数组	       //定义了一种通信格式
int face_information[5];

#if 1
void Openmv_Recive(u8 q[])           //-------接收坐标
{
	u8 i, t;
	u8 tmp[5];
	t = q[hh];
	if(t != 0xb3) return;  //不是坐标信息，直接返回
	for(i = 0; i < 5; i++) {
		tmp[i] = q[hh++];
		if(hh == maxn) hh = 0;
	}
	if(tmp[0] != 0xb3 && tmp[1] != 0xb3 && tmp[4] != 0x5b){
		openmv[i] = tmp[i];
	}
	else {
		for(i = 0; i < 5; i++) openmv[i] = 0;
	}
	Openmv_data();
}


void face_inf_Rec(u8 q[])             //-------接收身份信息
{
	u8 i, t, tmp[5];
	t = q[hh];
	if(t != 0xb1) return;
	for(i = 0; i < 5; i++){
		tmp[i] = q[hh++];
		if(hh == maxn) hh = 0;
	}
	if(tmp[0] == 0xb1 && tmp[1] == 0xb1 && tmp[4] == 0x5b){
		for(i = 0; i < 5; i++){
			face_information[i] = tmp[i];
		}
	}
	else{
		for(i = 0; i < 5; i++) face_information[i] = 0;
	}
	Face_data();
}


void Openmv_data()
{
	Openmv_X = openmv[2];      //x
	Openmv_Y = openmv[3];      //y
}


void Face_data()
{
	Face_Inf = face_information[2];
}


#elif 0
uint8_t judge_index=0;       //1--坐标判断     2--人脸信息判断

void Openmv_Recive(int data)     //0x01--坐标数据      0x02--人脸识别信息，
{
	u8 i;
	static u8 state = 0;
		
	if(state==0&&data==0x01)
	{
		state = 1;
		judge_index = 1;
		openmv[0] = data;
	}
	else if(state==0&&data==0x02)
	{
		state = 1;
		judge_index = 2;
		face_information[0] = data;
	}
	
	switch(judge_index)
	{
		case 1:
				judge_index = 0;
				if(state==1&&data==0xb3)
				{
					state = 2;
					openmv[1] = data;
				}
				else if(state==2&&data==0xb3)
				{
					state = 3;
					openmv[2] = data;
				}
				else if(state==3)
				{
					state = 4;
					openmv[3] = data;      //X坐标
				}
				else if(state==4)
				{
					state = 5;
					openmv[4] = data;	   //Y坐标
				}
				else if(state==5)
				{
					if(data == 0x5b)
					{
						state = 0;
						openmv[5] = data;
					}
					else if(data != 0x5b)
					{
						state = 0;
						for(i=0;i<6;i++)
						{
							openmv[i] = 0x00;
						}
					}
				}
				else
				{
					state = 0;
					for(i=0;i<6;i++)
					{
						openmv[i] = 0x00;
					}
				}
				break;
		case 2:
				judge_index = 0;
				if(state==1&&data==0xb3)
				{
					state = 2;
					face_information[1] = data;
				}
				else if(state==2&&data==0xb3)
				{
					state = 3;
					face_information[2] = data;
				}
				else if(state==3)
				{
					state = 4;
					face_information[3] = data;      //人脸序号，用于LCD屏幕上显示对应名字
				}
				else if(state==4)
				{
					state = 5;
					face_information[4] = data;	   
				}
				else if(state==5)
				{
					if(data == 0x5b)
					{
						state = 0;
						face_information[5] = data;
					}
					else if(data != 0x5b)
					{
						state = 0;
						for(i=0;i<6;i++)
						{
							face_information[i] = 0x00;
						}
					}
				}
				else
				{
					state = 0;
					for(i=0;i<6;i++)
					{
						face_information[i] = 0x00;
					}
				}
				break;
	}	
}


void Openmv_data()
{
	Openmv_X = openmv[3];      //x
	Openmv_Y = openmv[4];      //y
}

#endif
