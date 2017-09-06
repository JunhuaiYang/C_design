#include"global.h"


/**
 *
 * \brief 该文件主要保存界面实现方面的函数
 *
 */


/**
 * 函数名称: InitInterface
 * 函数功能: 初始化界面.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void InitInterface()
{
    WORD att = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
               | BACKGROUND_INTENSITY;  /*黄色前景和灰色背景*/

    SetConsoleTextAttribute(gh_std_out, att);  /*设置控制台屏幕缓冲区字符属性*/

    ClearScreen();  /* 清屏*/

    /*创建弹出窗口信息堆栈，将初始化后的屏幕窗口当作第一层弹出窗口*/
    gp_scr_att = (char *)calloc(SCR_COL * SCR_ROW, sizeof(char));//屏幕字符属性,SCR_COL是行数，相当于对应屏幕上的每一个坐标
    gp_top_layer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));  //gp_top_layer屏幕窗口信息链结点结点结构,屏幕窗口信息链结点结点结构
    gp_top_layer->LayerNo = 0;      /*弹出窗口的层号为0*/
    gp_top_layer->rcArea.Left = 0;  /*弹出窗口的区域为整个屏幕窗口*/
    gp_top_layer->rcArea.Top = 0; //大概是边距？
    gp_top_layer->rcArea.Right = SCR_COL - 1;
    gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
    gp_top_layer->pContent = NULL;  //弹出窗口区域字符单元原信息存储缓冲区
    gp_top_layer->pScrAtt = gp_scr_att; //弹出窗口区域字符单元原属性值存储缓冲区,gp_scr_att = 已经初始化
    gp_top_layer->next = NULL;

    ShowMenu();     /*显示菜单栏*/

    return;
}

/**
 * 函数名称: ReFresh
 * 函数功能: 刷新界面.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */

void ReFresh()
{
    ClearScreen();  /* 清屏*/
    ShowMenu();     /*显示菜单栏*/

    return;
}

/**
 * 函数名称:
 * 函数功能: 刷新界面.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void GotoXY(int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(gh_std_out, pos);
}
/**
 * 函数名称:
 * 函数功能: 显示或隐藏控制台光标.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */

void Show_Cursor(BOOL boo)
{
    CONSOLE_CURSOR_INFO lpCur; //控制台光标信息

    GetConsoleCursorInfo(gh_std_out, &lpCur); //检索有关指定的控制台屏幕缓冲区的光标的可见性和大小信息。
    lpCur.bVisible = boo;
    SetConsoleCursorInfo(gh_std_out, &lpCur);
}

/**
 * 函数名称: ClearScreen
 * 函数功能: 清除屏幕信息.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void ClearScreen(void)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD home = {0, 0}; //COORD是一个结构，表示坐标
    unsigned long size;

    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );//检索取屏幕缓冲区信息
    size =  bInfo.dwSize.X * bInfo.dwSize.Y; /*计算屏幕缓冲区字符单元数*/

    /*将屏幕缓冲区所有单元的字符属性设置为当前屏幕缓冲区字符属性*/
    FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &ul); //API函数，用来给控制台的部分字体着色。

    /*将屏幕缓冲区所有单元填充为空格字符*/
    FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &ul);

    return;
}

/**
 * 函数名称: ShowMenu
 * 函数功能: 在屏幕上显示主菜单, 并设置热区, 在主菜单第一项上置选中标记.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void ShowMenu()
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    CONSOLE_CURSOR_INFO lpCur; //控制台光标信息
    COORD size;
    COORD pos = {0, 0};
    int i, j;
    int PosA = 2, PosB;
    char ch;

    GetConsoleScreenBufferInfo( gh_std_out, &bInfo ); //用于检索指定的控制台屏幕缓冲区的信息。
    size.X = bInfo.dwSize.X;
    size.Y = 1;
    SetConsoleCursorPosition(gh_std_out, pos);
    for (i=0; i < 5; i++) /*在窗口第一行第一列处输出主菜单项*/
    {
        printf("  %s  ", ga_main_menu[i]);//菜单在这里输出
    }

    GetConsoleCursorInfo(gh_std_out, &lpCur); //检索有关指定的控制台屏幕缓冲区的光标的可见性和大小信息。
    lpCur.bVisible = FALSE;
    SetConsoleCursorInfo(gh_std_out, &lpCur);  /*隐藏光标*/

    /*申请动态存储区作为存放菜单条屏幕区字符信息的缓冲区*/
    gp_buff_menubar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    SMALL_RECT rcMenu ={0, 0, size.X-1, 0} ;

    /*将窗口第一行的内容读入到存放菜单条屏幕区字符信息的缓冲区中*/
    ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);

    /*将这一行中英文字母置为红色，其他字符单元置为白底黑字*/
//不是很理解
    for (i=0; i<size.X; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                               | BACKGROUND_RED;
        ch = (char)((gp_buff_menubar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }

    /*修改后的菜单条字符信息回写到窗口的第一行*/
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    COORD endPos = {0, 1};
    SetConsoleCursorPosition(gh_std_out, endPos);  /*将光标位置设置在第2行第1列*/

    /*将菜单项置为热区，热区编号为菜单项号，热区类型为0(按钮型)*/
    i = 0;
    do
    {
        PosB = PosA + strlen(ga_main_menu[i]);  /*定位第i+1号菜单项的起止位置*/
        for (j=PosA; j<PosB; j++)
        {
            gp_scr_att[j] |= (i+1) << 2; //??/*设置菜单项所在字符单元的属性值*/
        }
        PosA = PosB + 4;
        i++;
    } while (i<5);

    TagMainMenu(gi_sel_menu);  /*在选中主菜单项上做标记，gi_sel_menu初值为1*/

    return;
}

/**
 * 函数名称: ShowState
 * 函数功能: 显示状态条.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明: 状态条字符属性为白底黑字, 初始状态无状态信息.
 */
void ShowState()
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size;
    COORD pos = {0, 0};
    int i;

    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );
    size.X = bInfo.dwSize.X;
    size.Y = 2;
    SMALL_RECT rcMenu ={0, bInfo.dwSize.Y-2, size.X-1, bInfo.dwSize.Y-1};

    if (gp_buff_stateBar_info == NULL)
    {
        gp_buff_stateBar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
        ReadConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);
    }

    for (i=0; i<size.X*size.Y; i++)
    {
        (gp_buff_stateBar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                                | BACKGROUND_RED;
/*
        ch = (char)((gp_buff_stateBar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_stateBar_info+i)->Attributes |= FOREGROUND_RED;
        }
*/
    }

    WriteConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);

    return;
}

/**
 * 函数名称: TagMainMenu
 * 函数功能: 在指定主菜单项上置选中标志.
 * 输入参数: num 选中的主菜单项号
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void TagMainMenu(int num)
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

    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );
    size.X = bInfo.dwSize.X;
    size.Y = 1;

    /*去除选中菜单项前面的菜单项选中标记*/
    for (i=0; i<PosA; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                               | BACKGROUND_RED;
        ch = (gp_buff_menubar_info+i)->Char.AsciiChar;
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }

    /*在选中菜单项上做标记，黑底白字*/
    for (i=PosA; i<PosB; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN
                                               | FOREGROUND_RED;
    }

    /*去除选中菜单项后面的菜单项选中标记*/
    for (i=PosB; i<bInfo.dwSize.X; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                               | BACKGROUND_RED;
        ch = (char)((gp_buff_menubar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }

    /*将做好标记的菜单条信息写到窗口第一行*/
    SMALL_RECT rcMenu ={0, 0, size.X-1, 0};
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);

    return;
}


/**
 * 函数名称: RunSys
 * 函数功能: 运行系统, 在系统主界面下运行用户所选择的功能模块.
 * 输入参数: 无
 * 输出参数: phead 主链头指针的地址
 * 返 回 值: 无
 *
 * 调用说明:
 */
void RunSys(ROAD_DATA **phead)
{
    INPUT_RECORD inRec;
    DWORD res;       //double word -> unsigned long
    COORD pos = {0, 0};
    BOOL bRet = TRUE;
    int i, loc, num;
    int cNo, cAtt;      /*cNo:字符单元层号, cAtt:字符单元属性*/
    char vkc, asc;      /*vkc:虚拟键代码, asc:字符的ASCII码值*/

    while (bRet)
    {
        /*从控制台输入缓冲区中读一条记录*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res); //用于读键盘或者鼠标事件

        // win10在进行鼠标操作时必须把控制台选项中的快速选择和插入关闭

        if (inRec.EventType == MOUSE_EVENT) /*如果记录由鼠标事件产生*/
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;  /*获取鼠标坐标位置*/
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; //取该位置的层号 3 -> 0111
            cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2;/*取该字符单元属性*/
            if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
            {
                /* cAtt > 0 表明该位置处于热区(主菜单项字符单元)
                 * cAtt != gi_sel_menu 表明该位置的主菜单项未被选中
                 * gp_top_layer->LayerNo > 0 表明当前有子菜单弹出
                 */
                if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)  //上面三个条件都满足的话
                {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                    PopMenu(cAtt);       /*弹出鼠标所在主菜单项对应的子菜单*/
                }
            }
            else if (cAtt > 0) /*鼠标所在位置为弹出子菜单的菜单项字符单元*/
            {
                TagSubMenu(cAtt); /*在该子菜单项上做选中标记*/
            }

            //鼠标
            if (inRec.Event.MouseEvent.dwButtonState
                == FROM_LEFT_1ST_BUTTON_PRESSED) /*如果按下鼠标左边第一键*/
            {
                if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
                {
                    if (cAtt > 0) /*如果该位置处于热区(主菜单项字符单元)*/
                    {
                        PopMenu(cAtt);   /*弹出鼠标所在主菜单项对应的子菜单*/
                    }
                    /*如果该位置不属于主菜单项字符单元，且有子菜单弹出*/
                    else if (gp_top_layer->LayerNo > 0)
                    {
                        PopOff();            /*关闭弹出的子菜单*/
                        gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                    }
                }
                else /*层号不为0，表明该位置被弹出子菜单覆盖*/
                {
                    if (cAtt > 0) /*如果该位置处于热区(子菜单项字符单元)*/
                    {
                        PopOff(); /*关闭弹出的子菜单*/
                        gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/

                        /*执行对应功能函数:gi_sel_menu主菜单项号,cAtt子菜单项号*/
                        bRet = ExeFunction(gi_sel_menu, cAtt);
                    }
                }
            }
            else if (inRec.Event.MouseEvent.dwButtonState
                     == RIGHTMOST_BUTTON_PRESSED) /*如果按下鼠标右键*/
            {
                if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
                {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT  /*如果记录由按键产生*/
                 && inRec.Event.KeyEvent.bKeyDown) /*且键被按下*/
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /*获取按键的虚拟键码*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*获取按键的ASC码*/

            /*系统快捷键的处理*/
            if (vkc == 112) /*如果按下F1键*/
            {
                if (gp_top_layer->LayerNo != 0) /*如果当前有子菜单弹出*/
                {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                }
                bRet = ExeFunction(5, 1);  /*运行帮助主题功能函数*/
            }
            else if (inRec.Event.KeyEvent.dwControlKeyState
                     & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))    //Alt
            { /*如果按下左或右Alt键*/
                switch (vkc)  /*判断组合键Alt+字母*/
                {
                    case 88:  /*Alt+X 退出*/
                        if (gp_top_layer->LayerNo != 0)
                        {
                            PopOff();
                            gi_sel_sub_menu = 0;
                        }
                        bRet = ExeFunction(1,4);
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

            /** \brief 键盘码表
             *
             * \param Left Arrow   37  Right Arrow   39
             * \param Up Arrow   38   Down Arrow   40
             * \return
             *
             */

            else if (asc == 0) /*其他控制键的处理*/
            {
                if (gp_top_layer->LayerNo == 0) /*如果未弹出子菜单*/
                {
                    switch (vkc) /*处理方向键(左、右、下)，不响应其他控制键*/
                    {
                        case 37:    //←
                            gi_sel_menu--;
                            if (gi_sel_menu == 0)
                            {
                                gi_sel_menu = 5;
                            }
                            TagMainMenu(gi_sel_menu);
                            break;
                        case 39:   //→
                            gi_sel_menu++;
                            if (gi_sel_menu == 6)
                            {
                                gi_sel_menu = 1;
                            }
                            TagMainMenu(gi_sel_menu);
                            break;
                        case 40://↓
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                    }
                }
                else  /*已弹出子菜单时*/
                {
                    for (loc=0,i=1; i<gi_sel_menu; i++)
                    {
                        loc += ga_sub_menu_count[i-1];
                    }  /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
                    switch (vkc) /*方向键(左、右、上、下)的处理*/
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
            else if ((asc-vkc == 0) || (asc-vkc == 32)){  /*按下普通键*/
                if (gp_top_layer->LayerNo == 0)  /*如果未弹出子菜单*/
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
                        /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
                        for (loc=0,i=1; i<gi_sel_menu; i++)
                        {
                            loc += ga_sub_menu_count[i-1];
                        }

                        /*依次与当前子菜单中每一项的代表字符进行比较*/  //用于弹出的子菜单操作
                        for (i=loc; i<loc+ga_sub_menu_count[gi_sel_menu-1]; i++)
                        {
                            if (strlen(ga_sub_menu[i])>0 && vkc==ga_sub_menu[i][1])
                            { /*如果匹配成功*/
                                PopOff();
                                gi_sel_sub_menu = 0;
                                bRet = ExeFunction(gi_sel_menu, i-loc+1);
                            }
                        }
                    }
                }
            }
        }
    }
}


