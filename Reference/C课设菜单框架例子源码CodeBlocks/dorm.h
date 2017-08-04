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
#define TYPE_H_INCLUDED

#define SCR_ROW 25             /*屏幕行数*/
#define SCR_COL 80             /*屏幕列数*/

/**
 *缴费信息链结点结构
 */
typedef struct charge_node {
    char stu_id[12];           /**< 学号*/
    char name[10];             /**< 姓名*/
    char date[12];             /**< 缴费日期*/
    float sum;                 /**< 缴费金额*/
    char payee[10];            /**< 收费人*/
    char notes[5];             /**< 备注*/
    struct charge_node *next;  /**< 指向下一结点的指针*/
} CHARGE_NODE;

/**
 *学生基本信息链结点结构
 */
typedef struct stu_node {
    char stu_id[12];            /**< 学号*/
    char name[10];              /**< 姓名*/
    char sex;                   /**< 性别*/
    char birthday[12];          /**< 出生日期*/
    char type[3];               /**< 学生类别*/
    char term;                  /**< 学制*/
    char enroll_date[8];        /**< 入学年月*/
    char class_id[10];          /**< 班级*/
    char dorm_id[5];            /**< 宿舍楼号*/
    char room[5];               /**< 房间号*/
    char tel[20];               /**< 联系电话*/
    struct charge_node *cnext;  /**< 指向缴费信息支链的指针*/
    struct stu_node *next;      /**< 指向下一结点的指针*/
} STU_NODE;

/**
 *宿舍楼信息链结点结构
 */
typedef struct dorm_node {
    char dorm_id[5];         /**< 宿舍楼号*/
    char name[10];           /**< 姓名*/
    char tel[20];            /**< 联系电话*/
    short room_amount;       /**< 房间数目*/
    short bed_amount;        /**< 床位数目*/
    float fee;               /**< 每床位每年住宿费*/
    struct stu_node *snext;  /**< 指向学生基本信息支链的指针*/
    struct dorm_node *next;  /**< 指向下一结点的指针*/
} DORM_NODE;

/**
 *宿舍楼入住率统计信息链结点结构
 */
typedef struct use_rate_node {
    char dorm_id[5];            	/**< 宿舍楼号*/
    unsigned short total_num;     	/**< 可住学生数*/
    unsigned short used_num;     	/**< 在住学生数*/
    unsigned short free_num;     	/**< 空床位数*/
    float use_rate;              	/**< 入住率*/
    struct use_rate_node *next;   	/**< 指向下一结点的指针*/
} USE_RATE_NODE;

/**
 *在住学生分类统计信息链结点结构
 */
typedef struct stu_type_node {
    char grade[5];               	/**< 年级*/
    unsigned short school_boy1;   	/**< 男专科生数*/
    unsigned short school_girl1;   	/**< 女专科生数*/
    unsigned short school_boy2;   	/**< 男本科生数*/
    unsigned short school_girl2;   	/**< 女本科生数*/
    unsigned short school_boy3;   	/**< 男硕士生数*/
    unsigned short school_girl3;   	/**< 女硕士生数*/
    unsigned short school_boy4;   	/**< 男博士生数*/
    unsigned short school_girl4;   	/**< 女博士生数*/
    unsigned short school_boy5;   	/**< 其他男生数*/
    unsigned short school_girl5;   	/**< 其他女生数*/
    struct stu_type_node *next;    	/**< 指向下一结点的指针*/
} STU_TYPE_NODE;

/**
 *宿舍楼年度住宿费收入统计信息链结点结构
 */
typedef struct income_node {
    char dorm_id[5];            	/**< 宿舍楼号*/
    float year1_income;          	/**< 年度1收入*/
    float year2_income;          	/**< 年度2收入*/
    float year3_income;          	/**< 年度3收入*/
    float year4_income;          	/**< 年度4收入*/
    struct income_node *next;     	/**< 指向下一结点的指针*/
} INCOME_NODE;

/**
 *在住学生欠费统计信息链结点结构
 */
typedef struct uncharge_node {
    char stu_id[12];             	/**< 学号*/
    char name[10];             	    /**< 姓名*/
    char dorm_id[5];            	/**< 宿舍楼号*/
    char room_num[5];               /**< 房间号*/
    float amount;               	/**< 欠缴金额*/
    struct uncharge_node *next;   	/**< 指向下一结点的指针*/
} UNCHARGE_NODE;

/**
 *屏幕窗口信息链结点结点结构
 */
