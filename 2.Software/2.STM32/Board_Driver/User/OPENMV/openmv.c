#include "openmv.h"

int Openmv_X=100; /*OPENMV X �ᷴ������*/
int Openmv_Y=100; /*OPENMV Y �ᷴ������*/

int Face_Inf = 100;

int openmv[5];//stm32������������	       //������һ��ͨ�Ÿ�ʽ
int face_information[5];

#if 1
void Openmv_Recive(int data)           //-------��������
{
	u8 i;
	static u8 state = 0;
		
	if(state==0&&data==0xb3)
	{
		state = 1;
		openmv[0] = data;
	}
	else if(state==1&&data==0xb3)
	{
		state = 2;
		openmv[1] = data;
	}
	else if(state==2)
	{
		state = 3;
		openmv[2] = data;
	}
	else if(state==3)
	{
		state = 4;
		openmv[3] = data;
	}
	else if(state==4)
	{
		if(data == 0x5b)
		{
			state = 0;
			openmv[4] = data;
		}
		else if(data != 0x5b)
		{
			state = 0;
			for(i=0;i<5;i++)
			{
				openmv[i] = 0x00;
			}
		}
	}
	else
	{
		state = 0;
		for(i=0;i<5;i++)
		{
			openmv[i] = 0x00;
		}
	}	
}


void face_inf_Rec(int data)             //-------���������Ϣ
{
//	u8 j;
	static u8 flag = 0;
	if(flag==0 && data==0xb1)
	{
		flag = 1;
		face_information[0] = data;
	}
	else if(flag==1 && data==0xb1)
	{
		flag = 2;
		face_information[1] = data;
	}
	else if(flag==2)
	{
		flag = 3;
		face_information[2] = data;     //face information   200
	}
	else if(flag==3)
	{
		flag = 4;;
		face_information[3] = data;
	}
	else if(flag==4)
	{
		if(data == 0x5b)
		{
			flag = 0;
			face_information[4] = data;
		}
//		else if(data != 0x5b)
//		{
//			flag = 0;
//			for(j=0;j<5;j++)
//			{
//				face_information[j] = 0x00;
//			}
//		}
	}
	else
	{
		flag = 0;
//		for(j=0;j<5;j++)
//		{
//			face_information[j] = 0x00;
//		}
	}	
	
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
uint8_t judge_index=0;       //1--�����ж�     2--������Ϣ�ж�

void Openmv_Recive(int data)     //0x01--��������      0x02--����ʶ����Ϣ��
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
					openmv[3] = data;      //X����
				}
				else if(state==4)
				{
					state = 5;
					openmv[4] = data;	   //Y����
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
					face_information[3] = data;      //������ţ�����LCD��Ļ����ʾ��Ӧ����
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