/**
 * 函数名称: PopMenu
 * 函数功能: 弹出指定主菜单项对应的子菜单.
 * 输入参数: num 指定的主菜单项号
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void PopMenu(int num)
{
    LABEL_BUNDLE labels;   //类似的
    HOT_AREA areas;
    SMALL_RECT rcPop; //矩形
    COORD pos;
    WORD att; //颜色
    //char *pCh;
    //int j;
    int i, loc = 0;

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

    gi_sel_menu = num;    /*将选中主菜单项置为指定的主菜单项*/   //被选中的主菜单
    TagMainMenu(gi_sel_menu); /*在选中的主菜单项上做标记*/  //zhu
    LocSubMenu(gi_sel_menu, &rcPop); /*计算弹出子菜单的区域位置, 存放在rcPop中*/

    /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
    for (i=1; i<gi_sel_menu; i++)
    {
        loc += ga_sub_menu_count[i-1];   //子菜单个数
    }
    /*将该组子菜单项项名存入标签束结构变量*/
    labels.ppLabel = ga_sub_menu + loc;   /*标签束第一个标签字符串的地址*/
    labels.num = ga_sub_menu_count[gi_sel_menu-1]; /*标签束中标签字符串的个数*/
    COORD aLoc[labels.num];/*定义一个坐标数组，存放每个标签字符串输出位置的坐标*/  //位置
    for (i=0; i<labels.num; i++) /*确定标签字符串的输出位置，存放在坐标数组中*/
    {
        aLoc[i].X = rcPop.Left + 2;
        aLoc[i].Y = rcPop.Top + i + 1;
    }
    labels.pLoc = aLoc; /*使标签束结构变量labels的成员pLoc指向坐标数组的首元素*/
    /*设置热区信息*/
    areas.num = labels.num;       /*热区的个数，等于标签的个数，即子菜单的项数*/
    SMALL_RECT aArea[areas.num];                    /*定义数组存放所有热区位置*/
    char aSort[areas.num];                      /*定义数组存放所有热区对应类别*/
    char aTag[areas.num];                         /*定义数组存放每个热区的编号*/
    for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*热区定位*/
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*热区类别都为0(按钮型)*/
        aTag[i] = i + 1;           /*热区按顺序编号*/
    }
    areas.pArea = aArea;/*使热区结构变量areas的成员pArea指向热区位置数组首元素*/
    areas.pSort = aSort;/*使热区结构变量areas的成员pSort指向热区类别数组首元素*/
    areas.pTag = aTag;   /*使热区结构变量areas的成员pTag指向热区编号数组首元素*/

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);  /*给弹出窗口画边框*/

//    pos.X = rcPop.Left + 2;
//    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
//    { /*此循环用来在空串子菜项位置画线形成分隔，并取消此菜单项的热区属性*/
//        pCh = ga_sub_menu[loc+pos.Y-rcPop.Top-1];
//        if (strlen(pCh)==0) /*串长为0，表明为空串*/
//        {   /*首先画横线*/
//            //FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-3, pos, &ul);
//            for (j=rcPop.Left+2; j<rcPop.Right-1; j++)
//            {   /*取消该区域字符单元的热区属性*/
//                gp_scr_att[pos.Y*SCR_COL+j] &= 3; /*按位与的结果保留了低两位*/
//            }
//        }
//
//    }
    /*将子菜单项的功能键设为白底红字*/
    pos.X = rcPop.Left + 3;
    att =  FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    {
//        if (strlen(ga_sub_menu[loc+pos.Y-rcPop.Top-1])==0)
//        {
//            continue;  /*跳过空串*/
//        }
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    return;
}

/**
 * 函数名称: PopUp
 * 函数功能: 在指定区域输出弹出窗口信息, 同时设置热区, 将弹出窗口位置信息入栈.
 * 输入参数: pRc 弹出窗口位置数据存放的地址
 *           att 弹出窗口区域字符属性 颜色
 *           pLabel 弹出窗口中标签束信息存放的地址
             pHotArea 弹出窗口中热区信息存放的地址
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void PopUp(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea)
{
    LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pCh;
    int i, j, row;

    /*弹出窗口所在位置字符单元信息入栈*/
    size.X = pRc->Right - pRc->Left + 1;    /*弹出窗口的宽度*/
    size.Y = pRc->Bottom - pRc->Top + 1;    /*弹出窗口的高度*/
    /*申请存放弹出窗口相关信息的动态存储区*/
    nextLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    nextLayer->next = gp_top_layer;
    nextLayer->LayerNo = gp_top_layer->LayerNo + 1;
    nextLayer->rcArea = *pRc;
    nextLayer->pContent = (CHAR_INFO *)malloc(size.X*size.Y*sizeof(CHAR_INFO));
    nextLayer->pScrAtt = (char *)malloc(size.X*size.Y*sizeof(char));
    pCh = nextLayer->pScrAtt;
    /*将弹出窗口覆盖区域的字符信息保存，用于在关闭弹出窗口时恢复原样*/
    ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*此二重循环将所覆盖字符单元的原先属性值存入动态存储区，便于以后恢复*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            *pCh = gp_scr_att[i*SCR_COL+j];
            pCh++;
        }
    }
    gp_top_layer = nextLayer;  /*完成弹出窗口相关信息入栈操作*/
    /*设置弹出窗口区域字符的新属性*/
    pos.X = pRc->Left;
    pos.Y = pRc->Top;
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {
        FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);
        pos.Y++;
    }
    /*将标签束中的标签字符串在设定的位置输出*/
    for (i=0; i<pLabel->num; i++)
    {
        pCh = pLabel->ppLabel[i];
        if (strlen(pCh) != 0)
        {
            WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh),   // 在指定位置处插入指定数量的字符
                                        pLabel->pLoc[i], &ul);
        }
    }
    /*设置弹出窗口区域字符单元的新属性*/
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*此二重循环设置字符单元的层号*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = gp_top_layer->LayerNo;
        }
    }

    for (i=0; i<pHotArea->num; i++)
    {   /*此二重循环设置所有热区中字符单元的热区类型和热区编号*/
        row = pHotArea->pArea[i].Top;
        for (j=pHotArea->pArea[i].Left; j<=pHotArea->pArea[i].Right; j++)
        {
            gp_scr_att[row*SCR_COL+j] |= (pHotArea->pSort[i] << 6)
                                    | (pHotArea->pTag[i] << 2);
        }
    }
    return;
}

/**
 * 函数名称: PopOff
 * 函数功能: 关闭顶层弹出窗口, 恢复覆盖区域原外观和字符单元原属性.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void PopOff(void)
{
    LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pCh;
    int i, j;

    if ((gp_top_layer->next==NULL) || (gp_top_layer->pContent==NULL))
    {   /*栈底存放的主界面屏幕信息，不用关闭*/
        return;
    }
    nextLayer = gp_top_layer->next;
    /*恢复弹出窗口区域原外观*/
    size.X = gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1;
    size.Y = gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1;
    WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));
    /*恢复字符单元原属性*/
    pCh = gp_top_layer->pScrAtt;
    for (i=gp_top_layer->rcArea.Top; i<=gp_top_layer->rcArea.Bottom; i++)
    {
        for (j=gp_top_layer->rcArea.Left; j<=gp_top_layer->rcArea.Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = *pCh;
            pCh++;
        }
    }
    free(gp_top_layer->pContent);    /*释放动态存储区*/
    free(gp_top_layer->pScrAtt);
    free(gp_top_layer);
    gp_top_layer = nextLayer;
    gi_sel_sub_menu = 0;
    return;
}

/**
 * 函数名称: DrawBox
 * 函数功能: 在指定区域画边框.
 * 输入参数: pRc 存放区域位置信息的地址
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void DrawBox(SMALL_RECT *pRc)
{
    char chBox[] = {'+','-','|'};  /*画框用的字符*/
    COORD pos = {pRc->Left, pRc->Top};  /*定位在区域的左上角*/

    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框左上角*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*此循环画上边框横线*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框右上角*/
    for (pos.Y = pRc->Top+1; pos.Y < pRc->Bottom; pos.Y++)
    {   /*此循环画边框左边线和右边线*/
        pos.X = pRc->Left;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
        pos.X = pRc->Right;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
    }
    pos.X = pRc->Left;
    pos.Y = pRc->Bottom;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框左下角*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*画下边框横线*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框右下角*/
    return;
}

/**
 * 函数名称: TagSubMenu
 * 函数功能: 在指定子菜单项上做选中标记.
 * 输入参数: num 选中的子菜单项号
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void TagSubMenu(int num)
{
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int width;

    LocSubMenu(gi_sel_menu, &rcPop);  /*计算弹出子菜单的区域位置, 存放在rcPop中*/
    if ((num<1) || (num == gi_sel_sub_menu) || (num>rcPop.Bottom-rcPop.Top-1))
    {   /*如果子菜单项号越界，或该项子菜单已被选中，则返回*/
        return;
    }

    pos.X = rcPop.Left + 2;
    width = rcPop.Right - rcPop.Left - 3;
    if (gi_sel_sub_menu != 0) /*首先取消原选中子菜单项上的标记*/
    {
        pos.Y = rcPop.Top + gi_sel_sub_menu;
        att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
        FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
        pos.X += 1;
        att |=  FOREGROUND_RED;/*白底红字*/
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    /*在制定子菜单项上做选中标记*/
    pos.X = rcPop.Left + 2;
    pos.Y = rcPop.Top + num;
    att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
    FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
    gi_sel_sub_menu = num;  /*修改选中子菜单项号*/
    return;
}

/**
 * 函数名称: LocSubMenu
 * 函数功能: 计算弹出子菜单区域左上角和右下角的位置.
 * 输入参数: num 选中的主菜单项号
 * 输出参数: rc 存放区域位置信息的地址
 * 返 回 值: 无
 *
 * 调用说明:
 */
void LocSubMenu(int num, SMALL_RECT *rc)
{
    int i, len, loc = 0;

    rc->Top = 1; /*区域的上边定在第2行，行号为1*/
    rc->Left = 1;
    for (i=1; i<num; i++)
    {   /*计算区域左边界位置, 同时计算第一个子菜单项在子菜单字符串数组中的位置*/
        rc->Left += strlen(ga_main_menu[i-1]) + 4;
        loc += ga_sub_menu_count[i-1];
    }
    rc->Right = strlen(ga_sub_menu[loc]);/*暂时存放第一个子菜单项字符串长度*/
    for (i=1; i<ga_sub_menu_count[num-1]; i++)
    {   /*查找最长子菜单字符串，将其长度存放在rc->Right*/
        len = strlen(ga_sub_menu[loc+i]);
        if (rc->Right < len)
        {
            rc->Right = len;
        }
    }
    rc->Right += rc->Left + 3;  /*计算区域的右边界*/
    rc->Bottom = rc->Top + ga_sub_menu_count[num-1] + 1;/*计算区域下边的行号*/
    if (rc->Right >= SCR_COL)  /*右边界越界的处理*/
    {
        len = rc->Right - SCR_COL + 1;
        rc->Left -= len;
        rc->Right = SCR_COL - 1;
    }
    return;
}

/**
 * 函数名称: DealInput
 * 函数功能: 在弹出窗口区域设置热区, 等待并相应用户输入.
 * 输入参数: pHotArea
 *           piHot 焦点热区编号的存放地址, 即指向焦点热区编号的指针
 * 输出参数: piHot 用鼠标单击、按回车或空格时返回当前热区编号
 * 返 回 值:
 *
 * 调用说明:
 */
int DealInput(HOT_AREA *pHotArea, int *piHot)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    int num, arrow, iRet = 0;
    int cNo, cTag, cSort,cAtt;/*cNo:层号, cTag:热区编号, cSort: 热区类型*/
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/

    SetHotPoint(pHotArea, *piHot);
    while (TRUE)
    {    /*循环*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res); //从控制台输入缓冲区中读一条记录

        if (inRec.EventType == MOUSE_EVENT) /*如果记录由鼠标事件产生*/
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;  /*获取鼠标坐标位置*/
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /*取该位置的层号*/
            cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;
            cSort = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 6) & 3;
            cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2;/*取该字符单元属性*/
            if (cAtt > 0) /*鼠标所在位置为弹出子菜单的菜单项字符单元*/
            {
                SetHotPoint(pHotArea, cTag);
            }
        }

        if ((inRec.EventType == MOUSE_EVENT) &&
            (inRec.Event.MouseEvent.dwButtonState
             == FROM_LEFT_1ST_BUTTON_PRESSED))  //左键按下
        {
            if ((cNo == gp_top_layer->LayerNo) && cTag > 0)
            {
                *piHot = cTag;
                SetHotPoint(pHotArea, *piHot);
                if (cSort == 0)
                {
                    iRet = 13;
                    break;
                }
            }
        }

//        if ((inRec.EventType == MOUSE_EVENT) &&
//            (inRec.Event.MouseEvent.dwButtonState
//             == FROM_LEFT_1ST_BUTTON_PRESSED))  //左键按下
//        {
//            pos = inRec.Event.MouseEvent.dwMousePosition;
//            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;
//            cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;
//            cSort = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 6) & 3;
//
//            if ((cNo == gp_top_layer->LayerNo) && cTag > 0)
//            {
//                *piHot = cTag;
//                SetHotPoint(pHotArea, *piHot);
//                if (cSort == 0)
//                {
//                    iRet = 13;
//                    break;
//                }
//            }
//        }
        else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode;
            asc = inRec.Event.KeyEvent.uChar.AsciiChar;;
            if (asc == 0)
            {
                arrow = 0;
                switch (vkc)
                {  /*方向键(左、上、右、下)的处理*/
                    case 37: arrow = 1; break;
                    case 38: arrow = 2; break;
                    case 39: arrow = 3; break;
                    case 40: arrow = 4; break;
                }
                if (arrow > 0)
                {
                    num = *piHot;
                    while (TRUE)
                    {
                        if (arrow < 3)
                        {
                            num--;
                        }
                        else
                        {
                            num++;
                        }
                        if ((num < 1) || (num > pHotArea->num) ||
                            ((arrow % 2) && (pHotArea->pArea[num-1].Top
                            == pHotArea->pArea[*piHot-1].Top)) || ((!(arrow % 2))
                            && (pHotArea->pArea[num-1].Top
                            != pHotArea->pArea[*piHot-1].Top)))
                        {
                            break;
                        }
                    }
                    if (num > 0 && num <= pHotArea->num)
                    {
                        *piHot = num;
                        SetHotPoint(pHotArea, *piHot);
                    }
                }
            }
            else if (vkc == 27)
            {  /*ESC键*/
                iRet = 27;
                break;
            }
            else if (vkc == 13 || vkc == 32)
            {  /*回车键或空格表示按下当前按钮*/
                iRet = 13;
                break;
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

    att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
    att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    for (i=0; i<pHotArea->num; i++)
    {  /*将按钮类热区置为白底黑字*/
        pos.X = pHotArea->pArea[i].Left;
        pos.Y = pHotArea->pArea[i].Top;
        width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
        if (pHotArea->pSort[i] == 0)
        {  /*热区是按钮类*/
            FillConsoleOutputAttribute(gh_std_out, att2, width, pos, &ul);
        }
    }

    pos.X = pHotArea->pArea[iHot-1].Left;
    pos.Y = pHotArea->pArea[iHot-1].Top;
    width = pHotArea->pArea[iHot-1].Right - pHotArea->pArea[iHot-1].Left + 1;
    if (pHotArea->pSort[iHot-1] == 0)
    {  /*被激活热区是按钮类*/
        FillConsoleOutputAttribute(gh_std_out, att1, width, pos, &ul);
    }
    else if (pHotArea->pSort[iHot-1] == 1)
    {  /*被激活热区是文本框类*/
        SetConsoleCursorPosition(gh_std_out, pos);
        GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = TRUE;
        SetConsoleCursorInfo(gh_std_out, &lpCur);   //光标移至输入框起始位置
    }
}

