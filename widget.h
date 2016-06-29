#ifndef WIDGET_H
#define WIDGET_H
#include"dialog.h"
#include"signalcan.h"

#include"camera.h"
#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private:
    Ui::Widget *ui;
    Dialog dialog;
    camera cameraobj;

    //线程
    SignalCan *mycan;

protected:
    void paintEvent(QPaintEvent *event);
private slots:
    void func();
    void on_lineEdit_2_textEdited(const QString &arg1);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_lineEdit_2_editingFinished();

    void cameraslot();

signals:
    void sendcamersignal();
};

#endif // WIDGET_H
