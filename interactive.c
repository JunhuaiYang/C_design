#include"global.h"

/**
 *
 * \brief 该文件主要保存交互方面的函数
 *
 */

 BOOL ExeFunction(int m, int s)
{
    BOOL bRet = TRUE;
    /*函数指针数组，用来存放所有功能函数的入口地址*/
    BOOL (*pFunction[ga_sub_menu_count[0]+ga_sub_menu_count[1]+ga_sub_menu_count[2]+ga_sub_menu_count[3]+ga_sub_menu_count[4]])(void);
    int i, loc;

    /*将功能函数入口地址存入与功能函数所在主菜单号和子菜单号对应下标的数组元素*/
    pFunction[0] = SaveData;
    pFunction[1] = ExitSys;

    pFunction[2] = StationCode;
    pFunction[3] = RoodCoad;
    pFunction[4] = TruckCode;

    pFunction[5] = FindStationRoad;
    pFunction[6] = FindRoadTime;
    pFunction[7] = FindRoadDistance;
    pFunction[8] = FindRoad;
    pFunction[9] = FindWeight;
    pFunction[10] = FindDriverGoods;
    pFunction[11] = FindDriverPhone;
    pFunction[12] = FindTruck;

    pFunction[13] = FormCreate;
    pFunction[14] = TransportMap;

    pFunction[15] = HelpTopic;
    pFunction[16] = About;

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
        SaveRoad();
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
                           "插入货物清单",
                           "编辑货物清单",
                           "删除货物清单",
                           "返回"
                          };

    int sTag = 1;
    int sRet;

    sRet = PopWindowMenu(plabel_name, 6, 5, &sTag); //弹出窗口
    if(sRet == 13 && sTag==1)
    {
        PopOff();
        EditTruck();
    }
    else if (sRet==13 && sTag ==2)
    {
        PopOff();
        InsertGoods();
    }
    else if (sRet==13 && sTag == 3)
    {
        PopOff();
        EditGoods();
    }
    else if (sRet==13 && sTag == 4)
    {
        PopOff();
        DeleteGoods();
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
    ROAD_DATA *proad;
    STATION_DATA *pstation;
    int find;
    STATION_CODE *tail;
    char tfind[20];
    int flag = 0;

    GotoXY(40,3);
    printf("查询站点路线");
    loop21:
    printf("\n\n\t\t请输入要查询的站点编号：");
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
            goto loop21;
        }
    }
    printf("\n\t\t该站点编号为： %d     站点名称为：%s",tail->station_num,tail->station_name);
    printf("\n\n\t\t经过该站点的路线有：\n");
    strcpy(tfind, tail->station_name);
    proad = gp_head;
    while(proad)
    {
        pstation = proad->station;
        while(pstation)
        {
            if(!strcmp(pstation->station_name,tfind))
            {
                flag = 1;
                printf("\t\t%s---%s\n",proad->road, proad->road_name);
                break;
            }
        }
        proad = proad->next;
    }
    if(flag == 0) printf("\n\t\t没有找到线路");

    printf("\n\t\t请按任意键继续");
    getch();
    ReFresh();



    return bRet;
}

BOOL FindRoadTime(void)
{
    BOOL bRet = TRUE;
    float longest, shortest;
    ROAD_DATA *proad;
    char road_num[6],road_name[10];


    GotoXY(40,3);
    printf("查询耗时最长及最短的路线");

    if(gp_head != NULL)
    {
        proad = gp_head;
        longest = proad->full_time;
        strcpy(road_num,proad->road);
        strcpy(road_name,proad->road_name);
        proad = proad->next;
        while(proad)
        {
            if(proad->full_time > longest)
            {
                longest = proad->full_time;
                strcpy(road_num,proad->road);
                strcpy(road_name,proad->road_name);
            }
            proad = proad->next;
        }
        printf("\n\n\t\t耗时最长的路线为 %s----%s  路线所需时间为：%.2f（分钟）",road_num,road_name,longest);

        proad = gp_head;
        shortest = proad->full_time;
        strcpy(road_num,proad->road);
        strcpy(road_name,proad->road_name);
        proad = proad->next;
        while(proad)
        {
            if(proad->full_time < shortest)
            {
                shortest = proad->full_time;
                strcpy(road_num,proad->road);
                strcpy(road_name,proad->road_name);
            }
            proad = proad->next;
        }
        printf("\n\n\t\t耗时最短的路线为 %s----%s  路线所需时间为：%.2f（分钟）",road_num,road_name,shortest);
    }
    else  //空链头处理
    {
        printf("\n\n\t\t当前没有录入路线，请去录入路线");
    }
    printf("\n\n\t\t请按任意键继续...");
    getch();
    ReFresh();

    return bRet;
}