/**
 * 函数名称: ShowModule
 * 函数功能: 弹出说明窗口
 * 输入参数: pString 字符串
 *           n n是字符串行数
 * 输出参数: bRet BOOL值
 * 返 回 值:
 *
 * 调用说明:
 */

BOOL ShowModule(char **pString, int n)   //n是字符串行数
{
    LABEL_BUNDLE labels; //标签束
    HOT_AREA areas;   //热区
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;  //定义了左上角和右下角的坐标的一个矩形。
    COORD pos;  //COORD是定义行与列的坐标结构
    WORD att;  //WORD就是unsigned short，两个字节，DWORD就是unsigned long，四个字节。 此处att用于表示颜色
    int iHot = 1;
    int i, maxlen, str_len;

    for (i=0,maxlen=0; i<n; i++) {       //确定字符串最大长度
        str_len = strlen(pString[i]);
        if (maxlen < str_len) {
            maxlen = str_len;
        }
    }

    pos.X = maxlen + 6;    //x+6 即左边距+3 右边距+3
    pos.Y = n + 6;  //n是行数
    rcPop.Left = (SCR_COL - pos.X) / 2;  //SCR_COL为80 、Left为矩形左上角的x坐标
    rcPop.Right = rcPop.Left + pos.X - 1;  //Right为矩形右下角x坐标
    rcPop.Top = (SCR_ROW - pos.Y) / 2;  //左上角的y坐标
    rcPop.Bottom = rcPop.Top + pos.Y - 1; //右下角的y坐标

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = n;
    labels.ppLabel = pString;
    COORD aLoc[n];   //定义每个字符串的边距

    //标题位置
    aLoc[0].X = rcPop.Left + 3;
    aLoc[0].Y = rcPop.Top + 1;

    for (i=1; i<n; i++) {
        aLoc[i].X = rcPop.Left + 3; //字符边距
        aLoc[i].Y = rcPop.Top + 3 + i; //即位置向下一行
    }

    str_len = strlen(pString[0]);        //标题设置居中
    aLoc[0].X = rcPop.Left + 3 + (maxlen-str_len)/2;

    str_len = strlen(pString[n-1]);
    aLoc[n-1].X = rcPop.Left + 3 + (maxlen-str_len)/2;  //最后的确定设置居中
    aLoc[n-1].Y = aLoc[n-1].Y + 2;  //+2为绘制空格和分割线所需

    labels.pLoc = aLoc;  //标签束位置

    areas.num = 1;   //热区个数
    SMALL_RECT aArea[] = {{aLoc[n-1].X, aLoc[n-1].Y,                //确定键的位置，即热区大小
                           aLoc[n-1].X + 3, aLoc[n-1].Y}};

    char aSort[] = {0};   //类别
    char aTag[] = {1};    //序号

    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;

    //弹出窗口
    PopUp(&rcPop, att, &labels, &areas);

    //画上面的线
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 2;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul); //第三个参数是字符个数

    //划下面的线用
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 3 + n;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul); //第三个参数是字符个数

    DealInput(&areas, &iHot);
    PopOff();

    return bRet;

}


int PopWindowMenu(char **pString, int n,int areanum, int *tag)
{
    LABEL_BUNDLE labels; //标签束
    HOT_AREA areas;   //热区
    SMALL_RECT rcPop;  //定义了左上角和右下角的坐标的一个矩形。
    COORD pos;  //COORD是定义行与列的坐标结构
    WORD att;  //WORD就是unsigned short，两个字节，DWORD就是unsigned long，四个字节。 此处att用于表示颜色
    int iRet; //按键信息

    int i, maxlen, str_len;

    for (i=0,maxlen=0; i<n; i++) {       //确定字符串最大长度
        str_len = strlen(pString[i]);
        if (maxlen < str_len) {
            maxlen = str_len;
        }
    }

    pos.X = maxlen + 6;    //x+6 即左边距+3 右边距+3
    pos.Y = n + 6;  //n是行数
    rcPop.Left = (SCR_COL - pos.X) / 2;  //SCR_COL为80 、Left为矩形左上角的x坐标
    rcPop.Right = rcPop.Left + pos.X - 1;  //Right为矩形右下角x坐标
    rcPop.Top = (SCR_ROW - pos.Y) / 2;  //左上角的y坐标
    rcPop.Bottom = rcPop.Top + pos.Y - 1; //右下角的y坐标

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = n;
    labels.ppLabel = pString;
    COORD aLoc[n];   //定义每个字符串的边距

    //标题位置
    aLoc[0].X = rcPop.Left + 3;
    aLoc[0].Y = rcPop.Top + 1;

    for (i=1; i<n; i++) {
        aLoc[i].X = rcPop.Left + 3; //字符边距
        aLoc[i].Y = rcPop.Top + 3 + i; //即位置向下一行
    }

    str_len = strlen(pString[0]);        //标题设置居中
    aLoc[0].X = rcPop.Left + 3 + (maxlen-str_len)/2;

    str_len = strlen(pString[n-1]);
    aLoc[n-1].X = rcPop.Left + 3 + (maxlen-str_len)/2;  //最后的确定设置居中
    aLoc[n-1].Y = aLoc[n-1].Y + 2;  //+2为绘制空格和分割线所需

    labels.pLoc = aLoc;  //标签束位置

    //热区信息
    areas.num = areanum ;   //热区的个数
    SMALL_RECT aArea[areas.num];                    /*定义数组存放所有热区位置*/
    char aSort[areas.num];                      /*定义数组存放所有热区对应类别*/
    char aTag[areas.num];                         /*定义数组存放每个热区的编号*/
    for (i=0; i<areas.num-1; i++)
    {
        aArea[i].Left = rcPop.Left + 3;  /*热区定位*/
        aArea[i].Top = rcPop.Top + i + 4;
        aArea[i].Right = rcPop.Right - 3;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*热区类别都为0(按钮型)*/
        aTag[i] = i + 1;           /*热区按顺序编号*/
    }
    aArea[areas.num-1].Left = rcPop.Left + 3 + (maxlen-str_len)/2;;  /*热区定位*/
    aArea[areas.num-1].Top = aLoc[n-1].Y ;
    aArea[areas.num-1].Right = aArea[areas.num-1].Left+ strlen(pString[n-1])-1;
    aArea[areas.num-1].Bottom = aArea[areas.num-1].Top;
    aSort[areas.num-1] = 0;       /*热区类别都为0(按钮型)*/
    aTag[areas.num-1] = areas.num;           /*热区按顺序编号*/

    areas.pArea = aArea;/*使热区结构变量areas的成员pArea指向热区位置数组首元素*/
    areas.pSort = aSort;/*使热区结构变量areas的成员pSort指向热区类别数组首元素*/
    areas.pTag = aTag;   /*使热区结构变量areas的成员pTag指向热区编号数组首元素*/
        //弹出窗口
    PopUp(&rcPop, att, &labels, &areas);

    //画上面的线
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 2;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul); //第三个参数是字符个数

    //划下面的线用
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 3 + n;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul); //第三个参数是字符个数

    iRet = DealInput(&areas, tag);

    return iRet;
}

/**
 * 函数名称: PopTextBox
 * 函数功能: 弹出文本框.
 * 输入参数: hot 代表当前状态
 *          1 只有确认
 *          2 只有确认 下一页
 *          3 都有
 *          4 确认 上一页
 * 输出参数: 无
 * 返 回 值: 返回iRet是按键值，tag是标签位置
 *
 * 调用说明: 总共有24行
 */

int PopTextBox(char **ppstring, int hot, int *tag)
{
    HOT_AREA areas;   //热区
    SMALL_RECT rcPop;  //定义了左上角和右下角的坐标的一个矩形。
    COORD pos;  //COORD是定义行与列的坐标结构
    int iRet; //按键信息
    WORD att;  //WORD就是unsigned short，两个字节，DWORD就是unsigned long，四个字节。 此处att用于表示颜色
    LABEL_BUNDLE labels; //标签束
    int i;

    char *str[] = {"确认","下一页","上一页" };
    att =   BACKGROUND_BLUE |
            FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;  /*蓝底黄字*/

    rcPop.Left =0;
    rcPop.Top = 2;
    rcPop.Right = SCR_COL-1;
    rcPop.Bottom = SCR_ROW-3;

    GotoXY(40,1);   //移动标签位置
    printf("%s",*ppstring);

    COORD aLoc[3];   //定位标签束位置
    for(i=0;i<3;i++)
    {
        aLoc[i].X = 40 + i*10;
        aLoc[i].Y = SCR_ROW - 2;
    }

    pos.X=40;
    pos.Y=SCR_ROW-2;

    labels.num = 4;
    labels.ppLabel = str;
    labels.pLoc = aLoc;

    DrawBox(&rcPop);

    switch (hot)   //设置热区
    {
    case 1:
        areas.num = 1;
        SMALL_RECT aArea1[]={{pos.X, pos.Y, pos.X+3, pos.Y}};
        char aSort1[] = {0};
        char aTag1[] = {1};
        areas.pArea = aArea1;
        areas.pSort = aSort1;
        areas.pTag = aTag1;
        break;
    case 2:
        areas.num = 2;
        SMALL_RECT aArea2[]= {{pos.X, pos.Y, pos.X+3, pos.Y},
                              {pos.X+10, pos.Y, pos.X+10+5, pos.Y}};
        char aSort2[] = {0, 0};
        char aTag2[] = {1, 2};
        areas.pArea = aArea2;
        areas.pSort = aSort2;
        areas.pTag = aTag2;
        break;
    case 3:
        areas.num = 3;
        SMALL_RECT aArea3[]= {{pos.X, pos.Y, pos.X+3, pos.Y},
                                {pos.X+10, pos.Y, pos.X+10+5, pos.Y},
                                {pos.X+20, pos.Y, pos.X+20+5, pos.Y}};
        char aSort3[] = {0, 0, 0};
        char aTag3[] = {1, 2, 3};
        areas.pArea = aArea3;
        areas.pSort = aSort3;
        areas.pTag = aTag3;
        break;
    default:
        areas.num = 2;
        SMALL_RECT aArea4[]= {{pos.X, pos.Y, pos.X+3, pos.Y},
                                {pos.X+20, pos.Y, pos.X+20+5, pos.Y}};
        char aSort4[] = {0, 0};
        char aTag4[] = {1, 2};
        areas.pArea = aArea4;
        areas.pSort = aSort4;
        areas.pTag = aTag4;
        break;
    }

    rcPop.Bottom = SCR_ROW-1;
    PopUp(&rcPop,att,&labels,&areas);
    ShowState();
    iRet = DealInput(&areas,tag);   //按键信息

    return iRet;
}


int PopPrompt(char** ppString, int* iHot)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[2];
    char Ch[]="确定    取消";
    pCh[0]=*ppString;
    pCh[1]= Ch;
    int iRet;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 7;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+5, rcPop.Top+5}};
    labels.pLoc = aLoc;

    areas.num = 2;
    SMALL_RECT aArea[] = {{rcPop.Left + 5, rcPop.Top + 5,
                           rcPop.Left + 8, rcPop.Top + 5},
                          {rcPop.Left + 13, rcPop.Top + 5,
                           rcPop.Left + 16, rcPop.Top + 5}};
    char aSort[] = {0, 0};
    char aTag[] = {1, 2};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);

    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 4;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);

    iRet = DealInput(&areas, iHot);

    return iRet;
}








BOOL ExeFunction(int m, int s)
{
    BOOL bRet = TRUE;
    /*函数指针数组，用来存放所有功能函数的入口地址*/
    BOOL (*pFunction[ga_sub_menu_count[0]+ga_sub_menu_count[1]+ga_sub_menu_count[2]+ga_sub_menu_count[3]+ga_sub_menu_count[4]])(void);
    int i, loc;

    /*将功能函数入口地址存入与功能函数所在主菜单号和子菜单号对应下标的数组元素*/
    pFunction[0] = SaveData;
    pFunction[1] = BackupData;
    pFunction[2] = RestoreData;
    pFunction[3] = ExitSys;

    pFunction[4] = StationCode;
    pFunction[5] = RoodCoad;
    pFunction[6] = TruckCode;

    pFunction[7] = FindStationRoad;
    pFunction[8] = FindRoadTime;
    pFunction[9] = FindRoadDistance;
    pFunction[10] = FindRoad;
    pFunction[11] = FindWeight;
    pFunction[12] = FindDriverGoods;
    pFunction[13] = FindDriverPhone;
    pFunction[14] = FindTruck;

    pFunction[15] = FormCreate;
    pFunction[16] = TransportMap;

    pFunction[17] = HelpTopic;
    pFunction[18] = About;

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
    char *pCh= "确认保存数据吗？";
    int iHot = 1;
    int sRet;
    BOOL bRet;

    sRet = PopPrompt(&pCh, &iHot);
    if (sRet == 13 && iHot == 1)
    {
        /////
        bRet = TRUE;
    }
    else
    {
        bRet = TRUE;
    }
    PopOff();


    return bRet;
}

