#ifndef _MY_FUN_H_
#define _MY_FUN_H_

#include "ALL_include.h"

typedef struct _position_st     _position_st;

struct _position_st{
    u8 id;
    s16 x;
    s16 y;
    _position_st* next;
    u8 turn;
    u8 direction;               //1前   2后     3左    4右
    s16 distance;
    s16 work;
} ;

#define postion_A_X 50   
#define postion_A_Y 204

#define camera_A_X 215
#define camera_A_Y 278

#define location_data 	640
#define	width_data		135

extern _position_st map[];
extern _position_st* map_go;
extern _position_st* map_work;
extern u8 work_mod ;
extern u16 location;
extern u16 width;
extern u8 number;

void MAP_init(s16 x,s16 y);//A点坐标21号
u8 MAP_updata(u8 dat);//map更新
void fly_velocity_updata(void);
void sow_judgment(void);

#endif
