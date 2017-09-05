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

//函数定义
BOOL MakeDir(void);                          //创建文件夹
int CreatList(ROAD_DATA **pphead);              /*数据链表初始化*/
int LoadCode(void);                 //代码表加载
void InitInterface(void);                 /*系统界面初始化*/
void ClearScreen(void);                         /*清屏*/
void ShowMenu(void);                            /*显示菜单栏*/
void PopMenu(int num);                          /*显示下拉菜单*/
int PopPrompt(char** ppString,int* iHot);                        /*显示弹出窗口*/
void PopUp(SMALL_RECT *, WORD, LABEL_BUNDLE *, HOT_AREA *);  /*弹出窗口屏幕信息维护*/
void PopOff(void);                              /*关闭顶层弹出窗口*/
void DrawBox(SMALL_RECT *parea);                /*绘制边框*/
void LocSubMenu(int num, SMALL_RECT *parea);    /*主菜单下拉菜单定位*/
void ShowState(void);                           /*显示状态栏*/
void TagMainMenu(int num);                      /*标记被选中的主菜单项*/
void TagSubMenu(int num);                       /*标记被选中的子菜单项*/
int DealInput(HOT_AREA *phot_area, int *pihot_num);  /*控制台输入处理*/
void SetHotPoint(HOT_AREA *phot_area, int hot_num);     /*设置热区*/
void RunSys(ROAD_DATA **pphd);                  /*系统功能模块的选择和运行*/
BOOL ExeFunction(int main_menu_num, int sub_menu_num);  /*功能模块的调用*/
void CloseSys(ROAD_DATA *phd);                  /*退出系统*/
BOOL ShowModule(char **pString, int n);  //提示信息
int PopWindowMenu(char **pString, int n,int areanum, int* tag); //弹出窗口菜单
void ReFresh();   // 刷新界面
void GotoXY(int x, int y); //移动光标
int PopTextBox(char **ppstring, int hot, int *tag); //文本框
void Show_Cursor(BOOL boo); //光标显示

BOOL LoadData(void);           /*数据加载*/
BOOL SaveData(void);           /*保存数据*/
BOOL BackupData(void);         /*备份数据*/
BOOL RestoreData(void);        /*恢复数据*/
BOOL ExitSys(void);            /*退出系统*/
BOOL HelpTopic(void);          /*帮助主体*/
BOOL About(void);          /*关于系统*/

void SeceletSort(ROAD_DATA **head); //排序

BOOL StationCode(void);  //站点信息维护
BOOL RoodCoad(void);  //路线信息维护
BOOL TruckCode(void);  // 车辆信息维护

BOOL FindStationRoad(void); //查询站点路线
BOOL FindRoadTime(void);  //查询路线耗时
BOOL FindRoadDistance(void);  //查询路线里程
BOOL FindRoad(void);  //查询路线经停站点
BOOL FindWeight(void);  //查询可载重量
BOOL FindDriverGoods(void); //查询指定司机配送清单
BOOL FindDriverPhone(void);  //查询司机联系方式
BOOL FindTruck(void);  //查询制定车辆的配送路线

BOOL FormCreate(void);   //生成表格
BOOL TransportMap(void);  //生成配送图

BOOL NewStation(void); //录入站点信息
BOOL LookStation(void); //查看已录入的站点
BOOL EditStation(void); //编辑站点信息
BOOL DeleteStation(void); //删除站点信息

BOOL NewRoad(void);  //录入路线
BOOL LookRoad(void); //查看现有路线
BOOL EditRoad(void); //编辑路线
BOOL DeleteRoad(void); //删除路线
BOOL SaveRoad(void);  //保存路线信息

BOOL EditTruck(void); //编辑车辆和驾驶员信息

BOOL SaveStation(void);//保存代码表到文件

BOOL EditRoadBase(void); //编辑路线基本信息
BOOL EditStationBase(void); //编辑站点基本信息

BOOL InsertRoad(void); //插入路线
BOOL InsertStation(void); //插入站点
BOOL InsertGoods(void); //插入货物清单

#endif // TRANSPORT_H_INCLUDED