BOOL FindRoadDistance(void)
{
BOOL bRet = TRUE;
    float longest, shortest;
    ROAD_DATA *proad;
    char road_num[6],road_name[10];


    GotoXY(40,3);
    printf("查询路线公里数最长及最短的路线");

    if(gp_head != NULL)
    {
        proad = gp_head;
        longest = proad->full_distance;
        strcpy(road_num,proad->road);
        strcpy(road_name,proad->road_name);
        proad = proad->next;
        while(proad)
        {
            if(proad->full_distance > longest)
            {
                longest = proad->full_distance;
                strcpy(road_num,proad->road);
                strcpy(road_name,proad->road_name);
            }
            proad = proad->next;
        }
        printf("\n\n\t\t公里数最长的路线为 %s----%s  路线总里程为：%.2f（公里）",road_num,road_name,longest);

        proad = gp_head;
        shortest = proad->full_distance;
        strcpy(road_num,proad->road);
        strcpy(road_name,proad->road_name);
        proad = proad->next;
        while(proad)
        {
            if(proad->full_distance < shortest)
            {
                shortest = proad->full_distance;
                strcpy(road_num,proad->road);
                strcpy(road_name,proad->road_name);
            }
            proad = proad->next;
        }
        printf("\n\n\t\t公里数最短的路线为 %s----%s  路线总里程为：%.2f（公里）",road_num,road_name,shortest);
    }
    else  //空链头处理
    {
        printf("\n\n\t\t当前没有录入路线，请去录入路线");
    }
    printf("\n\n\t\t请按任意键继续...");
    getch();
    ReFresh();

    return bRet;
}

BOOL FindRoad(void)
{
    BOOL bRet = TRUE;
    ROAD_DATA *proad;
    STATION_DATA *pstation;
    char fname[6];
    char key;

    GotoXY(40,3);
    printf("查询指定路线的所有经停站点。");

    printf("\n\n\t\t\t当前所有路线：");
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
    loop24:
    printf("\n\n\t\t请输入要查询的路线(如1号线)：");
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
            goto loop24;
        }
    }
    printf("\n\t\t已找到该线路！该路线的经停站点信息为：\n\n\t\t");
    pstation = proad->station;
    while(pstation->next)
    {
        printf("%s -> ",pstation->station_name);
        pstation = pstation->next;
    }
    printf("%s",pstation->station_name);

    printf("\n\n\t\t请按任意键继续");
    getch();
    ReFresh();

    return bRet;
}

BOOL FindWeight(void)
{
    BOOL bRet = TRUE;
    ROAD_DATA *proad;
    STATION_DATA *pstation;
    TRUCK_DATA *ptruck, *ptruck_head;
    char fstation[20];
    float fvloume;
    int flag = 0;

    GotoXY(30,3);
    printf("查询经停某站点且剩余可载货容量为指定重量的车辆");

    if(gp_head != NULL)
    {
        printf("\n\n\t\t请输入要查询的站点:");
        Show_Cursor(TRUE);
        scanf("%s",fstation);
        printf("\n\t\t请输入指定的重量：");
        scanf("%f",&fvloume);
        Show_Cursor(FALSE);

        ReFresh();
        GotoXY(30,3);
        printf("查询经停某站点且剩余可载货容量为指定重量的车辆");
        printf("\n\n\t\t查询结果为：");
        proad = gp_head;
        while(proad)
        {
            pstation = proad->station;
            ptruck_head = pstation->truck;
            while(pstation)
            {
                ptruck = pstation->truck;
                if(strcmp(pstation->station_name,fstation) == 0)
                {
                    if(ptruck->left_volume > fvloume)
                    {
                        flag = 1;
                        printf("\n\n\t\t%s---%s\t站点序号:%s\t站点名称:%s\n\t\t车辆牌照：%s\t司机姓名：%s\t司机移动电话：%s\n\t\t剩余可载货量为:%.2f",
                               proad->road,proad->road_name,pstation->station_id,pstation->station_name,ptruck_head->number,ptruck_head->driver,ptruck_head->phone,ptruck->left_volume);
                    }
                }
                pstation = pstation->next;
            }
            proad = proad->next;
        }
        if(flag == 0)
        {
            printf("\n\n\t\t没有找到！");
        }
    }
    else
    {
        printf("\n\t\t当前没有路线录入！请去录入路线");
    }

    printf("\n\t\t请按任意键继续...");
    getch();
    ReFresh();

    return bRet;
}

