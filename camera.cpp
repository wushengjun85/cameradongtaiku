#include "camera.h"
#include "ui_camera.h"
#include <QDebug>
#include<QPainter>
#include<QString>


extern unsigned char DC;  //倒车信号（倒车+）
bool ok = true;
camera::camera(QWidget *parent) :
    QDialog(parent),

    ui(new Ui::camera)
{
    ui->setupUi(this);
    resize(800,600);

    connect(ui->show_ptn,SIGNAL(clicked()),this,SLOT(start_thread()));

    connect(ui->stop_ptn,SIGNAL(clicked()),this,SLOT(stop_thread()));
    connect(ui->quit_ptn,SIGNAL(clicked()),this,SLOT(close()));
//    if (ok)
//    {
//         start_thread();
//    }
//    else
//    {
//         //stop_thread();
//    }

/*
    if (DC == 0)
    {
        this->close();
    }
*/
    camera::move(0,0);//回到原来主窗口的位置
}

void camera::start_thread()
{
    video = new video_thread();
    video->start();
    connect(video,SIGNAL(image_data(const QImage &)),this,SLOT(show_picture(const QImage &)));
   // connect(video,SIGNAL(image_data(QImage &)),this,SLOT(show_picture(QImage &)));

}
void camera::show_picture(const QImage &img)
//void camera::show_picture(QImage &img)
{

    //qreal width = pix.width(); //获得以前图片的宽和高
    //qreal height = pix.height();
    //img = img.scaled(360,240,Qt::KeepAspectRatio);
    //img.scaled()
    ui->show_label->setPixmap(QPixmap::fromImage(img));
}
void camera::stop_thread()
{
    delete video;
}



void camera::on_pushButton_clicked()
{
    this->close();
}

void camera::paintEvent(QPaintEvent *event)
{
    QPainter dp(this);
    QPixmap dppix;
    dppix.load("./img2/camera.bmp");
    dp.drawPixmap(0,0,800,600,dppix);
}

camera::~camera()
{
    delete ui;
}
