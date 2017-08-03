#include "market.h"
unsigned long ul;
int main()
{
    COORD size = {SCR_COL, SCR_ROW};              /*窗口缓冲区大小*/      gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE); /* 获取标准输出设备句柄*/     gh_std_in = GetStdHandle(STD_INPUT_HANDLE);   /* 获取标准输入设备句柄*/      SetConsoleTitle(gp_sys_name);                 /*设置窗口标题*/     SetConsoleScreenBufferSize(gh_std_out, size); /*设置窗口缓冲区大小80*25*/


    LoadData();                   /*数据加载*/     InitInterface();          /*界面初始化*/     RunSys(&gp_head);             /*系统功能模块的选择及运行*/     CloseSys(gp_head);            /*退出系统*/      return 0;
}  /**  * 函数名称: LoadData  * 函数功能: 三类基础数据从数据文件载入到内存缓冲区和十字链表中.  * 输入参数: 无  * 输出参数: 无  * 返回值: BOOL类型, 功能函数中除了函数ExitSys的返回值可以为FALSE外,  *           其他函数的返回值必须为TRUE.  *  * 调用说明: 为了能够以统一的方式调用各功能函数, 将这些功能函数的原型设为  *           一致, 即无参数且返回值为BOOL. 返回值为FALSE时, 结束程序运行.  */ BOOL LoadData()
{
    int Re = 0;
    Re = CreatList(&gp_head);
    if (Re < 28)
    {
        /*数据加载提示信息*/         printf("\n系统基础数据不完整!\n");
    }
    printf("\n按任意键继续...\n");


    getch();
    return TRUE;
}  /**  * 函数名称: CreatList  * 函数功能: 从数据文件读取基础数据, 并存放到所创建的十字链表中.  * 输入参数: 无  * 输出参数: phead 主链头指针的地址, 用来返回所创建的十字链.  * 返回值: int型数值, 表示链表创建的情况.  *           0  空链, 无数据  *           4  已加载商品分类信息数据，无商品基本信息和商品销售信息数据  *           12 已加载商品分类信息和商品基本信息数据，无商品销售信息数据  *           28 三类基础数据都已加载  *  * 调用说明:  */ int CreatList(TYPE_NODE **phead)
{
    TYPE_NODE *hd = NULL, *pTypeNode, tmp1;
    ITEM_NODE *pItemNode, tmp2;
    SALE_NODE *pSaleNode, tmp3;
    FILE *pFile;
    int find;
    int re = 0;
    if ((pFile = fopen(gp_type_info_filename, "rb")) == NULL)
    {
        printf("商品分类信息数据文件打开失败!\n");
        return re;
    }


    printf("商品分类信息数据文件打开成功!\n");      /*从数据文件中读商品分类信息数据，存入以后进先出方式建立的主链中*/     while (fread(&tmp1, sizeof(TYPE_NODE), 1, pFile) == 1)
    {
        pTypeNode = (TYPE_NODE *)malloc(sizeof(TYPE_NODE));
        *pTypeNode = tmp1;
        pTypeNode->inext = NULL;
        pTypeNode->next = hd;
        hd = pTypeNode;
    }
    fclose(pFile);
    if (hd == NULL)
    {
        printf("商品分类信息数据文件加载失败!\n");
        return re;
    }
    printf("商品分类信息数据文件加载成功!\n");
    *phead = hd;
    re += 4;
    if ((pFile = fopen(gp_item_info_filename, "rb")) == NULL)
    {
        printf("商品基本信息数据文件打开失败!\n");
        return re;
    }
    printf("商品基本信息数据文件打开成功!\n");
    re += 8;      /*从数据文件中读取商品基本信息数据，存入主链对应结点的商品基本信息支链中*/     while (fread(&tmp2, sizeof(ITEM_NODE), 1, pFile) == 1)


    {
        /*创建结点，存放从数据文件中读出的商品基本信息*/         pItemNode = (ITEM_NODE *)malloc(sizeof(ITEM_NODE));
        *pItemNode = tmp2;
        pItemNode->snext = NULL;          /*在主链上查找该商品所属类别对应的主链结点*/         pTypeNode = hd;
        while (pTypeNode != NULL && pTypeNode->type_id != pItemNode->type_id)
        {
            pTypeNode = pTypeNode->next;
        }
        if (pTypeNode != NULL) /*如果找到，则将结点以后进先出方式插入商品基本信息支链*/
        {
            pItemNode->next = pTypeNode->inext;
            pTypeNode->inext = pItemNode;
        }
        else  /*如果未找到，则释放所创建结点的内存空间*/
        {
            free(pItemNode);
        }
    }
    fclose(pFile);
    if ((pFile = fopen(gp_sale_info_filename, "rb")) == NULL)
    {
        printf("商品销售信息数据文件打开失败!\n");
        return re;
    }
    printf("商品销售信息数据文件打开成功!\n");


    re += 16;      /*从数据文件中读取商品销售信息数据，存入基本信息支链对应结点的销售信息支链中*/     while (fread(&tmp3, sizeof(SALE_NODE), 1, pFile) == 1)
    {
        /*创建结点，存放从数据文件中读出的商品销售信息*/         pSaleNode = (SALE_NODE *)malloc(sizeof(SALE_NODE));
        *pSaleNode = tmp3;          /*查找商品基本信息支链上对应基本信息结点*/         pTypeNode = hd;
        find = 0;
        while (pTypeNode != NULL && find == 0)
        {
            pItemNode = pTypeNode->inext;
            while (pItemNode != NULL && find == 0)
            {
                if (pItemNode->item_id == pSaleNode->item_id)
                {
                    find = 1;
                    break;
                }
                pItemNode = pItemNode->next;
            }
            pTypeNode = pTypeNode->next;
        }
        if (find)  /*如果找到，则将结点以后进先出方式插入商品销售信息支链中*/
        {
            pSaleNode->next = pItemNode->snext;
            pItemNode->snext = pSaleNode;


        }
        else /*如果未找到，则释放所创建结点的内存空间*/
        {
            free(pSaleNode);
        }
    }
    fclose(pFile);
    return re;
}  /**  * 函数名称: InitInterface  * 函数功能: 初始化界面.  * 输入参数: 无  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */ void InitInterface()
{
    WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY                | BACKGROUND_BLUE;  /*黄色前景和蓝色背景*/      SetConsoleTextAttribute(gh_std_out, att);  /*设置控制台屏幕缓冲区字符属性*/      ClearScreen();  /* 清屏*/      /*创建弹出窗口信息堆栈，将初始化后的屏幕窗口当作第一层弹出窗口*/


    gp_scr_att = (char *)calloc(SCR_COL * SCR_ROW, sizeof(char));/*屏幕字符属性*/     gp_top_layer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    gp_top_layer->LayerNo = 0;      /*弹出窗口的层号为0*/     gp_top_layer->rcArea.Left = 0;  /*弹出窗口的区域为整个屏幕窗口*/     gp_top_layer->rcArea.Top = 0;
    gp_top_layer->rcArea.Right = SCR_COL - 1;
    gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
    gp_top_layer->pContent = NULL;
    gp_top_layer->pScrAtt = gp_scr_att;
    gp_top_layer->next = NULL;
    ShowMenu();     /*显示菜单栏*/     ShowState();    /*显示状态栏*/      return;
}  /**  * 函数名称: ClearScreen  * 函数功能: 清除屏幕信息.  * 输入参数: 无  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */ void ClearScreen()
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD home = {0, 0};
    unsigned long size;


    GetConsoleScreenBufferInfo(gh_std_out, &bInfo );/*取屏幕缓冲区信息*/     size =  bInfo.dwSize.X * bInfo.dwSize.Y; /*计算屏幕缓冲区字符单元数*/      /*将屏幕缓冲区所有单元的字符属性设置为当前屏幕缓冲区字符属性*/     FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &ul);      /*将屏幕缓冲区所有单元填充为空格字符*/     FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &ul);
    return;
}  /**  * 函数名称: ShowMenu  * 函数功能: 在屏幕上显示主菜单, 并设置热区, 在主菜单第一项上置选中标记.  * 输入参数: 无  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */ void ShowMenu()
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    CONSOLE_CURSOR_INFO lpCur;
    COORD size;
    COORD pos = {0, 0};
    int i, j;
    int PosA = 2, PosB;
    char ch;
    GetConsoleScreenBufferInfo(gh_std_out, &bInfo );


    size.X = bInfo.dwSize.X;
    size.Y = 1;
    SetConsoleCursorPosition(gh_std_out, pos);
    for (i=0; i < 5; i++) /*在窗口第一行第一列处输出主菜单项*/
    {
        printf("  %s  ", ga_main_menu[i]);
    }
    GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
    SetConsoleCursorInfo(gh_std_out, &lpCur);  /*隐藏光标*/      /*申请动态存储区作为存放菜单条屏幕区字符信息的缓冲区*/     gp_buff_menubar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    SMALL_RECT rcMenu = {0, 0, size.X-1, 0};      /*将窗口第一行的内容读入到存放菜单条屏幕区字符信息的缓冲区中*/     ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);      /*将这一行中英文字母置为红色，其他字符单元置为白底黑字*/     for (i=0; i<size.X; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN                                                | BACKGROUND_RED;
        ch = (char)((gp_buff_menubar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }


    /*修改后的菜单条字符信息回写到窗口的第一行*/     WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    COORD endPos = {0, 1};
    SetConsoleCursorPosition(gh_std_out, endPos);  /*将光标位置设置在第2行第1列*/      /*将菜单项置为热区，热区编号为菜单项号，热区类型为0(按钮型)*/     i = 0;
    do
    {
        PosB = PosA + strlen(ga_main_menu[i]);  /*定位第i+1号菜单项的起止位置*/         for (j=PosA; j<PosB; j++)
        {
            gp_scr_att[j] |= (i+1) << 2; /*设置菜单项所在字符单元的属性值*/
        }
        PosA = PosB + 4;            /**  * 函数名称: PopMenu  * 函数功能: 弹出指定主菜单项对应的子菜单.  * 输入参数: num 指定的主菜单项号  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */
        i++;
    }
    while (i < 5);
    TagMainMenu(gi_sel_menu);  /*在选中主菜单项上做标记，gi_sel_menu初值为1*/      return;
}


void PopMenu(int num)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh;
    int i, j, loc = 0;
    if (num != gi_sel_menu)       /*如果指定主菜单不是已选中菜单*/
    {
        if (gp_top_layer->LayerNo != 0) /*如果此前已有子菜单弹出*/
        {
            PopOff();
            gi_sel_sub_menu = 0;
        }
    }
    else if (gp_top_layer->LayerNo != 0) /*若已弹出该子菜单，则返回*/
    {
        return;
    }
    gi_sel_menu = num;    /*将选中主菜单项置为指定的主菜单项*/     TagMainMenu(gi_sel_menu); /*在选中的主菜单项上做标记*/     LocSubMenu(gi_sel_menu, &rcPop); /*计算弹出子菜单的区域位置, 存放在rcPop中*/      /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/     for (i=1; i<gi_sel_menu; i++)
    {
        loc += ga_sub_menu_count[i-1];
    }


    /*将该组子菜单项项名存入标签束结构变量*/     labels.ppLabel = ga_sub_menu + loc;   /*标签束第一个标签字符串的地址*/     labels.num = ga_sub_menu_count[gi_sel_menu-1]; /*标签束中标签字符串的个数*/     COORD aLoc[labels.num];/*定义一个坐标数组，存放每个标签字符串输出位置的坐标*/     for (i=0; i<labels.num; i++) /*确定标签字符串的输出位置，存放在坐标数组中*/
    {
        aLoc[i].X = rcPop.Left + 2;
        aLoc[i].Y = rcPop.Top + i + 1;
    }
    labels.pLoc = aLoc; /*使标签束结构变量labels的成员pLoc指向坐标数组的首元素*/     /*设置热区信息*/     areas.num = labels.num;       /*热区的个数，等于标签的个数，即子菜单的项数*/     SMALL_RECT aArea[areas.num];                    /*定义数组存放所有热区位置*/     char aSort[areas.num];                      /*定义数组存放所有热区对应类别*/     char aTag[areas.num];                         /*定义数组存放每个热区的编号*/     for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*热区定位*/         aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*热区类别都为0(按钮型)*/         aTag[i] = i + 1;           /*热区按顺序编号*/
    }
    areas.pArea = aArea;/*使热区结构变量areas的成员pArea指向热区位置数组首元素*/     areas.pSort = aSort;/*使热区结构变量areas的成员pSort指向热区类别数组首元素*/     areas.pTag = aTag;   /*使热区结构变量areas的成员pTag指向热区编号数组首元素*/      att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/     PopUp(&rcPop, att, &labels, &areas);


    DrawBox(&rcPop);  /*给弹出窗口画边框*/     pos.X = rcPop.Left + 2;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    {
        /*此循环用来在空串子菜项位置画线形成分隔，并取消此菜单项的热区属性*/         pCh = ga_sub_menu[loc+pos.Y-rcPop.Top-1];
        if (strlen(pCh)==0) /*串长为0，表明为空串*/
        {
            /*首先画横线*/             FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-3, pos, &num_written);
            for (j=rcPop.Left+2; j<rcPop.Right-1; j++)
            {
                /*取消该区域字符单元的热区属性*/                 gp_scr_att[pos.Y*SCR_COL+j] &= 3; /*按位与的结果保留了低两位*/
            }
        }
    }     /*将子菜单项的功能键设为白底红字*/     pos.X = rcPop.Left + 3;
    att =  FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    {
        if (strlen(ga_sub_menu[loc+pos.Y-rcPop.Top-1])==0)
        {
            continue;  /*跳过空串*/
        }
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &num_written);
    }
    return;


}  /**  * 函数名称: CloseSys  * 函数功能: 关闭系统.  * 输入参数: hd 主链头指针  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */ void CloseSys(TYPE_NODE *hd)
{
    TYPE_NODE *pTypeNode1 = hd, *pTypeNode2;
    ITEM_NODE *pItemNode1, *pItemNode2;
    SALE_NODE *pSaleNode1, *pSaleNode2;
    while (pTypeNode1 != NULL) /*释放十字交叉链表的动态存储区*/
    {
        pTypeNode2 = pTypeNode1->next;
        pItemNode1 = pTypeNode1->inext;
        while (pItemNode1 != NULL) /*释放商品基本信息支链的动态存储区*/
        {
            pItemNode2 = pItemNode1->next;
            pSaleNode1 = pItemNode1->snext;
            while (pSaleNode1 != NULL) /*释放商品销售信息支链的动态存储区*/
            {
                pSaleNode2 = pSaleNode1->next;
                free(pSaleNode1);
                pSaleNode1 = pSaleNode2;
            }


            free(pItemNode1);
            pItemNode1 = pItemNode2;
        }
        free(pTypeNode1);  /*释放主链结点的动态存储区*/         pTypeNode1 = pTypeNode2;
    }
    ClearScreen();        /*清屏*/      /*释放存放菜单条、状态条、商品类别代码等信息动态存储区*/     free(gp_buff_menubar_info);
    free(gp_buff_stateBar_info);      /*关闭标准输入和输出设备句柄*/     CloseHandle(gh_std_out);
    CloseHandle(gh_std_in);      /*将窗口标题栏置为运行结束*/     SetConsoleTitle("运行结束");
    return;
}  /**  * 函数名称: RunSys  * 函数功能: 运行系统, 在系统主界面下运行用户所选择的功能模块.  * 输入参数: 无  * 输出参数: phead 主链头指针的地址  * 返回值: 无  *


 * 调用说明:  */ void RunSys(TYPE_NODE **phead)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    BOOL bRet = TRUE;
    int i, loc, num;
    int cNo, cAtt;      /*cNo:字符单元层号, cAtt:字符单元属性*/     char vkc, asc;      /*vkc:虚拟键代码, asc:字符的ASCII码值*/      while (bRet)
    {
        /*从控制台输入缓冲区中读一条记录*/         ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        if (inRec.EventType == MOUSE_EVENT) /*如果记录由鼠标事件产生*/
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;  /*获取鼠标坐标位置*/             cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /*取该位置的层号*/             cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2;/*取该字符单元属性*/             if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
            {
                if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)
                {
                    PopOff();            /*关闭弹出的子菜单*/                     gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/                     PopMenu(cAtt);       /*弹出鼠标所在主菜单项对应的子菜单*/
                }
            }
            else if (cAtt > 0)               /*鼠标所在位置为弹出子菜单的菜单项字符单元*/


            {
                TagSubMenu(cAtt); /*在该子菜单项上做选中标记*/
            }
            if (inRec.Event.MouseEvent.dwButtonState                 == FROM_LEFT_1ST_BUTTON_PRESSED) /*如果按下鼠标左边第一键*/
            {
                if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
                {
                    if (cAtt > 0) /*如果该位置处于热区(主菜单项字符单元)*/
                    {
                        PopMenu(cAtt);   /*弹出鼠标所在主菜单项对应的子菜单*/
                    }                     /*如果该位置不属于主菜单项字符单元，且有子菜单弹出*/                     else if (gp_top_layer->LayerNo > 0)
                    {
                        PopOff();            /*关闭弹出的子菜单*/                         gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                    }
                }
                else /*层号不为0，表明该位置被弹出子菜单覆盖*/
                {
                    if (cAtt > 0) /*如果该位置处于热区(子菜单项字符单元)*/
                    {
                        PopOff(); /*关闭弹出的子菜单*/                         gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/                          /*执行对应功能函数:gi_sel_menu主菜单项号, cAtt子菜单项号*/                         bRet = ExeFunction(gi_sel_menu, cAtt);
                    }
                }


            }
            else if (inRec.Event.MouseEvent.dwButtonState                      == RIGHTMOST_BUTTON_PRESSED) /*如果按下鼠标右键*/
            {
                if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
                {
                    PopOff();            /*关闭弹出的子菜单*/                     gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT  /*如果记录由按键产生*/ && inRec.Event.KeyEvent.bKeyDown) /*且键被按下*/
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /*获取按键的虚拟键码*/             asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*获取按键的ASC码*/              /*系统快捷键的处理*/             if (vkc == 112) /*如果按下F1键*/
            {
                if (gp_top_layer->LayerNo != 0) /*如果当前有子菜单弹出*/
                {
                    PopOff();            /*关闭弹出的子菜单*/                     gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                }
                bRet = ExeFunction(5, 1);  /*运行帮助主题功能函数*/
            }
            else if (inRec.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
            {
                /*如果按下左或右Alt键*/                 switch (vkc)  /*判断组合键Alt+字母*/
                {


                case 88:  /*Alt+X 退出*/
                    if (gp_top_layer->LayerNo != 0)
                    {
                        PopOff();
                        gi_sel_sub_menu = 0;
                    }
                    bRet = ExeFunction(1, 4);
                    break;
                case 70:  /*Alt+F*/
                    PopMenu(1);
                    break;
                case 77: /*Alt+M*/
                    PopMenu(2);
                    break;
                case 81: /*Alt+Q*/
                    PopMenu(3);
                    break;
                case 83: /*Alt+S*/
                    PopMenu(4);
                    break;
                case 72: /*Alt+H*/
                    PopMenu(5);
                    break;
                }
            }
            else if (asc == 0) /*其他控制键的处理*/
            {
                if (gp_top_layer->LayerNo == 0) /*如果未弹出子菜单*/
                {
                    switch (vkc) /*处理方向键(左、右、下)，不响应其他控制键*/
                    {
                    case 37:


                        gi_sel_menu--;
                        if (gi_sel_menu == 0)
                        {
                            gi_sel_menu = 5;
                        }
                        TagMainMenu(gi_sel_menu);
                        break;
                    case 39:
                        gi_sel_menu++;
                        if (gi_sel_menu == 6)
                        {
                            gi_sel_menu = 1;
                        }
                        TagMainMenu(gi_sel_menu);
                        break;
                    case 40:
                        PopMenu(gi_sel_menu);
                        TagSubMenu(1);
                        break;
                    }
                }
                else  /*已弹出子菜单时*/
                {
                    for (loc=0, i=1; i<gi_sel_menu; i++)
                    {
                        loc += ga_sub_menu_count[i-1];
                    }  /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/                     switch (vkc) /*方向键(左、右、上、下)的处理*/
                    {
                    case 37:
                        gi_sel_menu--;


                        if (gi_sel_menu < 1)
                        {
                            gi_sel_menu = 5;
                        }
                        TagMainMenu(gi_sel_menu);
                        PopOff();
                        PopMenu(gi_sel_menu);
                        TagSubMenu(1);
                        break;
                    case 38:
                        num = gi_sel_sub_menu - 1;
                        if (num < 1)
                        {
                            num = ga_sub_menu_count[gi_sel_menu-1];
                        }
                        if (strlen(ga_sub_menu[loc+num-1]) == 0)
                        {
                            num--;
                        }
                        TagSubMenu(num);
                        break;
                    case 39:
                        gi_sel_menu++;
                        if (gi_sel_menu > 5)
                        {
                            gi_sel_menu = 1;
                        }
                        TagMainMenu(gi_sel_menu);
                        PopOff();
                        PopMenu(gi_sel_menu);
                        TagSubMenu(1);
                        break;


                    case 40:
                        num = gi_sel_sub_menu + 1;
                        if (num > ga_sub_menu_count[gi_sel_menu-1])
                        {
                            num = 1;
                        }
                        if (strlen(ga_sub_menu[loc+num-1]) == 0)
                        {
                            num++;
                        }
                        TagSubMenu(num);
                        break;
                    }
                }
            }
            else if ((asc-vkc == 0) || (asc-vkc == 32))
            {
                /*按下普通键*/                 if (gp_top_layer->LayerNo == 0)  /*如果未弹出子菜单*/
                {
                    switch (vkc)
                    {
                    case 70: /*f或F*/
                        PopMenu(1);
                        break;
                    case 77: /*m或M*/
                        PopMenu(2);
                        break;
                    case 81: /*q或Q*/
                        PopMenu(3);
                        break;
                    case 83: /*s或S*/
                        PopMenu(4);
                        break;


                    case 72: /*h或H*/
                        PopMenu(5);
                        break;
                    case 13: /*回车*/
                        PopMenu(gi_sel_menu);
                        TagSubMenu(1);
                        break;
                    }
                }
                else /*已弹出子菜单时的键盘输入处理*/
                {
                    if (vkc == 27) /*如果按下ESC键*/
                    {
                        PopOff();
                        gi_sel_sub_menu = 0;
                    }
                    else if(vkc == 13) /*如果按下回车键*/
                    {
                        num = gi_sel_sub_menu;
                        PopOff();
                        gi_sel_sub_menu = 0;
                        bRet = ExeFunction(gi_sel_menu, num);
                    }
                    else /*其他普通键的处理*/
                    {
                        /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/                         for (loc=0, i=1; i<gi_sel_menu; i++)
                        {
                            loc += ga_sub_menu_count[i-1];
                        }


                        /*依次与当前子菜单中每一项的代表字符进行比较*/                         for (i=loc; i<loc+ga_sub_menu_count[gi_sel_menu-1]; i++)
                        {
                            if (strlen(ga_sub_menu[i])>0 && vkc==ga_sub_menu[i][1])
                            {
                                /*如果匹配成功*/                                 PopOff();
                                gi_sel_sub_menu = 0;
                                bRet = ExeFunction(gi_sel_menu, i-loc+1);
                            }
                        }
                    }
                }
            }
        }
    }
}  /**  * 函数名称: PopUp  * 函数功能: 在指定区域输出弹出窗口信息, 同时设置热区, 将弹出窗口位置信息入栈.  * 输入参数: pRc 弹出窗口位置数据存放的地址  *           att 弹出窗口区域字符属性  *           pLabel 弹出窗口中标签束信息存放的地址              pHotArea 弹出窗口中热区信息存放的地址  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */ void PopUp(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea)
{


    LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pCh;
    int i, j, row;      /*弹出窗口所在位置字符单元信息入栈*/     size.X = pRc->Right - pRc->Left + 1;    /*弹出窗口的宽度*/     size.Y = pRc->Bottom - pRc->Top + 1;    /*弹出窗口的高度*/     /*申请存放弹出窗口相关信息的动态存储区*/     nextLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    nextLayer->next = gp_top_layer;
    nextLayer->LayerNo = gp_top_layer->LayerNo + 1;
    nextLayer->rcArea = *pRc;
    nextLayer->pContent = (CHAR_INFO *)malloc(size.X*size.Y*sizeof(CHAR_INFO));
    nextLayer->pScrAtt = (char *)malloc(size.X*size.Y*sizeof(char));
    pCh = nextLayer->pScrAtt;     /*将弹出窗口覆盖区域的字符信息保存，用于在关闭弹出窗口时恢复原样*/     ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {
        /*此二重循环将所覆盖字符单元的原先属性值存入动态存储区，便于以后恢复*/         for (j=pRc->Left; j<=pRc->Right; j++)
        {
            *pCh = gp_scr_att[i*SCR_COL+j];
            pCh++;
        }
    }
    gp_top_layer = nextLayer;  /*完成弹出窗口相关信息入栈操作*/     /*设置弹出窗口区域字符的新属性*/     pos.X = pRc->Left;
    pos.Y = pRc->Top;


    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {
        FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);
        pos.Y++;
    }      /*将标签束中的标签字符串在设定的位置输出*/     for (i=0; i<pLabel->num; i++)
    {
        pCh = pLabel->ppLabel[i];
        if (strlen(pCh) != 0)
        {
            WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh),                                         pLabel->pLoc[i], &ul);
        }
    }      /*设置弹出窗口区域字符单元的新属性*/     for (i=pRc->Top; i<=pRc->Bottom; i++)
    {
        /*此二重循环设置字符单元的层号*/         for (j=pRc->Left; j<=pRc->Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = gp_top_layer->LayerNo;
        }
    }
    for (i=0; i<pHotArea->num; i++)
    {
        /*此二重循环设置所有热区中字符单元的热区类型和热区编号*/         row = pHotArea->pArea[i].Top;
        for (j=pHotArea->pArea[i].Left; j<=pHotArea->pArea[i].Right; j++)
        {
            gp_scr_att[row*SCR_COL+j] |= (pHotArea->pSort[i] << 6)


                                         | (pHotArea->pTag[i] << 2);
        }
    }
    return;
}  /**  * 函数名称: PopOff  * 函数功能: 关闭顶层弹出窗口, 恢复覆盖区域原外观和字符单元原属性.  * 输入参数: 无  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */ void PopOff(void)
{
    LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pCh;
    int i, j;
    if ((gp_top_layer->next==NULL) || (gp_top_layer->pContent==NULL))
    {
        /*栈底存放的主界面屏幕信息，不用关闭*/         return;
    }
    nextLayer = gp_top_layer->next;     /*恢复弹出窗口区域原外观*/     size.X = gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1;


    size.Y = gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1;
    WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));     /*恢复字符单元原属性*/     pCh = gp_top_layer->pScrAtt;
    for (i=gp_top_layer->rcArea.Top; i<=gp_top_layer->rcArea.Bottom; i++)
    {
        for (j=gp_top_layer->rcArea.Left; j<=gp_top_layer->rcArea.Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = *pCh;                /**  * 函数名称: DrawBox  * 函数功能: 在指定区域画边框.  * 输入参数: pRc 存放区域位置信息的地址  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */
            pCh++;
        }
    }
    free(gp_top_layer->pContent);    /*释放动态存储区*/     free(gp_top_layer->pScrAtt);
    free(gp_top_layer);
    gp_top_layer = nextLayer;
    gi_sel_sub_menu = 0;
    return;
}


