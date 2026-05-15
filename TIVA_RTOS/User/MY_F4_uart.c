#include "MY_F4_uart.h"
#include "Drv_Uart.h"
#include "ANO_Uart_V8.h"
#include "N100.h"

_postion_def Fly_postion;

u8 F4_data[50] = {0};
u8 F4_rxstate = 0;
u8 _data_len = 0, _data_cnt = 0, _data_index = 0;

void F4_get_onebyte(u8 data)
{
//	DrvUart1SendBuf(&F4_rxstate,1);

	if (F4_rxstate == 0 && data == 0xFE)				//帧头
	{
		F4_rxstate++;
		_data_len = 0;
	}
	else if (F4_rxstate == 1)							//长度
	{
		_data_len = data;
		F4_rxstate++;		
		_data_cnt = 0;
		_data_index = 0;
	}
	else if (F4_rxstate == 2)							//数据
	{
		if(_data_index < _data_len - 1)
		{
			F4_data[_data_index++] = data;
			_data_cnt += data;
		}
		else
			F4_rxstate++;
	}
	else if (F4_rxstate == 3 && data == _data_cnt)		//校验
	{
		F4_rxstate++;
	}
	else if (F4_rxstate == 4 && data == 0xEF)			//帧尾
	{
		F4_rxstate = 0;
		F4_data_handle();
	}
	else
	{		
		F4_rxstate = 0;
	}
}

double AHRS_yaw_change;

void F4_data_handle(void)	//数据处理函数
{
	Fly_postion.x = F4_data[0] + (F4_data[1] << 8);
	Fly_postion.y = F4_data[2] + (F4_data[3] << 8);
	Fly_postion.z = F4_data[4] + (F4_data[5] << 8);
	
	Fly_postion.x *= -1;
	Fly_postion.y *= -1;
	
	AHRS_yaw_change = MY_N100.AHRS.Yaw-90 < -180?(MY_N100.AHRS.Yaw-90 + 360):MY_N100.AHRS.Yaw-90;
//	ANO_Send_Data(0x32,3,(int)Fly_postion.x,(int)Fly_postion.y,(int)Fly_postion.z);
	ANO_Send_Data(0xF1,5,(int)Fly_postion.x,(int)Fly_postion.y,(int)Fly_postion.z,(int)(AHRS_yaw_change),0x01);
	ANO_Send_Data(0x03,4,(int)(MY_N100.AHRS.Roll*100),(int)(MY_N100.AHRS.Pitch*100),(int)((AHRS_yaw_change)*100),0x01);
	
//	print_u1("F4:%d/%d/%d\r\n",Fly_postion.x,Fly_postion.y,Fly_postion.z);
}
