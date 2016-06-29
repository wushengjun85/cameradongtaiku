#include "canread.h"
extern unsigned char flagWidthlamp;

//2016.6.22  extern widget.cpp 中的变量。
/********************************************************************************************************************/
//速度定义 speed
extern unsigned short int speed = 0;
//unsigned short int
extern unsigned char daoche = 0;


//开关量
//
extern unsigned char KL;  //空滤报警开关（空滤-）
extern unsigned char DC;  //倒车信号（倒车+）
extern unsigned char SK;  //位置灯（位置灯+）
extern unsigned char SouSa;//手刹指示灯（手刹指示灯+）
extern unsigned char Jinguang; //近光指示灯（近光指示灯）
extern unsigned char YuanGuang; //远光指示灯（远光指示灯）
extern unsigned char Youzhuan;  //右转向灯（右转向灯）
extern unsigned char Zuozhuan; //左转向灯（左转向灯）
extern unsigned char CongDian; //充电指示灯-
extern unsigned char Lihe;    //主离合信号+
extern unsigned char YeyaGuolv2;//液压过滤器压力2报警（液压油压力-）
extern unsigned char YeyaGuolv1;//液压过滤器压力1报警（粮满信号线-）
extern unsigned char QBKG;     //翘板开关二
extern unsigned char LCJ1;     //粮仓降(翘板开关一)
extern unsigned char LCS;      //粮仓升(手柄输入六)
extern unsigned char HTJ;      //还田降(手柄输入五)
extern unsigned char HTS;     //还田升(手柄输入四)
extern unsigned char GTJ;    //割台降(手柄输入三)
extern unsigned char GTS;    //割台升(手柄输入二)
extern unsigned char Chongdian; //充电指示灯(手柄输入一)

extern unsigned char LCJDCQ8; //粮仓降电磁线圈（输出八）
extern unsigned char LCJDCQ7; //粮仓降电磁线圈（输出七）
extern unsigned char LCJDCQ6; //粮仓降电磁线圈（输出六）
extern unsigned char LCJDCQ5; //粮仓降电磁线圈（输出五）
extern unsigned char LCJDCQ4; //粮仓降电磁线圈（输出四）
extern unsigned char LCJDCQ3; //粮仓降电磁线圈（输出三）
extern unsigned char LCJDCQ2; //粮仓降电磁线圈（输出二）
extern unsigned char LCJDCQ1; //粮仓降电磁线圈（输出一）
extern unsigned char DZSSC;   //电子扇输出
extern unsigned char YLFSC9;  //输出九溢流阀

//模拟量
//18FF11F6
extern unsigned char SuiWen;//水温；
extern unsigned char YeyaYouLiang; //液压油油温（液压油油温）
extern unsigned char YouLiang;     //油量（油量）
extern unsigned char Youya;       //油压
extern unsigned short int DianYa;      //电压


//18FF21F6
extern unsigned short int ZhuangSu;   //转速
extern unsigned int XiaoshiJi;        //小时计

//
extern unsigned short int CheSu; //车速(预留）
extern unsigned int   MiJi;      //米计（预留）
extern unsigned int   LiCheng;   //里程（预留）

//故障码
//18FFF1F6
//暂时空一下。


/********************************************************************************************************************/


/********************************************************************************************************************/
//2016 6.1  wushengjun
//定义变量
//
/********************************************************************************************************************/

extern unsigned char flagLeft;  //左转


extern unsigned char flagBattery;//电瓶指示灯


extern unsigned char flagWidthlamp; //示宽灯


extern unsigned char  flagYG; //远光灯


extern unsigned char  flagSW; //水温


extern unsigned char  flagJG; //近光灯



extern unsigned char  flagJY; //机油


extern unsigned char  flagLCM; //粮仓满


extern unsigned char   flagFDJYR; //发动机预热


extern unsigned char   flagGL; //过滤


extern unsigned char   flagYL; //油量


extern unsigned char    flagBSQ;//变速器


extern unsigned char    flagECU;//ecu


extern unsigned char    flagPark;//停车


extern unsigned char   flagFDJGZ; //发动机故障


extern unsigned char   flagRight; //右转


/********************************************************************************************************************/











Canread::Canread()
{
}