void DrawBox(SMALL_RECT *pRc)
{
    char chBox[] = {'+', '-', '|'};  /*画框用的字符*/     COORD pos = {pRc->Left, pRc->Top};  /*定位在区域的左上角*/      WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框左上角*/     for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {
        /*此循环画上边框横线*/         WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框右上角*/     for (pos.Y = pRc->Top+1; pos.Y < pRc->Bottom; pos.Y++)
    {
        /*此循环画边框左边线和右边线*/         pos.X = pRc->Left;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
        pos.X = pRc->Right;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
    }
    pos.X = pRc->Left;
    pos.Y = pRc->Bottom;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框左下角*/     for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {
        /*画下边框横线*/         WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框右下角*/


    return;
}  /**  * 函数名称: LocSubMenu  * 函数功能: 计算弹出子菜单区域左上角和右下角的位置.  * 输入参数: num 选中的主菜单项号  * 输出参数: rc 存放区域位置信息的地址  * 返回值: 无  *  * 调用说明:  */ void LocSubMenu(int num, SMALL_RECT *rc)
{
    int i, len, loc = 0;
    rc->Top = 1; /*区域的上边定在第2行，行号为1*/     rc->Left = 1;
    for (i=1; i<num; i++)
    {
        /*计算区域左边界位置, 同时计算第一个子菜单项在子菜单字符串数组中的位置*/         rc->Left += strlen(ga_main_menu[i-1]) + 4;
        loc += ga_sub_menu_count[i-1];
    }
    rc->Right = strlen(ga_sub_menu[loc]);/*暂时存放第一个子菜单项字符串长度*/     for (i=1; i<ga_sub_menu_count[num-1]; i++)
    {
        /*查找最长子菜单字符串，将其长度存放在rc->Right*/         len = strlen(ga_sub_menu[loc+i]);
        if (rc->Right < len)
        {
            rc->Right = len;
        }
    }


    rc->Right += rc->Left + 3;  /*计算区域的右边界*/     rc->Bottom = rc->Top + ga_sub_menu_count[num-1] + 1;/*计算区域下边的行号*/     if (rc->Right >= SCR_COL)  /*右边界越界的处理*/
    {
        len = rc->Right - SCR_COL + 1;
        rc->Left -= len;
        rc->Right = SCR_COL - 1;
    }
    return;
}  /**  * 函数名称: ShowState  * 函数功能: 显示状态条.  * 输入参数: 无  * 输出参数: 无  * 返回值: 无  *  * 调用说明: 状态条字符属性为白底黑字, 初始状态无状态信息.  */ void ShowState()
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size;
    COORD pos = {0, 0};
    int i;
    GetConsoleScreenBufferInfo(gh_std_out, &bInfo );
    size.X = bInfo.dwSize.X;
    size.Y = 1;
    SMALL_RECT rcMenu = {0, bInfo.dwSize.Y-1, size.X-1, bInfo.dwSize.Y-1};


    if (gp_buff_stateBar_info == NULL)
    {
        gp_buff_stateBar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
        ReadConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);
    }
    for (i=0; i<size.X; i++)
    {
        (gp_buff_stateBar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN                                                 | BACKGROUND_RED;
    }
    WriteConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);
    return;
}  /**  * 函数名称: TagMainMenu  * 函数功能: 在指定主菜单项上置选中标志.  * 输入参数: num 选中的主菜单项号  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */ void TagMainMenu(int num)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size;


    COORD pos = {0, 0};
    int PosA = 2, PosB;
    char ch;
    int i;
    if (num == 0) /*num为0时，将会去除主菜单项选中标记*/
    {
        PosA = 0;
        PosB = 0;
    }
    else  /*否则，定位选中主菜单项的起止位置: PosA为起始位置, PosB为截止位置*/
    {
        for (i=1; i<num; i++)
        {
            PosA += strlen(ga_main_menu[i-1]) + 4;
        }
        PosB = PosA + strlen(ga_main_menu[num-1]);
    }
    GetConsoleScreenBufferInfo(gh_std_out, &bInfo );
    size.X = bInfo.dwSize.X;
    size.Y = 1;      /*去除选中菜单项前面的菜单项选中标记*/     for (i=0; i<PosA; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN                                                | BACKGROUND_RED;
        ch = (gp_buff_menubar_info+i)->Char.AsciiChar;
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {


            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }      /*在选中菜单项上做标记，黑底白字*/     for (i=PosA; i<PosB; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN                                                | FOREGROUND_RED;
    }      /*去除选中菜单项后面的菜单项选中标记*/     for (i=PosB; i<bInfo.dwSize.X; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN                                                | BACKGROUND_RED;
        ch = (char)((gp_buff_menubar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }      /*将做好标记的菜单条信息写到窗口第一行*/     SMALL_RECT rcMenu = {0, 0, size.X-1, 0};
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    return;
}


/**  * 函数名称: TagSubMenu  * 函数功能: 在指定子菜单项上做选中标记.  * 输入参数: num 选中的子菜单项号  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */ void TagSubMenu(int num)
{
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int width;
    LocSubMenu(gi_sel_menu, &rcPop);  /*计算弹出子菜单的区域位置, 存放在rcPop中*/     if ((num<1) || (num == gi_sel_sub_menu) || (num>rcPop.Bottom-rcPop.Top-1))
    {
        /*如果子菜单项号越界，或该项子菜单已被选中，则返回*/         return;
    }
    pos.X = rcPop.Left + 2;
    width = rcPop.Right - rcPop.Left - 3;
    if (gi_sel_sub_menu != 0) /*首先取消原选中子菜单项上的标记*/
    {
        pos.Y = rcPop.Top + gi_sel_sub_menu;
        att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/         FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
        pos.X += 1;
        att |=  FOREGROUND_RED;/*白底红字*/


        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }     /*在制定子菜单项上做选中标记*/     pos.X = rcPop.Left + 2;
    pos.Y = rcPop.Top + num;
    att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/     FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
    gi_sel_sub_menu = num;  /*修改选中子菜单项号*/     return;
}  /**  * 函数名称: DealInput  * 函数功能: 在弹出窗口区域设置热区, 等待并相应用户输入.  * 输入参数: pHotArea  *           piHot 焦点热区编号的存放地址, 即指向焦点热区编号的指针  * 输出参数: piHot 用鼠标单击、按回车或空格时返回当前热区编号  * 返回值:  *  * 调用说明: */ int DealInput(HOT_AREA *pHotArea, int *piHot)
{
    INPUT_RECORD inRec;
    CONSOLE_CURSOR_INFO lpCur;
    DWORD res;
    COORD pos = {0, 0};
    int iRet = 0;
    int cNo, cTag, cSort;/*cNo:层号, cTag:热区编号, cSort: 热区类型*/     char vkc;       /*vkc:虚拟键代码*/


    SetHotPoint(pHotArea, *piHot);
    while (TRUE)
    {
        /*循环*/         ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        if ((inRec.EventType == MOUSE_EVENT) &&             (inRec.Event.MouseEvent.dwButtonState              == FROM_LEFT_1ST_BUTTON_PRESSED))
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;    /* 获取光标位置 */             cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;                /*如果是键盘输入*/
            cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;
            cSort = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 6) & 3;    /* 获取该处页面属性 */              if ((cNo == gp_top_layer->LayerNo) && cTag > 0)
            {
                /* 如果点在热区上 */                 *piHot = cTag;
                SetHotPoint(pHotArea, *piHot);
                if (cSort == 0)
                {
                    iRet = cTag;
                    return iRet;
                }
            }
            else if(cTag == 0)
            {
                /* 如果点在非热点区域 */                 GetConsoleCursorInfo(gh_std_out, &lpCur);
                lpCur.bVisible = FALSE;
                SetConsoleCursorInfo(gh_std_out, &lpCur);    /* 隐藏光标 */
            }
        }


        else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode;
            switch(vkc)
            {
            case 37 :/* 处理上、左键 */
            case 38 :
                -- *piHot;
                if(*piHot < 1)
                {
                    *piHot = pHotArea->num;
                }
                SetHotPoint(pHotArea, *piHot);
                GetConsoleCursorInfo(gh_std_out, &lpCur);
                break;
            case 13 :/*回车键或空格表示按下当前按钮*/
            case 32 :
                if(pHotArea->pSort[*piHot-1] == 0)                  //若为按钮                     GetConsoleCursorInfo(gh_std_out, &lpCur);                     lpCur.bVisible = FALSE;                     SetConsoleCursorInfo(gh_std_out, &lpCur);    /* 隐藏光标 */                     iRet = *piHot;                     return iRet;                 }             case 39 :/* 处理右、下键 */             case 40 :                 ++ *piHot;                 if(*piHot > pHotArea->num)                 {
                {


                    *piHot = 1;
                }
                SetHotPoint(pHotArea, *piHot);
                GetConsoleCursorInfo(gh_std_out, &lpCur);
                break;
            case 27 :/* 按ESC键 */
                GetConsoleCursorInfo(gh_std_out, &lpCur);
                lpCur.bVisible = FALSE;
                SetConsoleCursorInfo(gh_std_out, &lpCur);    /* 隐藏光标 */                 iRet = 27;
                return iRet;
            }
        }
    }
    return iRet;
}
void SetHotPoint(HOT_AREA *pHotArea, int iHot)
{
    CONSOLE_CURSOR_INFO lpCur;
    COORD pos = {0, 0};
    WORD att1, att2;
    int i, width;
    att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/     att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/     for (i=0; i<pHotArea->num; i++)
    {
        /*将按钮类热区置为白底黑字*/         pos.X = pHotArea->pArea[i].Left;
        pos.Y = pHotArea->pArea[i].Top;


        width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
        if (pHotArea->pSort[i] == 0)
        {
            /*热区是按钮类*/             FillConsoleOutputAttribute(gh_std_out, att2, width, pos, &ul);
        }
    }
    pos.X = pHotArea->pArea[iHot-1].Left;
    pos.Y = pHotArea->pArea[iHot-1].Top;
    width = pHotArea->pArea[iHot-1].Right - pHotArea->pArea[iHot-1].Left + 1;
    if (pHotArea->pSort[iHot-1] == 0)
    {
        /*被激活热区是按钮类*/         GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
        SetConsoleCursorInfo(gh_std_out, &lpCur);    /*隐藏光标*/         FillConsoleOutputAttribute(gh_std_out, att1, width, pos, &ul);
    }
    else if (pHotArea->pSort[iHot-1] == 1)
    {
        /*被激活热区是文本框类*/         SetConsoleCursorPosition(gh_std_out, pos);
        GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = TRUE;
        SetConsoleCursorInfo(gh_std_out, &lpCur);   /* 光标移至输入框起始位置 */
    }
}  /**  * 函数名称: SaveSysData  * 函数功能: 保存系统代码表和三类基础数据.  * 输入参数: hd 主链头结点指针  * 输出参数:  * 返回值: BOOL类型, 总是为TRUE


  *  * 调用说明:  */ BOOL SaveSysData(TYPE_NODE *hd)
{
    TYPE_NODE *ptype_node;
    ITEM_NODE *pitem_node;
    SALE_NODE *psale_node;
    FILE *pfout;
    pfout = fopen(gp_type_info_filename, "wb");
    for(ptype_node = hd; ptype_node!= NULL; ptype_node = ptype_node->next)
    {
        /*保存商品分类信息*/         fwrite(ptype_node, sizeof(TYPE_NODE), 1, pfout);
    }
    fclose(pfout);
    pfout = fopen(gp_item_info_filename, "wb");
    for(ptype_node = hd; ptype_node != NULL; ptype_node = ptype_node->next)
    {
        /*保存商品基本信息*/         pitem_node = ptype_node->inext;
        while(pitem_node != NULL)
        {
            fwrite(pitem_node, sizeof(ITEM_NODE), 1, pfout);
            pitem_node = pitem_node->next;
        }
    }
    fclose(pfout);


    pfout = fopen(gp_sale_info_filename, "wb");
    for(ptype_node = hd; ptype_node != NULL; ptype_node = ptype_node->next)
    {
        /*保存销售信息*/         pitem_node = ptype_node->inext;
        while(pitem_node != NULL)
        {
            psale_node = pitem_node->snext;
            while(psale_node != NULL)
            {
                fwrite(psale_node, sizeof(SALE_NODE), 1, pfout);
                psale_node = psale_node->next;
            }
            pitem_node = pitem_node->next;
        }
    }
    fclose(pfout);
    return TRUE;
}  /**  * 函数名称: BackupSysData  * 函数功能: 将系统代码表和三类基础数据备份到一个数据文件.  * 输入参数: hd 主链头结点指针  *           filename 数据文件名  * 输出参数:  * 返回值: BOOL类型, 总是为TRUE  *  * 调用说明:


  */ BOOL BackupSysData(TYPE_NODE *hd, char *filename)
{
    TYPE_NODE *ptype_node;
    ITEM_NODE *pitem_node;
    SALE_NODE *psale_node;
    unsigned long type_node_num = 0;
    unsigned long item_node_num = 0;
    unsigned long sale_node_num = 0;
    int handle;   /* 遍历十字链表，分别统计三种基础信息的记录总数 */     for(ptype_node = hd; ptype_node != NULL; ptype_node = ptype_node->next)
    {
        type_node_num++;
        pitem_node = ptype_node->inext;
        while(pitem_node != NULL)
        {
            item_node_num++;
            psale_node = pitem_node->snext;
            while(psale_node != NULL)
            {
                sale_node_num++;
                psale_node = psale_node->next;
            }
            pitem_node = pitem_node->next;
        }
    }
    if((handle = open(filename,O_WRONLY | O_BINARY)) == -1)
    {
        handle = open(filename,O_CREAT | O_BINARY,S_IWRITE);


        handle = open(filename,O_WRONLY | O_BINARY);
    }   /* 保存三类基础数据的记录总数 */     write(handle, (char *)&type_node_num, sizeof(type_node_num));
    write(handle, (char *)&item_node_num, sizeof(item_node_num));
    write(handle, (char *)&sale_node_num, sizeof(sale_node_num));
    for(ptype_node = hd; ptype_node != NULL; ptype_node = ptype_node->next)
    {
        /*保存商品分类信息*/         write(handle, (char *)ptype_node, sizeof(TYPE_NODE));
    }
    for(ptype_node = hd; ptype_node != NULL; ptype_node = ptype_node->next)
    {
        /*保存商品基本信息*/         pitem_node = ptype_node->inext;
        while(pitem_node != NULL)
        {
            write(handle, (char *)pitem_node, sizeof(ITEM_NODE));
            pitem_node = pitem_node->next;
        }
    }
    for(ptype_node = hd; ptype_node != NULL; ptype_node = ptype_node->next)
    {
        /*保存商品销售信息*/         pitem_node = ptype_node->inext;
        while(pitem_node != NULL)
        {
            psale_node = pitem_node->snext;


            while(psale_node != NULL)
            {
                write(handle, (char*)psale_node, sizeof(SALE_NODE));
                psale_node = psale_node->next;
            }
            pitem_node = pitem_node->next;
        }
    }
    close(handle);
    return TRUE;
}  /**  * 函数名称: RestoreSysData  * 函数功能: 从指定数据文件中恢复三类基础数据.  * 输入参数: phead 主链头结点指针的地址  *           filename 存放备份数据的数据文件名  * 输出参数:  * 返回值: BOOL类型, 总是为TRUE  *  * 调用说明:  */ BOOL RestoreSysData(TYPE_NODE **phead, char *filename)
{
    TYPE_NODE *hd = NULL;
    TYPE_NODE *ptype_node;
    ITEM_NODE *pitem_node;
    SALE_NODE *psale_node;
    unsigned long type_node_num = 0;
    unsigned long item_node_num = 0;


    unsigned long sale_node_num = 0;
    unsigned long ulloop;
    int handle;
    int find;
    if((handle = open(filename,O_CREAT | O_BINARY)) == -1)
    {
        handle = open(filename,O_CREAT | O_BINARY,S_IREAD);
    }     /*读取三种基本数据信息的记录数*/     read(handle, (char *)&type_node_num, sizeof(type_node_num));
    read(handle, (char *)&item_node_num, sizeof(item_node_num));
    read(handle, (char *)&sale_node_num, sizeof(sale_node_num));      /*读取商品分类信息，建立十字主链*/     for(ulloop = 1; ulloop <= type_node_num; ulloop++)
    {
        ptype_node = (TYPE_NODE *)malloc(sizeof(TYPE_NODE));
        read(handle, (char *)ptype_node, sizeof(TYPE_NODE));
        ptype_node->inext = NULL;
        ptype_node->next = hd;
        hd = ptype_node;
    }     *phead = hd;      /*读取商品基本信息，建立商品信息支链*/     for(ulloop = 1; ulloop <= item_node_num; ulloop++)
    {
        pitem_node = (ITEM_NODE *)malloc(sizeof(ITEM_NODE));
        read(handle, (char *)pitem_node, sizeof(ITEM_NODE));
        pitem_node->snext = NULL;


        ptype_node = hd;
        while(ptype_node != NULL && (ptype_node->type_id != pitem_node->type_id))
        {
            ptype_node = ptype_node->next;
        }
        if(ptype_node != NULL)
        {
            pitem_node->next = ptype_node->inext;
            ptype_node->inext = pitem_node;
        }
        else
        {
            free(pitem_node);
        }
    }      /*读取商品销售信息，建立销售信息链表*/     for(ulloop = 1; ulloop <= sale_node_num; ulloop++)
    {
        psale_node = (SALE_NODE *)malloc(sizeof(SALE_NODE));
        read(handle, (char *)psale_node, sizeof(SALE_NODE));
        ptype_node = hd;
        find = 0;
        while(ptype_node != NULL && find == 0)
        {
            pitem_node = ptype_node -> inext;
            while(pitem_node != NULL && find == 0)
            {
                if(pitem_node -> item_id == psale_node -> item_id)


                {
                    find = 1;
                    break;
                }
                pitem_node = pitem_node -> next;
            }
            ptype_node = ptype_node -> next;
        }
        if(find)
        {
            psale_node -> next = pitem_node -> snext;
            pitem_node -> snext = psale_node;
        }
        else
        {
            free(psale_node);
        }
    }
    close(handle);
    SaveSysData(hd);    /*将内存中数据保存到数据文件*/      return TRUE;
}
BOOL ShowModule(char **pString, int n)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;


    COORD pos;
    WORD att;
    int iHot = 1;
    int i, maxlen, str_len;
    for (i=0,maxlen=0; i<n; i++)
    {
        str_len = strlen(pString[i]);
        if (maxlen < str_len)
        {
            maxlen = str_len;
        }
    }
    pos.X = maxlen + 6;
    pos.Y = n + 5;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/     labels.num = n;
    labels.ppLabel = pString;
    COORD aLoc[n];
    for (i=0; i<n; i++)
    {
        aLoc[i].X = rcPop.Left + 3;
        aLoc[i].Y = rcPop.Top + 2 + i;
    }
    str_len = strlen(pString[n-1]);
    aLoc[n-1].X = rcPop.Left + 3 + (maxlen-str_len)/2;


    aLoc[n-1].Y = aLoc[n-1].Y + 2;
    labels.pLoc = aLoc;
    areas.num = 1;
    SMALL_RECT aArea[] = {{         aLoc[n-1].X, aLoc[n-1].Y,         aLoc[n-1].X + 3, aLoc[n-1].Y         }     };
    char aSort[] = {0};
    char aTag[] = {1};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 2 + n;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
    DealInput(&areas, &iHot);
    PopOff();
    return bRet;
}  /**  * 函数名称: ExeFunction


  * 函数功能: 执行由主菜单号和子菜单号确定的功能函数.  * 输入参数: m 主菜单项号  *           s 子菜单项号  * 输出参数: 无  * 返回值: BOOL类型, TRUE 或 FALSE  *  * 调用说明: 仅在执行函数ExitSys时, 才可能返回FALSE, 其他情况下总是返回TRUE  */ BOOL ExeFunction(int m, int s)
{
    BOOL bRet = TRUE;     /*函数指针数组，用来存放所有功能函数的入口地址*/     BOOL (*pFunction[ga_sub_menu_count[0]+ga_sub_menu_count[1]+ga_sub_menu_count[2]+ga_sub_menu_count[3]+ga_sub_menu_count[4]])(void);
    int i, loc;      /*将功能函数入口地址存入与功能函数所在主菜单号和子菜单号对应下标的数组元素*/     pFunction[0] = SaveData;
    pFunction[1] = BackupData;
    pFunction[2] = RestoreData;
    pFunction[3] = ExitSys;
    pFunction[4] = MaintainTypeInfo;
    pFunction[5] = MaintainItemInfo;
    pFunction[6] = MaintainSaleInfo;
    pFunction[7] = QueryTypeInfo;
    pFunction[8] = QueryItemInfo;
    pFunction[9] = QuerySaleInfo;
    pFunction[10] = StatType;
    pFunction[11] = StatItem;
    pFunction[12] = StatClient;


    pFunction[13] = HelpTopic;
    pFunction[14] = NULL;
    pFunction[15] = AboutMarket;
    for (i=1,loc=0; i<m; i++)  /*根据主菜单号和子菜单号计算对应下标*/
    {
        loc += ga_sub_menu_count[i-1];
    }
    loc += s - 1;
    if (pFunction[loc] != NULL)
    {
        bRet = (*pFunction[loc])();  /*用函数指针调用所指向的功能函数*/
    }
    return bRet;
}
BOOL SaveData(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：文件",                            "子菜单项：数据保存",                            "确认"                           };
    ShowModule(plabel_name, 3);
    SaveSysData(gp_head);
    char *plabel_name1[] = {"数据保存成功！",                            "确认"                           };


    ShowModule(plabel_name1, 2);
    return bRet;
}
BOOL BackupData(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：文件",                            "子菜单项：数据备份",                            "确认"                           };
    ShowModule(plabel_name, 3);
    BackupSysData(gp_head, "Backup.dat");
    char *plabel_name1[] = {"数据备份成功！",                            "确认"                           };
    ShowModule(plabel_name1, 2);
    ClearScreen();
    ShowMenu();
    return bRet;
}
BOOL RestoreData(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;


    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"当前数据将被覆盖。继续执行？", "  确定取消"};
    int iHot = 1;
    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 7;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/     labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},                     {rcPop.Left+5, rcPop.Top+5}};
    labels.pLoc = aLoc;
    areas.num = 2;
    SMALL_RECT aArea[] = {{rcPop.Left + 7, rcPop.Top + 5,                            rcPop.Left + 10, rcPop.Top + 5},                           {rcPop.Left + 23, rcPop.Top + 5,                            rcPop.Left + 26, rcPop.Top + 5}};
    char aSort[] = {0, 0};
    char aTag[] = {1, 2};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);


    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 4;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
    if (DealInput(&areas, &iHot) == 1 && iHot == 1)
    {
        PopOff();
        RestoreSysData(&gp_head, "Backup.dat");
        char *plabel_name1[] = {"数据恢复成功！",                                 "确认"                                 };
        ShowModule(plabel_name1, 2);
    }
    else   PopOff();
    return TRUE;
}
BOOL ExitSys(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"确认退出系统吗？", "确定取消"};
    int iHot = 1;


    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 7;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/     labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},                     {rcPop.Left+5, rcPop.Top+5}};
    labels.pLoc = aLoc;
    areas.num = 2;
    SMALL_RECT aArea[] = {{rcPop.Left + 5, rcPop.Top + 5,                            rcPop.Left + 8, rcPop.Top + 5},                           {rcPop.Left + 13, rcPop.Top + 5,                            rcPop.Left + 16, rcPop.Top + 5}};
    char aSort[] = {0, 0};
    char aTag[] = {1, 2};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 4;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
    if (DealInput(&areas, &iHot) == 1 && iHot == 1)


    {
        bRet = FALSE;
        PopOff();
        SaveMaintain();
    }
    else
    {
        PopOff();
        bRet = TRUE;
    }
    return bRet;
}
BOOL MaintainTypeInfo(void)
{
    char *pString[] = {"插入商品分类信息",                    "修改商品分类信息",                    "删除商品分类信息",                    "返回"                    };
    int x = 0;
    while(x != 4)
    {
        x = MyModule4(pString, 4);/*选择要进行的操作*/      switch (x)
        {
        case 1:
            InsertTypeNode(gp_head);
            break;
        case 2:
            ModifTypeNode(gp_head);


            break;
        case 3:
            DelTypeNode(gp_head);
            break;
        case 4:
            SaveMaintain();/*退出时提示保存数据*/    break;
        }
    }
    return TRUE;
}
BOOL MaintainItemInfo(void)
{
    char *pString[] = {"插入商品基本信息",                    "修改商品基本信息",                    "删除商品基本信息",                    "返回"                    };
    int x = 0;
    while(x != 4)
    {
        x = MyModule4(pString,4);/*选择要进行的操作*/         switch (x)
        {
        case 1:
            InsertItemNode(gp_head);
            break;
        case 2:
            ModifItemNode(gp_head);
            break;


        case 3:
            DelItemNode(gp_head);
            break;
        case 4:
            SaveMaintain();/*退出时提示保存数据*/    break;
        }
    }
    return TRUE;
}
BOOL MaintainSaleInfo(void)
{
    char *pString[] = {"插入商品销售信息",                    "修改商品销售信息",                    "删除商品销售信息",                    "返回"                    };
    int x = 0;
    while(x != 4)
    {
        x = MyModule4(pString,4);/*选择要进行的操作*/         switch (x)
        {
        case 1:
            InsertSaleNode(gp_head);
            break;
        case 2:
            ModifSaleNode(gp_head);
            break;


        case 3:
            DelSaleNode(gp_head);
            break;
        case 4:
            SaveMaintain();/*退出时提示保存数据*/    break;
        }
    }
    return TRUE;
}
BOOL QueryTypeInfo(void)
{
    char *pString[] = {"按分类编码查询",                        "   输出全部",                        "返回"                       };
    int x = 0;
    while(x!=3)
    {
        x = MyModule3(pString,3);/*选择要进行的操作*/      switch (x)
        {
        case 1:
            SeekType_id(gp_head);/*按分类编码查询*/       break;
        case 2:
            SeekType_All(gp_head);/*输出全部*/                 break;
        case 3:
            break;/*返回*/


        }
    }
    return TRUE;
}
BOOL QueryItemInfo(void)
{
    char *pString[] = {"按名称中关键词查询",                    "按分类码和单价查询",                    "     输出全部",                    "返回"                    };
    int x = 0;
    while(x != 4)
    {
        x = MyModule4(pString,4);/*选择要进行的操作*/         switch (x)
        {
        case 1:
            SeekName(gp_head);/*按名称中关键词查询*/        break;
        case 2:
            SeekId_Price(gp_head);/*按分类码和单价查询*/                 break;
        case 3:
            SeekItem_All(gp_head);/*输出全部*/                 break;
        case 4:
            break;/*返回*/
        }


    }
    return TRUE;
}
BOOL QuerySaleInfo(void)
{
    char *pString[] = {"按客户名称和日期查找",                     "以商品名称为条件查找",                     "      输出全部",                        "返回"                        };
    int x = 0;
    while(x != 4)
    {
        x = MyModule4(pString, 4);
        switch (x)
        {
        case 1:
            SeekClient_Date(gp_head);/*按客户名称和日期查找*/        break;
        case 2:
            SeekSaleName(gp_head);/*以商品名称为条件查找*/                 break;
        case 3:
            SeekSale_All(gp_head);/*输出全部*/                 break;
        case 4:
            break;/*返回*/
        }
    }


    return TRUE;
}
BOOL StatType(void)
{
    int x = 0;
    char *pString[] = {"年度分类销售情况",                        "某客户购买情况",                        "返回"                        };
    while(x != 3)
    {
        x = MyModule3(pString, 3);/*选择统计对象的范围*/         switch (x)
        {
        case 1:
            StatTypeSale();/*统计所有分类总体销售情况*/    break;
        case 2:
            StatTypeClient();/*统计某客户购买情况*/    break;
        case 3:
            break;
        }
    }
    return TRUE;
}
BOOL StatItem(void)
{


    int x = 0;
    char *pString[] = {"年度销售情况",                        "某客户购买情况",                        "返回"                        };
    while(x != 3)
    {
        x = MyModule3(pString, 3);/*选择统计对象的范围*/         switch (x)
        {
        case 1:
            StatItemYear();/*统计某年销售情况*/    break;
        case 2:
            StatItemClient();/*统计某客户购买情况*/    break;
        case 3:
            break;
        }
    }
    return TRUE;
}
BOOL StatClient(void)
{
    int x = 0;
    char *pString[] = {"统计某分类的情况",                        "统计某商品的情况",                        "返回"                        };
    while(x != 3)


    {
        x = MyModule3(pString, 3);/*选择统计对象的范围*/         switch (x)
        {
        case 1:
            StatCType();/*统计某分类的情况*/    break;
        case 2:
            StatCItem();/*统计某商品的情况*/    break;
        case 3:
            break;/*返回*/
        }
    }
    return TRUE;
}
BOOL HelpTopic(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"本程序支持中文输入，鼠标和键盘操作。",                            "  按ALT+字母可以快捷选择主菜单。",                            "确认"                           };
    ShowModule(plabel_name, 3);
    return bRet;
}
BOOL AboutMarket(void)
{


    char *plabel_name[] = {"本程序由CS1510班徐心兰同学制作",                            "       学号：U201514769",                            "确认"                           };
    ShowModule(plabel_name, 3);
    return TRUE;
}  /**  * 函数名称: InsertTypeNode  * 函数功能: 在十字链表中插入一个分类信息结点.  * 输入参数: hd 主链头指针  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */ void InsertTypeNode(TYPE_NODE* hd)
{
    TYPE_NODE *p, *pt = hd;
    gotoxy(25, 4);
    printf("请输入您所要添加的商品分类的信息：");
    p = (TYPE_NODE*)malloc(sizeof(TYPE_NODE));/*新建分类信息结点*/     gotoxy(29, 6);
    printf("分类编码：\n\n\t\t\t    分类名称：");
    gotoxy(39, 6);
    scanf("%c", &p->type_id);
    gotoxy(39, 8);
    scanf("%s", p->name);
    getchar();


    if(strlen(p->name)>10)
    {
        char *notice1[] = {"输入名称过长。",                               "插入分类信息失败。",                               "确认"                              };
        ShowModule(notice1, 3);
        free(p);
        ClearScreen();
        ShowMenu();
        return;
    }
    while(pt != NULL)  /*遍历分类信息链表，检查分类名称或分类编号是否已被使用*/
    {
        if(p->type_id == pt->type_id)
        {
            char *notice2[] = {"此分类编号已被使用。",                               "插入分类信息失败。",                               "确认"                              };
            ShowModule(notice2, 3);
            ClearScreen();
            ShowMenu();
            free(p);
            return;
        }


        else if(strcmp(p->name,pt->name) == 0)
        {
            char *notice3[] = {"此分类名称已被使用。",                             "插入分类信息失败。",                             "确认"                             };
            ShowModule(notice3, 3);
            ClearScreen();
            ShowMenu();
            free(p);
            return;
        }
        pt = pt->next;
    }
    p->inext = NULL;
    p->next = hd;
    gp_head = p;/*若名称和编号都没有重复，将新建的结点作为新的主链的头结点*/     char *notice4[] = {"插入分类信息成功。",                     "确认"                     };
    ShowModule(notice4, 2);
    ClearScreen();
    ShowMenu();
    return;
}


