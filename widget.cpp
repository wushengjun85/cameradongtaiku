#include "widget.h"
#include "ui_widget.h"

#include"signalcan.h"

#include<QTime>
#include<QTimer>
#include<QPainter>
#include<QDebug>

int num = 0;
int numtmp = 0;



/********************************************************************************************************************/
//2016 6.1  wushengjun
//定义变量
//
/********************************************************************************************************************/

unsigned char flagLeft = 0;  //左转
unsigned char countLeft = 0; //左转

unsigned char flagBattery = 0;//电瓶指示灯
unsigned char countBattery = 0;//电瓶指示灯

unsigned char flagWidthlamp = 0; //示宽灯
unsigned char countWidthlamp = 0;//示宽灯

unsigned char  flagYG = 0; //远光灯
unsigned char  countYG = 0;//远光灯

unsigned char  flagSW  = 0; //水温
unsigned char  countSW = 0; //水温

unsigned char  flagJG = 0; //近光灯
unsigned char  countJG = 0; //近光灯


unsigned char  flagJY = 0; //机油
unsigned char  countJY = 0; //机油

unsigned char  flagLCM = 0; //粮仓满
unsigned char  countLCM = 0; //粮仓满

unsigned char   flagFDJYR = 0; //发动机预热
unsigned char   countFDJYR = 0; //发动机预热

unsigned char   flagGL = 0; //过滤
unsigned char   countGL = 0; //过滤

unsigned char   flagYL = 0; //油量
unsigned char    countYL = 0;//油量

unsigned char    flagBSQ = 0;//变速器
unsigned char    countBSQ = 0;//变速器

unsigned char    flagECU = 0;//ecu
unsigned char    countECU = 0;//ecu

unsigned char    flagPark = 0;//停车
unsigned char    countPark = 0; //停车

unsigned char   flagFDJGZ = 0; //发动机故障
unsigned char   countFDJGZ = 0; //发动机故障

unsigned char   flagRight = 0; //右转
unsigned char    countRight = 0;//右转

/********************************************************************************************************************/


//2016.6.20 wsj
//下面是显示can数据 值的变量
//临时定义，后边产品要更根据情况去改和规范 变量的定义。
/********************************************************************************************************************/

//开关量
//
unsigned char KL;  //空滤报警开关（空滤-）
unsigned char DC;  //倒车信号（倒车+）
unsigned char SK;  //位置灯（位置灯+）
unsigned char SouSa;//手刹指示灯（手刹指示灯+）
unsigned char Jinguang; //近光指示灯（近光指示灯）
unsigned char YuanGuang; //远光指示灯（远光指示灯）
unsigned char Youzhuan;  //右转向灯（右转向灯）
unsigned char Zuozhuan; //左转向灯（左转向灯）
unsigned char CongDian; //充电指示灯-
unsigned char Lihe;    //主离合信号+
unsigned char YeyaGuolv2;//液压过滤器压力2报警（液压油压力-）
unsigned char YeyaGuolv1;//液压过滤器压力1报警（粮满信号线-）
unsigned char QBKG;     //翘板开关二
unsigned char LCJ1;     //粮仓降(翘板开关一)
unsigned char LCS;      //粮仓升(手柄输入六)
unsigned char HTJ;      //还田降(手柄输入五)
unsigned char HTS;     //还田升(手柄输入四)
unsigned char GTJ;    //割台降(手柄输入三)
unsigned char GTS;    //割台升(手柄输入二)
unsigned char Chongdian; //充电指示灯(手柄输入一)

unsigned char LCJDCQ8; //粮仓降电磁线圈（输出八）
unsigned char LCJDCQ7; //粮仓降电磁线圈（输出七）
unsigned char LCJDCQ6; //粮仓降电磁线圈（输出六）
unsigned char LCJDCQ5; //粮仓降电磁线圈（输出五）
unsigned char LCJDCQ4; //粮仓降电磁线圈（输出四）
unsigned char LCJDCQ3; //粮仓降电磁线圈（输出三）
unsigned char LCJDCQ2; //粮仓降电磁线圈（输出二）
unsigned char LCJDCQ1; //粮仓降电磁线圈（输出一）
unsigned char DZSSC;   //电子扇输出
unsigned char YLFSC9;  //输出九溢流阀

//模拟量
//18FF11F6
unsigned char SuiWen;//水温；
unsigned char YeyaYouLiang; //液压油油温（液压油油温）
unsigned char YouLiang;     //油量（油量）
unsigned char Youya;       //油压
unsigned short int  DianYa;      //电压


