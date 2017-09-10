#ifndef TRANSPORT_H_INCLUDED
#define TRANSPORT_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<windows.h>  //Win调用相关
#include<wincon.h>
#include<stdbool.h>
#include<conio.h>  //控制台函数用的头文件
#include<io.h>    //IO
#include<fcntl.h>  //文件相关
#include<sys\stat.h>  //基本系统数据类型
#include<time.h>

#define SCR_ROW 30 //屏幕行数
#define SCR_COL 100 //屏幕列数
#define PAGE_LINE 23  //显示区域行数

//货运清单信息
typedef struct goods_data{
    char road[6]; //固定配送路线编号
    char station_id[10]; //站点序号
    int number;   //货物编号
    char name[15];   //货物名称
    float quantity;    //数量
    struct goods_data *next;
} GOODS_DATA;


//配送车辆基本信息
typedef struct truck_data{
    char station_id[10]; //站点序号
    char road[6];  //执行配送路线编号
    char number[8]; //车辆牌照
    char driver[8]; //司机姓名
    char phone[11]; //司机移动电话
    float volume;   //车辆容积
    float left_volume;  //剩余载货量
    GOODS_DATA *goods; //指向货物清单
}TRUCK_DATA;

//经停站点详细信息
typedef struct station_data{
    char road[6]; //固定配送路线编号
    int station_num; //站点编号
    char station_id[10]; //站点序号
    char station_name[50]; //站点名称
    float distance_init; //与起始站点距离
    float distance_up; //与上一个站点距离
    float using_time; //与上一个站点交通耗时
    float stay_time; //停留耗时
    char across_num[20]; //若有交叉固定路线编号，在此给出
    struct station_data *next;
    TRUCK_DATA *truck; //指向车辆基本信息
}STATION_DATA;

typedef struct station_code{
    int station_num;
    char station_name[50];
    struct station_code *next;
}STATION_CODE;

//配送路线基本信息
typedef struct road_data{
    char road[6]; //固定配送路线编号
    char road_name[20]; //固定配送路线名称
    short full_station; //固定配送路线总站点数
    float full_distance; //固定配送路线总公里数
    float full_time; //全站点配送总耗时
    int init_station; //起始站点编号
    int fin_station; //终止站点编号
    char charge_person[8]; //负责人姓名
    char call[8]; //负责人办公室电话
    char phone[11]; //负责人移动电话
    char email[50]; //负责人电子邮箱
    struct road_data *next;
    STATION_DATA *station; //指向经停站信息
}ROAD_DATA;

/**
 *屏幕窗口信息链结点结点结构
 */
typedef struct layer_node {
    char LayerNo;            /**< 弹出窗口层数*/
    SMALL_RECT rcArea;       /**< 结构，弹出窗口区域坐标*/
    CHAR_INFO *pContent;     /**< 结构，弹出窗口区域字符单元原信息存储缓冲区*/
    char *pScrAtt;           /**< 弹出窗口区域字符单元原属性值存储缓冲区*/
    struct layer_node *next; /**< 指向下一结点的指针*/
} LAYER_NODE;

/**
 *标签束结构
 */
typedef struct labe1_bundle {
    char **ppLabel;        /**< 标签字符串数组首地址*/
    COORD *pLoc;           /**< 结构，标签定位数组首地址*/
    int num;               /**< 标签个数*/
} LABEL_BUNDLE;

/**
 *热区结构
 热区就是可点击的地方
 */
typedef struct hot_area {
    SMALL_RECT *pArea;     /**< 热区定位数组首地址*/
    char *pSort;           /**< 热区类别(按键、文本框、选项框)数组首地址*/
    char *pTag;            /**< 热区序号数组首地址*/
    int num;               /**< 热区个数*/
} HOT_AREA;


#endif // TRANSPORT_H_INCLUDED
