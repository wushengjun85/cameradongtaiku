/********************************************************************************
** Form generated from reading UI file 'camera.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERA_H
#define UI_CAMERA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_camera
{
public:
    QPushButton *show_ptn;
    QPushButton *stop_ptn;
    QPushButton *quit_ptn;
    QLabel *show_label;
    QPushButton *pushButton;

    void setupUi(QDialog *camera)
    {
        if (camera->objectName().isEmpty())
            camera->setObjectName(QString::fromUtf8("camera"));
        camera->resize(685, 622);
        show_ptn = new QPushButton(camera);
        show_ptn->setObjectName(QString::fromUtf8("show_ptn"));
        show_ptn->setGeometry(QRect(20, 10, 131, 31));
        stop_ptn = new QPushButton(camera);
        stop_ptn->setObjectName(QString::fromUtf8("stop_ptn"));
        stop_ptn->setGeometry(QRect(190, 10, 151, 31));
        quit_ptn = new QPushButton(camera);
        quit_ptn->setObjectName(QString::fromUtf8("quit_ptn"));
        quit_ptn->setGeometry(QRect(380, 10, 141, 31));
        show_label = new QLabel(camera);
        show_label->setObjectName(QString::fromUtf8("show_label"));
        show_label->setGeometry(QRect(230, 110, 341, 261));
        pushButton = new QPushButton(camera);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(570, 10, 101, 31));

        retranslateUi(camera);

        QMetaObject::connectSlotsByName(camera);
    } // setupUi

    void retranslateUi(QDialog *camera)
    {
        camera->setWindowTitle(QApplication::translate("camera", "Dialog", 0, QApplication::UnicodeUTF8));
        show_ptn->setText(QApplication::translate("camera", "start", 0, QApplication::UnicodeUTF8));
        stop_ptn->setText(QApplication::translate("camera", "stop", 0, QApplication::UnicodeUTF8));
        quit_ptn->setText(QApplication::translate("camera", "quit", 0, QApplication::UnicodeUTF8));
        show_label->setText(QString());
        pushButton->setText(QApplication::translate("camera", "back", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class camera: public Ui_camera {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERA_H