void Canread::shutdownfd(int fd)
{
    close(fd);
}


 int Canread::initcan(int *retfd)
 {
     int s;
         int ret;
         struct sockaddr_can addr;
         struct ifreq ifr;



     srand(time(NULL));
         s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
         if (s < 0) {
             perror("socket PF_CAN failed");
             return 1;
         }
         else
         {
            *retfd = s;
         }

         strcpy(ifr.ifr_name, "can0");
         ret = ioctl(s, SIOCGIFINDEX, &ifr);
         if (ret < 0) {
             perror("ioctl failed");
             return 1;
         }

         addr.can_family = PF_CAN;
         addr.can_ifindex = ifr.ifr_ifindex;

         ret = bind(s, (struct sockaddr *)&addr, sizeof(addr));
         if (ret < 0) {
             perror("bind failed");
             return 1;
         }
     if (1) {
         struct can_filter filter[3];
         filter[0].can_id = 0x200 | CAN_EFF_FLAG;
         filter[0].can_mask = 0xFFF;

         filter[1].can_id = 0x20F | CAN_EFF_FLAG;
         filter[1].can_mask = 0xFFF;

         filter[2].can_id = 0x18ff01f6;
         filter[2].can_mask = 0xfff;


         ret = setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter));
         if (ret < 0) {
             perror("setsockopt failed");
             return 1;
         }
     }

 }


 void Canread::print_frame(struct can_frame *fr)
{
    int i;
    printf("%08x\n", fr->can_id & CAN_EFF_MASK);
    //printf("%08x\n", fr->can_id);
    printf("dlc = %d\n", fr->can_dlc);
    printf("data = ");
    for (i = 0; i < fr->can_dlc; i++)
        printf("%02x ", fr->data[i]);//
    printf("\n");
}


 void Canread::handle_err_frame(const struct can_frame *fr)
{
    if (fr->can_id & CAN_ERR_TX_TIMEOUT) {
        errout("CAN_ERR_TX_TIMEOUT");
    }
    if (fr->can_id & CAN_ERR_LOSTARB) {
        errout("CAN_ERR_LOSTARB");
        errcode(fr->data[0]);
    }
    if (fr->can_id & CAN_ERR_CRTL) {
        errout("CAN_ERR_CRTL");
        errcode(fr->data[1]);
    }
    if (fr->can_id & CAN_ERR_PROT) {
        errout("CAN_ERR_PROT");
        errcode(fr->data[2]);
        errcode(fr->data[3]);
    }
    if (fr->can_id & CAN_ERR_TRX) {
        errout("CAN_ERR_TRX");
        errcode(fr->data[4]);
    }
    if (fr->can_id & CAN_ERR_ACK) {
        errout("CAN_ERR_ACK");
    }
    if (fr->can_id & CAN_ERR_BUSOFF) {
        errout("CAN_ERR_BUSOFF");
    }
    if (fr->can_id & CAN_ERR_BUSERROR) {
        errout("CAN_ERR_BUSERROR");
    }
    if (fr->can_id & CAN_ERR_RESTARTED) {
        errout("CAN_ERR_RESTARTED");
    }
}

 int Canread::test_can_rw(int fd, int master)
{
    int ret, i;
    struct can_frame fr, frdup;
    struct timeval tv;
    fd_set rset;

    while (1) {
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        FD_ZERO(&rset);
        FD_SET(fd, &rset);
        printf("=====\r\n");
/*
        ret = select(fd+1, &rset, NULL, NULL, NULL);
        if (ret == 0) {
            myerr("select time out");
            return -1;
        }
*/
        printf("------------------------ \n");
        ret = read(fd, &frdup, sizeof(frdup));
        if (ret < sizeof(frdup)) {
            myerr("read failed");
            return -1;
        }
        if (frdup.can_id & CAN_ERR_FLAG) { /* 出错设备错误 */
            handle_err_frame(&frdup);
            myerr("CAN device error");
            continue;
        }

/*
       if(((frdup.can_id)&CAN_EFF_MASK) == 0x18ff01f6)
        {
           if (frdup.data[0] == 0x99)
           {
                daoche = 1;
           }
           else
           {
                daoche = 0;
           }


            if(frdup.data[3] == 0x55)
            {
                flagWidthlamp = 0;
                printf("WSJ flagWidthlamp: %d\r\n",flagWidthlamp);
            }
            else
            {
                printf("456789");
                 flagWidthlamp = 1;
            }
        }
        */


       //解析数据
       switch(frdup.can_id&CAN_EFF_MASK)
       {
            case 0x18ff01f6:
            speed = frdup.data[0]|frdup.data[1];


            /********************************************************************/
            //仪表值 有可以不用定义的变量，目前来说只是实验，等到正式产品的时候可以精简一下。
            /********************************************************************/
            //byte1 移位操作
            KL = (frdup.data[0]>>7)&0x01;
            flagGL = KL;

            DC = (frdup.data[0]>>6)&0x01;


            SK = (frdup.data[0]>>5)&0x01;
            flagWidthlamp = SK;

            SouSa = (frdup.data[0]>>4)&0x01;
            flagPark = SouSa;

            Jinguang = (frdup.data[0]>>3)&0x01;
            flagJG = Jinguang;


            YuanGuang = (frdup.data[0]>>2)&0x01;
            flagYG = YuanGuang;

            Youzhuan = (frdup.data[0]>>1)&0x01;
            flagRight = Youzhuan;

            Zuozhuan = frdup.data[0]&0x01;
            flagLeft = Zuozhuan;

            //byte2
            CongDian = (frdup.data[1]>>3)&0x01;
            flagBattery = CongDian;

            Lihe     = (frdup.data[1]>>2)&0x01;

            YeyaGuolv2 = (frdup.data[1]>>1)&0x01;

            YeyaGuolv1 = frdup.data[1]&0x01;

            //byte3
            QBKG = (frdup.data[2]>>7)&0x01; //翘板开关二
            LCJ1 = (frdup.data[2]>>6)&0x01;//粮仓降(翘板开关一)
            LCS  = (frdup.data[2]>>5)&0x01;//粮仓升(手柄输入六)
            HTJ  = (frdup.data[2]>>4)&0x01;//还田降(手柄输入五)
            HTS = (frdup.data[2]>>3)&0x01; //还田升(手柄输入四)
            GTJ = (frdup.data[2]>>2)&0x01; //割台降(手柄输入三)
            GTS = (frdup.data[2]>>1)&0x01; //割台升(手柄输入二)
            Chongdian = (frdup.data[2])&0x01; //充电指示灯(手柄输入一)

            //byte4
            LCJDCQ8 = (frdup.data[3]>>7)&0x01; //粮仓降电磁线圈（输出八）
            LCJDCQ7 = (frdup.data[3]>>6)&0x01; //粮仓降电磁线圈（输出七）
            LCJDCQ6 = (frdup.data[3]>>5)&0x01; //粮仓降电磁线圈（输出六）
            LCJDCQ5 = (frdup.data[3]>>4)&0x01; //粮仓降电磁线圈（输出五）
            LCJDCQ4 = (frdup.data[3]>>3)&0x01; //粮仓降电磁线圈（输出四）
            LCJDCQ3 = (frdup.data[3]>>2)&0x01; //粮仓降电磁线圈（输出三）
            LCJDCQ2 = (frdup.data[3]>>1)&0x01; //粮仓降电磁线圈（输出二）
            LCJDCQ1 = frdup.data[3]&0x01; //粮仓降电磁线圈（输出一）

            //byte5
            DZSSC = (frdup.data[4]>>1)&0x02; //电子扇输出
            YLFSC9 = frdup.data[4]&0x01; //输出九溢流阀
            break;


            case 0x18ff11f6:

            //模拟量
            //18FF11F6

           SuiWen = frdup.data[0]; //水温；
           YeyaYouLiang = frdup.data[1];//液压油油温（液压油油温）
           YouLiang = frdup.data[2]; //油量（油量）
           Youya = frdup.data[3]; //油压
           DianYa = frdup.data[4]|frdup.data[5];  //电压

            break;
            case 0x18ff21f6:
           //
           ZhuangSu = frdup.data[0]|frdup.data[1];
           //XiaoshiJi = frdup.data[2]     //待处理

            break;
            case 0x18ff31f6: //暂时用这个值做处理 2016.6.25



            break;

            default:
            break;








       }


        print_frame(&frdup);

        memset(&frdup,0,sizeof(frdup));
/*
        ret = write(fd, &frdup, sizeof(frdup));
        if (ret < 0) {
            myerr("write failed");
            return -1;
        }
*/
        //sleep(2);


    }

    return 0;
}


