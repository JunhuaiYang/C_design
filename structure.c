#include"global.h"

/**
 *
 *  \brief 该文件主要用于保存数据与文件方面的函数
 *
 */



/**
 * 函数名称: LoadData
 * 函数功能: 将代码表和三类基础数据从数据文件载入到内存缓冲区和十字链表中.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 功能函数中除了函数ExitSys的返回值可以为FALSE外,
 *           其他函数的返回值必须为TRUE.
 *
 * 调用说明: 为了能够以统一的方式调用各功能函数, 将这些功能函数的原型设为
 *           一致, 即无参数且返回值为BOOL. 返回值为FALSE时, 结束程序运行.
 */
BOOL LoadData()
{
    int Re = 0;

    if(MakeDir())
    {
        printf("文件夹创建成功！\n");
    }
    else
    {
        printf("文件夹创建失败！\n");
    }

    Re = CreatList(&gp_head);
    gul_station_count = LoadCode();
    if(gul_station_count<1)
    {
        printf("\n 站点代码表加载失败 ！\n");
    }
    else
    {
        printf("\n站点代码表加载成功！ \n");
    }

    if(Re<60) //60表示四类基础数据已加载
    {
        printf("\n 系统基础数据不完整 ！\n");
        printf("\n 请在系统关闭前保存数据！ ！\n");
    }

    printf("\n按任意键继续...\n");
    getch();
    return TRUE;
}

/**
 * 函数名称: MakeDir
 * 函数功能: 创建文件夹
 * 输入参数:
 * 输出参数:
 * 返 回 值:  TRUE FALSE
 *
 * 调用说明:
 */
BOOL MakeDir(void)
{

    if(access("data",0) == EOF)   //access用来判断文件夹是否已存在，参数0表示判断
    {
        if( mkdir("data") == EOF)
        {
            return FALSE;
        }
    }
    if( access("data\\road",0) == EOF)
    {
        if( mkdir("data\\road") == EOF)
        {
            return FALSE;
        }
    }
    if (access("data\\station",0) == EOF)
    {
        if (mkdir("data\\station") == EOF)
        {
            return FALSE;
        }
    }
    if( access("data\\truck",0) == EOF)
    {
        if( mkdir("data\\truck") == EOF)
        {
            return FALSE;
        }
    }

    return TRUE;

}

/**
 * 函数名称: LoadCode
 * 函数功能: 将代码表从数据文件载入到一个单向链表中
 * 输入参数: FileName 存放代码表的数据文件名.
 * 输出参数:
 * 返 回 值: 存放代码表的内存缓冲区大小(以字节为单位).
 *
 * 调用说明:
 */
int LoadCode(void)
{
    STATION_CODE *phead, *tail;
    FILE *pFile = NULL;
    int re = 0,temp;
    char ch[50];

    pFile = fopen(gp_station_code_filename , "r");

    if(pFile == NULL)
    {
        printf("\n站点代码表打开失败\n");
        if(fopen(gp_station_code_filename,"w") == NULL)
        {
            printf("站点代码表文件创建失败！ \n");
        }
        else
        {
            printf("站点代码表文件创建成功！ \n");
        }
        return re;
    }


    phead = (STATION_CODE*)malloc(sizeof(STATION_CODE));
    tail = phead;

    if(fscanf(pFile,"%d%s",&temp,ch) != EOF)
    {
        tail->station_num = temp;
        strcpy(tail->station_name,ch);
    }
    else
    {
        phead = NULL;
        gp_station_code = phead;
        free(tail);
        return 0;
    }

    while(fscanf(pFile,"%d%s",&temp,ch) != EOF)
    {
        tail->next = (STATION_CODE*)malloc(sizeof(STATION_CODE));
        tail = tail->next;
        tail->station_num = temp;
        strcpy(tail->station_name,ch);
    }

    tail -> next = NULL;

    gp_station_code = phead;
    re = temp;

    fclose(pFile);  //关闭文件！！！

    return temp;

}

