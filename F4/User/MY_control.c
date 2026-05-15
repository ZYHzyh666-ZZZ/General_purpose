#include "ALL_include.h"

float Yaw_target = 0;

My_mission_task task_mission_1[]=
{
    {mission_1_1,3.0f},//起飞
    {mission_1_2,3.0f},//定A
    {mission_1_3,3.0f},//播撒
	{mission_1_4,2.0f},//返回
	{mission_1_5,2.0f},//降落
	// {mission_1_6,5.0f},
	// {mission_1_7,2.0f},
	// {mission_1_8,10.0f},
	// {mission_1_9,2.0f},//3
	// {mission_1_10,2.0f},//2
	// {mission_1_11,2.0f},//1
	// {mission_1_12,2.0f},//降落点
	// {mission_1_13,2.0f},//降落
    // {mission_1_14,2.0f},//降落
	// {mission_1_15,2.0f},//降落
	// {mission_1_13,2.0f},//降落
};
My_task_state task_1;
u8 start_task1_flag = 0;		//任务开始标志

My_mission_task task_mission_2[]=
{
	{mission_2_1,2.0f},//起飞
	{mission_2_2,2.0f},//旋转
	{mission_2_3,2.0f},//上下
	{mission_2_4,5.0f},//起飞
	{mission_2_5,4.0f},//前后
	{mission_2_6,2.0f},//识别
	{mission_2_7,4.0f},//打击目标
	{mission_2_8,2.0f},//打击
};
My_task_state task_2;
u8 start_task2_flag = 0;		//任务开始标志

u8 takeoff_once = 0;			//任务开始第一次标志位

/*-----------------------------*/

s16 postion_begin_x = 0;			//记录起飞坐标_X
s16 postion_begin_y = 0;			//记录起飞坐标_Y

/**
  * 函数作用：  任务状态检测
  * 参数1：     （u16）系统时间ms
  * 返回值：     无
  * 
  * 备注：      放置任务中调用检测
  * 更新日期：   2025-11-27：无
  * 
  */
void task_check(u16 ms)
{
	if(task_1.finish == 1 || task_2.finish == 1)//完成一次任务音效提示
	{
		MY_BEEP_ON(300);
	}
	if(task_1.finish == 1)
	{
		task_1.finish = 0;
		task_1.mission_times = 0;
		task_1.step++;
	}
	else if(start_task1_flag == 1)				//无锁桨检测
	// else if(start_task1_flag == 1 && fc_sta.unlock_sta == 1)	
		task_1.mission_times = task_1.mission_times + (float)ms/1000;//记录时间

	if(task_2.finish == 1)
	{
		task_2.finish = 0;
		task_2.mission_times = 0;
		task_2.step++;
	}
	else if(start_task2_flag == 1)
		task_2.mission_times = task_2.mission_times + (float)ms/1000;//记录时间
}

/**
  * 函数作用：  任务运行函数
  * 参数1：     （u16）系统时间ms
  * 返回值：     无
  * 
  * 备注：      放置在5ms定时器中调用
  * 更新日期：   2025-11-27：无
  * 
  */
void task_run(u16 ms)//任务运行
{
	task_check(ms);
	if(start_task1_flag == 1)			//任务1开始标志
	{
		if(task_1.step < task_1.task_num)
			task_1.mission[task_1.step].task_mission();
		else
			task_1.step = task_1.task_num;//任务结束
	}

	if(start_task2_flag == 1)			//任务2开始标志
	{
		if(task_2.step < task_2.task_num)
			task_2.mission[task_2.step].task_mission();
		else
			task_2.step = task_2.task_num;//任务结束
	}
}

/**
  * 函数作用：  任务1创建函数
  * 参数1：     无
  * 返回值：    无
  * 
  * 备注：      无
  * 更新日期：   2025-11-27：无
  * 
  */
void create_task_1(void)//创建任务
{
	task_1.mission = task_mission_1;

	task_1.finish = 0;
	task_1.step = 0;
	task_1.task_num = sizeof(task_mission_1)/sizeof(task_mission_1[0]);
	task_1.mission_times = 0;
}

/**
  * 函数作用：  任务2创建函数
  * 参数1：     无
  * 返回值：    无
  * 
  * 备注：      无
  * 更新日期：   2025-11-27：无
  * 
  */
void create_task_2(void)//创建任务
{
	task_2.mission = task_mission_2;

	task_2.finish = 0;
	task_2.step = 0;
	task_2.task_num = sizeof(task_mission_2)/sizeof(task_mission_2[0]);
	task_2.mission_times = 0;
}