BOOL FindDriverGoods(void)
{
    BOOL bRet = TRUE;
    char fdriver[8];
    ROAD_DATA *proad;
    STATION_DATA *pstation;
    TRUCK_DATA *ptruck;
    GOODS_DATA *pgoods;
    int flag = 0;

    GotoXY(35,3);
    printf("查询指定司机的配送清单");
    printf("\n\n\t\t请输入需要查询的司机：");
    Show_Cursor(TRUE);
    scanf("%s",fdriver);

    if(gp_head != NULL)
    {
        proad = gp_head;
        while(proad)
        {
            pstation = proad->station;
            ptruck = pstation->truck;
            if(!strcmp(fdriver,ptruck->driver))
            {
                flag = 1;
                printf("\n\n\t\t%s---%s\t站点序号：%s\t站点名称：%s",proad->road,proad->road_name,pstation->station_id,pstation->station_name);
                printf("\n\t\t本站载货");
                pgoods = ptruck->goods;
                printf("\n\t\t货物编号\t货物名称\t数量（吨）");
                while(pgoods)
                {
                    printf("\n\t\t%d\t\t%s\t%.2f",pgoods->number,pgoods->name,pgoods->quantity);
                    pgoods = pgoods->next;
                }
                pstation = pstation->next;
                while(pstation)
                {
                    ptruck = pstation->truck;
                    pgoods = ptruck->goods;
                    printf("\n\n\t\t%s---%s\t站点序号：%s\t站点名称：%s",proad->road,proad->road_name,pstation->station_id,pstation->station_name);
                    printf("\n\t\t本站卸货");
                    printf("\n\t\t货物编号\t货物名称\t数量（吨）");
                    while(pgoods)
                    {
                        printf("\n\t\t%d\t\t%s\t%.2f",pgoods->number,pgoods->name,pgoods->quantity);
                        pgoods = pgoods->next;
                    }
                    pstation = pstation->next;
                }
            }
            proad = proad->next;
        }
        if(flag == 0) printf("\n\t\t没有找到该司机！");
    }
    else
    {
        printf("\n\t\t当前没有路线录入！请去录入路线");
    }

    printf("\n\n\t\t请按任意键继续...");
    getch();
    ReFresh();


    return bRet;
}

BOOL FindDriverPhone(void)
{
    BOOL bRet = TRUE;
    char fdriver[8];
    ROAD_DATA *proad;
    STATION_DATA *pstation;
    TRUCK_DATA *ptruck;
    int flag = 0;

    GotoXY(35,3);
    printf("查询指定车辆的司机联系方式");
    printf("\n\n\t\t请输入需要查询的车牌号：");
    Show_Cursor(TRUE);
    scanf("%s",fdriver);

    if(gp_head != NULL)
    {
        proad = gp_head;
        while(proad)
        {
            pstation = proad->station;
            ptruck = pstation->truck;
            if(!strcmp(fdriver,ptruck->number))
            {
                flag = 1;
                printf("\n\n\t\t%s---%s",proad->road,proad->road_name);
                printf("\n\t\t车牌号：%s\t司机姓名：%s\n\t\t司机联系方式:%s",ptruck->number,ptruck->driver,ptruck->phone);
            }
            proad = proad->next;
        }
        if(flag == 0) printf("\n\t\t没有找到该车辆！");
    }
    else
    {
        printf("\n\t\t当前没有路线录入！请去录入路线");
    }

    printf("\n\n\t\t请按任意键继续...");
    getch();
    ReFresh();




    return bRet;
}

BOOL FindTruck(void)
{
    BOOL bRet = TRUE;
    char fdriver[8];
    ROAD_DATA *proad;
    STATION_DATA *pstation;
    TRUCK_DATA *ptruck;
    int flag = 0;


    GotoXY(35,3);
    printf("查询指定车辆的配送路线");
    printf("\n\n\t\t请输入需要查询的车牌号：");
    Show_Cursor(TRUE);
    scanf("%s",fdriver);

    if(gp_head != NULL)
    {
        proad = gp_head;
        while(proad)
        {
            pstation = proad->station;
            ptruck = pstation->truck;
            if(!strcmp(fdriver,ptruck->number))
            {
                flag = 1;
                printf("\n\n\t\t当前路线为:%s---%s\n\t\t",proad->road,proad->road_name);
                while(pstation->next)
                {
                    printf("%s->",pstation->station_name);
                    pstation = pstation->next;
                }
                printf("%s",pstation->station_name);

            }
            proad = proad->next;
        }
        if(flag == 0) printf("\n\t\t没有找到该车辆！");
    }
    else
    {
        printf("\n\t\t当前没有路线录入！请去录入路线");
    }
    Show_Cursor(FALSE);
    printf("\n\n\t\t请按任意键继续");
    getch();
    ReFresh();


    return bRet;
}

