#include "qmyserial.h"

extern bool isMount;
extern bool isMainSubOpen;

QMySerial::QMySerial(QObject *parent) :
    QThread(parent)
{
    isRecord = false;
    serial = new QSerialPort(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(sendSerial()));
    this->openSerial(tr("/dev/ttymxc2"));
    timer->start(1000);
}


void QMySerial::run()
{
    while(1)
    {
        this->writeSerial(QByteArray::fromHex(QByteArray("02")));
        usleep(1000000);
    }
}


void QMySerial::sendSerial()
{
    this->writeSerial(QByteArray("02"));
}

void QMySerial::openSerial(QString name)
{

    serial->setPortName(name);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {

        //connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
          //      SLOT(handleError(QSerialPort::SerialPortError)));

    //! [2]
        //connect(serial, SIGNAL(readyRead()), this, SLOT(readSerial()));
    //! [2]
       // connect(serial, SIGNAL(getData(QByteArray)), this, SLOT(writeSerial(QByteArray)));
        connect(serial, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }

}

void QMySerial::closeSerial()
{
    serial->close();
}

void QMySerial::readSerial()
{
    QByteArray data = serial->readAll();

    qDebug()<<data.toHex();
    /*
    switch(data.toHex().toInt(0,16)){

    case 0x01:
        emit sendKey(Qt::Key_Up);
        break;

    case 0x02:
        emit sendKey(Qt::Key_Down);
        break;

    case 0x03:
        emit sendKey(Qt::Key_Left);
        break;

    case 0x04:
        emit sendKey(Qt::Key_Right);
        break;

    case 0x05:
        emit sendKey(Qt::Key_Return);
        break;

    default:
        break;
    }
*/
/*
    qDebug()<<data;
    if(data == QByteArray("R"))
    {
        emit sendRotate();
    }
    else if(data == QByteArray("L"))
    {
        emit sendLock(data);
    }
    else if(data == QByteArray("U"))
    {
        emit sendLock(data);
    }
    else if(data == QByteArray("T"))
    {
        if(isMount)
            emit sendPhoto();
    }
    else if(data == QByteArray("l"))
    {
        emit sendLight(data);
    }
    else if(data == QByteArray("S"))
    {
        emit sendSet();
    }
    else if(data == QByteArray("r"))
    {
        if(isMount)
        {
            if(!isRecord)
            {
                emit sendStartRecord();
                isRecord = true;
            }
                else
            {
                emit sendStopRecord();
                isRecord = false;
            }
        }
      }
*/
 /*   switch(data){

    case QByteArray("R90"):
        emit sendRotate(data);
        break;

    case QByteArray("R180"):
        emit sendRotate(data);
        break;

    case QByteArray("R270"):
        emit sendRotate(data);
        break;

    default:
        break;

    }*/

    //emit sendSerialInfo(data);

    return;
}

void QMySerial::writeSerial(QByteArray data)
{
    serial->write(data);
    qDebug()<<data;
}

