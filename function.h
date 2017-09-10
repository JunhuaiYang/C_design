#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

#include"global.h"
#include"transport.h"

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
BOOL DeleteStationBase(void);  //删除站点

BOOL InsertRoad(void); //插入路线
BOOL InsertStation(void); //插入站点

BOOL InsertGoods(void); //插入货物清单
BOOL EditGoods(void);  //编辑货物清单
BOOL DeleteGoods(void);  //删除货物



#endif // FUNCTION_H_INCLUDED