BOOL FormCreate(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"提示信息",
                           "该功能暂未完成（请查看配送计划图）",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL TransportMap(void)
{
    BOOL bRet = TRUE;
    ROAD_DATA *proad;
    STATION_DATA *pstation;
    TRUCK_DATA *ptruck;
    GOODS_DATA *pgoods;
    SMALL_RECT Rc;
    COORD pos,size;
    int page = 0, page_count = 0, line_count = 0, i = 0;


    ClearScreen();
    if(gp_head == NULL)
    {
        printf("\t\t当前没有录入路线，请先录入路线！");
        getch();
        return FALSE;
    }

    //总页数计数
    proad = gp_head;
    while(proad)
    {
        page++;
        proad = proad->next;
    }

    size.X = SCR_COL;
    size.Y = page*80;  //大概估计行数
    SetConsoleScreenBufferSize(gh_std_out, size);
    //显示报表
    proad = gp_head;
    while(proad)
    {
        //初始化
        pstation = proad->station;
        ptruck = pstation->truck;
        pgoods = ptruck->goods;
        //绘制边框初始化
        Rc.Left = 4;
        Rc.Right = 95;
        Rc.Top = line_count;
        page_count++;
        line_count++;
        GotoXY(40,line_count);
        printf("%s 配送计划",ptruck->number);
        line_count++;
        pos.X = 5;
        pos.Y = line_count;
        //绘制分割
        for(i=0;i<90;i++,pos.X++)
            WriteConsoleOutputCharacter(gh_std_out,"-",1,pos,&ul);
        line_count+=2;
        GotoXY(0,line_count);
        printf("\t\t\t\t司机姓名：%s    司机移动电话：%s",ptruck->driver,ptruck->phone);
        line_count+=2;
        GotoXY(0,line_count);
        printf("\t固定配送路线编号:%s    固定配送路线名称:%s",proad->road,proad->road_name);
        line_count++;
        printf("\n\t固定配送路线总站点数:%d    固定配送路线总公里数:%.2f    全站点配送总耗时:%.2f（分钟）",proad->full_station,proad->full_distance,proad->full_time);
        line_count+=2;
        printf("\n\n\t负责人姓名:%s\t\t  负责人办公室电话:%s",proad->charge_person,proad->call);
        line_count++;
        printf("\n\t负责人移动电话:%s\t  负责人电子邮箱:%s",proad->phone,proad->email);

        line_count+=2;
        printf("\n\n\t路线经停站点：");
        i = 0;
        line_count++;
        printf("\n\t");
        while(pstation->next)
        {
            i++;
            if(i%8 == 0)
            {
                printf("\n\t");
                line_count++;
            }
            printf("%s -> ",pstation->station_name);
            pstation = pstation->next;
        }
        printf("%s\n",pstation->station_name);
        line_count++;

        //送货清单
        line_count++;
        pos.X = 5;
        pos.Y = line_count;
        //绘制分割
        for(i=0;i<90;i++,pos.X++)
            WriteConsoleOutputCharacter(gh_std_out,"-",1,pos,&ul);
        printf("\n\n\t\t\t\t\t送货清单");
        line_count++;

        pstation = proad->station;
        ptruck = pstation->truck;
        pgoods = ptruck->goods;
        printf("\n\n\t\t\t站点序号：%s\t站点名称：%s\n",pstation->station_id,pstation->station_name);
        line_count+=3;
        printf("\n\t\t本站载货");
        line_count++;
        printf("\n\t\t货物编号\t货物名称\t数量（吨）");
        line_count++;
        while(pgoods)
        {
            printf("\n\t\t%d\t\t%-16s%.2f",pgoods->number,pgoods->name,pgoods->quantity);
            line_count++;
            pgoods = pgoods->next;
        }
        pstation = pstation->next;
        while(pstation)
        {
            ptruck = pstation->truck;
            pgoods = ptruck->goods;
            printf("\n\n\t\t\t站点序号：%s\t站点名称：%s\n",pstation->station_id,pstation->station_name);
            printf("\n\t\t本站卸货");
            printf("\n\t\t货物编号\t货物名称\t数量（吨）");
            line_count+=5;
            while(pgoods)
            {
                printf("\n\t\t%d\t\t%-16s%.2f",pgoods->number,pgoods->name,pgoods->quantity);
                line_count++;
                pgoods = pgoods->next;
            }
            pstation = pstation->next;
        }

        //留边距
        line_count+=3;

        Rc.Bottom = line_count;
        //绘制边框
        DrawBox(&Rc);
        line_count++;
        GotoXY(85,line_count);
        printf("第%d页，共%d页",page_count,page);

        line_count+=5;

        proad = proad->next;
    }
    //重新设置屏幕缓冲区使其刚好合适
    size.Y = line_count+4;
    SetConsoleScreenBufferSize(gh_std_out, size);

    printf("\n\n\n\t\t请按任意键退出...");
    getch();
    ReFresh();
    size.X = SCR_COL;
    size.Y = SCR_ROW;
    SetConsoleScreenBufferSize(gh_std_out, size); /*设置窗口缓冲区大小100*30*/



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
                           "本程序站点按钮为站点代码表的更改，路线按钮和车辆按钮才是对路线信息的编辑。",
                           "",
                           "Attention: 在鼠标操作时，注意将控制台属性中的 快速选择 关闭！",
                           "确认"
                          };

    ShowModule(plabel_name, 9);

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
    printf("\n\t\t请输入该车辆容量（吨）：");
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
    printf("\n\t\t请按格式 货物名称 数量（吨） 输入！\n\n");
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
            printf("\n\t\t货物名称：%s\t请输入卸货数量（吨）：",pgoods_find->name);
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
                printf("\n\t\t货物名称：%s\t请输入卸货数量（吨）：",pgoods_find->name);
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
        fvolume_up = ptruck->left_volume;
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
        printf("\n\t\t货物名称：%s\t请输入卸货数量（吨）：",pgoods_find->name);
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
            printf("\n\t\t货物名称：%s\t请输入卸货数量（吨）：",pgoods_find->name);
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
    printf("\n\n\t\t路线全站点配送总耗时（min）：%.2f",ftime);
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
    printf("\n\n\t\t全站点配送总耗时:%.2f（分钟）",proad->full_time);
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
                printf("\n\n\t\t全站点配送总耗时:%.2f（分钟）",proad->full_time);
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
                printf("\n\n\t车辆总容量:%.2f（吨）",ptruck->volume);
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
                    printf("\n\t\t%d\t\t%-16s%.2f",pgoods->number,pgoods->name,pgoods->quantity);
                    pgoods = pgoods->next;
                }
                printf("\n\t\t车辆总容量%.2f（吨）,剩余容量:%.2f（吨）",ptruck->volume,ptruck->left_volume);

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
                    printf("\n\t\t%d\t\t%-16s%.2f",pgoods->number,pgoods->name,pgoods->quantity);
                    pgoods = pgoods->next;
                }
                printf("\n\t\t车辆总容量%.2f（吨）,剩余容量:%.2f（吨）",ptruck->volume,ptruck->left_volume);

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
    ROAD_DATA *proad, *proad_up = NULL;
    STATION_DATA *pstation, *pstation_up;
    TRUCK_DATA *ptruck;
    GOODS_DATA *pgoods, *pgoods_up;
    char *pCh = "确认删除该路线吗？";
    int iHot = 2;
    int sRet, road_count = 1;
    char *plusname="号线";
    char roadnum[6], fname[6], key;

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
    loop15:
    printf("\n\n\t\t请输入要删除的路线(如1号线)：");
    Show_Cursor(TRUE);
    scanf("%s",fname);
    Show_Cursor(FALSE);
    proad = gp_head;
    while(strcmp(fname,proad->road) != 0)
    {
        road_count++;
        proad_up = proad;
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
            goto loop15;
        }
    }
    printf("\n\n\t\t已找到该线路！请按任意键查看线路！\n");
    getch();
    ReFresh();

    GotoXY(40,3);
    printf("删除路线");
    pstation = proad->station;
    printf("\n\n\t\t当前路线经停站点\n\n\t\t");
    while(pstation)
    {
        printf("%s\t",pstation->station_name);
        pstation=pstation->next;
    }

    sRet = PopPrompt(&pCh, &iHot);
    if (sRet== 13 && iHot == 1)
    {
        bRet = TRUE;

        //对链头的处理
        if(proad == gp_head)
        {
            gp_head = proad->next;
            //proad_up = gp_head;
        }
        else
        {
        //链接两路线
        proad_up->next = proad->next;
        }

        //删除信息,释放空间
        pstation = proad->station;
        ptruck = pstation->truck;
        pgoods = ptruck->goods;

        while(pstation)
        {
            pstation_up = pstation;
            pstation = pstation->next;

            while(pgoods)
            {
                pgoods_up = pgoods;
                pgoods = pgoods->next;
                free(pgoods_up);
            }
            free(ptruck);
            free(pstation_up);
        }
        free(proad);

        //更改后面的路线信息
        gul_road_count--;

        if(proad_up != NULL) proad = proad_up->next;
        else proad = gp_head;

        while(proad)
        {
            itoa(road_count,roadnum,10);  //数字转换为字符串
            strcat(roadnum,plusname);  //字符串链接
            strcpy(proad->road, roadnum);
            pstation = proad->station;
            //更名
            while(pstation)
            {
                strcpy(pstation->road,roadnum);
                ptruck = pstation->truck;
                strcpy(ptruck->road,roadnum);
                pgoods = ptruck->goods;
                while(pgoods)
                {
                    strcpy(pgoods->road, roadnum);
                    pgoods = pgoods->next;
                }
                pstation = pstation->next;
            }
            road_count++;
            proad = proad->next;
        }

        SaveRoad();
    }
    else
    {
        bRet = FALSE;
    }
    PopOff();
    ReFresh();

    if(bRet == TRUE) printf("\n\n\t\t删除成功，请按任意键继续"), getch();
    ReFresh();

    return bRet;
}