/**  * 函数名称: DelTypeNode  * 函数功能: 从十字链表中删除指定的分类信息结点.  * 输入参数: hd 主链头指针  * 输出参数: 无  * 返回值: 无  *  * 调用说明: 根据分类名称删除信息  */ void DelTypeNode(TYPE_NODE *hd)
{
    TYPE_NODE *pt = hd, *p = NULL;
    ITEM_NODE *pi1, *pi2;
    SALE_NODE *ps1, *ps2;
    char name[15];
    printf("\n\n\t\t请输入您要删除的商品分类的名称：");
    scanf("%s", name);
    getchar();
    for(; pt!=NULL && strcmp(name, pt->name); p = pt, pt=pt->next);/*根据分类名称查找分类信息结点pt,并使p作为pt的前驱*/      if(pt == NULL)/*若无符合条件的结点，提示删除失败*/
    {
        char *notice[] = {"此分类不存在。",                         "删除分类信息失败。",                         "确认"                         };
        ShowModule(notice, 3);
        ClearScreen();


        ShowMenu();
        return;
    }
    else if(pt == gp_head)      /*判断是否为头结点*/
    {
        gp_head = pt->next;/*修改头结点*/
    }
    else
    {
        p->next = pt->next;/*使pt的前驱指向pt的后继*/
    }
    pi1 = pt->inext;
    while(pi1 != NULL)
    {
        /*释放pt对应的基础和销售信息的存储空间*/         pi2 = pi1->next;
        ps1 = pi1->snext;
        while(ps1 != NULL)
        {
            ps2 = ps1->next;
            free(ps1);
            ps1 = ps2;
        }
        free(pi1);
        pi1 = pi2;
    }
    free(pt);/*释放pt的存储空间*/     char *notice2[] = {"删除分类信息成功。",                     "确认"


                                                               };
    ShowModule(notice2,2);
    ClearScreen();
    ShowMenu();
    return;
}  /**  * 函数名称: ModifTypeNode  * 函数功能: 从十字链表中修改指定的分类信息结点.  * 输入参数: hd 主链头指针  * 输出参数: 无  * 返回值: 无  *  * 调用说明: 根据分类名称查找分类信息并作出修改  */ void ModifTypeNode(TYPE_NODE *hd)
{
    TYPE_NODE *pt = hd, *p1;
    ITEM_NODE *pi;
    char type_id, name[16];
    int x = 0;
    char *notice1[] = {"修改分类信息失败。",                     "确认"                     };
    char *notice2[] = {"修改分类信息成功。",                     "确认"                     };
    gotoxy(23, 2);


    printf("请输入您要修改的商品分类的名称：");
    scanf("%s", name);
    getchar();
    while(pt!=NULL)/*查找分类信息结点*/
    {
        if(strcmp(name, pt->name) == 0)
        {
            break;
        }
        pt=pt->next;
    }
    if(pt == NULL)/*若无符合条件的结点，提示修改失败*/
    {
        gotoxy(23, 4);
        printf("此分类不存在。");
        ShowModule(notice1, 2);
        ClearScreen();
        ShowMenu();
        return;
    }
    printf("\n\t\t\t  当前此分类信息详情如下：\n\n\t\t\t分类编码\t分类名称\n\t\t\t%-8c\t%-10s\n", pt->type_id, pt->name);
    printf("\n\t\t\t  按任意键继续。。。");
    getch();
    char *pString[] = {"修改分类编号",                     "修改分类名称",                     "返回"                     };


    while(x != 3)
    {
        ClearScreen();
        ShowMenu();
        x = MyModule3(pString,3);    /*选择修改的内容，其中x为pString数组的下标*/      switch (x)
        {
        case 1:
            printf("\n\t\t\t\t请输入新编号:");
            scanf("%c", &type_id);
            getchar();
            p1 = hd;
            while(p1 != NULL)
            {
                if(p1->type_id == type_id)
                {
                    break;
                }
                p1 =p1->next;
            }    /*检查新编号是否已被使用*/                  if (p1 != NULL)
            {
                printf("\n\t\t\t\t此分类编码已被使用。");
                getch();
                ShowModule(notice1, 2);
            }
            else
            {
                /*修改分类信息对应的基本信息中的商品编号*/                     pt->type_id = type_id;
                for(pi=pt->inext; pi!=NULL; pi->type_id=type_id, pi=pi->next);


                ShowModule(notice2, 2);
            }
            break;
        case 2:
            printf("\n\t\t\t\t请输入新名称：");
            scanf("%s", name);
            getchar();
            if(strlen(name) > 10)
            {
                printf("\n\t\t\t\t输入的名称过长");
                ShowModule(notice1, 2);
                break;
            }
            p1 = hd;
            while (p1 != NULL)
            {
                if (strcmp(p1->name, name) == 0)
                {
                    break;
                }
                p1 = p1->next;
            }  /*检查新名称是否已被使用*/                  if (p1 != NULL)
            {
                printf("\n\t\t\t\t此分类名称已被使用。");
                getch();
                ShowModule(notice1, 2);
            }


            else
            {
                strcpy(pt->name, name);
                ShowModule(notice2, 2);
            }
            break;
        case 3:
            break;
        }
    }
}  /**  * 函数名称: InsertItemNode  * 函数功能: 在十字链表中插入一个基本信息结点.  * 输入参数: hd 主链头指针  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */ void InsertItemNode(TYPE_NODE* hd)
{
    TYPE_NODE *pt = hd, *ptype = hd;
    ITEM_NODE *pi, *p;
    int ITEM_ID = 0;
    char type_name[10];
    gotoxy(26, 4);
    printf("请输入您所要添加的商品基本信息：");
    p = (ITEM_NODE*)malloc(sizeof(ITEM_NODE));
    gotoxy(30, 6);


    printf("分类名称: ");
    scanf("%s", type_name);
    getchar();
    for(pt = hd; pt != NULL && strcmp(type_name, pt->name); pt = pt->next);  /*查找基本信息对应的分类结点pt*/     if(pt == NULL)
    {
        char *notice1[] = {"此分类不存在。",                         "插入基本信息失败。",                         "确认"                         };
        ShowModule(notice1, 3);
        ClearScreen();
        ShowMenu();
        return;
    }
    p->type_id = pt->type_id;
    gotoxy(30, 8);
    pi=pt->inext;
    printf("商品名称: ");
    scanf("%s", p->name);
    getchar();
    if(strlen(p->name)>20)
    {
        char *notice2[] = {"输入名称过长。",                               "插入基本信息失败。",                               "确认"                              };


        ShowModule(notice2, 3);
        free(p);
        ClearScreen();
        ShowMenu();
        return;
    }
    gotoxy(30, 10);
    while(ptype != NULL)
    {
        pi = ptype->inext;
        while(pi != NULL)
        {
            /*遍历链表，检查名称是否重复,并找出商品中最大的商品编号*/             if(strcmp(p->name, pi->name) == 0)
            {
                char *notice3[] = {"此商品名称已被使用。",                                   "插入基本信息失败。",                                   "确认"                                   };
                ClearScreen();
                ShowMenu();
                free(p);
                ShowModule(notice3, 3);
                return;
            }
            if(ITEM_ID < pi -> item_id)
            {
                ITEM_ID = pi -> item_id;
            }


            pi = pi->next;
        }
        ptype = ptype->next;
    }
    printf("产地: \n\n\t\t\t     单价(元/斤): ");
    gotoxy(36, 10);
    scanf("%s", p->producer);
    if(strlen(p->producer)>20)
    {
        free(p);
        ClearScreen();
        ShowMenu();
        char *notice4[] = {"输入的产地名称过长。",                               "插入基本信息失败。",                               "确认"                              };
        ShowModule(notice4, 3);
        return;
    }
    gotoxy(43, 12);
    scanf("%f", &p->price);
    getchar();
    p->sale = 0;
    p->snext = NULL;
    p->next = pt->inext;
    pt->inext = p;


    p->item_id = 0;
    pt = hd;
    p->item_id = ITEM_ID + 1;
    gotoxy(30, 14);
    printf("商品编号(自动生成): %d\n", p->item_id);
    gotoxy(30,16);
    printf("按任意键继续。。。\n");
    getch();
    char *notice5[] = {"插入基本信息成功。",                     "确认"                     };
    ClearScreen();
    ShowMenu();
    ShowModule(notice5, 2);
    return;
}  /**  * 函数名称: DelItemNode  * 函数功能: 从十字链表中删除指定的基本信息结点.  * 输入参数: hd 主链头指针  * 输出参数: 无  * 返回值: 无  *  * 调用说明: 根据商品名称删除信息  */ void DelItemNode(TYPE_NODE *hd)
{
    TYPE_NODE *pt = hd;


    ITEM_NODE *pi, *pi2 = NULL;
    SALE_NODE *ps;
    char name[20];
    gotoxy(20, 4);
    printf("请输入您所要删除的基本信息的商品名称：");
    scanf("%s", name);
    getchar();
    while(pt != NULL)
    {
        /*根据商品名称查找商品基本信息结点pi,以及pi的前驱pi2*/         pi = pt->inext;
        while(pi != NULL)
        {
            if(strcmp(name, pi->name) == 0)
            {
                if(pt->inext == pi)      /*判断是否为头结点*/
                {
                    pt->inext = pi->next;
                }
                else
                {
                    pi2->next = pi->next;
                }
                ps = pi->snext;    /*释放此商品对应的销售信息结点的存储空间*/                 while(ps != NULL)
                {
                    free(ps);
                    ps = ps->next;
                }


                free(pi);
                char *notice1[] = {"删除基本信息成功。",                                 "确认"                                 };
                ShowModule(notice1,2);
                ClearScreen();
                ShowMenu();
                return;
            }
            pi2 = pi;
            pi = pi->next;
        }
        pt = pt->next;
    }    /*若没有符合条件的基本信息结点，提示删除失败*/     char *notice2[] = {"此商品名称不存在！",                         "删除基本信息失败。",                         "确认"                         };
    ShowModule(notice2, 3);
    ClearScreen();
    ShowMenu();
}  /**  * 函数名称: ModifItemNode  * 函数功能: 从十字链表中修改指定的基本信息结点.  * 输入参数: hd 主链头指针  * 输出参数: 无


  * 返回值: 无  *  * 调用说明: 根据商品名称查找基本信息并作出修改  */ void ModifItemNode(TYPE_NODE *hd)
{
    TYPE_NODE *pt = hd, *pt2;
    ITEM_NODE *pi = NULL, *pitem = NULL, *pi2;
    SALE_NODE *ps;
    int flag = 0, find = 0;
    char name[25], type_id, producer[25];
    char *notice1[] = {"修改基本信息失败。",                     "确认"                     };
    char *notice2[] = {"修改基本信息成功。",                     "确认"                     };
    gotoxy(18, 2);
    printf("请输入您所要修改的基本信息的商品名称：");
    scanf("%s", name);
    getchar();
    while(pt != NULL)
    {
        pi = pt->inext;
        while(pi != NULL)
        {
            /*根据商品名称查找基本信息结点pi,以及pi的前驱pi2*/             if(strcmp(name, pi->name) == 0)
            {
                flag = 1;
                break;
            }


            pitem = pi;
            pi = pi->next;
        }
        if(flag == 1)
        {
            break;
        }
        pt = pt->next;
    }
    if (pi == NULL)
    {
        gotoxy(30, 4);
        printf("此商品名称不存在！");
        ShowModule(notice1, 2);
        ClearScreen();
        ShowMenu();
        return;
    }
    printf("\n\t\t\t当前此商品基本信息详情如下：\n\n商品编号商品名称\t      分类编码产地\t\t    单价(元/斤)\t售出数量(斤)\n");
    printf("%-10d%-20s%-10c%-20s%-4.2f\t%.2f\n", pi->item_id, pi->name, pi->type_id, pi->producer, pi->price, pi->sale);
    printf("\n\t\t\t\t按任意键继续。。。");
    getch();
    char *pString[] = {"请选择要修改的内容",                     "商品名称",                     "分类编码",


                       "产地",                     "单价",                     "返回",
                      };
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int i, n = 6;    //标签行数     int iHot = 1;     int maxlen, str_len;     int x;     for (i=0,maxlen=0; i<n; i++)     {         str_len = strlen(pString[i]);          if (maxlen < str_len)         {             maxlen = str_len;         }     }      pos.X = maxlen + 6;     pos.Y = n + 5;     rcPop.Left = (SCR_COL - pos.X) / 2;     rcPop.Right = rcPop.Left + pos.X - 1;     rcPop.Top = (SCR_ROW - pos.Y) / 2;     rcPop.Bottom = rcPop.Top + pos.Y - 1;      att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白


    底黑字*/     labels.num = 6;
    labels.ppLabel = pString;
    COORD aLoc[6];
    aLoc[0].X = rcPop.Left + 3;
    aLoc[0].Y = rcPop.Top + 2;
    for (i=1; i<6; i++)
    {
        str_len = strlen(pString[i]);
        aLoc[i].X = rcPop.Left + 3 + (maxlen-str_len)/2;
        aLoc[i].Y = rcPop.Top + 2 + i;
    }
    aLoc[5].Y ++;
    labels.pLoc = aLoc;
    areas.num = 5;
    SMALL_RECT aArea[] = {{aLoc[1].X, aLoc[1].Y,                            aLoc[1].X + 7, aLoc[1].Y},          {aLoc[2].X, aLoc[2].Y,                            aLoc[2].X + 7, aLoc[2].Y},      {aLoc[3].X, aLoc[3].Y,                            aLoc[3].X + 3, aLoc[3].Y},      {aLoc[4].X, aLoc[4].Y,                            aLoc[4].X + 3, aLoc[4].Y},                            {aLoc[5].X, aLoc[5].Y,                            aLoc[5].X + 3, aLoc[5].Y}};
    char aSort[] = {0, 0, 0, 0, 0};
    char aTag[] = {1, 2, 3, 4, 5};
    areas.pArea = aArea;
    areas.pSort = aSort;


    areas.pTag = aTag;
    while(x != 5)
    {
        ClearScreen();
        ShowMenu();
        PopUp(&rcPop, att, &labels, &areas);
        DrawBox(&rcPop);
        pos.X = rcPop.Left + 1;
        pos.Y = rcPop.Top + 7;
        FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
        x = DealInput(&areas, &iHot);
        PopOff();
        switch (x)
        {
        case 5:
            break;
        case 4:
            gotoxy(32,6);
            printf("请输入新单价：");
            scanf("%f", &pi->price);
            getchar();
            ps = pi->snext;
            while(ps != NULL)
            {
                /*修改对应销售信息结点中的销售金额*/                     ps->sales_amount = ps->weight * pi->price;
                ps = ps->next;
            }
            ShowModule(notice2, 2);
            break;
        case 3:


            gotoxy(32,6);
            printf("请输入新产地：");
            scanf("%s", producer);
            getchar();
            if(strlen(producer)>20)
            {
                gotoxy(32,8);
                printf("输入的产地名称过长。");
                gotoxy(32,10);
                printf("按任意键继续。。。");
                getch();
                ShowModule(notice1, 2);
                break;
            }
            strcpy(pi->producer,producer);
            ShowModule(notice2, 2);
            break;
        case 2:
            gotoxy(32,6);
            printf("请输入新分类编码：");
            scanf("%c", &type_id);
            getchar();
            for(pt2 = hd; pt2 != NULL && type_id != pt2->type_id; pt2 = pt2->next);/*查找分类码对应的分类信息结点*/                  if(pt2 == NULL)
            {
                gotoxy(32,8);
                printf("此分类不存在。");
                ShowModule(notice1, 2);
            }


            else
            {
                /*将pi作为新的基本信息支链的头结点*/                     pi->type_id = type_id;
                if(pt->inext == pi)
                {
                    pt->inext = pi->next;
                }
                else
                {
                    pitem->next = pi->next;
                }
                pi->next = pt2->inext;
                pt2->inext = pi;
                pt = pt2;
                ShowModule(notice2, 2);
            }
            break;
        case 1:
            find = 0;
            gotoxy(32,6);
            printf("请输入新名称：");
            scanf("%s", name);
            getchar();
            if(strlen(name) > 20)
            {
                gotoxy(32,8);
                printf("输入的新名称过长。");
                gotoxy(32,10);


                printf("按任意键继续。。。");
                getch();
                ShowModule(notice1, 2);
                break;
            }
            pt2 = gp_head;
            while(pt2 != NULL && find == 0)
            {
                pi2 = pt2->inext;
                while(pi2 != NULL && find == 0)
                {
                    /*遍历链表，检查新名称是否已被使用*/                         if(strcmp(pi2->name, name) == 0)
                    {
                        find = 1;
                    }
                    pi2 = pi2->next;
                }
                pt2 = pt2->next;
            }
            if(find == 1)
            {
                gotoxy(32,8);
                printf("此名称已被使用。");
                ShowModule(notice1,2);
            }
            else
            {
                strcpy(pi->name, name);
                ShowModule(notice2,2);
            }


        }
    }
}  /**  * 函数名称: InsertSaleNode  * 函数功能: 在十字链表中插入一个销售信息结点.  * 输入参数: hd 主链头指针  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */ void InsertSaleNode(TYPE_NODE* hd)
{
    TYPE_NODE *pt = hd;
    ITEM_NODE *pi;
    SALE_NODE *ps, *p;
    char name[20];
    int find, SALE_ID = 0;
    gotoxy(25, 4);
    printf("请输入您要添加的商品销售信息：");
    p = (SALE_NODE*)malloc(sizeof(SALE_NODE));
    gotoxy(29, 6);
    printf("商品名称: ");
    scanf("%s", name);
    getchar();
    find = 0;
    for(pi = pt->inext; pt != NULL && find == 0; pt = pt->next)         for(pi=pt->inext; pi != NULL; pi = pi->next)
        {
            if(strcmp(pi->name, name) == 0)


            {
                find = 1;
                break;
            }
        }/*根据商品名称查找对应的基本信息结点*/      if(find == 0)
    {
        char *notice1[] = {"此商品不存在。",                         "插入销售信息失败。",                         "确认"                         };
        ShowModule(notice1, 3);
        ClearScreen();
        ShowMenu();
        return;
    }
    p->item_id = pi->item_id;
    printf("\n\t\t\t    销售重量(斤): \n\n\t\t\t    销售日期(例20120101): \n\n\t\t\t    客户名称: ");
    gotoxy(43, 8);
    scanf("%f", &p->weight);
    gotoxy(51, 10);
    scanf("%s", p->date);
    if(strlen(p->date)!=8)
    {
        /*检查输入的日期是否符合格式，拖不符合，提示添加信息失败*/         free(p);
        ClearScreen();
        ShowMenu();


        char *notice2[] = {"日期格式错误。",                         "插入销售信息失败。",                         "确认"                         };
        ShowModule(notice2, 3);
        return;
    }
    gotoxy(39, 12);
    scanf("%s", p->client_name);
    getchar();
    if(strlen(p->client_name) > 20)
    {
        free(p);
        ClearScreen();
        ShowMenu();
        char *notice3[] = {"客户名称过长。",                         "插入销售信息失败。",                         "确认"                         };
        ShowModule(notice3, 3);
        return;
    }
    p->sales_amount = p->weight * pi->price;
    pi->sale += p->weight;
    p->next = pi->snext;


    pi->snext = p;
    p->sale_id = 0;
    for(pt = hd; pt != NULL; pt = pt->next)         for(pi = pt->inext; pi != NULL; pi = pi->next)             for(ps = pi->snext; ps!= NULL; ps = ps->next)                 if(ps->sale_id > SALE_ID)
                {
                    SALE_ID = ps->sale_id;
                }/*查找最大的销售编号*/      p->sale_id = SALE_ID + 1;
    printf("\n\t\t\t    销售编号(自动生成): %d\n\t\t\t    按任意键继续。。。", p->sale_id);
    getch();
    ClearScreen();
    ShowMenu();
    char *notice4[] = {"插入销售信息成功。",                     "确认"                     };
    ShowModule(notice4, 2);
}  /**  * 函数名称: DelSaleNode  * 函数功能: 从十字链表中删除指定的基本信息结点.  * 输入参数: hd 主链头指针  * 输出参数: 无  * 返回值: 无  *  * 调用说明: 根据销售编号查找并删除信息  */ void DelSaleNode(TYPE_NODE *hd)