//int main(int argc, char *argv[])
//{
//    int s;
//        int ret;
//        struct sockaddr_can addr;
//        struct ifreq ifr;
//    int master;


//    srand(time(NULL));
//        s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
//        if (s < 0) {
//            perror("socket PF_CAN failed");
//            return 1;
//        }

//        strcpy(ifr.ifr_name, "can0");
//        ret = ioctl(s, SIOCGIFINDEX, &ifr);
//        if (ret < 0) {
//            perror("ioctl failed");
//            return 1;
//        }

//        addr.can_family = PF_CAN;
//        addr.can_ifindex = ifr.ifr_ifindex;

//        ret = bind(s, (struct sockaddr *)&addr, sizeof(addr));
//        if (ret < 0) {
//            perror("bind failed");
//            return 1;
//        }
//    if (1) {
//        struct can_filter filter[2];
//        filter[0].can_id = 0x200 | CAN_EFF_FLAG;
//        filter[0].can_mask = 0xFFF;

//        filter[1].can_id = 0x20F | CAN_EFF_FLAG;
//        filter[1].can_mask = 0xFFF;


//        ret = setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter));
//        if (ret < 0) {
//            perror("setsockopt failed");
//            return 1;
//        }
//    }
//    test_can_rw(s, master);

//    close(s);
//    return 0;
//}