//18FF21F6
unsigned short int ZhuangSu;   //转速
unsigned int XiaoshiJi;        //小时计

//
unsigned short int CheSu; //车速(预留）
unsigned int   MiJi;      //米计（预留）
unsigned int   LiCheng;   //里程（预留）

//故障码
//18FFF1F6
//暂时空一下。


/********************************************************************************************************************/

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);   //声明一个定时器
    QTimer *timertst = new QTimer(this);

    //update()会自动产生重绘消息，调用paintEvent()
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));  //连接信号槽，定时器超时触发窗体更新
    connect(timertst, SIGNAL(timeout()), this, SLOT(func()));  //连接信号槽，定时器超时触发窗体更新
    timer->start(500);   //启动定时器
    timertst->start(1000);
    setWindowTitle(tr("Haovel"));  //设置窗体名称
    resize(800,600);
    //setWindowFlags(windowFlags()|Qt::FramelessWindowHint|Qt::WindowTitleHint); //删除 最小化、最大化、关闭按钮


    //线程初始化与启动
    mycan = new SignalCan();
    mycan->start();

    //倒车摄像头信号与槽
    connect(this,SIGNAL(sendcamersignal()),this,SLOT(cameraslot()));





}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{

    QTime time = QTime::currentTime();   //获取当前的时间
    QPainter painter(this);
    QPixmap pix;
    pix.load("./img2/xingzou.bmp");
    painter.drawPixmap(0,0,800,600,pix);



    int side = qMin(width(), height());  //绘制的范围(宽、高中最小值)

    painter.setRenderHint(QPainter::Antialiasing);//绘制的图像反锯齿
    //painter.translate(width() / 2, height() / 2);//重新定位坐标起始点，把坐标原点放到窗体的中央

    painter.translate(405, 284);//重新定位坐标起始点，把坐标原点放到窗体的中央
    //painter.scale(side / 400.0, side / 300.0);//设定画布的边界,用窗体宽高的最小值来计算时钟的大小，防止窗体拉伸导致的时钟变形以及显示不全




    //下面两个数组用来定义表针的两个顶点，以便后面的填充
    static const QPoint hourHand[4] = {
        QPoint(8, 0),
        QPoint(-8,0),
        QPoint(-1.5, -170),
        QPoint(1.5, -170)
    };
    static const QPoint minuteHand[4] = {
        QPoint(4, 0),
        QPoint(-4, 0),
        QPoint(-1, -100),
        QPoint(1, -100)
    };

    //km r/mini

    painter.setPen(Qt::NoPen);//填充时针，不需要边线所以NoPen
    painter.setBrush(Qt::red);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.save();
    //painter.rotate(6.0 *time.second());  //设旋转(角度 = 6° * (分钟 + 秒 / 60))
    painter.rotate(-124);
    //painter.rotate(6.0 *numtmp);  //设置旋转(6° * 秒)
    painter.rotate(numtmp++);  //设置旋转(6° * 秒)


    painter.drawConvexPolygon(hourHand, 4);  //填充分针部分

    painter.restore();


    /***********************************************/
    //2016.6.25   画白圈
    painter.setBrush(Qt::darkGray); //画上中心原点/home/vmuser/qtworkplace/chanpin/img2
    painter.save();
    painter.drawEllipse(QPoint(0,0),25,25);
    painter.restore();

    //画白圈
    /***********************************************/

    painter.setBrush(Qt::black);             //画上中心原点/home/vmuser/qtworkplace/chanpin/img2
    painter.save();
    painter.drawEllipse(QPoint(0,0),20,20);
    painter.restore();


    //km/h
    painter.translate(0,148);//重新定位坐标起始点，把坐标原点放到窗体的中央
    //painter.scale(side / 400.0, side / 300.0);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.save();

    //painter.rotate(6.0 *time.second());  //设旋转(角度 = 6° * (分钟 + 秒 / 60))
    painter.rotate(-120);
    painter.rotate(6.0 *ZhuangSu);  //设旋转(角度 = 6° * (分钟 + 秒 / 60))
    //内测用
    /****************************************************************************/
    //qDebug()<<"time: "<<time.second()<<endl;

    /****************************************************************************/

    painter.drawConvexPolygon(minuteHand, 4);  //填充分针部分
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.restore();


    /***********************************************/
    //2016.6.25   画白圈
    painter.setBrush(Qt::darkGray); //画上中心原点/home/vmuser/qtworkplace/chanpin/img2
    painter.save();
    painter.drawEllipse(QPoint(0,0),13,13);
    painter.restore();

    //画白圈
    /***********************************************/


    painter.setBrush(Qt::black);
    painter.save();//画上中心原点
    painter.drawEllipse(QPoint(0,0),10,10);
    painter.restore();


    //chuli pupian shansuo
    painter.translate(-405,-432);//平移到左上角






    /***************************************************************************************/
    /*仪表值   2016.6.22                                                                    */
    /***************************************************************************************/

    //暂时命名和使用   自己定义，没有严格按照协议，因为协议是原来的贴图，并不完善。
    //ui->label->setText(QString::number(YouLiang,10));
    ui->label->setText(QString::number(DC,10));
    ui->label_2->setText(QString::number(SuiWen,10));
    ui->label_3->setText(QString::number(Youya,10));

    if(DC==1)
    {
        emit sendcamersignal();
        //DC = 0;
    }





    //加载上方图标 闪烁用到
    //zuo zhuandeng
    QPainter paintLeft(this);
    QPixmap pixLeft;


    if(flagLeft) //闪烁
    {
        countLeft++;
        if (countLeft>2)
            countLeft = 0;
        switch(countLeft)
        {
            case 1:pixLeft.load("./img2/27.png");//14.jpg
            break;
        }
        paintLeft.drawPixmap(0,0,43,34,pixLeft);

    }


    //电瓶指示灯
    QPainter paintBattery(this);
    QPixmap pixBattery;


    if(flagBattery) //闪烁
    {
        countBattery++;
        if (countBattery>2)
            countBattery = 0;
        switch(countBattery)
        {
            case 1:pixBattery.load("./img2/21.png");//14.jpg
            break;
        }
        paintBattery.drawPixmap(51,0,43,34,pixBattery);
    }

    //示宽灯
    QPainter paintWidthlamp(this);
    QPixmap pixWidthlamp;
    if(flagWidthlamp) //闪烁
    {
        countWidthlamp++;
        if (countWidthlamp>2)
            countWidthlamp = 0;
        switch(countWidthlamp)
        {
            case 1:pixWidthlamp.load("./img2/25.png");//14.jpg
            break;
        }
        paintWidthlamp.drawPixmap(102,0,43,34,pixWidthlamp);
    }

    //远光灯
    QPainter paintYG(this);
    QPixmap pixYG;
    if(flagYG) //闪烁
    {
        countYG++;
        if (countYG>2)
            countYG = 0;
        switch(countYG)
        {
            case 1:pixYG.load("./img2/24.png");//14.jpg
            break;
        }
        paintYG.drawPixmap(153,0,43,34,pixYG);
    }

    //水温
    QPainter paintSW(this);
    QPixmap pixSW;
    if(flagSW) //闪烁
    {
        countSW++;
        if (countSW>2)
            countSW = 0;
        switch(countSW)
        {
            case 1:pixSW.load("./img2/12.png");//14.jpg
            break;
        }
        paintSW.drawPixmap(204,0,35,35,pixSW);
    }

    //近光灯flagJG

    QPainter paintJG(this);
    QPixmap pixJG;
    if(flagJG) //闪烁
    {
        countJG++;
        if (countJG>2)
            countJG = 0;
        switch(countJG)
        {
            case 1:pixJG.load("./img2/15.png");//14.jpg
            break;
        }
        paintJG.drawPixmap(255,0,38,41,pixJG);
    }

    //机油
    QPainter paintJY(this);
    QPixmap pixJY;
    if(flagJY) //闪烁
    {
        countJY++;
        if (countJY>2)
            countJY = 0;
        switch(countJY)
        {
            case 1:pixJY.load("./img2/22.png");//14.jpg
            break;
        }
        paintJY.drawPixmap(306,0,43,43,pixJY);
    }

    //flagLCM = 1; //粮仓满
    QPainter paintLCM(this);
    QPixmap pixLCM;
    if(flagLCM) //闪烁
    {
        countLCM++;
        if (countLCM>2)
            countLCM = 0;
        switch(countLCM)
        {
            case 1:pixLCM.load("./img2/7.png");//14.jpg
            break;
        }
        paintLCM.drawPixmap(357,0,43,43,pixLCM);
    }

    //flagFDJYR = 1; //发动机预热
    QPainter paintFDJYR(this);
    QPixmap pixFDJYR;
    if(flagFDJYR) //闪烁
    {
        countFDJYR++;
        if (countFDJYR>2)
            countFDJYR = 0;
        switch(countFDJYR)
        {
            case 1:pixFDJYR.load("./img2/17.png");//14.jpg
            break;
        }
        paintFDJYR.drawPixmap(357,0,43,43,pixFDJYR);
    }

    // flagGL = 1; //过滤
    QPainter paintGL(this);
    QPixmap pixGL;
    if(flagGL)
    {
        countGL++;
        if (countGL>2)
            countGL = 0;
        switch(countGL)
        {
            case 1:pixGL.load("./img2/20.png");//14.jpg
            break;
        }
        paintGL.drawPixmap(408,0,43,43,pixGL);
    }

    //flagYL = 1; //油量
    QPainter paintYL(this);
    QPixmap pixYL;
    if(flagYL)
    {
        countYL++;
        if (countYL>2)
            countYL = 0;
        switch(countYL)
        {
            case 1:pixYL.load("./img2/23.png");//14.jpg
            break;
        }
        paintYL.drawPixmap(459,0,43,43,pixYL);
    }


    //以上比例都是 + 51

    //flagBSQ = 1;//变速器
    QPainter paintBSQ(this);
    QPixmap pixBSQ;
    if(flagBSQ)
    {
        countBSQ++;
        if (countBSQ>2)
            countBSQ = 0;
        switch(countBSQ)
        {
            case 1:pixBSQ.load("./img2/16.png");//14.jpg
            break;
        }
        paintBSQ.drawPixmap(523,0,43,43,pixBSQ);

        //ecu
        QPainter paintECU(this);
        QPixmap pixECU;
        if(flagECU)
        {
            countECU++;
            if (countECU>2)
                countECU = 0;
            switch(countECU)
            {
                case 1:pixECU.load("./img2/91.png");//14.jpg
                break;
            }
            paintECU.drawPixmap(585,0,43,32,pixECU);
        }


        //flagPark = 1;//停车
        QPainter paintPark(this);
        QPixmap pixPark;
        if(flagPark)
        {
            countPark++;
            if (countPark>2)
                countPark = 0;
            switch(countPark)
            {
                case 1:pixPark.load("./img2/18.png");//14.jpg
                break;
            }
            paintPark.drawPixmap(640,0,43,34,pixPark);
        }

        //
        //flagFDJGZ = 1; //发动机故障
        QPainter paintFDJGZ(this);
        QPixmap pixFDJGZ;
        if(flagFDJGZ)
        {
            countFDJGZ++;
            if (countFDJGZ>2)
                countFDJGZ = 0;
            switch(countFDJGZ)
            {
                case 1:pixFDJGZ.load("./img2/8.png");//14.jpg
                break;
            }
            paintFDJGZ.drawPixmap(697,0,43,37,pixFDJGZ);
        }

        //flagRight = 1; //右转
        QPainter paintRight(this);
        QPixmap pixRight;
        if(flagRight)
        {
            countRight++;
            if (countRight>2)
                countRight = 0;
            switch(countRight)
            {
                case 1:pixRight.load("./img2/right.png");//14.jpg
                break;
            }
            paintRight.drawPixmap(752,0,48,48,pixRight);
        }









    }



}

//界面切换




void Widget::func()
{

     QString timeStr= QTime::currentTime().toString();     //绘制当前的时间

     ui->lineEdit->setText(timeStr);
}

void Widget::on_lineEdit_2_textEdited(const QString &arg1)
{
    bool ok = true;
    numtmp = arg1.toInt(&ok,10);

}

void Widget::on_pushButton_clicked()
{
    this->hide();
    dialog.show();
    dialog.exec();
    this->show();
}

void Widget::on_pushButton_2_clicked()
{
    this->hide();
    cameraobj.show();
    cameraobj.exec();

    this->show();
}


//测试用
void Widget::on_lineEdit_2_editingFinished()
{
    QString tmp = ui->lineEdit_2->text();
    ZhuangSu = tmp.toShort();
    /*
    DC = ZhuangSu;
    if(DC >7)
    {
        emit sendcamersignal();
    }
    */
}


//测试倒车信号来时，显示倒车视频。
//
void Widget::cameraslot()
{
    this->hide();
    cameraobj.show();
    cameraobj.exec();

    this->show();


    qDebug()<<"daoche:"<<DC<<endl;
}