{
    TYPE_NODE *pt = hd;
    ITEM_NODE *pi;
    SALE_NODE *ps, *ps2;
    int id;
    int flag = 0;
    char *notice1[] = {"删除销售信息成功。",                     "确认"                     };
    char *notice2[] = {"此销售编号不存在。",                     "删除销售信息失败。",                     "确认"                     };
    gotoxy(20, 4);
    printf("请输入您所要删除的销售信息的销售编号：");
    scanf("%d", &id);
    getchar();
    while(pt != NULL && flag != 1)
    {
        /*遍历链表，查找销售信息结点*/         pi = pt->inext;
        while(pi != NULL && flag != 1)
        {
            ps = pi->snext;
            while(ps != NULL && flag != 1)
            {
                if(id == ps->sale_id)
                {
                    flag = 1;
                    if(pi->snext == ps)
                    {


                        pi->snext =ps->next;  /*判断是否为头结点*/
                    }
                    else
                    {
                        ps2->next = ps->next;
                    }
                    pi->sale = pi->sale - ps->weight;   /*修改对应的基本信息的销售重量等数据*/                     free(ps);
                    ShowModule(notice1, 2);
                    ClearScreen();
                    ShowMenu();
                    return;
                }
                ps2 = ps;
                ps = ps->next;
            }
            pi = pi->next;
        }
        pt = pt->next;
    }
    ShowModule(notice2, 3);
    ClearScreen();
    ShowMenu();
}  /**  * 函数名称: ModifSaleNode  * 函数功能: 从十字链表中修改指定的基本信息结点.  * 输入参数: hd 主链头指针  * 输出参数: 无


  * 返回值: 无  *  * 调用说明: 根据销售编号查找并修改信息  */ void ModifSaleNode(TYPE_NODE *hd)
{
    TYPE_NODE *pt = hd;
    ITEM_NODE *pi, *pi2;
    SALE_NODE *ps = NULL, *psale = NULL;
    int sale_id, item_id;
    int flag = 0, find = 0;
    float weight;
    char date[10], client[20];
    char *notice1[] = {"修改销售信息成功。",                     "确认"                     };
    char *notice2[] = {"修改销售信息失败。",                     "确认"                     };
    gotoxy(24, 3);
    printf("请输入您要修改的销售信息的销售编号：");
    scanf("%d", &sale_id);
    getchar();
    while(pt != NULL && flag != 1)
    {
        /*根据销售编码查号销售信息节点*/         pi = pt->inext;
        while(pi != NULL && flag != 1)
        {
            ps = pi->snext;
            while(ps != NULL)
            {
                if(sale_id == ps->sale_id)


                {
                    flag = 1;
                    break;
                }
                psale = ps;
                ps = ps->next;
            }
            if(flag != 1)
            {
                pi = pi->next;
            }
        }
        pt = pt->next;
    }
    if(flag == 0)
    {
        gotoxy(34,5);
        printf("此销售编号不存在！");
        gotoxy(34,7);
        printf("按任意键继续。。。");
        getch();
        ShowModule(notice2, 2);
        ClearScreen();
        ShowMenu();
        return;
    }
    printf("\n\t\t\t\t当前销售信息如下：\n");
    printf("\n商品编号销售编号销售日期销售重量(斤)\t销售金额\t客户姓名\n");
    printf("%-10d%-10d%-8s  %-10.2f\t%.2f\t        %-20s\n", ps->item_id, ps->sale_id,


           ps->date, ps->weight, ps->sales_amount, ps->client_name);
    printf("\n\t\t\t       按任意键继续。。。");
    getch();
    char *pString[] = {"请选择要修改的内容",                       "商品编号",                       "销售重量",                       "销售日期",                       "客户名称",                       "返回"};
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int i, n = 6;    //标签行数     int iHot = 1;     int maxlen, str_len;     int x;     for (i=0,maxlen=0; i<n; i++)     {         str_len = strlen(pString[i]);          if (maxlen < str_len)         {             maxlen = str_len;         }     }      pos.X = maxlen + 6;     pos.Y = n + 5;     rcPop.Left = (SCR_COL - pos.X) / 2;     rcPop.Right = rcPop.Left + pos.X - 1;


    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/     labels.num = 6;
    labels.ppLabel = pString;
    COORD aLoc[6];
    aLoc[0].X = rcPop.Left + 3;
    aLoc[0].Y = rcPop.Top + 2;
    for (i=1; i<6; i++)
    {
        str_len = strlen(pString[i]);
        aLoc[i].X = rcPop.Left + 3 + (maxlen-str_len)/2;
        aLoc[i].Y = rcPop.Top + 2 + i;
    }
    aLoc[5].Y ++;
    labels.pLoc = aLoc;
    areas.num = 5;
    SMALL_RECT aArea[] = {{aLoc[1].X, aLoc[1].Y,                            aLoc[1].X + 7, aLoc[1].Y},          {aLoc[2].X, aLoc[2].Y,                            aLoc[2].X + 7, aLoc[2].Y},      {aLoc[3].X, aLoc[3].Y,                            aLoc[3].X + 7, aLoc[3].Y},      {aLoc[4].X, aLoc[4].Y,                            aLoc[4].X + 7, aLoc[4].Y},                            {aLoc[5].X, aLoc[5].Y,                            aLoc[5].X + 3, aLoc[5].Y}};
    char aSort[] = {0, 0, 0, 0, 0};


    char aTag[] = {1, 2, 3, 4, 5};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    while(x != 5)
    {
        ClearScreen();
        ShowMenu();
        PopUp(&rcPop, att, &labels, &areas);
        DrawBox(&rcPop);
        pos.X = rcPop.Left + 1;
        pos.Y = rcPop.Top + 7;
        FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
        x = DealInput(&areas, &iHot);
        PopOff();
        gotoxy(32,6);
        switch(x)
        {
        case 1:
            printf("请输入新的商品编号：");
            scanf("%d", &item_id);
            getchar();
            pt = hd;
            find = 0;
            while(pt != NULL && find == 0)
            {
                /*根据商品编号查找基本信息节点*/                 pi2 = pt->inext;
                while(pi2 != NULL && find == 0)
                {
                    if(pi2->item_id == item_id)


                    {
                        if(pi->snext == ps)
                        {
                            pi->snext = ps->next;
                        }/*判断销售信息结点是否为支链头结点*/                         else
                        {
                            psale->next = ps->next;
                        }
                        ps->next = pi2->snext;
                        pi2->snext = ps;     /*将销售信息结点作为新的基本信息支链的头结点*/                         ps->item_id = pi2->item_id;
                        ps->sales_amount = ps->weight * pi2->price;
                        pi->sale -= ps->weight;
                        pi2->sale += ps->weight; /*修改销售信息和基本信息的销售金额与数量等数据*/                         pi = pi2;
                        ShowModule(notice1, 2);
                        find = 1;
                        break;
                    }
                    pi2 = pi2->next;
                }
                pt = pt->next;
            }
            if(find == 0)
            {
                gotoxy(26,8);
                printf("\t\t此商品编号不存在。");


                getch();
                ShowModule(notice2, 2);
            }
            break;
        case 2:
            printf("请输入新的销售重量：");
            scanf("%f", &weight);
            getchar();
            pi->sale += (weight - ps->weight);
            ps->weight = weight;
            ps->sales_amount = weight * pi->price;
            ShowModule(notice1, 2);
            break;
        case 3:
            printf("请输入新的销售日期：");
            scanf("%s", date);
            getchar();
            if(strlen(date)!=8)
            {
                /*检查输入的日期的格式，格式错误提示修改失败*/                 printf("\n\t\t\t\t日期格式错误。\n\t\t\t\t按任意键继续。。。");
                getch();
                ShowModule(notice2, 2);
                break;
            }
            strcpy(ps->date,date);
            ShowModule(notice1, 2);
            break;
        case 4:


            printf("请输入新的客户姓名：");
            scanf("%s", client);
            getchar();
            if(strlen(client) > 20)
            {
                printf("\n\t\t\t\t客户姓名过长。\n\n\t\t\t\t按任意键继续。。。");
                getch();
                ShowModule(notice2, 2);
                break;
            }
            strcpy(ps->client_name, client);
            ShowModule(notice1, 2);
            break;
        case 5:
            break;
        }
    }
}  /**  * 函数名称: SeekType_id  * 函数功能: 根据输入的分类编码查找分类信息  * 输入参数: head 主链头指针  * 输出参数: 满足条件的商品分类信息  * 返回值: 无  *  * 调用说明:  */ void SeekType_id(TYPE_NODE *head)
{
    char type_Id;


    TYPE_NODE *find;
    gotoxy(27, 6);
    printf("请输入商品分类编码:___");
    gotoxy(47, 6);
    scanf("%c", &type_Id);
    getchar();
    for(find = gp_head; find != NULL; find = find->next)         if(find->type_id == type_Id)             break;   /*遍历分类信息链表，根据分类编码查找分类信息结点*/      if(find != NULL)
    {
        printf("\n\t\t\t     分类编码\t分类名称\n");            /**  * 函数名称: SeekType_All  * 函数功能: 遍历分类信息链表，输出所有分类信息  * 输入参数: head 主链头指针  * 输出参数: 所有的商品分类信息  * 返回值: 无  *  * 调用说明:  */
        printf("\t\t\t     %-8c\t%-10s\n", find->type_id, find->name);
    }
    else printf("\n\t\t\t     此分类编码不存在！\n");
    printf("\n\t\t\t\t按任意键继续。。。");
    getch();
    ClearScreen();
    ShowMenu();
}