BOOL BackupData(void)
{
    char *pCh = "确认备份数据吗？";
    int iHot = 1;
    int sRet;
    BOOL bRet;

    sRet = PopPrompt(&pCh, &iHot);
    if (sRet == 13 && iHot == 1)
    {
        /////
        bRet = TRUE;
    }
    else
    {
        bRet = TRUE;
    }
    PopOff();
    return bRet;
}

BOOL RestoreData(void)
{
    char *pCh= "确认恢复数据吗？";
    int iHot = 1;
    int sRet;
    BOOL bRet;

    sRet = PopPrompt(&pCh, &iHot);
    if (sRet == 13 && iHot == 1)
    {
        /////
        bRet = TRUE;
    }
    else
    {
        bRet = TRUE;
    }
    PopOff();
    return bRet;
}

BOOL ExitSys(void)
{
    char *pCh = "确认退出系统吗？";
    int iHot = 1;
    int sRet;
    BOOL bRet;

    sRet = PopPrompt(&pCh, &iHot);

    if (sRet== 13 && iHot == 1)
    {
        bRet = FALSE;
    }
    else
    {
        bRet = TRUE;
    }
    PopOff();

    return bRet;
}

BOOL StationCode(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"站点信息维护",
                           "查看已录入的站点",
                           "录入站点信息",
                           "编辑站点信息",
                           "删除站点信息",
                           "返回"
                          };

    int sTag = 1;
    int sRet;

    sRet = PopWindowMenu(plabel_name, 6, 5, &sTag); //弹出窗口

    if (sRet== 13 && sTag == 1)
    {
        PopOff();
        LookStation();
    }
    else if (sRet==13 && sTag ==2)
    {
        PopOff();
        NewStation();
    }
    else if (sRet==13 && sTag == 3)
    {
        PopOff();
        EditStation();
    }
    else if (sRet==13 && sTag == 4)
    {
        PopOff();
        DeleteStation();
    }
    else
    {
        PopOff();
    }

    return bRet;
}

BOOL RoodCoad(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"路线信息维护",
                           "查看现有路线",
                           "录入路线",
                           "编辑路线",
                           "删除路线",
                           "返回"
                          };
    int sTag = 1;
    int sRet;

    sRet = PopWindowMenu(plabel_name, 6, 5, &sTag); //弹出窗口

    if (sRet== 13 && sTag == 1)
    {
        PopOff();
        LookRoad();
    }
    else if (sRet==13 && sTag ==2)
    {
        PopOff();
        NewRoad();
    }
    else if (sRet==13 && sTag == 3)
    {
        PopOff();
        EditRoad();
    }
    else if (sRet==13 && sTag == 4)
    {
        PopOff();
        DeleteRoad();
    }
    else
    {
        PopOff();
    }

    return bRet;
}

BOOL TruckCode(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"车辆及驾驶员信息维护",
                           "编辑车辆及驾驶员信息",
                           "返回"
                          };

    int sTag = 1;
    int sRet;

    sRet = PopWindowMenu(plabel_name, 3, 2, &sTag); //弹出窗口
    if(sRet == 13 && sTag==1)
    {
        PopOff();
        EditTruck();
    }
    else
    {
        PopOff();
    }

    return bRet;
}

BOOL FindStationRoad(void)
{
    BOOL bRet = TRUE;
    char plabel_name[20] ;

    printf("\n\n\t\tdsfsdfdfs");
    getch();
    scanf("%s",plabel_name);
    printf("\n%s",plabel_name);
    getch();
    ReFresh();

    return bRet;
}