BOOL EditTruck(void)
{
    BOOL bRet = TRUE;
    ROAD_DATA *proad;
    STATION_DATA *pstation;
    TRUCK_DATA *ptruck;
    float fvolume;
    int key;
    char fname[6];

    GotoXY(40,3);
    printf("编辑车辆信息");
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
    loop15:
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
            goto loop15;
        }
    }
    printf("\n\n\t\t已找到该线路！请按任意键查看线路！\n");
    getch();
    ReFresh();

    GotoXY(40,3);
    printf("编辑车辆信息");
    pstation = proad->station;
    ptruck = pstation->truck;

    printf("\n\n\t执行配送路线编号:%s",ptruck->road);
    printf("\n\n\t请选择要更改的信息：");
    printf("\n\n\t1.车辆牌照:%s",ptruck->number);
    printf("\n\t2.司机姓名:%s",ptruck->driver);
    printf("\n\t3.司机移动电话:%s",ptruck->phone);
    printf("\n\t4.车辆总容量:%.2f",ptruck->volume);
    printf("\n\t5.全部更改！");
    printf("\n\n\t按其余任意键返回");
    printf("\n\n\t请输入编号：");

    Show_Cursor(TRUE);
    scanf("%d",&key);

    ReFresh();
    GotoXY(40,3);
    printf("编辑车辆信息");

    Show_Cursor(TRUE);
    switch (key)
    {
    case 1:
        printf("\n\n\t当前车辆牌照:%s",ptruck->number);
        printf("\n\n\t请输入新的车辆牌照：");
        scanf("%s",ptruck->number);
        break;

    case 2:
        printf("\n\t当前司机姓名:%s",ptruck->driver);
        printf("\n\n\t请输入新的司机姓名：");
        scanf("%s",ptruck->driver);
        break;

    case 3:
        printf("\n\t当前司机移动电话:%s",ptruck->phone);
        printf("\n\n\t请输入新的司机移动电话：");
        scanf("%s",ptruck->phone);
        break;

    case 4:
        printf("\n\t当前车辆总容量:%.2f",ptruck->volume);
        printf("\n\n\t请输入新的总容量：");
        scanf("%f",&ptruck->volume);
        break;

    case 5:
        printf("\n\n\t当前车辆牌照:%s",ptruck->number);
        printf("\n\t请输入新的车辆牌照：");
        scanf("%s",ptruck->number);
        printf("\n\n\t当前司机姓名:%s",ptruck->driver);
        printf("\n\t请输入新的司机姓名：");
        scanf("%s",ptruck->driver);
        printf("\n\n\t当前司机移动电话:%s",ptruck->phone);
        printf("\n\t请输入新的司机移动电话：");
        scanf("%s",ptruck->phone);
        printf("\n\n\t当前车辆总容量:%.2f（吨）",ptruck->volume);
        printf("\n\t请输入新的总容量（吨）：");
        scanf("%f",&ptruck->volume);
        break;

    default:
        break;
    }

    //更改车辆总容量
    if(key == 4 || key == 5)
    {
        fvolume = ptruck->volume;
        while(pstation)
        {
            ptruck = pstation->truck;
            ptruck->volume = fvolume;
            ptruck->left_volume += fvolume;
            pstation = pstation->next;
        }
    }
    printf("\n\t\t请按任意键继续...");
    getch();
    ReFresh();
    SaveRoad();  //保存数据


    return bRet;
}

