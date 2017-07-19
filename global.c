#include"transport.h"

//全局变量定义

LAYER_NODE *gp_top_layer = NULL;               /*弹出窗口信息链链头*/
ROAD_DATA *gp_mhead = NULL;


CHAR_INFO *gp_buff_menubar_info = NULL;     /*存放菜单条屏幕区字符信息的缓冲区*/
CHAR_INFO *gp_buff_stateBar_info = NULL;    /*存放状态条屏幕区字符信息的缓冲区*/

//用途？？
char *gp_scr_att = NULL;    /*存放屏幕上字符单元属性值的缓冲区*/
char *gp_station_code = NULL;   /*存放站点代码表的数据缓冲区*/
char gc_sys_state = '\0';   /*用来保存系统状态的字符*/

//句柄
HANDLE gh_std_out;          /*标准输出设备句柄*/
HANDLE gh_std_in;           /*标准输入设备句柄*/