typedef struct layer_node {
    char LayerNo;            /**< 弹出窗口层数*/
    SMALL_RECT rcArea;       /**< 弹出窗口区域坐标*/
    CHAR_INFO *pContent;     /**< 弹出窗口区域字符单元原信息存储缓冲区*/
    char *pScrAtt;           /**< 弹出窗口区域字符单元原属性值存储缓冲区*/
    struct layer_node *next; /**< 指向下一结点的指针*/
} LAYER_NODE;

/**
 *标签束结构
 */
typedef struct labe1_bundle {
    char **ppLabel;        /**< 标签字符串数组首地址*/
    COORD *pLoc;           /**< 标签定位数组首地址*/
    int num;               /**< 标签个数*/
} LABEL_BUNDLE;

/**
 *热区结构
 */
typedef struct hot_area {
    SMALL_RECT *pArea;     /**< 热区定位数组首地址*/
    char *pSort;           /**< 热区类别(按键、文本框、选项框)数组首地址*/
    char *pTag;            /**< 热区序号数组首地址*/
    int num;               /**< 热区个数*/
} HOT_AREA;

LAYER_NODE *gp_top_layer = NULL;               /*弹出窗口信息链链头*/
DORM_NODE *gp_head = NULL;                     /*主链头指针*/

char *gp_sys_name = "学生住宿信息管理系统";    /*系统名称*/
char *gp_stu_info_filename = "stu.dat";        /*学生基本信息数据文件*/
char *gp_charge_info_filename = "charge.dat";  /*住宿缴费信息数据文件*/
char *gp_dorm_info_filename = "Dorm.dat";      /*宿舍楼信息数据文件*/
char *gp_sex_code_filename = "sex.dat";        /*性别代码数据文件*/
char *gp_type_code_filename = "type.dat";      /*学生类别代码数据文件*/

char *ga_main_menu[] = {"文件(F)",             /*系统主菜单名*/
    	                "数据维护(M)",
                        "数据查询(Q)",
                        "数据统计(S)",
                        "帮助(H)"
                       };

char *ga_sub_menu[] = {"[S] 数据保存",          /*系统子菜单名*/
                       "[B] 数据备份",
                       "[R] 数据恢复",
                       "[X] 退出    Alt+X",
                       "[S] 性别代码",
                       "[T] 学生类别代码",
                       "",           /*空串用来在弹出菜单中分隔子菜单项，下同*/
                       "[D] 宿舍楼信息",
                       "[P] 学生基本信息",
                       "[C] 住宿缴费信息",
                       "[S] 性别代码",
                       "[T] 学生类别代码",
                       "",
                       "[D] 宿舍楼信息",
                       "[P] 学生基本信息",
                       "[C] 住宿缴费信息",
                       "[I] 宿舍楼入住率",
                       "[T] 在住学生分类",
                       "[C] 住宿费缴纳情况",
                       "[U] 住宿费欠缴情况",
                       "[T] 帮助主题",
                       "",
                       "[A] 关于..."
                      };

int ga_sub_menu_count[] = {4, 6, 6, 4, 3};  /*各主菜单项下子菜单的个数*/
int gi_sel_menu = 1;                        /*被选中的主菜单项号,初始为1*/
int gi_sel_sub_menu = 0;                    /*被选中的子菜单项号,初始为0,表示未选中*/

CHAR_INFO *gp_buff_menubar_info = NULL;     /*存放菜单条屏幕区字符信息的缓冲区*/
CHAR_INFO *gp_buff_stateBar_info = NULL;    /*存放状态条屏幕区字符信息的缓冲区*/

char *gp_scr_att = NULL;    /*存放屏幕上字符单元属性值的缓冲区*/
char *gp_sex_code = NULL;   /*存放性别代码表的数据缓冲区*/
char *gp_type_code = NULL;  /*存放学生类别代码表的数据缓冲区*/
char gc_sys_state = '\0';   /*用来保存系统状态的字符*/

unsigned long gul_sex_code_len = 0;    /*性别代码表长度*/
unsigned long gul_type_code_len = 0;   /*学生类别代码表长度*/

HANDLE gh_std_out;          /*标准输出设备句柄*/
HANDLE gh_std_in;           /*标准输入设备句柄*/