void SeekType_All(TYPE_NODE *head)
{
    TYPE_NODE *pt = head;
    int node_num = 0;
    printf("\n\t\t\t     分类编码\t分类名称\n");
    while(pt != NULL)
    {
        if(node_num == 15)/*每页输出15条*/
        {
            printf("\n\t\t按任意键切换至下一页。。。");
            getch();
            ClearScreen();
            ShowMenu();
            printf("\n\t\t\t     分类编码\t分类名称\n");
            node_num = 0;
        }
        printf("\t\t\t     %-8c\t%-10s\n", pt->type_id, pt->name);
        pt = pt->next;
        node_num ++;
    }
    printf("\n\t\t\t\t按任意键继续。。。");
    getch();
    ClearScreen();
    ShowMenu();
} /**  * 函数名称: SeekName  * 函数功能: 按商品名称中的关键词模糊查找商品基本信息  * 输入参数: head 主链头指针  * 输出参数: 所有满足条件的商品基本信息  * 返回值: 无


  *  * 调用说明:  */ void SeekName(TYPE_NODE *head)
{
    TYPE_NODE *pt;
    ITEM_NODE *pi;
    char name[20];
    int f = 0, node_num = 0;
    pt = head;
    gotoxy(30, 2);
    printf("请输入名称中的关键词：");
    scanf("%s", name);
    getchar();
    printf("\n分类码商品编号\t商品名称产地单价(元/斤)\t售出数量(斤)\n");
    while(pt != NULL)
    {
        pi = pt->inext;
        while(pi != NULL)
        {
            if(strstr(pi->name, name))   /*模糊查找基本信息结点*/
            {
                if(node_num == 15)/*每页输出15条*/
                {
                    printf("\n\t\t按任意键切换至下一页。。。");
                    getch();
                    ClearScreen();
                    ShowMenu();
                    printf("\n分类码商品编号\t商品名称产地单价(元/斤)\t售出数量(斤)\n");
                    node_num = 0;
                }


                printf("%-6c %-8d\t%-20s%-20s%-9.2f\t%-9.2f\n", pi->type_id, pi->item_id, pi->name, pi->producer, pi->price, pi->sale);
                f = 1;
                node_num++;
            }
            pi = pi->next;
        }
        pt = pt->next;
    }   /*遍历链表，输出符合条件的基本信息*/      if(f == 0)
    {
        printf("\n\t\t\t     无包含该名称的商品。\n");
    }
    printf("\n\t\t\t    按任意键继续。。。");
    getch();
    ClearScreen();
    ShowMenu();
}   /**  * 函数名称: SeekId_Price  * 函数功能: 按分类码和单价区间查找商品基本信息  * 输入参数: hd 主链头指针  * 输出参数: 所有满足条件的商品基本信息  * 返回值: 无  *  * 调用说明:


  */ void SeekId_Price(TYPE_NODE *head)
{
    TYPE_NODE *pt = head;
    ITEM_NODE *pi;
    char id;
    float low, high;
    int find = 0, node_num = 0;
    gotoxy(30, 2);
    printf("请输入分类编码：");
    scanf("%c", &id);
    gotoxy(30, 4);
    printf("请输入需要查询的单价区间：\n\n\t\t\t        从__________(元/斤)\n\n\t\t\t        至__________(元/斤)");
    gotoxy(36, 6);
    scanf("%f",&low);
    gotoxy(36, 8);
    scanf("%f", &high);
    getchar();
    if(low > high)
    {
        low += high;
        high = low - high;
        low -= high;
    }
    printf("\n分类码商品编号\t商品名称产地单价(元/斤)\t售出数量(斤)\n");
    for(; pt!=NULL && pt->type_id!=id; pt = pt->next);
    if(pt == NULL)


    {
        printf("\n\t\t\t     此分类编码不存在。\n");
        printf("\n\t\t\t     按任意键继续。。。");
        getch();
        ClearScreen();
        ShowMenu();
        return;
    }
    else
    {
        pi = pt->inext;
        while(pi != NULL)
        {
            if((pi->price <= high) && (pi->price >= low))
            {
                /*查找处于价格区间内的基本信息结点（包括端点）*/                 if(node_num == 15)/*每页输出15条*/
                {
                    printf("\n\t\t按任意键切换至下一页。。。");
                    getch();
                    ClearScreen();
                    ShowMenu();
                    printf("\n分类码商品编号\t商品名称产地单价(元/斤)\t售出数量(斤)\n");
                    node_num = 0;
                }
                printf("%-6c %-8d\t%-20s%-20s%-9.2f\t%-9.2f\n", pi->type_id, pi->item_id, pi->name, pi->producer, pi->price, pi->sale);
                find = 1;
                node_num++;
            }


            pi = pi->next;
        }
    }
    if(find == 0)
    {
        printf("\n\t\t\t     无符合条件的商品。\n");
    }
    printf("\n\t\t\t     按任意键继续。。。");
    getch();
    ClearScreen();
    ShowMenu();
}   /**  * 函数名称: SeekItem_All  * 函数功能: 无条件输出所有基本信息  * 输入参数: hd 主链头指针  * 输出参数: 所有商品基本信息  * 返回值: 无  *  * 调用说明:  */ void SeekItem_All(TYPE_NODE *head)
{
    TYPE_NODE *pt = head;
    ITEM_NODE *pi = NULL;
    int node_num = 0;
    printf("\n分类码商品编号\t商品名称产地单价(元/斤)\t售出数量(斤)\n");
    while(pt != NULL)


    {
        pi = pt->inext;
        while(pi != NULL)
        {
            /*遍历链表输出所有基本信息*/             if(node_num == 15)/*每页输出15条*/
            {
                printf("\n\t\t按任意键切换至下一页。。。");
                getch();
                ClearScreen();
                ShowMenu();
                printf("\n分类码商品编号\t商品名称产地单价(元/斤)\t售出数量(斤)\n");
                node_num = 0;
            }
            printf("%-6c %-8d\t%-20s%-20s%-9.2f\t%-9.2f\n", pi->type_id, pi->item_id, pi->name, pi->producer, pi->price, pi->sale);
            pi = pi->next;
            node_num++;
        }
        pt = pt->next;
    }
    printf("\n\t\t\t     按任意键继续。。。\n");
    getch();
    ClearScreen();
    ShowMenu();
}  /**  * 函数名称: SeekClient_Date  * 函数功能: 按客户名称和销售日期查找商品销售信息  * 输入参数: head 主链头指针  * 输出参数: 所有满足条件的商品销售信息


  * 返回值: 无  *  * 调用说明:  */ void SeekClient_Date(TYPE_NODE *head)
{
    TYPE_NODE *pt;
    ITEM_NODE *pi;
    SALE_NODE *ps;
    char Client[20];
    char Date[10];
    int f = 0, node_num = 0;
    pt = head;
    gotoxy(28, 2);
    printf("请输入客户名称: ");
    scanf("%s", Client);
    gotoxy(24, 4);
    printf("请输入销售日期(格式如20150101): ");
    scanf("%s", Date);
    if(strlen(Date) != 8)
    {
        /*检查输入的日期格式*/         printf("\n\t\t\t日期格式错误。\n\t\t\t按任意键继续。。。");
        getch();
        ClearScreen();
        ShowMenu();
        return;
    }
    getchar();


    printf("\n商品编号销售编号销售日期销售重量(斤)\t销售金额\t客户姓名\n");
    while(pt != NULL)
    {
        /*遍历链表，输出符合条件的销售信息*/         pi = pt->inext;
        while(pi != NULL)
        {
            ps = pi->snext;
            while(ps != NULL)
            {
                if(strcmp(Client, ps->client_name) == 0 && strcmp(Date, ps->date) == 0)
                {
                    if(node_num == 15)/*每页输出15条*/
                    {
                        printf("\n\t\t按任意键切换至下一页。。。");
                        getch();
                        ClearScreen();
                        ShowMenu();
                        printf("\n商品编号销售编号销售日期销售重量(斤)\t销售金额\t客户姓名\n");
                        node_num = 0;
                    }
                    f = 1;
                    printf("%-10d%-10d%-8s  %-10.2f\t%.2f\t        %s\n", ps->item_id, ps->sale_id, ps->date, ps->weight, ps->sales_amount, ps->client_name);
                    node_num++;
                }
                ps = ps->next;
            }
            pi = pi->next;
        }
        pt = pt->next;
    }


    if(f == 0)
    {
        printf("\n\t\t\t无符合条件的商品销售信息。\n");
    }
    printf("\n\t\t\t    按任意键继续。。。");
    getch();
    ClearScreen();
    ShowMenu();
}   /**  * 函数名称: SeekSaleName  * 函数功能: 按商品名称查找商品销售信息  * 输入参数: head 主链头指针  * 输出参数: 所有满足条件的商品销售信息  * 返回值: 无  *  * 调用说明:  */ void SeekSaleName(TYPE_NODE *head)
{
    TYPE_NODE *pt = head;
    ITEM_NODE *pi;
    SALE_NODE *ps;
    char name[20];
    int f = 0, findsale = 0, node_num = 0;
    gotoxy(30, 2);
    printf("请输入商品名称：");
    scanf("%s", name);
    getchar();


    printf("\n商品编号销售编号销售日期销售重量(斤)\t销售金额\t客户姓名\n");
    while(pt != NULL && f == 0)
    {
        pi = pt->inext;
        while(pi != NULL && f == 0)
        {
            /*遍历链表，输出符合条件的销售信息*/             if(strcmp(pi->name, name) == 0)
            {
                f = 1;   /*若查找到对应的基本信息结点，f标记为1*/                 ps = pi->snext;
                while(ps!=NULL)
                {
                    if(node_num == 15)/*每页输出15条*/
                    {
                        printf("\n\t\t按任意键切换至下一页。。。");
                        getch();
                        ClearScreen();
                        ShowMenu();
                        printf("\n商品编号销售编号销售日期销售重量(斤)\t销售金额\t客户姓名\n");
                        node_num = 0;
                    }
                    findsale = 1; /*若有输出销售信息，findsale标记为1*/                     printf("%-10d%-10d%-8s  %-10.2f\t%.2f\t        %-20s\n", ps->item_id, ps->sale_id, ps->date, ps->weight, ps->sales_amount, ps->client_name);
                    ps = ps->next;
                    node_num++;
                }
            }
            pi = pi->next;


        }
        pt = pt->next;
    }
    if(f == 0)
    {
        printf("\n\t\t\t\t商品名称不存在。\n");
    }
    else if(findsale == 0)
    {
        printf("\n\t\t\t\t无此商品的销售信息。\n");
    }
    printf("\n\t\t\t\t按任意键继续。。。");
    getch();
    ClearScreen();
    ShowMenu();
}   /**  * 函数名称: SeekSale_ALL  * 函数功能: 输出所有商品销售信息  * 输入参数: head 主链头指针  * 输出参数: 所有商品销售信息  * 返回值: 无  *  * 调用说明:  */  void SeekSale_All(TYPE_NODE *head)
{


    TYPE_NODE *pt = head;
    ITEM_NODE *pi = NULL;
    SALE_NODE *ps = NULL;
    int node_num = 0;
    printf("\n商品编号销售编号销售日期销售重量(斤)\t销售金额\t客户姓名\n");
    while(pt != NULL)
    {
        pi = pt->inext;
        while(pi != NULL)
        {
            ps = pi->snext;
            while(ps != NULL)
            {
                if(node_num == 15)/*每页输出15条*/
                {
                    printf("\n\t\t按任意键切换至下一页。。。");
                    getch();
                    ClearScreen();
                    ShowMenu();
                    printf("\n商品编号销售编号销售日期销售重量(斤)\t销售金额\t客户姓名\n");
                    node_num = 0;
                }
                printf("%-10d%-10d%-8s  %-10.2f\t%.2f\t        %-20s\n", ps->item_id, ps->sale_id, ps->date, ps->weight, ps->sales_amount, ps->client_name);
                ps = ps->next;
                node_num++;
            }
            pi = pi->next;
        }


        pt = pt->next;
    }
    printf("\n\t\t\t     按任意键继续。。。\n");
    getch();
    ClearScreen();
    ShowMenu();
}  /**  * 函数名称: StatTypeSale  * 函数功能: 按类别统计所有类别总体的年度销售情况  * 输入参数: 无  * 输出参数: 所有满足条件的商品销售信息  * 返回值: 无  *  * 调用说明:  */ void StatTypeSale(void)
{
    TYPE_STAT *head;
    int node_num = 0;
    char year[5];
    printf("\n\t\t     请输入需要查询的年份（例2015）：");
    scanf("%s",year);
    getchar();
    if(strlen(year) != 4)
    {
        printf("\n\t\t     年份格式输入错误。\n\n\t\t     按任意键继续。。。");
        getch();
        ClearScreen();
        ShowMenu();


        return;
    }
    head = StatTypeInfo(gp_head, year);  /*调用函数StatTypeInfo,生成分类信息统计链表*/     printf("\n\t\t分类名称  \t销售总重量(斤)\t销售总额\n");
    if(head == NULL)
    {
        printf("\n\t\t     无分类信息。");
    }
    while(head !=NULL)
    {
        if(node_num == 15)/*每页输出15条*/
        {
            printf("\n\t\t按任意键切换至下一页。。。");
            getch();
            ClearScreen();
            ShowMenu();
            printf("\n\t\t分类名称  \t销售总重量(斤)\t销售总额\n");
            node_num = 0;
        }
        printf("\t\t%-10s\t%-14.2f\t%-8.2f\n", head->type, head->weight, head->sale);
        head = head->next;
        node_num++;
    }
    printf("\n\t\t     按任意键继续。。。");
    getch();
    ClearScreen();
    ShowMenu();
}


