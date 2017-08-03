#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wincon.h>
#include <conio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <ctype.h>
#include <time.h>
#ifndef TYPE_H_INCLUDED

#endif // TYPE_H_INCLUDED#define TYPE_H_INCLUDED
#define SCR_ROW 25             /*屏幕行数*/
#define SCR_COL 80             /*屏幕列数*/  /**  *菜市场商品分类信息表  */
typedef struct type_node
{
    char type_id;              /**<分类编码*/     char name[10];             /**<分类名称*/     struct item_node *inext;   /**<指向商品基本信息支链的指针*/     struct type_node *next;    /**<指向下一结点的指针*/
} TYPE_NODE;  /**  *菜市场商品基本信息表*/
typedef struct item_node
{
    int item_id;               /**<商品编号*/     char name[20];             /**<商品名称*/     char type_id;                 /**<分类码*/     char producer[20];         /**<产地(可以简单用文字描述)*/     float price;               /**<单价(单位：元/斤)*/     float sale;                /**<销售总重量“斤”*/     struct sale_node *snext;   /**<指向商品销售信息支链的指针*/     struct item_node *next;    /**<指向下一结点的指针*/
} ITEM_NODE;  /**  *菜市场商品销售信息表  */ typedef struct sale_node
{
    int sale_id;             /**<销售编号*/     int item_id;             /**<商品编号*/     float weight;            /**<销售重量*/     float sales_amount;      /**<销售金额*/     char date[10];           /**<销售日期*/     char client_name[20];    /**<客户名称*/     struct sale_node *next;  /**<指向下一结点的指针*/
} SALE_NODE;  /**  *各类商品消费情况表  */ typedef struct type_stat
{
    char type[10];             /**<分类名称*/     float weight;              /**<总重量*/     float sale;              /**<消费金额*/


    struct type_stat *next;    /**<指向下一结点的指针*/
} TYPE_STAT;  /**  *各种商品消费情况表  */ typedef struct item_stat
{
    char type[10];             /**<分类名称*/     char item[20];             /**<商品名称*/     float weight;              /**<总重量*/     float sale;                /**<销售金额*/     struct item_stat *next;    /**<指向下一结点的指针*/
} ITEM_STAT;  /**  *按客户名称统计的商品消费情况表  */ typedef struct client_stat
{
    char client[20];           /**<客户姓名*/     float weight;              /**<总重量*/     float sale;                /**<销售金额*/     struct client_stat *next;    /**<指向下一结点的指针*/
} CLIENT_STAT;  /**  *屏幕窗口信息链结点结点结构  */
typedef struct layer_node
{
    char LayerNo;            /**<弹出窗口层数*/     SMALL_RECT rcArea;       /**<弹出窗口区域坐标*/     CHAR_INFO *pContent;     /**<弹出窗口区域字符单元原信息存储缓冲区*/     char *pScrAtt;           /**<弹出窗口区域字符单元原属性值存储缓冲区*/


    struct layer_node *next; /**<指向下一结点的指针*/
} LAYER_NODE;  /**  *标签束结构  */ typedef struct labe1_bundle
{
    char **ppLabel;        /**<标签字符串数组首地址*/     COORD *pLoc;           /**<标签定位数组首地址*/     int num;               /**<标签个数*/
} LABEL_BUNDLE;  /**  *热区结构  */ typedef struct hot_area
{
    SMALL_RECT *pArea;     /**<热区定位数组首地址*/     char *pSort;           /**<热区类别(按键、文本框、选项框)数组首地址*/     char *pTag;            /**<热区序号数组首地址*/     int num;               /**<热区个数*/
} HOT_AREA;
unsigned long num_written;                        /* Win 7 中使用, 该变量用于某几个控制台输出函数 */  LAYER_NODE *gp_top_layer = NULL;               /*弹出窗口信息链链头*/ TYPE_NODE *gp_head = NULL;                     /*主链头指针*/  void gotoxy(int x, int y) //goto语句 { COORD pos; pos.X = x - 1;


