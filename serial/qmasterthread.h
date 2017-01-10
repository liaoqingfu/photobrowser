#ifndef QMASTERTHREAD_H
#define QMASTERTHREAD_H

#include <QThread>
#include <QSerialPort>
#include <QtDebug>

#include "serial/address.h"

class QMasterThread : public QThread
{
    Q_OBJECT
public:
    explicit QMasterThread();

    //打开串口
    void openSerial();

    //run
    void run();

    //关闭串口
    void closeCom();

signals:

public slots:

private:
    QSerialPort *myCom;

    bool isRun;

};

#endif // QMASTERTHREAD_H