/**
 * 函数名称: CreatList
 * 函数功能: 从数据文件读取基础数据, 并存放到所创建的十字链表中.
 * 输入参数: 无
 * 输出参数: phead 主链头指针的地址, 用来返回所创建的十字链.
 * 返 回 值: int型数值, 表示链表创建的情况.
 *           0  空链, 无数据
 *           4  已加载路线信息数据，无站点基本信息和车辆信息数据及货物清单数据
 *           12 已加载路线信息和站点信息数据，无车辆信息数据及货物清单数据
 *           28 三类基础数据已加载，无货物清单数据
 *           60 四类基础数据都已加载
 * 调用说明:
 */

int CreatList(ROAD_DATA **phead)
{
    ROAD_DATA *hd = NULL, *pRoadData, tmp1;
    STATION_DATA *pStationData, tmp2, *psup;
    TRUCK_DATA *pTruckData, tmp3;
    GOODS_DATA *pGoodsData, tmp4,*pgup;
    FILE *pFile;
    int find, road_count=0;
    int re = 0, i;

    if((pFile = fopen(gp_road_filename,"rb"))== NULL)
    {
        printf("路线情况基本信息打开失败！ \n");
        return re;
    }
    printf("路线情况基本信息打开成功！\n");

    //从数据文件中读宿舍楼信息数据，存入以后进先出方式建立的主链中

    while(fread(&tmp1, sizeof(ROAD_DATA),1,pFile)==1)
    {
        pRoadData = (ROAD_DATA*)malloc( sizeof(ROAD_DATA));
        *pRoadData= tmp1;
        pRoadData->station = NULL;
        pRoadData->next = hd;
        hd = pRoadData;
        road_count++;
    }
    fclose(pFile);
    gul_road_count = road_count;

    //判断是否成功读取
    if (hd == NULL)
    {
        printf("路线情况基本信息加载失败！\n");
        return re;
    }
    printf("路线情况基本信息加载成功！\n");
    *phead = hd;
    re += 4;

    if((pFile = fopen(gp_station_filename,"rb"))==NULL)
    {
        printf("站点基本信息数据文件打开失败！\n");
        return re;
    }
    printf("站点基本信息数据文件打开成功！\n");

        /*从数据文件中读取学生基本信息数据，存入主链对应结点的学生基本信息支链中*/

    //i =1;
    while(fread(&tmp2,sizeof(STATION_DATA),1,pFile) == 1)
    {
        //创建节点
        pStationData = (STATION_DATA*)malloc(sizeof(STATION_DATA));
        *pStationData = tmp2 ;
        pStationData->truck=NULL;

        //在主链上查找对应的节点
        pRoadData = hd ;
        while(pRoadData != NULL
              && strcmp(pRoadData->road,pStationData->road)!= 0)//如果不相同
        {
            pRoadData = pRoadData ->next; //查找下一个节点
        }
        if(pRoadData != NULL) //如果找到，加入链表
        {
            //strcmp("1号站",pStationData->station_id)==0
            if(pRoadData->station == NULL) pRoadData->station = pStationData , psup = pStationData;
            psup->next = pStationData;
            psup = psup->next;
            pStationData->next = NULL;

        }
        else //没找到释放空间
        {
            free(pStationData);
        }
        //i++;
    }
    fclose(pFile);//记得关闭文件
    if(pStationData == NULL)
    {
        printf("站点情况基本信息加载失败！\n");
        return re;
    }
    else printf("站点情况基本信息加载成功！\n");
    re +=8;


    if ((pFile = fopen(gp_truck_filename,"rb"))==NULL)
    {
        printf("车辆基本信息打开失败！\n");
        return re;
    }
    printf("车辆基本信息打开成功！\n");

    //加载进入节点中
    while(fread(&tmp3,sizeof(TRUCK_DATA),1,pFile) ==1 )
    {
        //创建节点
        pTruckData = (TRUCK_DATA*)malloc(sizeof(TRUCK_DATA));
        *pTruckData = tmp3;
        pTruckData->goods=NULL;

        //查找站点支链上对应的车辆节点
        pRoadData=hd;
        find = 0;
        while(pRoadData != NULL &&find == 0)
        {
            pStationData = pRoadData->station;
            while(pStationData!= NULL && find ==0)
            {
                if(strcmp(pStationData->road,pTruckData->road) == 0
                   && strcmp(pStationData->station_id,pTruckData->station_id) == 0)//找到相同;
                {
                    find =1;
                    break;
                }
                pStationData=pStationData->next;
            }
            pRoadData= pRoadData->next;
        }
        if(find)//1为找到
        {
            pStationData->truck = pTruckData;
        }
        else//没找到释放
        {
            free(pTruckData);
        }
    }
    fclose(pFile);//记得关闭文件
    if(pTruckData == NULL)
    {
        printf("车辆情况基本信息加载失败！\n");
        return re;
    }
    else printf("车辆情况基本信息加载成功！\n");
    re += 16;


    //加载路线上的货物清单
    if((pFile = fopen(gp_goods_filename,"rb"))==NULL)
    {
        printf("货物清单信息数据文件打开失败！\n");
        return re;
    }
    printf("货物清单信息数据文件打开成功！\n");

    while(fread(&tmp4,sizeof(GOODS_DATA),1,pFile)==1)
    {
        //创建节点
        pGoodsData = (GOODS_DATA*)malloc(sizeof(GOODS_DATA));
        *pGoodsData = tmp4;

        //查找对应的车辆及站点
        pRoadData = hd ;
        find = 0;
        while(pRoadData !=NULL && find == 0 )
        {
            pStationData = pRoadData->station;
            while(pStationData != NULL && find == 0)
            {
                pTruckData = pStationData->truck;
                if(pTruckData != NULL && find ==0)
                {
                    if(strcmp(pStationData->road,pGoodsData->road)==0 //同时满足站点和路线名称两个条件
                       && strcmp(pStationData->station_id, pGoodsData->station_id) ==0)
                    {
                        find = 1;
                        break;
                    }
                }
                pStationData=pStationData->next;
            }
            pRoadData = pRoadData->next;
        }
        if(find)//如果找到
        {
            //pGoodsData->next = pTruckData ->goods;
            //pTruckData->goods = pGoodsData;
            if(pTruckData->goods == NULL) pTruckData->goods = pGoodsData , pgup = pGoodsData;
            pgup->next = pGoodsData;
            pgup = pgup->next;
            pGoodsData->next = NULL;
        }
        else//未找到释放空间
        {
            free(pGoodsData);
        }
    }
    fclose(pFile); //关闭文件

    if(pGoodsData == NULL)
    {
        printf("货物清单加载失败！\n");
        return re;
    }
    else printf("货物清单加载成功！\n");
    re += 32;

    return re;
}