BOOL EditRoadBase(void)
{
    BOOL bRet = TRUE;
    ROAD_DATA *proad;
    int key;
    char fname[6];

    GotoXY(40,3);
    printf("编辑路线信息");
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

    GotoXY(40,3);
    printf("编辑站点信息");
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

    GotoXY(40,3);
    printf("插入站点信息");
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

    GotoXY(40,3);
    printf("删除站点");
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

    if(bRet == TRUE) printf("\n\n\t\t删除成功，请按任意键继续"), getch();
    ReFresh();


    return bRet;

}

BOOL InsertGoods(void)
{
    BOOL bRet = TRUE;
    ROAD_DATA *proad;
    STATION_DATA *pstation;
    TRUCK_DATA *ptruck;
    GOODS_DATA *pgoods, *pgoods_initsta;
    char fname[6] , sfind[10];
    int count, key, find;
    float  fvolume;

    GotoXY(40,3);
    printf("插入货物");
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
    loop16:
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
            goto loop16;
        }
    }
    printf("\n\n\t\t已找到该线路！请按任意键查看线路！\n");
    getch();
    ReFresh();

    GotoXY(40,3);
    printf("插入货物");
    pstation = proad->station;
    printf("\n\t\t站点序号\t站点名称");
    while(pstation)
    {
        printf("\n\t\t%s\t%s",pstation->station_id,pstation->station_name);
        pstation=pstation->next;
    }
    loop17:
    printf("\n\n\t\t请输入需要插入货物的站点（如1号站）：");
    Show_Cursor(TRUE);
    scanf("%s",sfind);
    pstation = proad->station;
    while(pstation)
    {
        if(!strcmp(pstation->station_id,sfind)) break;
        pstation = pstation->next;
        if(pstation == NULL)
        {
            printf("\n\t\t没有找到站点，请重新输入！");
            goto loop17;
        }
    }

    ReFresh();
    GotoXY(40,3);
    printf("插入货物");
    printf("\n\n\t\t当前选中的路线和站点为：%s————%s",pstation->road,pstation->station_id);
    ptruck = pstation->truck;
    pgoods = ptruck->goods;
    printf("\n\n\t\t货物编号\t名称\t数量（吨）");
    while(pgoods)
    {
        printf("\n\t\t%d\t\t%s\t%.2f",pgoods->number,pgoods->name,pgoods->quantity);
        count = pgoods->number;
        if(pgoods->next == NULL)
        {
            pgoods->next = (GOODS_DATA*) malloc(sizeof(GOODS_DATA));
            break;
        }
        pgoods = pgoods->next;
    }
    //创建新节点
    pgoods = pgoods->next;
    pgoods->next = NULL;
    if(pstation == proad->station)
    {
        //创建新节点
        printf("\n\n\t\t请输入需要插入的货物名称和数量（吨）");
        Show_Cursor(TRUE);
        scanf("%s%f",pgoods->name,&pgoods->quantity);
        pgoods->number = ++count;
    }
    else
    {
        loop26:
        printf("\n\n\t\t请输入需要插入的货物编号：");
        Show_Cursor(TRUE);
        scanf("%d",&find);
        pgoods_initsta = proad->station->truck->goods;
        while(find != pgoods_initsta->number)
        {
            pgoods_initsta = pgoods_initsta->next;
            if(pgoods_initsta == NULL)
            {
                printf("\n\t\t没有找到该货物请重新输入：");
                goto loop26;
            }
        }
        printf("\n\n\t\t当前货物编号：%d   货物名称：%s",pgoods_initsta->number,pgoods_initsta->name);
        pgoods->number = pgoods_initsta->number;
        strcpy(pgoods->name,pgoods_initsta->name);
        printf("\n\n\t\t请输入卸货数量（吨）：");
        scanf("%f",&pgoods->quantity);
    }
    //补全信息
    strcpy(pgoods->road,pstation->road);
    strcpy(pgoods->station_id,pstation->station_id);
    //对余量修改
    fvolume = pgoods->quantity;
    //起始站点处理
    if(pstation == proad->station)
    {
        while(pstation)
        {
            ptruck = pstation->truck;
            ptruck->left_volume -= fvolume;
            pstation = pstation->next;
        }
    }
    else
    {
        while(pstation)
        {
            ptruck = pstation->truck;
            ptruck->left_volume += fvolume;
            pstation = pstation->next;
        }
    }

    printf("\n\n\t\t请按任意键继续");
    getch();
    ReFresh();
    SaveRoad();


    return bRet;
}

