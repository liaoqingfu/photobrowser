#ifndef MASTER_H
#define MASTER_H

#include <QObject>
#include <QtDebug>
#include <QTimer>
#include <QThread>
#include <QApplication>

//#define COM "/dev/ttySAC3"

#include <QtSerialPort/QSerialPort>
#include "crc16.h"
#include "address.h"
#include "serial/myserialport.h"

class Master : public QThread
{
    Q_OBJECT
public:
    explicit Master(QObject *parent = 0);

    void writeCom(QByteArray data);

    void InitForm();

    //CRC校验
    QByteArray crcOk(QByteArray data);


    //run
    void run();

    bool isRun;

    //关闭串口
    void CloseCom();

    //命令解析
    void AnalysisCmd();

    //写入摇杆使用者
    void writeRocker(QString user);

    //写入舵机最大角度(0~1000)
    void writeRudderMaxValue(int value);

    //写入LED最大电流(0~500mA)
    void writeLedMaxValue(int value);

    //读取电池状态
    void readBatStatus(int count);

    //读取按键键值
    void readKeyValue(int count);

    //读取X轴坐标
    void readXValue(int count);

    //摇杆标识位
    bool isRocker;
    bool isWriteRocker;

    //读写操作
    bool isREADorWRITE;


    //设置控制摇杆使用者
    void setRockerUser(QString user);

    //摇杆使用者
     QString rockerUser;
     QString curRockerUser;

public slots:
    //读取串口信息
    void readCom(QByteArray);

    //读取电量等信息
    void readInfo();

signals:
    void sendInfo(QByteArray);

    //发送电量信息
    void sendElecInfo(QString no_enough,QString charge,QString value);

    //发送LED信息
    void sendLedInfo(QString value);

    //发送Key信息
    void sendKeyInfo(QString value);

    //发送Rocker使用者信息
    void sendRockerInfo(QString value);

    //发送Rocker x-y坐标信息
    void sendRockerXYInfo(QString x,QString y);

protected:

private:
     MySerialPort *myCom;

     //读取次数
     int countNum;

     //定时器
     QTimer *timer;

     //电池电量标识位
     bool isBat;

     //LED标识位
     bool isLed;

     //按键标识位
     bool isKey;

    //摇杆值标识位
     bool isRockerValue;



     //缓存串口数据
     QByteArray dataArray;



};

#endif // MASTER_H