pos.Y = y - 1;
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}  char *gp_sys_name = "菜市场销售管理系统";               /*系统名称*/ char *gp_item_info_filename = "item.dat";               /*商品基本信息数据文件*/ char *gp_sale_info_filename = "sale.dat";               /*商品销售信息数据文件*/ char *gp_type_info_filename = "type.dat";               /*商品分类信息数据文件*/  char *ga_main_menu[] = {"文件(F)",             /*系统主菜单名*/                  "数据维护(M)",                         "数据查询(Q)",                         "数据统计(S)",                         "帮助(H)"                        };
char *ga_sub_menu[] = {"[S] 数据保存",          /*系统子菜单名*/                        "[B] 数据备份",                        "[R] 数据恢复",                        "[X] 退出    Alt+X",                        "[T] 商品分类信息",                        "[I] 商品基本信息",                        "[S] 商品销售信息",                        "[T] 商品分类信息",                        "[I] 商品基本信息",                        "[S] 商品销售信息",                        "[S] 按类别统计",                        "[Y] 按品种统计",                        "[C] 按客户姓名统计",                        "[H] 帮助主题",                        "",                        "[A] 关于..."


                      };
int ga_sub_menu_count[] = {4, 3, 3, 3, 3};  /*各主菜单项下子菜单的个数*/ int gi_sel_menu = 1;                        /*被选中的主菜单项号,初始为1*/ int gi_sel_sub_menu = 0;                    /*被选中的子菜单项号,初始为0,表示未选中*/  CHAR_INFO *gp_buff_menubar_info = NULL;     /*存放菜单条屏幕区字符信息的缓冲区*/ CHAR_INFO *gp_buff_stateBar_info = NULL;    /*存放状态条屏幕区字符信息的缓冲区*/  char *gp_scr_att = NULL;    /*存放屏幕上字符单元属性值的缓冲区*/ char *gp_type_code = NULL;        /*存放商品分类信息代码表的数据缓冲区*/ char *gp_item_code = NULL;        /*存放商品基本信息代码表的数据缓冲区*/ char *gp_sale_code = NULL;        /*存放商品销售信息代码表的数据缓冲区*/ char gc_sys_state = '\0';   /*用来保存系统状态的字符*/  unsigned long gul_type_code_len = 0;         /*商品分类信息代码表长度*/ unsigned long gul_item_code_len = 0;         /*商品基本信息代码表长度*/ unsigned long gul_sale_code_len = 0;         /*商品销售信息代码表长度*/  HANDLE gh_std_out;          /*标准输出设备句柄*/ HANDLE gh_std_in;           /*标准输入设备句柄*/  int CreatList(TYPE_NODE **pphead);
void InitInterface(void);                 /*系统界面初始化*/ void ClearScreen(void);                         /*清屏*/ void ShowMenu(void);                            /*显示菜单栏*/ void PopMenu(int num);                          /*显示下拉菜单*/ void PopPrompt(int num);                        /*显示弹出窗口*/ void PopUp(SMALL_RECT *, WORD, LABEL_BUNDLE *, HOT_AREA *);  /*弹出窗口屏幕信息维护*/


void PopOff(void);                              /*关闭顶层弹出窗口*/
void DrawBox(SMALL_RECT *parea);                /*绘制边框*/ void LocSubMenu(int num, SMALL_RECT *parea);    /*主菜单下拉菜单定位*/ void ShowState(void);                           /*显示状态栏*/ void TagMainMenu(int num);                      /*标记被选中的主菜单项*/ void TagSubMenu(int num);                       /*标记被选中的子菜单项*/ int DealConInput(HOT_AREA *phot_area, int *pihot_num);  /*控制台输入处理*/ void SetHotPoint(HOT_AREA *phot_area, int hot_num);     /*设置热区*/ void RunSys(TYPE_NODE **pphd);                  /*系统功能模块的选择和运行*/ BOOL ExeFunction(int main_menu_num, int sub_menu_num);  /*功能模块的调用*/ void CloseSys(TYPE_NODE *phd);                  /*退出系统*/ BOOL ShowModule(char **pString, int n);
BOOL LoadData(void);           /*数据加载*/ BOOL SaveData(void);           /*保存数据*/ BOOL BackupData(void);         /*备份数据*/ BOOL RestoreData(void);        /*恢复数据*/ BOOL ExitSys(void);            /*退出系统*/ BOOL HelpTopic(void);          /*帮助主体*/ BOOL AboutMarket(void);        /*关于系统*/  BOOL MaintainTypeInfo(void);         /*维护商品分类信息*/ BOOL MaintainItemInfo(void);         /*维护商品基本信息*/ BOOL MaintainSaleInfo(void);         /*维护商品销售信息*/  BOOL QueryTypeInfo(void);    /*查询商品分类信息*/ BOOL QueryItemInfo(void);    /*查询商品基本信息*/ BOOL QuerySaleInfo(void);    /*查询商品销售信息*/  BOOL StatType(void);      /*统计各类商品销售情况*/ BOOL StatItem(void);      /*统计各种年度销售情况*/ BOOL StatClient(void);    /*统计客户消费情况*/