/**  * 函数名称: StatTypeClient  * 函数功能: 按类别统计指定客户消费情况  * 输入参数: 无  * 输出参数: 所有满足条件的商品销售信息  * 返回值: 无  *  * 调用说明:  */ void StatTypeClient(void)
{
    char client[20];
    TYPE_STAT *head;
    int node_num = 0;
    printf("\n\t\t    请输入需要查询的客户的姓名：");
    scanf("%s", client);   /*根据获取的客户姓名确定统计的范围*/     getchar();
    head = StatT_ClientInfo(gp_head, client); /*调用函数StatT_ClientInfo，生成客户统计结果链表*/     ClearScreen();
    ShowMenu();
    printf("\n\t\t\t%s的消费情况如下表：\n",client);
    if(head == NULL)
    {
        printf("\n\t\t\t    无消费信息。\n");
    }
    else
    {
        printf("\n\t\t分类名称  \t销售总重量(斤)\t销售总额\n");


        while(head !=NULL)
        {
            /*遍历链表，输出统计结果*/             if(node_num == 15)/*每页输出15条*/
            {
                printf("\n\t\t按任意键切换至下一页。。。");
                getch();
                ClearScreen();
                ShowMenu();
                printf("\n\t\t分类名称  \t销售总重量(斤)\t销售总额\n");
                node_num = 0;
            }
            printf("\t\t%-10s\t%-12.2f\t%-8.2f\n", head->type, head->weight, head->sale);
            head = head->next;
            node_num++;
        }
    }
    printf("\n\t\t\t    按任意键继续。。。");
    getch();
    ClearScreen();
    ShowMenu();
    return;
}   /**  * 函数名称: StatItemYear  * 函数功能: 按商品名称统计年度销售情况  * 输入参数: 无  * 输出参数: 所有满足条件的商品销售信息  * 返回值: 无  *


  * 调用说明:  */ void StatItemYear(void)
{
    ITEM_STAT * head;
    char year[5];
    int node_num = 0;
    printf("\n\t\t\t请输入需要查询的年份：");
    scanf("%s", year); /*获取年份，确定统计范围*/     getchar();
    if(strlen(year) != 4)
    {
        printf("\n\t\t\t\t年份格式错误。\n\n\t\t\t\t按任意键继续。。。");
        getch();
        ClearScreen();
        ShowMenu();
        return;
    }
    head = StatYearInfo(gp_head, year);   /*调用StatYearInfo，生成统计结果链表*/      if(head == NULL)
    {
        printf("\n\t\t\t\t无此年的销售信息。\n\n\t\t\t\t按任意键继续。。。");
        getch();
        ClearScreen();
        ShowMenu();
        return;
    }
    printf("\n\t分类名称  \t商品名称            \t总重量(斤)\t销售金额");
    while(head !=NULL)


    {
        /*遍历链表，输出统计结果*/         if(node_num == 15)/*每页输出15条*/
        {
            printf("\n\t\t按任意键切换至下一页。。。");                /**  * 函数名称: StatItemClient  * 函数功能: 按商品名称统计指定客户的消费情况  * 输入参数: 无  * 输出参数: 所有满足条件的商品销售信息  * 返回值: 无  *  * 调用说明:  */
            getch();
            ClearScreen();
            ShowMenu();
            printf("\n\t分类名称  \t商品名称            \t总重量(斤)\t销售金额");
            node_num = 0;
        }
        printf("\n\t%-10s\t%-20s\t%-10.2f\t%-10.2f", head->type, head->item, head->weight, head->sale);
        head = head->next;
        node_num++;
    }
    printf("\n\t\t\t按任意键继续。。。");
    getch();
    ClearScreen();
    ShowMenu();
}


