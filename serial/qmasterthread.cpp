#include "qmasterthread.h"

QMasterThread::QMasterThread()
{
    myCom = new QSerialPort;
    myCom->setPortName(COM);
    myCom->setBaudRate(QSerialPort::Baud9600);
    myCom->setDataBits(QSerialPort::Data8);
    myCom->setParity(QSerialPort::NoParity);
    myCom->setStopBits(QSerialPort::OneStop);
    myCom->setFlowControl(QSerialPort::NoFlowControl);

    isRun = true;
}

//打开串口
void QMasterThread::openSerial()
{
    if(myCom->open(QIODevice::ReadWrite))
    {
        myCom->clear();
        qDebug()<<"open ok!";
        start();
    }
    else
    {
        qDebug()<<"open error!";
    }
}

//run
void QMasterThread::run()
{
    while(isRun){

        myCom->write(QByteArray("01"));

        this->msleep(500);

        qDebug()<<"01";
    }

}

//关闭串口
void QMasterThread::closeCom()
{
    isRun = false;
    this->msleep(500);
    myCom->close();
}
