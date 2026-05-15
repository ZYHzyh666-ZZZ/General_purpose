#include "MY_Fun.h"

#define __map_start &map[21]
#define __dat_work  map_work
#define __dat_go    map_go

u16 Camera_X = 0;
u16 Camera_Y = 0;

u16 location = 0;
u16 width = 0;
u8 number = 0;

/*
21   20  19  18  14  10  8   4   3   7   9   13  17  16  12  6   2   1   5   11  15  22  23  24  25  26  27  28
0    0   0   1   0   0   0   1   1   0   0   0   1   1   0   0   1   1   0   0   0   1   0   0   0   0   0   1
*/

_position_st map[] = 
{
                    {NULL,NULL,NULL,NULL  ,NULL,NULL,NULL,NULL},  //凑数
                    {1  ,0  ,0  ,&map[5]  ,1  ,2  ,14   ,1},          //1
                    {2  ,0  ,0  ,&map[1]  ,1  ,3  ,14   ,1},          //2
                    {3  ,0  ,0  ,&map[7]  ,1  ,2  ,14   ,1},          //3
                    {4  ,0  ,0  ,&map[3]  ,1  ,3  ,14   ,1},          //4
                    {5  ,0  ,0  ,&map[11] ,0  ,4  ,14   ,1},          //5
                    {6  ,0  ,0  ,&map[2]  ,0  ,3  ,14   ,1},          //6
                    {7  ,0  ,0  ,&map[9]  ,0  ,4  ,14   ,1},          //7
                    {8  ,0  ,0  ,&map[4]  ,0  ,3  ,14   ,1},          //8
                    {9  ,0  ,0  ,&map[13] ,0  ,4  ,14   ,1},          //9
                    {10 ,0  ,0  ,&map[8]  ,0  ,3  ,14   ,1},         //10
                    {11 ,0  ,0  ,&map[15] ,0  ,4  ,14   ,1},         //11
                    {12 ,0  ,0  ,&map[6]  ,0  ,3  ,14   ,1},         //12
                    {13 ,0  ,0  ,&map[17] ,0  ,4  ,14   ,1},         //13
                    {14 ,0  ,0  ,&map[10] ,0  ,3  ,14   ,1},         //14
                    {15 ,0  ,0  ,&map[22] ,0  ,4  ,14   ,1},         //15
                    {16 ,0  ,0  ,&map[12] ,1  ,2  ,14   ,1},         //16
                    {17 ,0  ,0  ,&map[16] ,1  ,4  ,14   ,1},         //17
                    {18 ,0  ,0  ,&map[14] ,1  ,2  ,14   ,1},         //18
                    {19 ,0  ,0  ,&map[18] ,0  ,2  ,14   ,1},         //19
                    {20 ,0  ,0  ,&map[19] ,0  ,2  ,14   ,1},         //20
                    {21 ,0  ,0  ,&map[20] ,0  ,0  ,00   ,1},         //21
                    {22 ,0  ,0  ,&map[23] ,1  ,4  ,14   ,1},         //22
                    {23 ,0  ,0  ,&map[24] ,0  ,1  ,14   ,1},         //23
                    {24 ,0  ,0  ,&map[25] ,0  ,1  ,14   ,1},         //24    
                    {25 ,0  ,0  ,&map[26] ,0  ,1  ,14   ,1},         //25
                    {26 ,0  ,0  ,&map[27] ,0  ,1  ,14   ,1},         //26
                    {27 ,0  ,0  ,&map[28] ,0  ,1  ,14   ,1},         //27
                    {28 ,0  ,0  ,NULL     ,1  ,1  ,14   ,1},         //28
};

_position_st* map_go = __map_start;
_position_st* map_work = __map_start;

u8 work_mod = 0;

