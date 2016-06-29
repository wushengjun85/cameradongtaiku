#include "signalcan.h"


SignalCan::SignalCan(QObject *parent) :
    QThread(parent)
{
}

void SignalCan::run()
{
    int master;
    int mycanid;
    Canread canread;
    canread.initcan(&mycanid);
    canread.test_can_rw(mycanid,master);
    canread.shutdownfd(mycanid);


    //    while (1)
    //    {
    //     can test;
    //     test.fun();

    // QThread::sleep(1);
    //    candata.data[0] = 77;
    //    emit sendcansignal(candata);
    //    }



}