void mission_1_1(void)//起飞
{
	if(takeoff_once == 0)
	{
		postion_target_z = Default_height;
		takeoff_once = 1;
		Yaw_target = N100.Yaw;
		postion_begin_x = Radar.Location_X;
		postion_begin_y = Radar.Location_Y;
		MAP_init(postion_begin_x + postion_A_X,postion_begin_y + postion_A_Y);

		ANO_DT_SendString(1,"mod1_begin");
		ANO_DT_SendString(2,"map create");	
	}
	Set_High(postion_target_z);
	mode_Hold_Yaw(Yaw_target);				//保持航向角稳定

	if(_time_ok(task_1) && ano_of.of_alt_cm > Default_height - 2)//起飞到150cm
	{
		task_1.finish = 1;
		takeoff_once = 0;
//		MY_Camera_print("OK");
		ANO_DT_SendString(2,"FLAY OK!");
		Move_stop();
		return;
	}
}

static int state = 0;

void mission_1_2(void)//定目标
{
	if(takeoff_once == 0)
	{
		postion_target_z = Default_height;
		work_mod = 1;
		takeoff_once = 1;
		LAZER_2_ON;
	}
	Set_High(postion_target_z);
	mode_Hold_Yaw(Yaw_target);				//保持航向角稳定

	Program_Ctrl_User_Set_HXYcmps(velocity_target_x,velocity_target_y);

	if(((ABS(postion_begin_x + postion_A_X - (int)Radar.Location_X) < 10) && (ABS(postion_begin_y + postion_A_Y - (int)Radar.Location_Y) < 10)) && _time_ok(task_1))
	{
		Move_stop();

		task_1.finish = 1;
		takeoff_once = 0;
		work_mod = 0;
		ANO_DT_SendString(2,"Find A OK!");

		return;
	}
}

void mission_1_3(void)//播撒
{
	if(takeoff_once == 0)
	{
		postion_target_z = Default_height;
		work_mod = 3;
		takeoff_once = 1;
		state = 0;
	}
	Set_High(postion_target_z);
	mode_Hold_Yaw(Yaw_target);				//保持航向角稳定
	Program_Ctrl_User_Set_HXYcmps(velocity_target_x,velocity_target_y);

	sow_judgment();
	if((state == 0) && ((ABS((int)Radar.Location_X - map_go->x) < 13) && (ABS((int)Radar.Location_Y - map_go->y) < 13)))
	{
		MY_BEEP_ON(200);
		state = MAP_updata(1);
	}
	if(state == 1 && _time_ok(task_1))
	{
		task_1.finish = 1;
		takeoff_once = 0;
		work_mod = 0;
		ANO_DT_SendString(2,"work OK!");
		return;
	}
}

void mission_1_4(void)//返回
{
	if(takeoff_once == 0)
	{
		postion_target_z = Default_height;
		takeoff_once = 1;
		work_mod = 4;
	}

	Set_High(postion_target_z);
	mode_Hold_Yaw(Yaw_target);				//保持航向角稳定

	Program_Ctrl_User_Set_HXYcmps(velocity_target_x,velocity_target_y);
	
	if(_time_ok(task_1) && abs(velocity_target_x) + abs(velocity_target_y) < 3 && ABS(postion_begin_x - Radar.Location_X) < 5 && ABS(postion_begin_y - Radar.Location_Y) < 5)
	{
		task_1.finish = 1;
		takeoff_once = 0;
		work_mod = 0;
		Move_stop();
		ANO_DT_SendString(2,"return OK!");
		return;
	}
}

void mission_1_5(void)//降落
{
	if(takeoff_once == 0)//
	{
		postion_target_z = 0;
		MY_HMI.TARGET_position.Z = postion_target_z;
		begin_land = 1;		
		takeoff_once = 1;
		Move_stop();
	}

	Set_High(postion_target_z);
	Program_Ctrl_User_Set_HXYcmps(velocity_target_x,velocity_target_y);
	if(ano_of.of_alt_cm < 25 && _time_ok(task_1))
	{
		task_1.finish = 1;
		takeoff_once = 0;
		FC_Lock();
		ANO_DT_SendString(2,"land OK!");
		return;
	}
}