BOOL EditGoods(void)
{
    BOOL bRet = TRUE;
    ROAD_DATA *proad;
    STATION_DATA *pstation;
    TRUCK_DATA *ptruck;
    GOODS_DATA *pgoods;
    char fname[6] , sfind[10];
    int  key, find;
    float  fvolume;

    GotoXY(40,3);
    printf("编辑货物");
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
    loop18:
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
            goto loop18;
        }
    }
    printf("\n\n\t\t已找到该线路！请按任意键查看线路！\n");
    getch();
    ReFresh();

    GotoXY(40,3);
    printf("编辑货物");
    pstation = proad->station;
    printf("\n\t\t站点序号\t站点名称");
    while(pstation)
    {
        printf("\n\t\t%s\t%s",pstation->station_id,pstation->station_name);
        pstation=pstation->next;
    }
    loop19:
    printf("\n\n\t\t请输入需要插入货物的站点（如1号站）：");
    Show_Cursor(TRUE);
    scanf("%s",sfind);
    pstation = proad->station;
    while(pstation)
    {
        if(!strcmp(pstation->station_id,sfind)) break;
        pstation = pstation->next;
        if(pstation == NULL)
        {
            printf("\n\t\t没有找到站点，请重新输入！");
            goto loop19;
        }
    }

    ReFresh();
    GotoXY(40,3);
    printf("编辑货物");
    printf("\n\n\t\t当前选中的路线和站点为：%s————%s",pstation->road,pstation->station_id);
    ptruck = pstation->truck;
    pgoods = ptruck->goods;
    printf("\n\n\t\t货物编号\t名称\t数量（吨）");
    while(pgoods)
    {
        printf("\n\t\t%d\t\t%s\t%.2f",pgoods->number,pgoods->name,pgoods->quantity);
        pgoods = pgoods->next;
    }

    loop20:
    printf("\n\n\t\t请输入需要更改的货物编号：");
    Show_Cursor(TRUE);
    scanf("%d",&find);
    pgoods = ptruck->goods;
    while(find != pgoods->number)
    {
        pgoods = pgoods->next;
        if(pgoods == NULL)
        {
            printf("\n\t\t没有找到该货物请重新输入：");
            goto loop20;
        }
    }
    printf("\n\n\t\t当前货物编号：%d   货物名称：%s  货物数量：%.2f",pgoods->number,pgoods->name,pgoods->quantity);
    printf("\n\n\t\t请输入新的货物名称：");
    scanf("%s",pgoods->name);
    printf("\n\t\t请输入新的货物数量：");
    scanf("%f",&pgoods->quantity);

    //对后续站点的处理
    fvolume = pgoods->quantity;
    //起始站点处理
    if(pstation == proad->station)
    {
        while(pstation)
        {
            ptruck = pstation->truck;
            ptruck->left_volume -= fvolume;
            pstation = pstation->next;
        }
    }
    else
    {
        while(pstation)
        {
            ptruck = pstation->truck;
            ptruck->left_volume += fvolume;
            pstation = pstation->next;
        }
    }

    printf("\n\n\t\t请按任意键继续");
    getch();
    ReFresh();
    SaveRoad();

    return bRet;
}