int LoadCode(char *filename, char **ppbuffer);  /*代码表加载*/
int CreatList(DORM_NODE **pphead);              /*数据链表初始化*/
void InitInterface(void);                 /*系统界面初始化*/
void ClearScreen(void);                         /*清屏*/
void ShowMenu(void);                            /*显示菜单栏*/
void PopMenu(int num);                          /*显示下拉菜单*/
void PopPrompt(int num);                        /*显示弹出窗口*/
void PopUp(SMALL_RECT *, WORD, LABEL_BUNDLE *, HOT_AREA *);  /*弹出窗口屏幕信息维护*/
void PopOff(void);                              /*关闭顶层弹出窗口*/
void DrawBox(SMALL_RECT *parea);                /*绘制边框*/
void LocSubMenu(int num, SMALL_RECT *parea);    /*主菜单下拉菜单定位*/
void ShowState(void);                           /*显示状态栏*/
void TagMainMenu(int num);                      /*标记被选中的主菜单项*/
void TagSubMenu(int num);                       /*标记被选中的子菜单项*/
int DealConInput(HOT_AREA *phot_area, int *pihot_num);  /*控制台输入处理*/
void SetHotPoint(HOT_AREA *phot_area, int hot_num);     /*设置热区*/
void RunSys(DORM_NODE **pphd);                  /*系统功能模块的选择和运行*/
BOOL ExeFunction(int main_menu_num, int sub_menu_num);  /*功能模块的调用*/
void CloseSys(DORM_NODE *phd);                  /*退出系统*/
BOOL ShowModule(char **pString, int n);

BOOL LoadData(void);           /*数据加载*/
BOOL SaveData(void);           /*保存数据*/
BOOL BackupData(void);         /*备份数据*/
BOOL RestoreData(void);        /*恢复数据*/
BOOL ExitSys(void);            /*退出系统*/
BOOL HelpTopic(void);          /*帮助主体*/
BOOL AboutDorm(void);          /*关于系统*/

BOOL MaintainSexCode(void);    /*维护性别代码*/
BOOL MaintainTypeCode(void);   /*维护学生类别代码*/
BOOL MaintainDormInfo(void);   /*维护宿舍楼信息*/
BOOL MaintainStuInfo(void);    /*维护学生基本信息*/
BOOL MaintainChargeInfo(void); /*维护学生缴费信息*/

BOOL QuerySexCode(void);       /*查询性别代码*/
BOOL QueryTypeCode(void);      /*查询学生类别代码*/
BOOL QueryDormInfo(void);      /*查询宿舍楼信息*/
BOOL QueryStuInfo(void);       /*查询学生基本信息*/
BOOL QueryChargeInfo(void);    /*查询缴费信息*/

BOOL StatUsedRate(void);       /*统计入住率*/
BOOL StatStuType(void);        /*学生分类统计*/
BOOL StatCharge(void);         /*学生缴费信息统计*/
BOOL StatUncharge(void);       /*学生欠费信息统计*/

BOOL InsertChargeNode(DORM_NODE *phd, CHARGE_NODE *pcharge_node);/*插入缴费信息结点*/
BOOL DelChargeNode(DORM_NODE *phd, char *stu_id, char *date);/*删除缴费信息结点*/
BOOL ModifChargeNode(DORM_NODE *phd, char *stu_id, char *date, CHARGE_NODE *pcharge_node);/*修改缴费信息结点*/
STU_NODE *SeekStuNode(DORM_NODE *phd, char *stu_id);  /*查找学生基本信息结点*/
CHARGE_NODE *SeekChargeNode(DORM_NODE *phd, char *stu_id, char *date);/*查找缴费信息结点*/
STU_NODE *SeekStuNodeM (DORM_NODE *phd, int cond_num, ...);/*模糊查询学生信息结点*/
BOOL JudgeStuNodeItem(STU_NODE *pstu_node, char *pcond);/*判断学生信息数据项是否满足查询条件*/
BOOL MatchString(char *string_item, char *pcond);/*字符串与查询条件是否满足指定的运算关系*/
BOOL MatchChar(char char_item, char *pcond);/*字符与查询条件是否满足指定的运算关系*/
UNCHARGE_NODE *StatUnchargeInfo(DORM_NODE *phd);        /*学生欠费信息统计*/
void SortUnchargeInfo(UNCHARGE_NODE *puncharge_hd);     /*欠费信息链排序*/
BOOL SaveSysData(DORM_NODE *phd);                       /*保存系统数据*/
BOOL BackupSysData(DORM_NODE *phd, char *filename);     /*备份系统数据*/
BOOL RestoreSysData(DORM_NODE **pphd, char *filename);  /*恢复系统数据*/

#endif /**< TYPE_H_INCLUDED*/