void mission_1_6(void)//巡线
{
	if(takeoff_once == 0)
	{
		// postion_target_z = Default_height;
		camera_flag[1] = 0;

		// width[0] = 0;
		// location[0] = 0;
		takeoff_once = 1;
	}

	Set_High(postion_target_z);
	Program_Ctrl_User_Set_HXYcmps(velocity_target_x,-18);
	mode_Hold_Yaw(Yaw_target);				//保持航向角稳定

	if(task_1.mission_times < 4.0f)//巡线4s内
		camera_flag[1] = 0;

	if(_time_ok(task_1) && camera_flag[1])
	{
		task_1.finish = 1;
		takeoff_once = 0;
		Move_stop();
		ANO_DT_SendString(2,"find OK!");
		return;
	}
}

void mission_1_7(void)//绕另一根杆
{
	if(takeoff_once == 0)
	{
		postion_target_z = 145;
		takeoff_once = 1;
	}
	// postion_target_z =  MY_fly.C_system.z + PID1_updata(location[0]);

	Set_High(postion_target_z);
	if(task_1.mission_times < 3.0f)//巡线3s内
		mode_Hold_Yaw(Yaw_target);				//保持航向角稳定
	else
		Program_Ctrl_User_Set_YAWdps(-5);//旋转速度
	Program_Ctrl_User_Set_HXYcmps(velocity_target_x,velocity_target_y);
		// Program_Ctrl_User_Set_HXYcmps(velocity_target_x,0);
	
	if(_time_ok(task_1) && abs(Angle_calculation(Yaw_target/100.f,MY_fly.body.yaw)) > 177)
	{
		task_1.finish = 1;
		takeoff_once = 0;
		ANO_DT_SendString(2,"zhuan OK!");
		return;
	}
}

void mission_1_8(void)//降落
{
	if(takeoff_once == 0)
	{
		postion_target_z = 145;
		Yaw_target = MY_fly.body.yaw * 100.f;
		// PID2_flag = 1;
		camera_flag[0] = 0;

		takeoff_once = 1;
	}
	// postion_target_z =  MY_fly.C_system.z + PID1_updata(location[0]);

	mode_Hold_Yaw(Yaw_target);				//保持航向角稳定
	if(task_1.mission_times < 1.0f)//巡线1s内
	{
		camera_flag[0] = 0;
		Program_Ctrl_User_Set_HXYcmps(0,-10);
		Set_High(postion_target_z);
	}	
	else
	{
		Program_Ctrl_User_Set_HXYcmps(0,0);
		Set_High(130);
	}

	if(_time_ok(task_1))
	{
		task_1.finish = 1;
		// PID2_flag = 0;
		takeoff_once = 0;
		ANO_DT_SendString(2,"zhuan OK!");
		return;
	}
}

void mission_1_9(void)//降落
{
	if(takeoff_once == 0)//
	{
		postion_target_z = 0;
		MY_HMI.TARGET_position.Z = postion_target_z;
		begin_land = 1;		
		takeoff_once = 1;
		Move_stop();
	}

	Set_High(postion_target_z);
	if(ano_of.of_alt_cm < 25 && _time_ok(task_1))
	{
		task_1.finish = 1;
		takeoff_once = 0;
		FC_Lock();
		ANO_DT_SendString(2,"land_OK!");
		return;
	}
}

/*---------------------------------------------------------*/
void mission_2_1(void)//起飞
{
	if(takeoff_once == 0)
	{
		postion_target_z = Default_height;
		takeoff_once = 1;
		Yaw_target = N100.Yaw;
		postion_begin_x = Radar.Location_X;
		postion_begin_y = Radar.Location_Y;
		MAP_init(postion_begin_x + postion_A_X,postion_begin_y + postion_A_Y);

		ANO_DT_SendString(1,"mod2_begin");
		ANO_DT_SendString(2,"map create");	
	}
	Set_High(postion_target_z);
	mode_Hold_Yaw(Yaw_target);				//保持航向角稳定

	if(_time_ok(task_2) && ano_of.of_alt_cm > Default_height - 2)//起飞到150cm
	{
		task_2.finish = 1;
		takeoff_once = 0;
//		MY_Camera_print("OK");
		ANO_DT_SendString(2,"FLAY OK!");
		Move_stop();
		return;
	}
}

s16	height = 150;