void StatItemClient(void)
{
    char client[20];
    ITEM_STAT *head;
    int node_num = 0;
    printf("\n\t\t    请输入需要查询的客户的姓名：");
    scanf("%s", client);    /*根据获取的客户名称确定统计范围*/     getchar();
    head = StatI_ClientInfo(gp_head, client);   /*调用StatI_ClientInfo，生成统计结果链表*/     printf("\n\t\t\t%s的消费情况如下表：\n",client);
    if(head == NULL)
    {
        printf("\n\t\t\t    无消费信息。\n");
    }
    else
    {
        printf("\n\t分类名称  \t商品名称            \t销售总重量(斤)\t销售金额\n");
        while(head !=NULL)
        {
            /*遍历链表，输出统计结果*/             if(node_num == 15)/*每页输出15条*/
            {
                printf("\n\t\t按任意键切换至下一页。。。");
                getch();
                ClearScreen();
                ShowMenu();
                printf("\n\t分类名称  \t商品名称            \t销售总重量(斤)\t销售金额\n");
                node_num = 0;
            }


            printf("\t%-10s\t%-20s\t%-12.2f\t%-8.2f\n", head->type, head->item, head->weight, head->sale);
            head = head->next;
            node_num++;
        }
    }
    printf("\n\t\t\t    按任意键继续。。。");
    getch();
    ClearScreen();
    ShowMenu();
}   /**  * 函数名称: StatCType  * 函数功能: 按客户姓名统计指定类别商品消费情况  * 输入参数: 无  * 输出参数: 所有满足条件的商品销售信息  * 返回值: 无  *  * 调用说明:  */ void StatCType(void)
{
    CLIENT_STAT *head = NULL;
    TYPE_NODE *pt = gp_head;
    char type[10];
    int node_num = 0;
    printf("\n\t\t    请输入需要查询商品类别的名称：");
    scanf("%s", type);
    getchar();


    while(pt != NULL)
    {
        /*根据分类名称查找分类信息结点*/         if(strcmp(pt->name, type) == 0)             break;
        pt = pt->next;
    }
    if(pt == NULL)
    {
        printf("\n\t\t\t    商品类别不存在！\n");
    }
    else
    {
        head = StatCTypeInfo(pt);/*调用函数StatCTypeInfo，生成统计结果链表*/          if(head == NULL)
        {
            printf("\n\t\t\t    此类别销售信息不存在！\n");
        }
        else
        {
            printf("\n\t客户姓名            \t购买总重量(斤)\t消费总金额\n");
            while(head !=NULL)
            {
                /*遍历链表，输出统计结果*/                 if(node_num == 15)/*每页输出15条*/
                {
                    printf("\n\t\t按任意键切换至下一页。。。");
                    getch();
                    ClearScreen();
                    ShowMenu();
                    printf("\n\t客户姓名            \t购买总重量(斤)\t消费总金额\n");
                    node_num = 0;


                }
                printf("\t%-20s\t%-12.2f\t%-8.2f\n", head->client, head->weight, head->sale);
                head = head->next;
                node_num++;
            }
        }
    }
    printf("\n\t\t\t    按任意键继续。。。");
    getch();
    ClearScreen();
    ShowMenu();
}   /**  * 函数名称: StatCItem  * 函数功能: 按客户姓名统计指定品种的商品消费情况  * 输入参数: 无  * 输出参数: 所有满足条件的商品销售信息  * 返回值: 无  *  * 调用说明:  */ void StatCItem(void)
{
    TYPE_NODE *pt = gp_head;
    ITEM_NODE *pi = NULL;
    CLIENT_STAT *head = NULL;
    char item[20];
    int find = 0, node_num = 0;


    printf("\n\t\t    请输入需要查询商品的名称：");
    scanf("%s", item);
    getchar();
    while(pt != NULL && find == 0)
    {
        /*遍历基本信息链表，查找指定的商品信息结点*/         pi = pt->inext;
        while(pi != NULL)
        {
            if(strcmp(pi->name, item) == 0)
            {
                find = 1;
                break;
            }
            pi = pi->next;
        }
        pt = pt->next;
    }
    if(pi == NULL)
    {
        printf("\n\t\t\t    此商品不存在！\n");
    }
    else
    {
        head = StatCItemInfo(pi);  /*调用函数StatCItemInfo，生成统计结果链表*/          if(head == NULL)             printf("\n\t\t\t    此商品销售信息不存在！\n");
        else
        {
            printf("\n\t客户姓名            \t购买总重量(斤)\t消费总金额\n");
            while(head != NULL)


            {
                if(node_num == 15)/*每页输出15条*/
                {
                    printf("\n\t\t按任意键切换至下一页。。。");                        /**  * 函数名称: StatTypeInfo  * 函数功能: 按类别统计所有分类的销售情况  * 输入参数: head 主链头指针  * 输出参数: 无  * 返回值: 统计得到的销售情况链表的链头指针phead  *  * 调用说明:  */
                    getch();
                    ClearScreen();
                    ShowMenu();
                    printf("\n\t客户姓名            \t购买总重量(斤)\t消费总金额\n");
                    node_num = 0;
                }
                printf("\t%-20s\t%-12.2f\t%-8.2f\n", head->client, head->weight, head->sale);
                head = head->next;
                node_num++;
            }
        }
    }
    printf("\n\t\t\t    按任意键继续。。。");
    getch();
    ClearScreen();
    ShowMenu();
}