BOOL DeleteGoods(void)
{
    BOOL bRet = TRUE;
    ROAD_DATA *proad;
    STATION_DATA *pstation;
    TRUCK_DATA *ptruck;
    GOODS_DATA *pgoods,*pgoods_up = NULL;
    char fname[6] , sfind[10];
    int  key, find;
    float  fvolume;
    char *pCh = "确认删除该货物吗？";
    int iHot = 2;
    int sRet;

    GotoXY(40,3);
    printf("删除货物");
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
    loop21:
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
            goto loop21;
        }
    }
    printf("\n\n\t\t已找到该线路！请按任意键查看线路！\n");
    getch();
    ReFresh();

    GotoXY(40,3);
    printf("删除货物");
    pstation = proad->station;
    printf("\n\t\t站点序号\t站点名称");
    while(pstation)
    {
        printf("\n\t\t%s\t%s",pstation->station_id,pstation->station_name);
        pstation=pstation->next;
    }
    loop22:
    printf("\n\n\t\t请输入需要插入货物的站点（如1号站）：");
    Show_Cursor(TRUE);
    scanf("%s",sfind);
    pstation = proad->station;
    while(pstation)
    {
        if(!strcmp(pstation->station_id,sfind)) break;
        pstation = pstation->next;
        if(pstation == NULL)
        {
            printf("\n\t\t没有找到站点，请重新输入！");
            goto loop22;
        }
    }

    ReFresh();
    GotoXY(40,3);
    printf("删除货物");
    printf("\n\n\t\t当前选中的路线和站点为：%s————%s",pstation->road,pstation->station_id);
    ptruck = pstation->truck;
    pgoods = ptruck->goods;
    printf("\n\n\t\t货物编号\t名称\t数量（吨）");
    while(pgoods)
    {
        printf("\n\t\t%d\t\t%s\t%.2f",pgoods->number,pgoods->name,pgoods->quantity);
        pgoods = pgoods->next;
    }

    loop23:
    printf("\n\n\t\t请输入需要删除的货物编号：");
    Show_Cursor(TRUE);
    scanf("%d",&find);
    pgoods = ptruck->goods;
    while(find != pgoods->number)
    {
        pgoods_up = pgoods;
        pgoods = pgoods->next;
        if(pgoods == NULL)
        {
            printf("\n\t\t没有找到该货物请重新输入：");
            goto loop23;
        }
    }

    ReFresh();
    sRet = PopPrompt(&pCh, &iHot);
    if (sRet == 13 && iHot == 1)
    {
        bRet = TRUE;

        //对后续站点的处理
        fvolume = pgoods->quantity;

        //处理链头
        if(ptruck->goods == pgoods)
            ptruck->goods = pgoods->next;
        else
            pgoods_up->next = pgoods->next;
        //释放空间
        free(pgoods);

//        if(pgoods_up != NULL) pgoods = pgoods_up->next;
//        else pgoods = ptruck->goods;
////        while(pgoods)
////        {
////            pgoods->number = count++;
////            pgoods = pgoods->next;
////        }

        //起始站点处理
        if(pstation == proad->station)
        {
            while(pstation)
            {
                ptruck = pstation->truck;
                ptruck->left_volume += fvolume;
                pstation = pstation->next;
            }
        }
        else
        {
            while(pstation)
            {
                ptruck = pstation->truck;
                ptruck->left_volume -= fvolume;
                pstation = pstation->next;
            }
        }

    }
    else
    {
        bRet = FALSE;
    }
    PopOff();
    ReFresh();
    if(bRet == TRUE) printf("\n\n\t\t删除成功");
    ReFresh();
    printf("\n\t\t请按任意键继续");
    getch();
    ReFresh();
    SaveRoad();


    return bRet;
}


