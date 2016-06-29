#include "dialog.h"
#include "ui_dialog.h"
#include<QPainter>
#include<QString>

/*******************************************************************************/
//Date:2016.6.1
//Author:wushengjun
//speed defined
int zl = 5;

/*******************************************************************************/
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    resize(800,600);
    Dialog::move(0,0);//回到原来主窗口的位置
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint|Qt::WindowTitleHint);//删除 最小化、最大化、关闭按钮
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    this->close();
}

void Dialog::paintEvent(QPaintEvent *event)
{
    QPainter dp(this);
    QPixmap dppix;
    dppix.load("./img2/setup.bmp");
    dp.drawPixmap(0,0,800,600,dppix);
}

void Dialog::on_pushButton_2_clicked()
{
    zl--;
    ui->label->setText(QString::number(zl,10));
}

void Dialog::on_pushButton_3_clicked()
{
    zl++;
    ui->label->setText(QString::number(zl,10));
}