TYPE_STAT * StatTypeInfo(TYPE_NODE *head,char year[5])
{
    TYPE_STAT *p1, *phead;
    ITEM_NODE *pi;
    SALE_NODE *ps;
    int len = 0, i;
    phead = (TYPE_STAT *)malloc(sizeof(TYPE_STAT));/*新建头结点*/     p1 = phead;     /*建立带头指针的先进先出的统计结果链表*/     while(head != NULL)
    {
        p1->next = (TYPE_STAT *)malloc(sizeof(TYPE_STAT));/*新建结点*/         p1 = p1->next;
        strcpy(p1->type, head->name);
        p1->weight = 0;
        p1->sale = 0;
        pi = head->inext;
        while(pi != NULL) /*遍历分类信息结点对应的基本信息支链，统计销售重量和金额*/
        {
            ps = pi->snext;
            while(ps != NULL)
            {
                for(i=0; year[i] == ps->date[i] && i<=3; i++);
                if(i == 4)
                {
                    p1->weight += ps->weight;
                    p1->sale += ps->sales_amount;
                }
                ps = ps->next;
            }


            pi = pi->next;
        }
        len++;  /*记录链表长度*/         head = head->next;  /*统计下一个分类的信息*/
    }
    p1->next = NULL;
    phead = phead->next;
    StatTypeSort(phead, len); /*调用StatTypeSort函数，对链表根据销售金额排序*/      return phead;
}   /**  * 函数名称: StatYearInfo  * 函数功能: 按年统计指定各品种销售情况并创建统计结果链表  * 输入参数: head 主链头指针  * 输出参数: 无  * 返回值: 统计得到的销售情况链表的链头指针phead  *  * 调用说明:  */ ITEM_STAT *StatYearInfo(TYPE_NODE *head, char year[5])
{
    TYPE_NODE *pt = head;
    ITEM_NODE *pi;
    SALE_NODE *ps;
    ITEM_STAT *phead = NULL, *p1;
    int i, len = 0;
    phead = (ITEM_STAT *)malloc(sizeof(ITEM_STAT));
    p1 = phead;


    /*建立带头指针的先进先出的统计结果链表*/     while(pt != NULL)
    {
        pi = pt->inext;
        while(pi != NULL)
        {
            p1->next = (ITEM_STAT *)malloc(sizeof(ITEM_STAT));/*新建统计结果结点*/             p1 = p1->next;
            strcpy(p1->type, pt->name);
            strcpy(p1->item, pi->name);
            p1->weight = 0;
            p1->sale = 0;
            ps = pi->snext;
            while(ps != NULL)
            {
                /*遍历基本信息结点对应的销售信息链表，按商品名称统计此商品符合条件的销售信息*/                 for(i=0; year[i] == ps->date[i] && i<=3; i++);
                if(i == 4)
                {
                    p1->weight += ps->weight;
                    p1->sale += ps->sales_amount;
                }
                ps = ps->next;
            }
            pi = pi->next;
            len++;   /*记录结果链表的长度*/
        }
        pt = pt->next;
    }
    p1->next = NULL;
    phead = phead->next; /*处理链表头指针和尾指针*/


    StatItemSort(phead, len); /*调用函数StatItemSort，按销售重量排序*/      return phead; /*返回结果链表的头指针*/
}   /**  * 函数名称: StatCTypeInfo  * 函数功能: 按客户姓名统计指定各类别销售情况并创建统计结果链表  * 输入参数: head 主链头指针，year[4] 年份  * 输出参数: 无  * 返回值: 统计得到的销售情况链表的链头指针phead  *  * 调用说明:  */ CLIENT_STAT *StatCTypeInfo(TYPE_NODE *pt)
{
    ITEM_NODE *pi;
    SALE_NODE *ps;
    CLIENT_STAT *phead, *p1 = NULL, *p2 = NULL;
    int find = 0, len = 0;
    phead = (CLIENT_STAT *)malloc(sizeof(CLIENT_STAT));
    phead->next = NULL;
    pi = pt->inext;
    while(pi != NULL)
    {
        ps = pi->snext;
        while(ps != NULL)
        {
            /*遍历指定分类信息结点对应的销售信息链表*/             p2 = phead->next;
            find = 0;
            while(p2 != NULL)


            {
                /*遍历统计结果链表，找存储的客户姓名与ps中的相同的结点p2*/                 if(strcmp(ps->client_name, p2->client) == 0)
                {
                    /*若找到结点，修改p2的销售金额和销售重量,并将find标记为1*/                     find = 1;
                    p2->weight += ps->weight;
                    p2->sale += ps->sales_amount;
                    break;
                }
                p2 = p2->next;
            }
            if(find == 0)
            {
                /*若find为0，即未找到结点，新建结点p1*/                 p1 = (CLIENT_STAT *)malloc(sizeof(CLIENT_STAT));
                p1->weight = ps->weight;
                p1->sale = ps->sales_amount;
                strcpy(p1->client, ps->client_name);  /*修改p1的销售信息和客户信息*/                 p1->next = phead->next;
                phead->next = p1;  /*将p1作为统计结果链表的新的首元结点*/                 len++;  /*记录链表长度*/
            }
            ps = ps->next;
        }
        pi = pi->next;
    }
    StatClientSort(phead->next, len); /*调用StatClientSort函数，对链表按购买重量排序*/      return phead->next;  /*返回首元结点的地址*/
}   /**


* 函数名称: StatCItemInfo  * 函数功能: 按客户姓名统计指定各品种销售情况并创建统计结果链表  * 输入参数: head 主链头指针  * 输出参数: 无  * 返回值: 统计得到的销售情况链表的链头指针phead  *  * 调用说明:  */ CLIENT_STAT *StatCItemInfo(ITEM_NODE *pi)
{
    SALE_NODE *ps;
    CLIENT_STAT *phead, *p1 = NULL, *p2 = NULL;
    int find = 0, len = 0;
    phead = (CLIENT_STAT *)malloc(sizeof(CLIENT_STAT));
    phead->next = NULL;
    ps = pi->snext;
    while(ps != NULL)
    {
        /*遍历指定基本信息结点对应的销售信息链表*/         p2 = phead->next;
        find = 0;
        while(p2 != NULL)
        {
            /*遍历统计结果链表，找存储的客户姓名与ps中的相同的结点*/             if(strcmp(ps->client_name, p2->client) == 0)
            {
                /*若找到结点，修改p2的销售金额和销售重量,并将find标记为1*/                 find = 1;
                p2->weight += ps->weight;
                p2->sale += ps->sales_amount;
                break;
            }
            p2 = p2->next;
        }


        if(find == 0)
        {
            /*若find为0，即未找到结点，新建结点p1*/             p1 = (CLIENT_STAT *)malloc(sizeof(CLIENT_STAT));
            p1->weight = ps->weight;
            p1->sale = ps->sales_amount;
            strcpy(p1->client, ps->client_name);  /*修改p1的销售信息和客户信息*/             p1->next = phead->next;
            phead->next = p1;  /*将p1作为统计结果链表的新的首元结点*/             len++;   /*记录链表长度*/
        }
        ps = ps->next;
    }
    StatClientSort(phead->next, len);  /*调用StatClientSort函数，对链表按购买重量排序*/      return phead->next;  /*返回首元结点的地址*/
}   /**  * 函数名称: StatT_ClientInfo  * 函数功能: 按分类名称统计指定客户消费情况并创建统计结果链表  * 输入参数: head 主链头指针，client[20] 客户姓名  * 输出参数: 无  * 返回值: 统计得到的销售情况链表的链头指针phead  *  * 调用说明:  */ TYPE_STAT *StatT_ClientInfo(TYPE_NODE *head, char client[20])
{
    TYPE_NODE *pt = head;


    ITEM_NODE *pi;
    SALE_NODE *ps;
    TYPE_STAT *phead = NULL, *p1;
    float weight;              /**<总重量*/     float sale;                /**<销售金额*/     int len = 0;
    phead = (TYPE_STAT *)malloc(sizeof(TYPE_STAT));
    phead->next = NULL; /*新建头结点*/     while(pt != NULL)
    {
        pi = pt->inext;
        sale = 0;
        weight = 0;
        while(pi != NULL)
        {
            ps = pi->snext;
            while(ps != NULL)
            {
                /*遍历各个分类信息结点对应的销售信息链表，统计指定客户在此分类的购买情况*/                 if(strcmp(client, ps->client_name) == 0)
                {
                    sale += ps->sales_amount;
                    weight += ps->weight;
                }
                ps = ps->next;
            }
            pi = pi->next;
        }
        if(sale != 0)


        {
            /*如果购买金额不为0，新建结点p1,并作为统计结果链表的新的首元结点*/             p1 = (TYPE_STAT *)malloc(sizeof(TYPE_STAT));
            p1->next = phead->next;
            phead->next = p1;
            strcpy(p1->type, pt->name);
            p1->sale = sale;
            p1->weight = weight;
            len++;
        }
        pt = pt->next;
    }
    StatTypeSort(phead->next, len); /*调用StatTypeSort函数，对链表按销售金额排序*/      return phead->next;
}    /**  * 函数名称: StatI_ClientInfo  * 函数功能: 按商品名称统计指定客户消费情况并创建统计结果链表  * 输入参数: head 主链头指针，client[20] 客户姓名  * 输出参数: 无  * 返回值: 统计得到的销售情况链表的链头指针phead  *  * 调用说明:  */ ITEM_STAT *StatI_ClientInfo(TYPE_NODE *head, char client[20])
{
    TYPE_NODE *pt = head;
    ITEM_NODE *pi;


    SALE_NODE *ps;
    ITEM_STAT *phead = NULL, *p1, *p2;
    char type[10];             /**<分类名称*/     char item[20];             /**<商品名称*/     float weight;              /**<总重量*/     float sale;                /**<销售金额*/     int i, j, len = 0;
    phead = (ITEM_STAT *)malloc(sizeof(ITEM_STAT));
    phead->next = NULL;     /*建立带头指针的先进先出的统计结果链表*/     while(pt != NULL)
    {
        pi = pt->inext;
        while(pi != NULL)
        {
            weight = 0;
            ps = pi->snext;
            while(ps != NULL)
            {
                if(strcmp(client, ps->client_name) == 0)                     weight += ps->weight;
                ps = ps->next;
            }
            if(weight != 0)
            {
                /*若客户购买的重量不为0，新建结点并作为链表的新的首元结点*/                 p1 = (ITEM_STAT *)malloc(sizeof(ITEM_STAT));
                p1->next = phead->next;
                phead->next = p1;
                strcpy(p1->type, pt->name);
                strcpy(p1->item, pi->name);
                p1->weight = weight;


                p1->sale = p1->weight * pi->price;
                len++; /*记录链表的长度*/
            }
            pi = pi->next;
        }
        pt = pt->next;
    }
    phead = phead -> next;
    p1 = phead;
    for(i = 0, p1 = phead; i < len-1; i++, p1 = p1->next)         for(j = i + 1, p2 = p1->next; j < len; j++, p2 = p2->next)             if(p1->sale < p2->sale)
            {
                strcpy(type, p1->type);
                strcpy(item, p1->item);
                weight = p1->weight;
                sale = p1->sale;
                strcpy(p1->type, p2->type);
                strcpy(p1->item, p2->item);
                p1->sale = p2->sale;
                p1->weight = p2->weight;
                strcpy(p2->type, type);
                strcpy(p2->item, item);
                p2->sale = sale;
                p2->weight = weight;
            }
    return phead;
}


/**  * 函数名称: StatClientSort  * 函数功能: 将按客户姓名统计的链表按销售总重排序  * 输入参数: head 主链头指针，len 链表长度  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */ void StatClientSort(CLIENT_STAT *head, int len)
{
    int i, j;
    float sale, weight;
    char client[20];
    CLIENT_STAT *p1 = NULL, *p2 = NULL;     /*根据购买重量通过交换数据域的方式进行冒泡排序*/     for(i = 0, p1 = head; i < len-1; i++, p1 = p1->next)         for(j = i + 1, p2 = p1->next; j < len; j++, p2 = p2->next)             if(p1->weight < p2->weight)
            {
                strcpy(client, p1->client);
                weight = p1->weight;
                sale = p1->sale;
                strcpy(p1->client, p2->client);
                p1->sale = p2->sale;
                p1->weight = p2->weight;
                strcpy(p2->client, client);
                p2->sale = sale;
                p2->weight = weight;
            }
}


/**  * 函数名称: StatTypetSort  * 函数功能: 将按分类名称统计的链表按销售总额排序  * 输入参数: head 主链头指针，len 链表长度  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */ void StatTypeSort(TYPE_STAT *head, int len)
{
    TYPE_STAT *p1, *p2;
    int i,j;
    char type[10];             /**<分类名称*/     float weight;              /**<总重量*/     float sale;                /**<销售金额*/     /*根据销售金额通过交换数据域的方式进行冒泡排序*/     for(i = 0, p1 = head; i < len-1; i++, p1 = p1->next)         for(j = i + 1, p2 = p1->next; j < len; j++, p2 = p2->next)             if(p1->sale < p2->sale)
            {
                strcpy(type, p1->type);
                weight = p1->weight;
                sale = p1->sale;
                strcpy(p1->type, p2->type);
                p1->sale = p2->sale;
                p1->weight = p2->weight;
                strcpy(p2->type, type);
                p2->sale = sale;
                p2->weight = weight;
            }


}   /**  * 函数名称: StatItemtSort  * 函数功能: 将按商品名称统计的链表按销售重量排序  * 输入参数: head 主链头指针，len 链表长度  * 输出参数: 无  * 返回值: 无  *  * 调用说明:  */ void StatItemSort(ITEM_STAT *head, int len)
{
    int i,j;
    char type[10];             /**<分类名称*/     char item[20];             /**<商品名称*/     float weight;              /**<总重量*/     float sale;                /**<销售金额*/     ITEM_STAT *p1, *p2;     /*根据销售重量通过交换数据域的方式进行冒泡排序*/     for(i = 0, p1 = head; i < len-1; i++, p1 = p1->next)         for(j = i + 1, p2 = p1->next; j < len; j++, p2 = p2->next)             if(p1->weight < p2->weight)
            {
                strcpy(type, p1->type);
                strcpy(item, p1->item);
                weight = p1->weight;
                sale = p1->sale;
                strcpy(p1->type, p2->type);
                strcpy(p1->item, p2->item);
                p1->sale = p2->sale;
                p1->weight = p2->weight;


                strcpy(p2->type, type);
                strcpy(p2->item, item);
                p2->sale = sale;
                p2->weight = weight;
            }
}  /**  * 函数名称: MyModule4  * 函数功能: 将含有3个字符串与1个"返回"字符串的标签束设置成热区并等待用户输入  * 输入参数: pString 指针，n标签个数  * 输出参数: 无  * 返回值: 用鼠标单击、按回车或空格时返回当前热区编号  *  * 调用说明:  */ int MyModule4(char **pString, int n)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int iHot = 1;
    int i, maxlen, str_len,x;
    for (i=0, maxlen=0; i<n; i++)
    {
        str_len = strlen(pString[i]);
        if (maxlen < str_len)
        {
            maxlen = str_len;


        }
    }
    pos.X = maxlen + 6;
    pos.Y = n + 5;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/     labels.num = n;
    labels.ppLabel = pString;
    COORD aLoc[n];
    for (i=0; i<n; i++)
    {
        aLoc[i].X = rcPop.Left + 3;
        aLoc[i].Y = rcPop.Top + 2 + i;
    }
    str_len = strlen(pString[n-1]);
    aLoc[n-1].X = rcPop.Left + 3 + (maxlen-str_len)/2;
    aLoc[n-1].Y = aLoc[n-1].Y + 2;
    labels.pLoc = aLoc;
    areas.num = 4;
    SMALL_RECT aArea[] = {{aLoc[0].X, aLoc[0].Y,                         aLoc[0].X + maxlen - 1, aLoc[0].Y},                         {
            aLoc[1].X, aLoc[1].Y,


            aLoc[1].X + maxlen - 1, aLoc[1].Y
        },                         {aLoc[2].X, aLoc[2].Y,                         aLoc[2].X + maxlen - 1, aLoc[2].Y},                         {aLoc[3].X, aLoc[3].Y,                         aLoc[3].X + 3, aLoc[3].Y}
    };
    char aSort[] = {0, 0, 0, 0};
    char aTag[] = {1, 2, 3, 4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 2 + n;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &num_written);
    x = DealInput(&areas, &iHot);
    PopOff();
    return x;
}  /**  * 函数名称: MyModule3  * 函数功能: 将含有2个字符串与1个"返回"字符串的标签束设置成热区并等待用户输入  * 输入参数: pString 指针，n标签个数  * 输出参数: 无  * 返回值: 用鼠标单击、按回车或空格时返回当前热区编号  *  * 调用说明:  */


int MyModule3(char **pString, int n)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int iHot = 1;
    int i, maxlen, str_len,x;
    for (i=0, maxlen=0; i<n; i++)
    {
        str_len = strlen(pString[i]);
        if (maxlen < str_len)
        {
            maxlen = str_len;
        }
    }
    pos.X = maxlen + 6;
    pos.Y = n + 5;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/     labels.num = n;
    labels.ppLabel = pString;
    COORD aLoc[n];


    for (i=0; i<n; i++)
    {
        aLoc[i].X = rcPop.Left + 3;
        aLoc[i].Y = rcPop.Top + 2 + i;
    }
    str_len = strlen(pString[n-1]);
    aLoc[n-1].X = rcPop.Left + 3 + (maxlen-str_len)/2;
    aLoc[n-1].Y = aLoc[n-1].Y + 2;
    labels.pLoc = aLoc;
    areas.num = 3;
    SMALL_RECT aArea[] = {{aLoc[0].X, aLoc[0].Y,                         aLoc[0].X + maxlen - 1, aLoc[0].Y},                         {aLoc[1].X, aLoc[1].Y,                         aLoc[1].X + maxlen - 1, aLoc[1].Y},                         {aLoc[2].X, aLoc[2].Y,                         aLoc[2].X + 3, aLoc[2].Y}                         };
    char aSort[] = {0, 0, 0};
    char aTag[] = {1, 2, 3};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 2 + n;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &num_written);


    x = DealInput(&areas, &iHot);
    PopOff();
    return x;
}  /**  * 函数名称: SaveMaintain  * 函数功能: 使用数据维护功能和退出系统时保存数据  * 输入参数: 无  * 输出参数: 无  * 返回值: 无  *  * 调用说明: 使用时可选择立即保存和稍后保存  */ void SaveMaintain()
{
    char *pString[] = {"保存数据",                        "暂时不保存"};
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int iHot = 1;
    int i, maxlen, str_len, n = 2;
    for (i=0, maxlen=0; i<n; i++)
    {
        str_len = strlen(pString[i]);
        if (maxlen < str_len)
        {


            maxlen = str_len;
        }
    }
    pos.X = maxlen + 6;
    pos.Y = n + 5;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/     labels.num = n;
    labels.ppLabel = pString;
    COORD aLoc[n];
    for (i=0; i<n; i++)
    {
        aLoc[i].X = rcPop.Left + 3;
        aLoc[i].Y = rcPop.Top + 2 + i;
    }
    str_len = strlen(pString[n-1]);
    aLoc[n-1].X = rcPop.Left + 3 + (maxlen-str_len)/2;
    aLoc[n-1].Y = aLoc[n-1].Y + 1;
    labels.pLoc = aLoc;
    areas.num = 2;
    SMALL_RECT aArea[] = {{aLoc[0].X, aLoc[0].Y,                         aLoc[0].X + maxlen - 1, aLoc[0].Y},


        {aLoc[1].X, aLoc[1].Y,                         aLoc[1].X + maxlen - 1, aLoc[1].Y},
    };
    char aSort[] = {0, 0};
    char aTag[] = {1, 2};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + n +1;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-3, pos, &num_written);
    switch(DealInput(&areas, &iHot))
    {
    case 1:
        PopOff();
        SaveSysData(gp_head); /*调用SaveSysData函数，保存三类数据*/         char *plabel_name1[] = {"数据保存成功！",                                "确认"                           };
        ShowModule(plabel_name1, 2);
        break;
    case 2:
        PopOff();
        break;
    }
}
