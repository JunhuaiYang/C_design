#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

/** 全局变量用的声明头文件
 *
 *  定义与详解在global.c中
 *
 */
#include"function.h"
#include"transport.h"


extern DWORD ul;
extern LAYER_NODE *gp_top_layer;
extern ROAD_DATA *gp_head;
extern CHAR_INFO *gp_buff_menubar_info;
extern CHAR_INFO *gp_buff_stateBar_info;
extern char *gp_scr_att;

extern STATION_CODE *gp_station_code;
extern char gc_sys_state;
extern HANDLE gh_std_out;
extern HANDLE gh_std_in;

extern unsigned long gul_station_count;
extern unsigned long gul_road_count;

//菜单文本
extern char *gp_sys_name ;    //系统名称
extern char *gp_road_filename ;   //保存线路文件名
extern char *gp_station_filename ;  //保存站点信息文件名
extern char *gp_truck_filename ;  //保存车辆信息文件名
extern char *gp_goods_filename ; //保存货物信息文件名
extern char *gp_station_code_filename; //保存站点代码表

extern char *ga_main_menu[] ;

extern char *ga_sub_menu[];

extern int ga_sub_menu_count[];      //各主菜单项下子菜单的个数
extern int gi_sel_menu ;
extern int gi_sel_sub_menu ;

#endif // GLOBAL_H_INCLUDED