BOOL SaveStation(void)
{
    FILE* pFile;
    STATION_CODE *tail;

    pFile = fopen( gp_station_code_filename,"w") ;
    if(pFile == NULL)
    {
        printf("文件打开失败!");
        return FALSE;
    }
    tail = gp_station_code;

    while(tail)
    {
        fprintf(pFile,"%d %s\n",tail->station_num,tail->station_name);
        tail = tail->next;
    }

    fclose(pFile);
    return TRUE;
}

BOOL SaveRoad(void)
{
    BOOL bRet = TRUE;
    FILE *pfroad, *pfstation, *pftruck, *pfgoods;
    ROAD_DATA *proad = gp_head;
    STATION_DATA *pstation;
    TRUCK_DATA *ptruck;
    GOODS_DATA *pgoods;

    //打开文件
    if((pfroad = fopen(gp_road_filename,"wb")) == NULL) return FALSE;
    if((pfstation = fopen(gp_station_filename,"wb")) == NULL) return FALSE;
    if((pftruck = fopen(gp_truck_filename,"wb")) == NULL) return FALSE;
    if((pfgoods = fopen(gp_goods_filename,"wb")) == NULL) return FALSE;

    while(proad != NULL)
    {
        fwrite(proad,sizeof(ROAD_DATA),1,pfroad);
        pstation = proad->station;
        while(pstation != NULL)
        {
            fwrite(pstation,sizeof(STATION_DATA),1,pfstation);
            ptruck = pstation->truck;
            if(ptruck==NULL)
            {
                pstation = pstation->next;
                break;
            }
            fwrite(ptruck,sizeof(TRUCK_DATA),1,pftruck);
            pgoods = ptruck->goods;
            while(pgoods != NULL)
            {
                fwrite(pgoods,sizeof(GOODS_DATA),1,pfgoods);
                pgoods = pgoods->next;
            }
            pstation = pstation->next;
        }
        proad = proad->next;
    }

    //关闭文件
    fclose(pfgoods);
    fclose(pfroad);
    fclose(pfstation);
    fclose(pftruck);

    return bRet;
}
