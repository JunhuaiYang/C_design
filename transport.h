#ifndef TRANSPORT_H_INCLUDED
#define TRANSPORT_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<windows.h>
#include<wincon.h>
#include<stdbool.h>
#include<conio.h>  //?
#include<io.h>    //?
#include<fcntl.h>  //?
#include<sys\stat.h>  //?
#include<time.h>  //?

#define SCR_ROW 25 //屏幕行数
#define SCR_COL 80 //屏幕列数

//货运清单信息
typedef struct transport_data{
    char name[15];   //货物名称
    int quantity;    //数量
    struct transport_date *next;
} TransportData;

//配送车辆基本信息
typedef struct truck_data{
    char number[8]; //车辆牌照
    char road[6];  //执行配送路线编号
    char driver[8]; //司机姓名
    char phone[11]; //司机移动电话
    struct truck_data *next;
    TransportData *cnext; //指向货物清单
}TruckData;

//经停站点详细信息
typedef struct station_data{
    char road[6]; //固定配送路线编号
    int station_num; //站点序号
    char station_id[10]; //站点编号
    char station_name[50]; //站点名称
    float distance_init; //与起始站点距离
    float distance_up; //与上一个站点距离
    float using_time; //与上一个站点交通耗时
    float stay_time; //停留耗时
    char across_num[20]; //未确定
    struct station_data *next;
    TruckData *cnext; //指向车辆基本信息
}StationData;

//配送路线基本信息
typedef struct road_data{
    char road[6]; //固定配送路线编号
    char road_name[20]; //固定配送路线名称
    short full_station; //固定配送路线总站点数
    float full_distance; //固定配送路线总公里数
    float full_time; //全站点配送总耗时
    char init_station[10]; //起始站点编号
    char fin_station[10]; //终止站点编号
    char charge_person[8]; //负责人姓名
    char call[8]; //负责人办公室电话
    char phone[11]; //负责人移动电话
    char email[50]; //负责人电子邮箱
    struct road_data *next;
    StationData *cnext; //指向经停站信息
}RoadData;



#endif // TRANSPORT_H_INCLUDED