void InsertTypeNode(TYPE_NODE* hd);  /*插入分类信息结点*/ void DelTypeNode(TYPE_NODE *hd);     /*删除分类信息结点*/ void ModifTypeNode(TYPE_NODE *hd);   /*修改分类信息结点*/ void InsertItemNode(TYPE_NODE* head);/*插入基本信息结点*/ void DelItemNode(TYPE_NODE *hd);   /*删除基本信息结点*/ void ModifItemNode(TYPE_NODE *hd); /*修改基本信息结点*/ void InsertSaleNode(TYPE_NODE *hd);/*插入销售信息结点*/ void DelSaleNode(TYPE_NODE *hd);   /*删除销售信息结点*/ void ModifSaleNode(TYPE_NODE *hd); /*修改销售信息结点*/  void SeekType_id(TYPE_NODE *head);  /*根据分类编码查找分类信息*/ void SeekType_All(TYPE_NODE *head);  /*输出所有分类信息*/ void SeekName(TYPE_NODE *head);     /*根据商品名称中文字符子串为条件查找基本信息*/ void SeekId_Price(TYPE_NODE *head);  /*以分类码和单价为条件查找基本信息*/ void SeekItem_All(TYPE_NODE *head);  /*输出所有基本信息*/ void SeekSaleName(TYPE_NODE *head); /*以商品名称为条件查找销售信息*/ void SeekClient_Date(TYPE_NODE *head); /*以客户名称和销售日期为条件查找销售信息*/ void SeekSale_All(TYPE_NODE *head);  /*输出所有销售信息*/  void StatTypeSale(void);
void StatTypeClient(void);
void StatItemYear(void);
void StatItemClient(void);
void StatCType(void);
void StatCItem(void);
TYPE_STAT *StatTypeInfo(TYPE_NODE *head, char year[5]);                      /*按类别统计年度销售情况*/ TYPE_STAT *StatT_ClientInfo(TYPE_NODE *head, char client[20]); /*按类别统计客户消费情况*/ ITEM_STAT *StatYearInfo(TYPE_NODE *head,char year[5]);         /*按品种统计年度各


销售情况*/ ITEM_STAT *StatI_ClientInfo(TYPE_NODE *head, char client[20]); /*按品种统计客户消费情况*/ CLIENT_STAT *StatCTypeInfo(TYPE_NODE *pt);    /*按客户名称统计类别消费情况*/ CLIENT_STAT *StatCItemInfo(ITEM_NODE *pi);    /*按客户名称统计各品种消费情况*/ void StatClientSort(CLIENT_STAT *head, int len); /*将按客户姓名统计的链表按销售总重排序*/ void StatTypeSort(TYPE_STAT *head, int len);  /*将按分类名称统计的链表按销售总重排序*/ void StatItemSort(ITEM_STAT *head, int len);  /*将按商品名称统计的链表按销售总额排序*/  BOOL SaveSysData(TYPE_NODE *hd);                       /*保存系统数据*/ BOOL BackupSysData(TYPE_NODE *hd, char *filename);     /*备份系统数据*/ BOOL RestoreSysData(TYPE_NODE **pphd, char *filename);  /*恢复系统数据*/ int MyModule4(char **pString, int n);
int MyModule3(char **pString, int n);
void SaveMaintain();/*使用数据维护功能时保存数据*/
#endif /**< TYPE_H_INCLUDED*/