BOOL FindRoadTime(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据维护",
                           "子菜单项：学生基本信息",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL FindRoadDistance(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据维护",
                           "子菜单项：住宿缴费信息",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL FindRoad(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据查询",
                           "子菜单项：性别代码",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL FindWeight(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据查询",
                           "子菜单项：学生类别代码",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL FindDriverGoods(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据查询",
                           "子菜单项：学生类别代码",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL FindDriverPhone(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据查询",
                           "子菜单项：宿舍楼信息",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL FindTruck(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据查询",
                           "子菜单项：学生基本信息",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL FormCreate(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据查询",
                           "子菜单项：住宿缴费信息",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL TransportMap(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据统计",
                           "子菜单项：宿舍楼入住率",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL StatStuType(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据统计",
                           "子菜单项：在住学生分类",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL StatCharge(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据统计",
                           "子菜单项：住宿费缴纳情况",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL StatUncharge(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据统计",
                           "子菜单项：住宿费欠缴情况",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL HelpTopic(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"帮助",
                           "本程序可由键盘或鼠标操作，键盘可由上下左右键，空格，回车操作。",
                           "一些快捷键：",
                           "F1  显示帮助主题    Alt+X  退出系统",
                           "Alt+菜单上的字母可快速选择菜单",
                           "",
                           "Attention: 在鼠标操作时，注意将控制台属性中的 快速选择 关闭！",
                           "确认"
                          };

    ShowModule(plabel_name, 8);

    return bRet;
}

BOOL About(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"关于",
                           "程序名称：物流配送管理信息系统",
                           "编译器：CodeBlocks",
                           "本程序由物联网1601班杨钧淮完成",
                           "学号：U201614907",
                           "Vision: 1.0",
                           "确认"
                          };

    ShowModule(plabel_name, 7);

    return bRet;
}

BOOL NewStation(void)
{
    BOOL bRet = TRUE;
    STATION_CODE *tail;
    char new_station[50];
    char* pCh = "是否保存数据？";
    int sHot = 1;

    GotoXY(40,3);
    printf("录入站点信息\n");
    printf("\t\t目前总站点数为：%lu\n\n",gul_station_count);
    printf("\t\t新站点编号为：%lu(自动生成)\n",gul_station_count+1);
    printf("\t\t请输入新站点名称（25字以内）:\n\t\t");

    Show_Cursor(TRUE);
    scanf("%s",new_station);
    Show_Cursor(FALSE);

    tail = gp_station_code;

    if (PopPrompt(&pCh,&sHot) == 13 && sHot == 1)
    {
        while(tail->next) tail = tail->next;
        tail->next = (STATION_CODE*)malloc(sizeof(STATION_CODE));
        tail = tail->next;
        tail->station_num= ++gul_station_count;
        strcpy(tail->station_name,new_station);
        tail->next=NULL;

        bRet = SaveStation();
    }

    PopOff();
    ReFresh();

    return bRet;
}



BOOL LookStation(void)
{
    BOOL bRet = TRUE;
    char *plabel_name = "查看已录入的站点";
    STATION_CODE *tail;
    int i,  page, sTag=1, sRet, count=1, flag = 1;
    const int Col = 4, Row=4;

    page = gul_station_count/PAGE_LINE + 1;
    tail = gp_station_code;
    GotoXY(Col,Row-1);           //移动光标
    printf("序号\t站点名称");   //表头
    if(page==1)   //第一页
    {
        for(i=0;tail;tail = tail->next,i++)
        {
            GotoXY(Col,Row+i);
            printf("%d\t\t%s",tail->station_num,tail->station_name);
        }
        GotoXY(80,1);
        printf("第 %d 页，共 %d 页",count,page);
        PopOff();
        sRet = PopTextBox(&plabel_name, flag, &sTag);
        PopOff();
        ReFresh();
    }
    /**
     * \brief 翻页代码实现！
     *
     */

    //后几页
    else
    {
        flag = 2;   //下一页标记
        for(i=0;tail&&i<23;tail = tail->next,i++)
        {      //先打印
            GotoXY(Col,Row+i);
            printf("%d\t\t%s",tail->station_num,tail->station_name);
        }
        GotoXY(80,1);
        printf("第 %d 页，共 %d 页",count,page);
        sRet = PopTextBox(&plabel_name, flag , &sTag);
        count = 1;   //第几页
        while(TRUE)
        {
            if(sRet == 13 && sTag ==2 && flag ==2)  //2，下一页
            {
                count++;
                if(count == page) flag = 4;
                else flag = 3;
            }
            else if(sRet == 13 && sTag ==2 && flag ==3)  //3, 下一页
            {
                count++;
                if(count == page) flag = 4;
                else flag = 3;
            }
            else if(sRet == 13 && sTag ==3 && flag ==3) //3,上一页
            {
                count--;
                if(count == 1) flag = 2;
                else flag = 3;
            }
            else if(sRet == 13 && sTag ==2 && flag ==4)//4,上一页
            {
                count--;
                if(count == 1) flag = 2;
                else flag = 3;
            }
            else
            {
                break;
            }
            PopOff();
            ReFresh();
            GotoXY(Col,Row-1);           //移动光标
            printf("序号\t站点名称");   //表头
            //for(tail = gp_station_code; (count-1)*23+1 == tail->station_num; tail= tail->next);
            tail = gp_station_code;
            while(TRUE)    //查找
            {
                if((count-1)*23+1 == tail->station_num) break;
                tail= tail->next;
            }
            for(i=0; tail&&i<23; tail = tail->next,i++)
                {
                    GotoXY(Col,Row+i);
                    printf("%d\t\t%s",tail->station_num,tail->station_name);
                }
            if(sTag == 3) sTag = 2;     //stag的处理
            GotoXY(80,1);
            printf("第 %d 页，共 %d 页",count,page);
            sRet = PopTextBox(&plabel_name, flag , &sTag);
        }
        PopOff();
        ReFresh();
    }


    return bRet;
}

BOOL EditStation(void)
{
    BOOL bRet = FALSE;
    STATION_CODE *tail;
    char new_station[50];
    char* pCh = "是否保存数据？";
    int sHot = 1, find;
    char key;

    loop:
    GotoXY(40,3);
    printf("编辑站点信息\n");
    printf("\t\t请输入要更改的站点编号：");
    Show_Cursor(TRUE);
    scanf("%d",&find);
    Show_Cursor(FALSE);

    tail = gp_station_code;
    while(tail->station_num != find )
    {
        tail = tail->next;
        if(tail == NULL)
        {
            printf("\t未查找到该编号，请重新尝试！");
            getch();
            ReFresh();
            goto loop;
        }
    }
    printf("\t\t该站点信息为：\n");
    printf("\t\t编号：%d\t站点名称：%s",tail->station_num,tail->station_name);
    printf("\n\t\t是否更改？ 确认请按Y，取消请按任意键\n");
    //getchar();
    key = getch();

    if (key == 'y' || key == 'Y')
    {
        printf("\t\t请输入站点名称：");
        Show_Cursor(TRUE);
        scanf("%s",new_station);
        Show_Cursor(FALSE);
        if (PopPrompt(&pCh,&sHot) == 13 && sHot == 1)
        {
            strcpy(tail->station_name,new_station);
            bRet = SaveStation();
        }
        PopOff();
    }
    printf("请按任意键继续……");
    getch();
    ReFresh();
    return bRet;
}

BOOL DeleteStation(void)
{
    BOOL bRet = TRUE;
    int find;
    char key;
    STATION_CODE *tail, *uper;

    loop1:
    GotoXY(40,3);
    printf("删除站点信息\n");
    printf("\t\t请输入要删除的站点编号：");
    Show_Cursor(TRUE);
    scanf("%d",&find);
    Show_Cursor(FALSE);
    tail = gp_station_code;

    while(tail->station_num != find )
    {
        uper = tail;
        tail = tail->next;
        if(tail == NULL)
        {
            printf("\t未查找到该编号，请重新尝试！");
            getch();
            ReFresh();
            goto loop1;
        }
    }
    printf("\t\t该站点信息为：\n");
    printf("\t\t编号：%d\t站点名称：%s",tail->station_num,tail->station_name);
    printf("\n\t\t是否删除？ 确认请按Y，取消请按任意键\n");
    key = getch();

    if (key == 'y' || key == 'Y')
    {
        uper->next = tail->next;
        free(tail);
        tail = uper->next;
        while(tail)
        {
            tail->station_num--;
            tail = tail->next;
        }
        gul_station_count--;
        bRet = SaveStation();
        if(bRet) printf("\t\t删除成功！！！");
        printf("\n\n\t\t按任意键继续……");
        getch();
    }
    ReFresh();

    return bRet;
}

BOOL NewRoad(void)
{
    BOOL bRet = TRUE;
    ROAD_DATA *proad;
    STATION_DATA *psta, *pista, *pfsta, *prun;
    TRUCK_DATA *ptruck;
    GOODS_DATA *pgoods, *pgoods_find;
    STATION_CODE *tail;
    int find, station_count=0, sHot=1, goods_count, goods_find;
    char key;
    char init[50],fin[50], roadnum[6], stationnum[10];
    char *plusname="号线";
    char *plussta="号站";
    float fdistance, ftime=0, fvolume, fvolume_up;
    char* pCh = "是否保存数据？";


    GotoXY(40,3);
    printf("录入路线信息\n");

    proad = (ROAD_DATA*)malloc(sizeof(ROAD_DATA));
    proad->station = NULL;
    proad->next = NULL;

    printf("\t\t当前路线条数：%lu\n",gul_road_count);
    gul_road_count++;
    itoa(gul_road_count,roadnum,10);  //数字转换为字符串
    strcat(roadnum,plusname);  //字符串链接

    printf("\n\t\t当前固定配送路线编号（自动生成）：%s\n", roadnum);
    strcpy(proad->road,roadnum);

    printf("\n\t\t请输入固定配送路线名称：\t\t如：关山线\n\t\t");
    Show_Cursor(TRUE);   //显示光标
    scanf("%s",proad->road_name);

    loop2:
    printf("\n\t\t请输入起始站点编号：");
    scanf("%d",&find);

    tail = gp_station_code;

    while(tail->station_num != find )
    {
        tail = tail->next;
        if(tail == NULL)
        {
            printf("\n\t未查找到该编号，请重新尝试！");
            getch();
            goto loop2;
        }
    }
    printf("\n\t\t该站点信息为：\n");
    printf("\t\t编号：%d\t站点名称：%s",tail->station_num,tail->station_name);
    printf("\n\t\t\t确认请按任意键，取消请按N");
    key = getch();
    if(key == 'N' || key == 'n')
    {
        goto loop2;
    }
    proad->init_station = tail->station_num;       //起始站点查找操作
    pista = (STATION_DATA*)malloc(sizeof(STATION_DATA));
    pista->next = NULL;
    pista->station_num = tail->station_num;
    strcpy(init,tail->station_name);
    strcpy(pista->station_name,tail->station_name);

    loop3:
    printf("\n\t\t请输入终点站点编号：");
    scanf("%d",&find);

    tail = gp_station_code;

    while(tail->station_num != find )
    {
        tail = tail->next;
        if(tail == NULL)
        {
            printf("\n\t未查找到该编号，请重新尝试！");
            getch();
            goto loop2;
        }
    }
    Show_Cursor(FALSE);  //隐藏光标
    printf("\t\t该站点信息为：\n");
    printf("\t\t编号：%d\t站点名称：%s",tail->station_num,tail->station_name);
    Show_Cursor(FALSE);  //隐藏光标
    printf("\n\t\t\t确认请按任意键，取消请按N");
    key = getch();
    if(key == 'N' || key == 'n')
    {
        goto loop3;
    }
    proad->fin_station = tail->station_num;         //终止站点 操作
    pfsta = (STATION_DATA*)malloc(sizeof(STATION_DATA));
    pfsta->station_num = tail->station_num;
    strcpy(fin,tail->station_name);
    strcpy(pfsta->station_name,tail->station_name);


    ReFresh(); //换下一页输入
    GotoXY(40,3);
    printf("录入路线信息\n");
    Show_Cursor(TRUE);

    printf("\n\t\t请输入负责人姓名：");
    scanf("%s",proad->charge_person);
    printf("\n\t\t请输入负责人办公室电话 （固定电话）：");
    scanf("%s",proad->call);
    printf("\n\t\t请输入负责人移动电话：");
    scanf("%s",proad->phone);
    printf("\n\t\t请输入负责人电子邮箱：");
    scanf("%s",proad->email);
    Show_Cursor(FALSE);  //隐藏光标
    printf("\n\n\n\t\t请按任意键继续，开始录入经停站点信息");
    key = getch();


    //起始站站点信息录入
    ReFresh(); //换下一页输入
    GotoXY(40,3);
    printf("录入路线信息\n");
    Show_Cursor(TRUE);

    proad->station = pista;

    printf("\n\t\t\t起始站点信息录入\n");
    station_count++;
    itoa(station_count,stationnum,10);   //站点序号生成
    strcat(stationnum,plussta);
    strcpy(pista->station_id,stationnum);
    printf("\n\t\t当前站点序号：%s",pista->station_id);
    printf("\n\t\t起始站点编号：%d",pista->station_num);
    printf("\n\t\t站点名称：%s",pista->station_name);
    printf("\n\t\t请输入在此停留耗时（分钟）：");
    scanf("%f",&pista->stay_time);
    Show_Cursor(FALSE);  //隐藏光标
    printf("\n\t\t该站点是否有交叉固定路线编号，若有请按Y，没有则按任意键跳过");
    key = getch();
    if(key == 'y' || key == 'Y')
    {
        Show_Cursor(TRUE);
        printf("\n\t\t请输入在此站点的交叉固定路线编号：");
        scanf("%s",pista->across_num);
    }
    else
    {
        pista->across_num[0] ='\0';
    }
    strcpy(pista->road, roadnum);
    pista->distance_init = 0;
    pista->distance_up = 0;
    pista->using_time = 0;
    pista->next = NULL;
    Show_Cursor(FALSE);  //隐藏光标
    printf("\n\n\t\t请按任意键进入车辆信息的输入");
    key = getch();

    //车辆信息录入
    ReFresh(); //换下一页输入
    GotoXY(40,3);
    printf("\n\n\t\t录入车辆信息\n");
    Show_Cursor(TRUE);

    ptruck = (TRUCK_DATA*)malloc(sizeof(TRUCK_DATA));
    pista->truck = ptruck;
    //ptruck->station_num = pista->station_num;
    strcpy(ptruck->station_id,pista->station_id);
    strcpy(ptruck->road,roadnum);

    printf("\n\t\t请输入车辆牌照：");
    scanf("%s",ptruck->number);
    printf("\n\t\t请输入司机姓名：");
    scanf("%s",ptruck->driver);
    printf("\n\t\t请输入司机移动电话：");
    scanf("%s",ptruck->phone);
    printf("\n\t\t请输入该车辆容量：");
    scanf("%f",&ptruck->volume);

    Show_Cursor(FALSE);  //隐藏光标
    printf("\n\n\t\t请按任意键进入载货清单的输入");
    key = getch();

    //载货信息录入
    ReFresh(); //换下一页输入
    GotoXY(40,3);
    printf("录入路线信息\n");
    Show_Cursor(TRUE);

    //链头
    fvolume =0;
    printf("\n\t\t\t起始站载货信息录入：\n");
    pgoods = (GOODS_DATA*)malloc(sizeof(GOODS_DATA));
    ptruck->goods = pgoods;
    printf("\n\t\t请按格式 货物名称 数量 输入！\n\n");
    printf("\t\t");
    Show_Cursor(TRUE);
    scanf("%s%f",pgoods->name,&pgoods->quantity);
    fvolume += pgoods->quantity;
    goods_count = 1;
    pgoods->number = goods_count++;
    //pgoods->station_num = ptruck->station_num;
    strcpy(ptruck->station_id,pista->station_id);
    strcpy(pgoods->station_id,pista->station_id);
    strcpy(pgoods->road, pista->road);  //补全路线信息
    Show_Cursor(FALSE);  //隐藏光标
    printf("\t\t当前货物编号：%d  请按任意键继续，完成请按N\n",pgoods->number);
    key = getch();

    while(key != 'n' && key != 'N')
    {
        Show_Cursor(TRUE);
        pgoods->next = (GOODS_DATA*)malloc(sizeof(GOODS_DATA));
        pgoods = pgoods->next;
        printf("\t\t");
        scanf("%s%f",pgoods->name,&pgoods->quantity);
        pgoods->number = goods_count++;
        fvolume += pgoods->quantity;
        //pgoods->station_num = ptruck->station_num;  //补全路线信息
        strcpy(pgoods->road, pista->road);
        strcpy(pgoods->station_id,pista->station_id);
        Show_Cursor(FALSE);  //隐藏光标
        printf("\t\t当前货物编号：%d  请按任意键继续，完成请按N\n",pgoods->number);
        key = getch();
        //if(key == 'n' || key == 'N') break;
    }
    pgoods->next = NULL;
    ptruck->left_volume = ptruck->volume - fvolume;
    fvolume_up = ptruck->volume - fvolume;

    psta=pista;

    printf("\n\t\t接下来请输入下一个站点信息，结束请按N");
    key = getch();

    while(key != 'n' && key != 'N')
    {
        psta->next = (STATION_DATA*)malloc(sizeof(STATION_DATA));
        psta=psta->next;
        psta->next = NULL;
        //经站点信息录入
        ReFresh(); //换下一页输入
        GotoXY(40,3);
        printf("录入路线信息\n");
        Show_Cursor(TRUE);

        printf("\n\t\t\t经停站点信息录入\n");
        station_count++;
        itoa(station_count,stationnum,10);   //站点序号生成
        strcat(stationnum,plussta);
        strcpy(psta->station_id,stationnum);
        strcpy(psta->road, roadnum);
        printf("\n\t\t当前站点序号：%s",psta->station_id);

        loop4:
        printf("\n\t\t请输入站点编号：");
        scanf("%d",&find);
        tail = gp_station_code;

        while(tail->station_num != find )
        {
            tail = tail->next;
            if(tail == NULL)
            {
                printf("\n\t未查找到该编号，请重新尝试！");
                getch();
                goto loop4;
            }
        }
        printf("\n\t\t该站点信息为：\n");
        printf("\t\t编号：%d\t站点名称：%s",tail->station_num,tail->station_name);
        printf("\n\t\t\t确认请按任意键，取消请按N");
        key = getch();
        if(key == 'N' || key == 'n')
        {
            goto loop4;
        }
        strcpy(psta->road, roadnum);
        psta->station_num = find;
        strcpy(psta->station_name,tail->station_name);
        printf("\n\t\t请输入与上一个站点距离（km）：");
        scanf("%f",&psta->distance_up);
        //计算与起始站点距离
        prun = pista;
        fdistance = 0;
        while(prun != NULL)
        {
            fdistance+=prun->distance_up;
            prun=prun->next;
        }
        printf("\n\t\t与起始站点距离（km）（自动生成）：%.2f",fdistance);
        psta->distance_init = fdistance;
        printf("\n\t\t请输入与与上一个站点交通耗时（min）：");
        scanf("%f",&psta->using_time);
        printf("\n\t\t请输入在此停留耗时（分钟）：");
        scanf("%f",&psta->stay_time);
        Show_Cursor(FALSE);  //隐藏光标
        printf("\n\t\t该站点是否有交叉固定路线编号，若有请按Y，没有则按任意键跳过");
        key = getch();
        if(key == 'y' || key == 'Y')
        {
            Show_Cursor(TRUE);
            printf("\n\t\t请输入在此站点的交叉固定路线编号：");
            scanf("%s",psta->across_num);
        }
        else
        {
            psta->across_num[0] ='\0';
        }
        strcpy(psta->road, proad->road);

        ptruck = (TRUCK_DATA*)malloc(sizeof(TRUCK_DATA));
        psta->truck = ptruck;
        strcpy(ptruck->road,roadnum);
        strcpy(ptruck->station_id,stationnum);
        printf("\n\n\t\t请按任意键进入卸货清单的输入");
        key = getch();

        //卸货信息录入
        ReFresh(); //换下一页输入
        GotoXY(40,3);
        printf("录入路线信息\n");
        Show_Cursor(TRUE);

        //链头
        fvolume = 0;
        printf("\n\t\t\t%s站卸货货信息录入：\n",psta->station_name);
        pgoods = (GOODS_DATA*)malloc(sizeof(GOODS_DATA));
        ptruck->goods = pgoods;
        printf("\n\t\t输入货物编号：");
        Show_Cursor(TRUE);
        scanf("%d",&goods_find);
        if(goods_find<=goods_count)
        {
            pgoods_find = pista->truck->goods;
            while(TRUE)
            {
                if(goods_find == pgoods_find->number) break;
                pgoods_find = pgoods_find->next;
                if(pgoods_find == NULL)
                {
                    printf("\t\t错误！请重启输入\n");
                    scanf("%d",&goods_find);
                    pgoods_find = proad->station->truck->goods;
                }
            }
            Show_Cursor(FALSE);  //隐藏光标
            printf("\n\t\t货物名称：%s\t请输入卸货数量：",pgoods_find->name);
            Show_Cursor(TRUE);
            scanf("%f",&pgoods->quantity);
            fvolume += pgoods->quantity;
            pgoods->number = goods_find;
            strcpy(pgoods->name ,pgoods_find->name);
            //pgoods->station_num = ptruck->station_num;  //补全路线信息
            strcpy(ptruck->station_id,psta->station_id);
            strcpy(pgoods->station_id,psta->station_id);
            strcpy(pgoods->road, psta->road);
            printf("\t\t请按任意键继续，完成请按N\n");
            key = getch();
        }
        else printf("错误！");    //找不到
        //第二个开始
        while(key != 'n' && key != 'N')
        {
            Show_Cursor(TRUE);
            pgoods->next = (GOODS_DATA*)malloc(sizeof(GOODS_DATA));
            pgoods = pgoods->next;
            printf("\n\t\t输入货物编号：");
            Show_Cursor(TRUE);
            scanf("%d",&goods_find);
            if(goods_find<=goods_count)
            {
                pgoods_find = pista->truck->goods;
                while(TRUE)
                {
                    if(goods_find == pgoods_find->number) break;
                    pgoods_find = pgoods_find->next;
                    if(pgoods_find == NULL)
                    {
                        printf("\t\t错误！请重启输入\n");
                        scanf("%d",&goods_find);
                        pgoods_find = proad->station->truck->goods;
                    }
                }
                Show_Cursor(FALSE);  //隐藏光标
                printf("\n\t\t货物名称：%s\t请输入卸货数量：",pgoods_find->name);
                Show_Cursor(TRUE);
                scanf("%f",&pgoods->quantity);
                fvolume += pgoods->quantity;
                pgoods->number = goods_find;
                strcpy(pgoods->name ,pgoods_find->name);
                //pgoods->station_num = ptruck->station_num;  //补全路线信息
                strcpy(ptruck->station_id,psta->station_id);
                strcpy(pgoods->station_id,psta->station_id);
                strcpy(pgoods->road, psta->road);
                Show_Cursor(FALSE);  //隐藏光标
                printf("\t\t请按任意键继续，完成请按N\n");
                key = getch();
                //if(key == 'n' || key == 'N') break;
            }
            else printf("错误！");
        }
        pgoods->next = NULL;
        ptruck->volume = proad->station->truck->volume;
        ptruck->left_volume = fvolume_up + fvolume;
        Show_Cursor(FALSE);  //隐藏光标
        printf("\n\t\t按任意键继续录入下一站点，录入终点站请按N\n");
        key = getch();
    }

    /*       终点站货物清单 */

    psta->next = pfsta;
    psta=psta->next;
    psta->next = NULL;
    //终点站信息录入
    ReFresh(); //换下一页输入
    GotoXY(40,3);
    printf("录入路线信息\n");
    Show_Cursor(TRUE);

    printf("\n\t\t\t终止站点信息录入\n");
    station_count++;
    itoa(station_count,stationnum,10);   //站点序号生成
    strcat(stationnum,plussta);
    strcpy(psta->station_id,stationnum);
    strcpy(psta->road, roadnum);
    psta->next=NULL;
    printf("\n\t\t当前站点序号：%s",psta->station_id);
    printf("\n\t\t起始站点编号：%d",psta->station_num);
    printf("\n\t\t站点名称：%s",psta->station_name);
    printf("\n\t\t请输入与上一个站点距离（km）：");
    scanf("%f",&psta->distance_up);
    //计算与起始站点距离
    prun = pista;
    fdistance = 0;
    while(prun)
    {
        fdistance+=prun->distance_up;
        prun=prun->next;
    }
    printf("\n\t\t与起始站点距离（km）（自动生成）：%.2f",fdistance);
    psta->distance_init = fdistance;
    proad->full_distance = fdistance;
    printf("\n\t\t请输入与与上一个站点交通耗时（min）：");
    scanf("%f",&psta->using_time);
    printf("\n\t\t请输入在此停留耗时（分钟）：");
    Show_Cursor(TRUE);
    scanf("%f",&psta->stay_time);
    Show_Cursor(FALSE);  //隐藏光标
    printf("\n\t\t该站点是否有交叉固定路线编号，若有请按Y，没有则按任意键跳过");
    key = getch();
    if(key == 'y' || key == 'Y')
    {
        Show_Cursor(TRUE);
        printf("\n\t\t请输入在此站点的交叉固定路线编号：");
        scanf("%s",psta->across_num);
        Show_Cursor(FALSE);  //隐藏光标
    }
    else
    {
        psta->across_num[0] ='\0';
    }
    strcpy(psta->road, proad->road);

    ptruck = (TRUCK_DATA*)malloc(sizeof(TRUCK_DATA));
    psta->truck = ptruck;
    strcpy(ptruck->road,roadnum);
    strcpy(ptruck->station_id,stationnum);
    printf("\n\n\t\t请按任意键进入卸货清单的输入");
    key = getch();

    //卸货信息录入
    ReFresh(); //换下一页输入
    GotoXY(40,3);
    printf("录入路线信息\n");
    Show_Cursor(TRUE);


    //链头，终点站
    fvolume = 0;
    printf("\n\t\t\t%s站卸货货信息录入：\n",psta->station_name);
    pgoods = (GOODS_DATA*)malloc(sizeof(GOODS_DATA));
    ptruck->goods = pgoods;
    printf("\n\t\t输入货物编号：");
    Show_Cursor(TRUE);
    scanf("%d",&goods_find);
    if(goods_find<=goods_count)
    {
        pgoods_find = pista->truck->goods;
        while(TRUE)
        {
            if(goods_find == pgoods_find->number) break;
            pgoods_find = pgoods_find->next;
            if(pgoods_find == NULL)
            {
                printf("\t\t错误！请重启输入\n");
                scanf("%d",&goods_find);
                pgoods_find = proad->station->truck->goods;
            }
        }
        Show_Cursor(FALSE);  //隐藏光标
        printf("\n\t\t货物名称：%s\t请输入卸货数量：",pgoods_find->name);
        Show_Cursor(TRUE);
        scanf("%f",&pgoods->quantity);
        fvolume += pgoods->quantity;
        Show_Cursor(FALSE);  //隐藏光标
        pgoods->number = goods_find;
        strcpy(pgoods->name ,pgoods_find->name);
        //pgoods->station_num = ptruck->station_num;  //补全路线信息
        strcpy(ptruck->station_id,psta->station_id);
        strcpy(pgoods->station_id,psta->station_id);
        strcpy(pgoods->road, psta->road);
        printf("\t\t请按任意键继续，完成请按N\n");
        key = getch();
    }
    else printf("错误！");    //找不到
    //第二个开始

    while(key != 'n' && key != 'N')
    {
        Show_Cursor(TRUE);
        pgoods->next = (GOODS_DATA*)malloc(sizeof(GOODS_DATA));
        pgoods = pgoods->next;
        printf("\n\t\t输入货物编号：");
        scanf("%d",&goods_find);
        if(goods_find<=goods_count)
        {
            pgoods_find = pista->truck->goods;
            while(TRUE)
            {
                if(goods_find == pgoods_find->number) break;
                pgoods_find = pgoods_find->next;
                if(pgoods_find == NULL)
                {
                    printf("\t\t错误！请重启输入\n");
                    scanf("%d",&goods_find);
                    pgoods_find = proad->station->truck->goods;
                }
            }
            Show_Cursor(FALSE);  //隐藏光标
            printf("\n\t\t货物名称：%s\t请输入卸货数量：",pgoods_find->name);
            Show_Cursor(TRUE);
            scanf("%f",&pgoods->quantity);
            fvolume += pgoods->quantity;
            Show_Cursor(FALSE);  //隐藏光标
            pgoods->number = goods_find;
            strcpy(pgoods->name ,pgoods_find->name);
            //pgoods->station_num = ptruck->station_num;  //补全路线信息
            strcpy(ptruck->station_id,psta->station_id);
            strcpy(pgoods->station_id,psta->station_id);
            strcpy(pgoods->road, psta->road);
            printf("\t\t请按任意键继续，完成请按N\n");
            key = getch();
            //if(key == 'n' || key == 'N') break;
        }
        else printf("错误！");
    }
    pgoods->next = NULL;
    ptruck->volume = proad->station->truck->volume;
    ptruck->left_volume = fvolume_up + fvolume;

    Show_Cursor(FALSE);
    printf("\n\t\t路线信息录入完成！按任意键继续");
    getch();
    ReFresh();


    //概括信息
    ReFresh(); //换下一页输入
    GotoXY(40,3);
    printf("录入路线信息\n");

    printf("\n\t\t\t路线信息一览\n\n\t");
    psta = pista;
    while(psta)
    {
        printf("%s  ",psta->station_name);
        ftime = psta->using_time + psta->stay_time + ftime;
        psta=psta->next;
    }
    printf("\n\n\t\t路线全站点配送总耗时：%.2f",ftime);
    printf("\n\n\t\t路线总站点数：%d",station_count);
    proad->full_time = ftime;
    proad->full_station = (short)station_count;


    Show_Cursor(FALSE);
    printf("\n\n\t\t按任意键继续");
    getch();
    ReFresh();

    //弹出窗口
    if (PopPrompt(&pCh,&sHot) == 13 && sHot == 1)
    {
        //如果保存
        if(gp_head != NULL)
        {
            proad->next = gp_head;
        }
        gp_head = proad;
        bRet = SaveRoad();
        SeceletSort(&gp_head);  //用于排序
    }
    else
    {
        //如果不保存，释放空间
        free(proad);
        free(psta);
        free(pgoods);
        free(pista);
        free(pfsta);
        free(prun);
        free(ptruck);
    }
    PopOff();
    ReFresh();

    return bRet;
}

BOOL LookRoad(void)
{
    BOOL bRet = TRUE;
    char *plabel_name = "查看已录入的路线";
    ROAD_DATA *proad;
    STATION_DATA *pstation, *phstation;
    TRUCK_DATA *ptruck;
    GOODS_DATA *pgoods;
    int   page, sTag=2, sRet, count=1, flag = 1, station_num, i;
    char fname[6], key;

    GotoXY(20,5);
    printf("当前所有路线：");
    proad = gp_head;
    printf("\n\t\t");
    if(gp_head == NULL)
    {
        printf("当前没有路线录入，请前去录入路线！");
        getch();
        ReFresh();
        return FALSE;
    }
    while(proad)
    {
        printf("\n\t\t%s--%s  ",proad->road,proad->road_name);
        proad=proad->next;
    }
    loop5:
    printf("\n\n\t\t请输入要查看的路线(如1号线)：");
    Show_Cursor(TRUE);
    scanf("%s",fname);
    Show_Cursor(FALSE);
    proad = gp_head;
    while(strcmp(fname,proad->road) != 0)
    {
        proad=proad->next;
        if(proad == NULL)
        {
            printf("\n\n\t\t没有找到该路线，请重新输入！按E退出");
            key = getch();
            if(key == 'e' || key == 'E')
            {
                ReFresh();
                return FALSE;
            }
            goto loop5;
        }
    }
    printf("\n\n\t\t已找到该线路！请按任意键继续！");
    getch();
    ReFresh();

    pstation = proad->station;
    phstation = pstation;
    ptruck = phstation->truck;
    page = 2 + proad->full_station;


    GotoXY(40,3);
    printf("路线基本信息");
    printf("\n\n\t\t固定配送路线编号:%s",proad->road);
    printf("\n\n\t\t固定配送路线名称:%s",proad->road_name);
    printf("\n\n\t\t固定配送路线总站点数:%hd",proad->full_station);
    printf("\n\n\t\t固定配送路线总公里数:%.2f",proad->full_distance);
    printf("\n\n\t\t全站点配送总耗时:%.2f",proad->full_time);
    printf("\n\n\t\t起始站点编号: %d",proad->init_station);
    printf("\n\n\t\t终止站点编号: %d",proad->fin_station);
    printf("\n\n\t\t负责人姓名:%s",proad->charge_person);
    printf("\n\n\t\t负责人办公室电话:%s",proad->call);
    printf("\n\n\t\t负责人移动电话:%s",proad->phone);
    printf("\n\n\t\t负责人电子邮箱:%s",proad->email);

    GotoXY(80,1);
    printf("第 %d 页，共 %d 页",count,page);
    flag = 2; //下一页标记
    sRet = PopTextBox(&plabel_name, flag, &sTag);
    PopOff();
    ReFresh();

    count = 1;   //第几页
    while(TRUE)
    {
        if(sRet == 13 && sTag ==2 && flag ==2)  //2，下一页
        {
            count++;
            if(count == page) flag = 4;
            else flag = 3;
        }
        else if(sRet == 13 && sTag ==2 && flag ==3)  //3, 下一页
        {
            count++;
            if(count == page) flag = 4;
            else flag = 3;
        }
        else if(sRet == 13 && sTag ==3 && flag ==3) //3,上一页
        {
            count--;
            if(count == 1) flag = 2;
            else flag = 3;
        }
        else if(sRet == 13 && sTag ==2 && flag ==4)//4,上一页
        {
            count--;
            if(count == 1) flag = 2;
            else flag = 3;
        }
        else
        {
            break;
        }
        PopOff();
        ReFresh();

        switch (count)
        {
        case 1:
            {
                GotoXY(40,3);
                printf("路线基本信息");
                printf("\n\n\t\t固定配送路线编号:%s",proad->road);
                printf("\n\n\t\t固定配送路线名称:%s",proad->road_name);
                printf("\n\n\t\t固定配送路线总站点数:%hd",proad->full_station);
                printf("\n\n\t\t固定配送路线总公里数:%.2f",proad->full_distance);
                printf("\n\n\t\t全站点配送总耗时:%.2f",proad->full_time);
                printf("\n\n\t\t起始站点编号: %d",proad->init_station);
                printf("\n\n\t\t终止站点编号: %d",proad->fin_station);
                printf("\n\n\t\t负责人姓名:%s",proad->charge_person);
                printf("\n\n\t\t负责人办公室电话:%s",proad->call);
                printf("\n\n\t\t负责人移动电话:%s",proad->phone);
                printf("\n\n\t\t负责人电子邮箱:%s",proad->email);
            }
            break;

        case 2:
            {
                GotoXY(40,3);
                printf("车辆基本信息");
                printf("\n\n\t执行配送路线编号:%s",ptruck->road);
                printf("\n\n\t车辆牌照:%s",ptruck->number);
                printf("\n\n\t司机姓名:%s",ptruck->driver);
                printf("\n\n\t司机移动电话:%s",ptruck->phone);
                printf("\n\n\t车辆总容量:%.2f",ptruck->volume);
                //printf("\n\n\t\:%s",);
            }
            break;

            //载货页
        case 3:
            {
                pstation = proad->station;
                GotoXY(40,3);
                printf("站点详细信息");
                printf("\n\n\t站点序号:%s",pstation->station_id);
                printf("\n\t站点编号:%d",pstation->station_num);
                printf("\n\t站点名称:%s",pstation->station_name);
                printf("\n\t与起始站点距离:%.2f（公里）",pstation->distance_init);
                printf("\n\t与上一个站点距离:%.2f（公里）",pstation->distance_up);
                printf("\n\t与上一个站点交通耗时:%.2f（分钟）",pstation->using_time);
                printf("\n\t停留耗时:%.2f（分钟）",pstation->stay_time);
                if(pstation->across_num[0] != '\0') printf("\n\n\t交叉固定路线编号:%s",pstation->across_num);
                printf("\n\n\t\t本站载货");
                printf("\n\t\t货物编号\t货物名称\t数量");
                ptruck = pstation->truck;
                pgoods = ptruck->goods;
                while(pgoods)
                {
                    printf("\n\t\t%d\t\t%s\t\t%.2f",pgoods->number,pgoods->name,pgoods->quantity);
                    pgoods = pgoods->next;
                }
                printf("\n\t\t车辆总容量%.2f,剩余容量:%.2f",ptruck->volume,ptruck->left_volume);

            }
            break;

            //卸货页
        default:
            {
                station_num = count-2;
                pstation = proad->station;
                i=1;
                while(i<station_num)
                {
                    pstation=pstation->next;
                    i++;
                }
                GotoXY(40,3);
                printf("站点详细信息");
                printf("\n\n\t站点序号:%s",pstation->station_id);
                printf("\n\t站点编号:%d",pstation->station_num);
                printf("\n\t站点名称:%s",pstation->station_name);
                printf("\n\t与起始站点距离:%.2f（公里）",pstation->distance_init);
                printf("\n\t与上一个站点距离:%.2f（公里）",pstation->distance_up);
                printf("\n\t与上一个站点交通耗时:%.2f（分钟）",pstation->using_time);
                printf("\n\t停留耗时:%.2f（分钟）",pstation->stay_time);
                if(pstation->across_num[0] != '\0') printf("\n\n\t交叉固定路线编号:%s",pstation->across_num);
                ptruck = pstation->truck;
                pgoods = ptruck->goods;
                printf("\n\n\t\t本站卸货");
                printf("\n\t\t货物编号\t货物名称\t数量");
                while(pgoods)
                {
                    printf("\n\t\t%d\t\t%s\t\t%.2f",pgoods->number,pgoods->name,pgoods->quantity);
                    pgoods = pgoods->next;
                }
                printf("\n\t\t车辆总容量%.2f,剩余容量:%.2f",ptruck->volume,ptruck->left_volume);

            }
            break;
        }
        if(sTag == 3) sTag = 2; //stag处理
        GotoXY(80,1);
        printf("第 %d 页，共 %d 页",count,page);
        sRet = PopTextBox(&plabel_name, flag , &sTag);
    }
    PopOff();
    ReFresh();


    return bRet;
}
BOOL EditRoad(void)
{
    BOOL bRet = TRUE;

    char *plabel_name[] = {"编辑路线",
                            "编辑路线基本信息",
                           "编辑站点基本信息",
                           "插入路线",
                           "插入经停站点",
                           "删除经停站点",
                           "返回"
                          };
    int sTag = 1;
    int sRet;

    sRet = PopWindowMenu(plabel_name, 7, 6, &sTag); //弹出窗口

    //选择操作
    if (sRet== 13 && sTag == 1)
    {
        PopOff();
        EditRoadBase();
    }
    else if (sRet==13 && sTag ==2)
    {
        PopOff();
        EditStationBase();
    }
    else if (sRet==13 && sTag == 3)
    {
        PopOff();
        InsertRoad();
    }
    else if (sRet==13 && sTag == 4)
    {
        PopOff();
        InsertStation();
    }
    else if (sRet==13 && sTag == 5)
    {
        PopOff();
        DeleteStationBase();
    }
    else
    {
        PopOff();
    }

    return bRet;
}
BOOL DeleteRoad(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据统计",
                           "子菜单项：住宿费欠缴情况",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}


BOOL EditTruck(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据统计",
                           "子菜单项：住宿费欠缴情况",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL EditRoadBase(void)
{
    BOOL bRet = TRUE;
    ROAD_DATA *proad;
    int key;
    char fname[6];

    GotoXY(20,5);
    printf("当前所有路线：");
    proad = gp_head;
    printf("\n\t\t");
    if(gp_head == NULL)
    {
        printf("当前没有路线录入，请前去录入路线！");
        getch();
        ReFresh();
        return FALSE;
    }
    while(proad)
    {
        printf("\n\t\t%s--%s  ",proad->road,proad->road_name);
        proad=proad->next;
    }

    //查找路线相关
    loop6:
    printf("\n\n\t\t请输入要更改的路线(如1号线)：");
    Show_Cursor(TRUE);
    scanf("%s",fname);
    Show_Cursor(FALSE);
    proad = gp_head;
    while(strcmp(fname,proad->road) != 0)
    {
        proad=proad->next;
        if(proad == NULL)
        {
            printf("\n\n\t\t没有找到该路线，请重新输入！按E退出");
            key = getch();
            if(key == 'e' || key == 'E')
            {
                ReFresh();
                return FALSE;
            }
            goto loop6;
        }
    }
    printf("\n\n\t\t已找到该线路！请按任意键继续！");
    getch();
    ReFresh();

    GotoXY(40,3);
    printf("请选择要编辑的路线信息");
    printf("\n\t\t不可更改的项目：");
    printf("\n\t\t固定配送路线编号:%s",proad->road);
    printf("\n\t\t固定配送路线总站点数:%hd",proad->full_station);
    printf("\n\t\t固定配送路线总公里数:%.2f",proad->full_distance);
    printf("\n\t\t全站点配送总耗时:%.2f",proad->full_time);
    printf("\n\t\t起始站点编号: %d",proad->init_station);
    printf("\n\t\t终止站点编号: %d",proad->fin_station);
    printf("\n\n\t\t1.固定配送路线名称:%s",proad->road_name);
    printf("\n\t\t2.负责人姓名:%s",proad->charge_person);
    printf("\n\t\t3.负责人办公室电话:%s",proad->call);
    printf("\n\t\t4.负责人移动电话:%s",proad->phone);
    printf("\n\t\t5.负责人电子邮箱:%s",proad->email);
    printf("\n\t\t6.五项信息都修改！");
    printf("\n\n\t\t请输入要修改信息的编号,按其他键返回：");
    ShowCursor(TRUE);
    scanf("%d",&key);

    ReFresh();
    GotoXY(40,3);
    printf("编辑路线信息");
    Show_Cursor(TRUE);

    switch (key)
    {
    case 1:
        printf("\n\n\t\t当前固定配送路线名称:%s",proad->road_name);
        printf("\n\t\t请输入新的固定配送路线名称：");
        scanf("%s",proad->road_name);
        break;

    case 2:
        printf("\n\t\t当前负责人姓名:%s",proad->charge_person);
        printf("\n\t\t请输入新的负责人姓名：");
        scanf("%s",proad->charge_person);
        break;

    case 3:
        printf("\n\t\t当前负责人办公室电话:%s",proad->call);
        printf("\n\t\t请输入新的负责人办公室电话：");
        scanf("%s",proad->call);
        break;

    case 4:
        printf("\n\t\t当前负责人移动电话:%s",proad->phone);
        printf("\n\t\t请输入新的负责人移动电话：");
        scanf("%s",proad->phone);
        break;

    case 5:
        printf("\n\t\t当前负责人电子邮箱:%s",proad->email);
        printf("\n\t\t请输入新的负责人电子邮箱：");
        scanf("%s",proad->email);
        break;

    case 6:
        printf("\n\n\t\t当前固定配送路线名称:%s",proad->road_name);
        printf("\n\t\t请输入新的固定配送路线名称：");
        scanf("%s",proad->road_name);
        printf("\n\t\t当前负责人姓名:%s",proad->charge_person);
        printf("\n\t\t请输入新的负责人姓名：");
        scanf("%s",proad->charge_person);
        printf("\n\t\t当前负责人办公室电话:%s",proad->call);
        printf("\n\t\t请输入新的负责人办公室电话：");
        scanf("%s",proad->call);
        printf("\n\t\t当前负责人移动电话:%s",proad->phone);
        printf("\n\t\t请输入新的负责人移动电话：");
        scanf("%s",proad->phone);
        printf("\n\t\t当前负责人电子邮箱:%s",proad->email);
        printf("\n\t\t请输入新的负责人电子邮箱：");
        scanf("%s",proad->email);
        printf("\n\t\t当前负责人电子邮箱:%s",proad->email);
        printf("\n\t\t请输入新的负责人电子邮箱：");
        scanf("%s",proad->email);
        break;

    default:
        break;
    }
    ShowCursor(FALSE);
    printf("\n\n\t\t请按任意键继续...");
    getch();
    ReFresh();
    SaveRoad();  //保存数据到文件


    return bRet;
}

BOOL EditStationBase(void)
{
    BOOL bRet = TRUE;
    ROAD_DATA *proad;
    STATION_DATA *pstation, *psta;
    STATION_CODE *tail;
    char sfind[10];
    int key, station_num;
    float ftime, fdistance;
    char fname[6];

    GotoXY(20,5);
    printf("当前所有路线：");
    proad = gp_head;
    printf("\n\t\t");
    if(gp_head == NULL)
    {
        printf("当前没有路线录入，请前去录入路线！");
        getch();
        ReFresh();
        return FALSE;
    }
    while(proad)
    {
        printf("\n\t\t%s--%s  ",proad->road,proad->road_name);
        proad=proad->next;
    }

    //查找路线相关
    loop7:
    printf("\n\n\t\t请输入要更改的路线(如1号线)：");
    Show_Cursor(TRUE);
    scanf("%s",fname);
    Show_Cursor(FALSE);
    proad = gp_head;
    while(strcmp(fname,proad->road) != 0)
    {
        proad=proad->next;
        if(proad == NULL)
        {
            printf("\n\n\t\t没有找到该路线，请重新输入！按E退出");
            key = getch();
            if(key == 'e' || key == 'E')
            {
                ReFresh();
                return FALSE;
            }
            goto loop7;
        }
    }
    printf("\n\n\t\t已找到该线路！请按任意键查看线路！\n");
    getch();
    ReFresh();

    GotoXY(40,3);
    pstation = proad->station;
    printf("\n\t\t站点序号\t站点名称");
    while(pstation)
    {
        printf("\n\t\t%s\t%s",pstation->station_id,pstation->station_name);
        pstation=pstation->next;
    }
    loop8:
    printf("\n\n\t\t请输入需要更改的站点（如1号站）：");
    scanf("%s",sfind);
    pstation = proad->station;
    while(pstation)
    {
        if(!strcmp(pstation->station_id,sfind)) break;
        pstation = pstation->next;
        if(pstation == NULL)
        {
            printf("没有找到站点，请重新输入！");
            goto loop8;
        }
    }

    ReFresh();
    GotoXY(40,3);
    printf("该站点详细信息");
    printf("\n\n\n\t站点序号:%s",pstation->station_id);
    printf("\n\t1.站点编号:%d",pstation->station_num);
    printf("\n\t站点名称:%s",pstation->station_name);
    printf("\n\t与起始站点距离:%.2f（公里）",pstation->distance_init);
    printf("\n\t2.与上一个站点距离:%.2f（公里）",pstation->distance_up);
    printf("\n\t3.与上一个站点交通耗时:%.2f（分钟）",pstation->using_time);
    printf("\n\t4.停留耗时:%.2f（分钟）",pstation->stay_time);
    if(pstation->across_num[0] == '\0') printf("\n\n\t5.交叉固定路线编号:空");
    else printf("\n\n\t5.交叉固定路线编号:%s",pstation->across_num);
    printf("\n\n\t6.更改所有");

    Show_Cursor(TRUE);
    printf("\n\n\t\t请输入要更改的编号，按其他键退出...");
    scanf("%d",&key);

    ReFresh();
    GotoXY(40,3);
    Show_Cursor(TRUE);
    switch (key)
    {
    case 1:
        loop10:
        printf("\n\t当前站点编号:%d",pstation->station_num);
        printf("\n\t\t请输入要更改的站点编号：");
        scanf("%d",&station_num);
        tail = gp_station_code;
        while(tail->station_num != station_num )
        {
            tail = tail->next;
            if(tail == NULL)
            {
                printf("\t未查找到该编号，请重新尝试！");
                getch();
                ReFresh();
                goto loop10;
            }
        }
        printf("\t\t该站点信息为：\n");
        printf("\t\t编号：%d\t站点名称：%s",tail->station_num,tail->station_name);
        printf("\n\t\t是否更改？ 确认请按Y，重新输入请按N，返回请按任意键\n");
        while(TRUE)
        {
            key = getch();
            if(key == 'Y' || key == 'y')
            {
                pstation->station_num = tail->station_num;
                strcpy(pstation->station_name, tail->station_name);
                break;
            }
            else if(key == 'N' || key == 'n')
            {
                break;
            }
            else break;
        }
        break;

    case 2:
        printf("\n\t当前与上一个站点距离:%.2f（公里）",pstation->distance_up);
        printf("\n\t请输入新的与上一个站点距离：");
        scanf("%f",&pstation->distance_up);
        break;

    case 3:
        printf("\n\t当前与上一个站点交通耗时:%.2f（分钟）",pstation->using_time);
        printf("\n\t请输入新的与上一个站点交通耗时：");
        scanf("%f",&pstation->using_time);
        break;

    case 4:
        printf("\n\t当前停留耗时:%.2f（分钟）",pstation->stay_time);
        printf("\n\t请输入新的停留耗时：");
        scanf("%f",&pstation->stay_time);
        break;

    case 5:
        printf("\n\n\t当前交叉固定路线编号:%s",pstation->across_num);
        printf("\n\t请输入新的交叉固定路线编号：");
        scanf("%s",pstation->across_num);
        break;

    case 6:
        loop9:
        printf("\n\t当前站点编号:%d",pstation->station_num);
        printf("\n\t\t请输入要更改的站点编号：");
        scanf("%d",&station_num);
        tail = gp_station_code;
        while(tail->station_num != station_num )
        {
            tail = tail->next;
            if(tail == NULL)
            {
                printf("\t未查找到该编号，请重新尝试！");
                getch();
                ReFresh();
                goto loop9;
            }
        }
        printf("\t\t该站点信息为：\n");
        printf("\t\t编号：%d\t站点名称：%s",tail->station_num,tail->station_name);
        printf("\n\t\t是否更改？ 确认请按Y，重新输入请按N，返回请按任意键\n");
        while(TRUE)
        {
            key = getch();
            if(key == 'Y' || key == 'y')
            {
                pstation->station_num = tail->station_num;
                strcpy(pstation->station_name, tail->station_name);
                break;
            }
            else if(key == 'N' || key == 'n')
            {
                break;
            }
            else break;
        }
        printf("\n\t当前与上一个站点距离:%.2f（公里）",pstation->distance_up);
        printf("\n\t请输入新的与上一个站点距离：");
        scanf("%f",&pstation->distance_up);
        printf("\n\t当前与上一个站点交通耗时:%.2f（分钟）",pstation->using_time);
        printf("\n\t请输入新的与上一个站点交通耗时：");
        scanf("%f",&pstation->using_time);
        printf("\n\t当前停留耗时:%.2f（分钟）",pstation->stay_time);
        printf("\n\t请输入新的停留耗时：");
        scanf("%f",&pstation->stay_time);
        printf("\n\n\t当前交叉固定路线编号:%s",pstation->across_num);
        printf("\n\t请输入新的交叉固定路线编号,若有请按Y,没有请按任意键继续：");
        key = getch();
        if(key=='y' || key =='Y') scanf("%s",pstation->across_num);

    default:
        ReFresh();
        return FALSE;
        break;
    }

    //遍历链表完成对路线基本信息的更改
    psta = proad->station;
    while(psta)
    {
        fdistance = psta->distance_up;
        psta->distance_init = fdistance;
        ftime = psta->using_time + psta->stay_time + ftime;
        psta=psta->next;
    }

    proad->full_time = ftime;
    proad->full_distance = fdistance;



    Show_Cursor(FALSE);
    printf("\n\n\t\t按任意键继续");
    getch();
    ReFresh();
    SaveRoad(); // 保存数据



    return bRet;
}

BOOL InsertRoad(void)
{
    BOOL bRet = TRUE;
    NewRoad();
    return bRet;
}

/**< 检查总距离 */

BOOL InsertStation(void)
{
    BOOL bRet = TRUE;
    ROAD_DATA *proad;
    STATION_DATA *pstation, *psta, *prun;
    TRUCK_DATA *ptruck;
    GOODS_DATA *pgoods, *pgoods_find;
    STATION_CODE *tail;
    char fname[6], *plussta="号站" ,stationnum[10], sfind[10];
    int station_count = 0, find, key, goods_find, goods_count=0;
    float  fdistance, fvolume, ftime;

    GotoXY(20,5);
    printf("当前所有路线：");
    proad = gp_head;
    printf("\n\t\t");
    if(gp_head == NULL)
    {
        printf("当前没有路线录入，请前去录入路线！");
        getch();
        ReFresh();
        return FALSE;
    }
    while(proad)
    {
        printf("\n\t\t%s--%s  ",proad->road,proad->road_name);
        proad=proad->next;
    }

    //查找路线相关
    loop10:
    printf("\n\n\t\t请输入要更改的路线(如1号线)：");
    Show_Cursor(TRUE);
    scanf("%s",fname);
    Show_Cursor(FALSE);
    proad = gp_head;
    while(strcmp(fname,proad->road) != 0)
    {
        proad=proad->next;
        if(proad == NULL)
        {
            printf("\n\n\t\t没有找到该路线，请重新输入！按E退出");
            key = getch();
            if(key == 'e' || key == 'E')
            {
                ReFresh();
                return FALSE;
            }
            goto loop10;
        }
    }
    printf("\n\n\t\t已找到该线路！请按任意键查看线路！\n");
    getch();
    ReFresh();

    GotoXY(40,3);
    printf("插入站点");
    pstation = proad->station;
    printf("\n\t\t站点序号\t站点名称");
    while(pstation)
    {
        printf("\n\t\t%s\t%s",pstation->station_id,pstation->station_name);
        pstation=pstation->next;
    }
    loop11:
    printf("\n\n\t\t请输入需要插入站点的前一个站点的序号（如1号站）：");
    Show_Cursor(TRUE);
    scanf("%s",sfind);
    fdistance = 0;
    pstation = proad->station;
    while(pstation)
    {
        station_count++;
        if(!strcmp(pstation->station_id,sfind)) break;
        pstation = pstation->next;
        if(pstation == NULL)
        {
            printf("\n\t\t没有找到站点，请重新输入！");
            goto loop11;
        }
    }
    if(pstation->next == NULL)
    {
        printf("\n\t\t请重新输入，只能插入经停站点");
        goto loop11;
    }


    ReFresh();
    //插入站点
    psta = (STATION_DATA*)malloc(sizeof(STATION_DATA));
    printf("\n\t\t\t经停站点信息录入\n");
    station_count++;
    itoa(station_count,stationnum,10);   //站点序号生成
    strcat(stationnum,plussta);
    strcpy(psta->station_id,stationnum);
    strcpy(psta->road, proad->road);
    printf("\n\t\t当前站点序号：%s",psta->station_id);

    loop12:
    printf("\n\t\t请输入站点编号：");
    Show_Cursor(TRUE);
    scanf("%d",&find);
    tail = gp_station_code;

    //站点查找

    while(tail->station_num != find )
    {
        tail = tail->next;
        if(tail == NULL)
        {
            printf("\n\t未查找到该编号，请重新尝试！");
            getch();
            goto loop12;
        }
    }
    printf("\n\t\t该站点信息为：\n");
    printf("\t\t编号：%d\t站点名称：%s",tail->station_num,tail->station_name);
    printf("\n\t\t\t确认请按任意键，取消请按N");
    key = getch();
    if(key == 'N' || key == 'n')
    {
        goto loop12;
    }
    strcpy(psta->road, proad->road);
    psta->station_num = find;
    strcpy(psta->station_name,tail->station_name);
    printf("\n\t\t请输入与上一个站点距离（km）：");
    scanf("%f",&psta->distance_up);
    //计算与起始站点距离

    fdistance = pstation->distance_init;
    fdistance += psta->distance_up;
    printf("\n\t\t与起始站点距离（km）（自动生成）：%.2f",fdistance);
    psta->distance_init = fdistance;
    printf("\n\t\t请输入与与上一个站点交通耗时（min）：");
    scanf("%f",&psta->using_time);
    printf("\n\t\t请输入在此停留耗时（分钟）：");
    scanf("%f",&psta->stay_time);
    Show_Cursor(FALSE);  //隐藏光标
    printf("\n\t\t该站点是否有交叉固定路线编号，若有请按Y，没有则按任意键跳过");
    key = getch();
    if(key == 'y' || key == 'Y')
    {
        Show_Cursor(TRUE);
        printf("\n\t\t请输入在此站点的交叉固定路线编号：");
        scanf("%s",psta->across_num);
    }
    else
    {
        psta->across_num[0] ='\0';
    }
    strcpy(psta->road, proad->road);

    ptruck = (TRUCK_DATA*)malloc(sizeof(TRUCK_DATA));
    psta->truck = ptruck;
    strcpy(ptruck->road,proad->road);
    strcpy(ptruck->station_id,stationnum);
    printf("\n\n\t\t请按任意键进入卸货清单的输入");
    key = getch();

    //卸货信息录入
    ReFresh(); //换下一页输入
    GotoXY(40,3);
    printf("插入站点\n");
    Show_Cursor(TRUE);

    //对货物数量计数
    pgoods_find = proad->station->truck->goods;
    while(pgoods_find)
    {
        goods_count++;
        pgoods_find = pgoods_find->next;
    }
    //链头
    fvolume = 0;
    printf("\n\t\t\t%s站卸货货信息录入：\n",psta->station_name);
    pgoods = (GOODS_DATA*)malloc(sizeof(GOODS_DATA));
    ptruck->goods = pgoods;
    printf("\n\t\t输入货物编号：");
    Show_Cursor(TRUE);
    scanf("%d",&goods_find);
    if(goods_find<=goods_count)
    {
        pgoods_find = proad->station->truck->goods;
        while(TRUE)
        {
            if(goods_find == pgoods_find->number) break;
            pgoods_find = pgoods_find->next;
            if(pgoods_find == NULL)
            {
                printf("\t\t错误！请重启输入\n");
                scanf("%d",&goods_find);
                pgoods_find = proad->station->truck->goods;
            }
        }
        Show_Cursor(FALSE);  //隐藏光标
        printf("\n\t\t货物名称：%s\t请输入卸货数量：",pgoods_find->name);
        Show_Cursor(TRUE);
        scanf("%f",&pgoods->quantity);
        fvolume += pgoods->quantity;
        pgoods->number = pgoods_find->number;
        strcpy(pgoods->name ,pgoods_find->name);
        //pgoods->station_num = ptruck->station_num;  //补全路线信息
        strcpy(ptruck->station_id,psta->station_id);
        strcpy(pgoods->station_id,psta->station_id);
        strcpy(pgoods->road, psta->road);
        printf("\t\t请按任意键继续，完成请按N\n");
        key = getch();
    }
    else printf("错误！");    //找不到
    //第二个开始
    while(key != 'n' && key != 'N')
    {
        Show_Cursor(TRUE);
        pgoods->next = (GOODS_DATA*)malloc(sizeof(GOODS_DATA));
        pgoods = pgoods->next;
        printf("\n\t\t输入货物编号：");
        Show_Cursor(TRUE);
        scanf("%d",&goods_find);
        if(goods_find<=goods_count)
        {
            pgoods_find = proad->station->truck->goods;
            while(TRUE)
            {
                if(goods_find == pgoods_find->number) break;
                pgoods_find = pgoods_find->next;
                if(pgoods_find == NULL)
                {
                    printf("\t\t错误！请重启输入\n");
                    scanf("%d",&goods_find);
                    pgoods_find = proad->station->truck->goods;
                }
            }
            Show_Cursor(FALSE);  //隐藏光标
            printf("\n\t\t货物名称：%s\t请输入卸货数量：",pgoods_find->name);
            Show_Cursor(TRUE);
            scanf("%f",&pgoods->quantity);
            fvolume += pgoods->quantity;
            pgoods->number = goods_find;
            strcpy(pgoods->name ,pgoods_find->name);
            //pgoods->station_num = ptruck->station_num;  //补全路线信息
            strcpy(ptruck->station_id,psta->station_id);
            strcpy(pgoods->station_id,psta->station_id);
            strcpy(pgoods->road, psta->road);
            Show_Cursor(FALSE);  //隐藏光标
            printf("\t\t请按任意键继续，完成请按N\n");
            key = getch();
            //if(key == 'n' || key == 'N') break;
        }
        else printf("错误！");
    }
    pgoods->next = NULL;
    //对剩余载货量的更改
    ptruck->volume = proad->station->truck->volume;
    ptruck->left_volume = pstation->truck->left_volume + fvolume;
    Show_Cursor(FALSE);  //隐藏光标

    //链接两链表
    psta->next = pstation->next;
    pstation->next = psta;

    //对余下站点剩余载货量和距离的修改
    prun = psta;
    psta = psta->next;
    while(psta)
    {
        ptruck = psta->truck;
        pgoods = ptruck->goods;
        station_count++;
        itoa(station_count,stationnum,10);   //站点序号更改
        strcat(stationnum,plussta);
        strcpy(psta->station_id,stationnum);
        strcpy(ptruck->station_id,stationnum); //！！修改车辆的信息
        while(pgoods)            //以及货物清单信息！！！
        {
            strcpy(pgoods->station_id,stationnum);
            pgoods = pgoods->next;
        }
        strcpy(psta->road, proad->road);
        psta->distance_init = prun->distance_init + psta->distance_up;  //距离
        psta->truck->left_volume += fvolume;  //容量
        psta = psta->next;
        prun = prun->next;
    }

    //遍历链表完成对路线基本信息的更改
    psta = proad->station;
    while(psta)
    {
        fdistance = psta->distance_init;
        ftime = psta->using_time + psta->stay_time + ftime;
        psta=psta->next;
    }

    //保存得到的信息
    proad->full_time = ftime;
    proad->full_distance = fdistance;
    proad->full_station++;

    Show_Cursor(FALSE);
    printf("\n\n\t\t按任意键继续");
    getch();
    ReFresh();
    SaveRoad(); // 保存数据

    return bRet;
}

BOOL DeleteStationBase(void)
{
    BOOL bRet = TRUE;
    ROAD_DATA *proad;
    STATION_DATA *pstation, *psta, *prun;
    TRUCK_DATA *ptruck;
    GOODS_DATA *pgoods, *pgoods_find;
    char fname[6], *plussta="号站" ,stationnum[10], sfind[10];
    int station_count = 0, key;
    float  fdistance, fvolume, ftime;
    char *pCh = "确认删除该站点吗？";
    int iHot = 2;
    int sRet;

    GotoXY(20,5);
    printf("当前所有路线：");
    proad = gp_head;
    printf("\n\t\t");
    if(gp_head == NULL)
    {
        printf("当前没有路线录入，请前去录入路线！");
        getch();
        ReFresh();
        return FALSE;
    }
    while(proad)
    {
        printf("\n\t\t%s--%s  ",proad->road,proad->road_name);
        proad=proad->next;
    }

    //查找路线相关
    loop13:
    printf("\n\n\t\t请输入要更改的路线(如1号线)：");
    Show_Cursor(TRUE);
    scanf("%s",fname);
    Show_Cursor(FALSE);
    proad = gp_head;
    while(strcmp(fname,proad->road) != 0)
    {
        proad=proad->next;
        if(proad == NULL)
        {
            printf("\n\n\t\t没有找到该路线，请重新输入！按E退出");
            key = getch();
            if(key == 'e' || key == 'E')
            {
                ReFresh();
                return FALSE;
            }
            goto loop13;
        }
    }
    printf("\n\n\t\t已找到该线路！请按任意键查看线路！\n");
    getch();
    ReFresh();

    GotoXY(40,3);
    printf("删除经停站点");
    pstation = proad->station;
    printf("\n\t\t站点序号\t站点名称");
    while(pstation)
    {
        printf("\n\t\t%s\t%s",pstation->station_id,pstation->station_name);
        pstation=pstation->next;
    }
    loop14:
    printf("\n\n\t\t请输入需要删除的站点（如1号站）：");
    Show_Cursor(TRUE);
    scanf("%s",sfind);
    fdistance = 0;
    pstation = proad->station;
    while(pstation)
    {
        station_count++;
        if(!strcmp(pstation->station_id,sfind)) break;
        psta = pstation;
        pstation = pstation->next;
        if(pstation == NULL)
        {
            printf("\n\t\t没有找到站点，请重新输入！");
            goto loop14;
        }
    }
    if(pstation->next == NULL || pstation == proad->station)
    {
        printf("\n\t\t请重新输入，只能删除经停站点");
        goto loop14;
    }

    ReFresh();
    GotoXY(40,3);
    printf("删除经停站点");
    printf("\n\n\t\t当前选中的路线和站点为：%s————%s",pstation->road,pstation->station_id);

    sRet = PopPrompt(&pCh, &iHot);
    if (sRet == 13 && iHot == 1)
    {
        bRet = TRUE;
        //先连接两链表
        psta->next = pstation->next;

        ptruck = pstation->truck;
        pgoods = ptruck->goods;
        fvolume = 0;
        //释放空间
        while(pgoods)
        {
            fvolume += pgoods->quantity; //对货物数量计数
            pgoods_find = pgoods;
            pgoods = pgoods->next;
            free(pgoods_find);
        }
        free(ptruck);
        free(pstation);

        //更改后续站点信息
        prun = psta;
        psta = psta->next;
        station_count--;
        while(psta)
        {
            ptruck = psta->truck;
            pgoods = ptruck->goods;
            station_count++;
            itoa(station_count,stationnum,10);   //站点序号更改
            strcat(stationnum,plussta);
            strcpy(psta->station_id,stationnum);
            strcpy(ptruck->station_id,stationnum); //！！修改车辆的信息
            while(pgoods)            //以及货物清单信息！！！
            {
                strcpy(pgoods->station_id,stationnum);
                pgoods = pgoods->next;
            }
            psta->distance_init = prun->distance_init + psta->distance_up;  //距离
            psta->truck->left_volume -= fvolume;  //容量
            psta = psta->next;
            prun = prun->next;
        }

        //遍历链表完成对路线基本信息的更改
        psta = proad->station;
        while(psta)
        {
            fdistance = psta->distance_init;
            ftime = psta->using_time + psta->stay_time + ftime;
            psta=psta->next;
        }

        //保存得到的信息
        proad->full_time = ftime;
        proad->full_distance = fdistance;
        proad->full_station--;

        ReFresh();
        SaveRoad(); // 保存数据

    }
    else
    {
        bRet = FALSE;
    }
    PopOff();
    ReFresh();

    Show_Cursor(FALSE);
    printf("\n\n\t\t删除成功， 按任意键继续");
    getch();
    ReFresh();


    return bRet;

}

BOOL InsertGoods(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据统计",
                           "子菜单项：住宿费欠缴情况",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}