void mission_2_2(void)//识别
{
	if(takeoff_once == 0)
	{
		postion_target_z = height;
		work_mod = 1;
		takeoff_once = 1;
		location = 0;
		width = 0;
		// LAZER_2_ON;
	}

	Program_Ctrl_User_Set_HXYcmps(velocity_target_x,velocity_target_y);

	mode_Hold_Yaw(Yaw_target);				//保持航向角稳定
	if(work_mod == 1 && location != 0 && width != 0)
	{
		Move_stop();
		MY_BEEP_ON(100);
		work_mod = 2;
		ANO_DT_SendString(1,"find");	
	}
	if(work_mod == 1)
		Set_High(postion_target_z);
	else if(work_mod == 2)
		Set_High(height);

	if(ABS(location - location_data) < 100 && ABS(width - width_data) < 5)
		// if(((int)(task_2.mission_times*100) % 1000))//1s
		{
			if(ABS(height - ano_of.of_alt_cm) < 5)
			{
				height = 280 - height;
			}
		}
//number != 0 && 
	if(_time_ok(task_2))
	{
		Move_stop();
		MY_Lazer_1_ON(number);
		task_2.finish = 1;
		takeoff_once = 0;
		work_mod = 0;
		ANO_DT_SendString(2,"Find number OK!");

		return;
	}
}

void mission_2_3(void)//锁定
{
	if(takeoff_once == 0)
	{
		postion_target_z = Default_height;
		work_mod = 1;
		takeoff_once = 1;
		LAZER_2_ON;
	}
	Set_High(postion_target_z);
	mode_Hold_Yaw(Yaw_target);				//保持航向角稳定

	Program_Ctrl_User_Set_HXYcmps(velocity_target_x,velocity_target_y);

	if(((ABS(postion_begin_x + postion_A_X - (int)Radar.Location_X) < 25) && (ABS(postion_begin_y + postion_A_Y - (int)Radar.Location_Y) < 25)) && _time_ok(task_2))
	{
		Move_stop();

		task_2.finish = 1;
		takeoff_once = 0;
		work_mod = 0;
		ANO_DT_SendString(2,"Find A OK!");

		return;
	}
}      

void mission_2_4(void)//转到中间位置
{
	if(takeoff_once == 0)
	{
		postion_target_z = Default_height;
		work_mod = 3;
		takeoff_once = 1;
		state = 0;
	}
	Set_High(postion_target_z);
	mode_Hold_Yaw(Yaw_target);				//保持航向角稳定
	Program_Ctrl_User_Set_HXYcmps(velocity_target_x,velocity_target_y);

	sow_judgment();
	if((state == 0) && ((ABS((int)Radar.Location_X - map_go->x) < 13) && (ABS((int)Radar.Location_Y - map_go->y) < 13)))
	{
		MY_BEEP_ON(200);
		state = MAP_updata(1);
	}
	if(state == 1 && _time_ok(task_2))
	{
		task_2.finish = 1;
		takeoff_once = 0;
		work_mod = 0;
		ANO_DT_SendString(2,"work OK!");
		return;
	}
}

void mission_2_5(void)//
{
	if(takeoff_once == 0)
	{
		postion_target_z = Default_height;
		takeoff_once = 1;
		work_mod = 4;
	}

	Set_High(postion_target_z);
	mode_Hold_Yaw(Yaw_target);				//保持航向角稳定

	Program_Ctrl_User_Set_HXYcmps(velocity_target_x,velocity_target_y);
	
	if(_time_ok(task_2) && abs(velocity_target_x) + abs(velocity_target_y) < 5 && ABS(postion_begin_x - Radar.Location_X) < 7 && ABS(postion_begin_y + number*10 - Radar.Location_Y) < 7)
	{
		task_2.finish = 1;
		takeoff_once = 0;
		work_mod = 0;
		Move_stop();
		ANO_DT_SendString(2,"return OK!");
		return;
	}
}

void mission_2_6(void)//右方
{
	if(takeoff_once == 0)//
	{
		postion_target_z = 0;
		MY_HMI.TARGET_position.Z = postion_target_z;
		begin_land = 1;		
		takeoff_once = 1;
		Move_stop();
	}

	Set_High(postion_target_z);
	Program_Ctrl_User_Set_HXYcmps(0,0);
	if(ano_of.of_alt_cm < 25 && _time_ok(task_2))
	{
		task_2.finish = 1;
		takeoff_once = 0;
		FC_Lock();
		ANO_DT_SendString(2,"land OK!");
		return;
	}
}

void mission_2_7(void)//降落
{

}


void mission_2_8(void)//降落
{
	// if(takeoff_once == 0)
	// {
	// 	postion_target_z = 0;
	// 	MY_HMI.TARGET_position.Z = postion_target_z;
	// 	begin_land = 1;		
	// 	takeoff_once = 1;
	// }

	// Set_High(postion_target_z);
	// if(ano_of.of_alt_cm < 25 && _time_ok(task_2))
	// {
	// 	task_2.finish = 1;
	// 	takeoff_once = 0;
	// 	FC_Lock();
	// 	ANO_DT_SendString(2,"mod2_land_OK!");
	// 	return;
	// }
}