void MAP_init(s16 x,s16 y)//A点坐标21号
{
    u8 i = 49;

    map[1].x = x + 6*i;
    map[1].y = y - 4*i;

    map[2].x = x + 5*i;
    map[2].y = y - 4*i;

    map[3].x = x + 4*i;
    map[3].y = y - 4*i;

    map[4].x = x + 3*i;
    map[4].y = y - 4*i;

    map[5].x = x + 6*i;
    map[5].y = y - 3*i;

    map[6].x = x + 5*i;
    map[6].y = y - 3*i;

    map[7].x = x + 4*i;
    map[7].y = y - 3*i;

    map[8].x = x + 3*i;
    map[8].y = y - 3*i;

    map[9].x = x + 4*i;
    map[9].y = y - 2*i;

    map[10].x = x + 3*i;
    map[10].y = y - 2*i;

    map[11].x = x + 6*i;
    map[11].y = y - 1*i;

    map[12].x = x + 5*i;
    map[12].y = y - 1*i;

    map[13].x = x + 4*i;
    map[13].y = y - 1*i;

    map[14].x = x + 3*i;
    map[14].y = y - 1*i;

    map[15].x = x + 6*i;
    map[15].y = y - 0*i;

    map[16].x = x + 5*i;
    map[16].y = y - 0*i;

    map[17].x = x + 4*i;
    map[17].y = y - 0*i;

    map[18].x = x + 3*i;
    map[18].y = y - 0*i;

    map[19].x = x + 2*i;
    map[19].y = y - 0*i;

    map[20].x = x + 1*i;
    map[20].y = y - 0*i;

    map[21].x = x + 0*i;
    map[21].y = y - 0*i;

    map[22].x = x + 6*i;
    map[22].y = y + 1*i;

    map[23].x = x + 5*i;
    map[23].y = y + 1*i;

    map[24].x = x + 4*i;
    map[24].y = y + 1*i;

    map[25].x = x + 3*i;
    map[25].y = y + 1*i;

    map[26].x = x + 2*i;
    map[26].y = y + 1*i;

    map[27].x = x + 1*i;
    map[27].y = y + 1*i;

    map[28].x = x + 0*i;
    map[28].y = y + 1*i;
}

u8 MAP_updata(u8 dat)//map更新
{
    if(map_work->next == NULL && map_go->next == NULL)
        return 1;

    // if(dat == 0 && map_work->next == NULL)
    //     return 1;
    if(dat == 1 && map_go->next == NULL)
        return 1;

    if(dat == 1)
    {
        while((map_go->next != NULL) && (map_go->next->turn == 0))
        {
            map_go = map_go->next;
        }
        if(map_go->next == NULL)
        {    
            return 1;
        }
        else
        {
            map_go = map_go->next;
        }
    }
    else
    {
        if(map_work->next != NULL)
            map_work = map_work->next;
    }
    // Printf_uart2("ID:%d\n",map_go->id);
    return 0;
}

void fly_velocity_updata(void)
{
    switch (work_mod)
    {
        case 0:
            /* code */
            break;
        case 1:         //A定点
        {
            velocity_target_x = PID3_updata(postion_begin_x + postion_A_X - (int)Radar.Location_X);
            velocity_target_y = PID3_updata(postion_begin_y + postion_A_Y - (int)Radar.Location_Y);
            /* code */
            break;
        }
        case 2:         //竖杆识别
        {
            velocity_target_x = (int)PID3_updata(width_data - width) * 0.5;
            velocity_target_y = (int)PID3_updata(location_data - location) * 0.3;

            // velocity_target_x = 0;
            // velocity_target_y = 0;
            /* code */
            break;
        }
        case 3:         //播撒
        {
            velocity_target_x = PID1_updata(map_go->x - (int)Radar.Location_X);
            velocity_target_y = PID2_updata(map_go->y - (int)Radar.Location_Y);
            /* code */
            break;
        }
        case 4:         //起点闭环
        {
            velocity_target_x = PID3_updata(postion_begin_x - (int)Radar.Location_X);
            velocity_target_y = PID3_updata(postion_begin_y + number*10 - (int)Radar.Location_Y);
            /* code */
        }
        default:
            break;
    }

    ANO_DT_Send_MY_DATA(0xF6,4,velocity_target_x,velocity_target_y,map_go->x,map_go->y);
}

void sow_judgment(void)
{
    u8 __switch_flag = 0;
    switch(map_work->direction)
    {
        case 1: //前
        {
            if((int)Radar.Location_X < map_work->x + map_work->distance)
                __switch_flag = 1;
            break;
        }
        case 2: //后
        {
            if((int)Radar.Location_X > map_work->x - map_work->distance)
                __switch_flag = 1;
            break;
        }
        case 3: //左
        {
            if((int)Radar.Location_Y < map_work->y + map_work->distance)
                __switch_flag = 1;
            break;
        }
        case 4: //右
        {
            if((int)Radar.Location_Y > map_work->y - map_work->distance)
                __switch_flag = 1;
            break;
        }
        case 0:
        {
            if((ABS((int)Radar.Location_X - map_work->x) < 20) && (ABS((int)Radar.Location_Y - map_work->y) < 20))
            {
                if(map_work->work ==1)
                    MY_Lazer_2_ON(1);
                // Printf_uart2("ID:%d OK\n",map_work->id);
                MAP_updata(0);
            }
            break;
        }
    }

    if(__switch_flag == 1)
    {
        MY_Lazer_2_ON(1);
        // Printf_uart2("ID:%d OK\n",map_work->id);
        MAP_updata(0);
        // return 1;
    }
    // return 0;
}
