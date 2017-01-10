#include "master.h"

Master::Master(QObject *parent) : QThread(parent)
{
    //InitForm();
    countNum = 1;
    myCom = new MySerialPort(COM);
    timer = new QTimer(this);
    //InitForm();

    isRun = true;

    //电池标识位
    isBat = false;

    //LED标识位
    isLed = false;

    //按键标识位
    isKey = false;

    //摇杆使用者标识位
    isRocker = false;
    isWriteRocker = false;

    //摇杆使用者标识
    rockerUser = SLAVE_Y;

    curRockerUser = SLAVE_Y;

    //读写标识位
    isREADorWRITE = false;

    //摇杆值标识位
    isRockerValue = false;
}


//初始化串口信息
void Master::InitForm()
{
/*
    myCom->setPortName(COM);
    myCom->setBaudRate(QSerialPort::Baud9600);
    myCom->setDataBits(QSerialPort::Data8);
    myCom->setParity(QSerialPort::NoParity);
    myCom->setStopBits(QSerialPort::OneStop);
    myCom->setFlowControl(QSerialPort::NoFlowControl);

    if(myCom->open(QIODevice::ReadWrite))
    {

        //connect(myCom,SIGNAL(readyRead()),this,SLOT(readCom()));
        connect(timer,SIGNAL(timeout()),this,SLOT(readInfo()));
        timer->start(500);
        //timer->moveToThread(this);
        //connect(this,SIGNAL(started()),this,SLOT(startRead()));
        qDebug()<<"open ok!";
    }
    else
    {
        qDebug()<<"open error!";
    }

    //connect(myCom,SIGNAL(readyRead()),this,SLOT(readCom()));
*/
    myCom->setCOM(COM,19200,8,'N',1);
    //qDebug()<<myCom->isOpen();
    connect(myCom,SIGNAL(sigDataReady(QByteArray)),this,SLOT(readCom(QByteArray)));

}

//run
void Master::run()
{
    while(isRun)
    {
            this->readInfo();
            this->msleep(200);
    }
}

//关闭串口
void Master::CloseCom()
{
    //timer->stop();
    myCom->close();
}

//读数据
void Master::readCom(QByteArray data)
{
    //qDebug()<<data.toHex()<<"data";

    //获取电池信息
    if(isBat)
    {
        dataArray += data;
        if(dataArray.size() == 7)
        {


            QString cmd(crcOk(dataArray));
            if(cmd.mid(0,6) == (SLAVE + READ + tr("02")))
            {
                //发送电量信息
                emit sendElecInfo(cmd.mid(6,1),cmd.mid(7,1),cmd.mid(9,1));
            }

            isBat = false;
        }
    }


    //获取LED和KEY信息
    if(isKey)
    {


        dataArray += data;

        if(dataArray.size() == 9)
        {


            QString cmd(crcOk(dataArray));
            if(cmd.mid(0,6) == (SLAVE + READ + tr("04")))
            {
                //发送Key信息
                emit sendKeyInfo(cmd.mid(6,4));
                emit sendLedInfo(cmd.mid(10,4));
            }

            isKey = false;

            //qDebug()<<dataArray.toHex();
        }
    }

    //获取摇杆使用者
    if(isRocker)
    {
        dataArray += data;
        qDebug()<<data;
        if(dataArray.size() == 8)
        {


            QString cmd(crcOk(dataArray));
            if(cmd.mid(0,8) == (SLAVE + WRITE + Y_ADDRESS))
            {
                //发送Rocker信息
                //curRockerUser = cmd.mid(8,4);
                //qDebug()<<curRockerUser;
                emit sendRockerInfo(cmd.mid(8,4));
                //curRockerUser = cmd.mid(8,4);
            }

            isRocker = false;

            //qDebug()<<dataArray.toHex();
        }
    }

    //获取摇杆XY的值
    if(isRockerValue)
    {
        dataArray += data;

        if(dataArray.size() == 9)
        {


            QString cmd(crcOk(dataArray));
            if(cmd.mid(0,6) == (SLAVE + READ + tr("04")))
            {
                //发送Rocker x y坐标信息
                emit sendRockerXYInfo(cmd.mid(6,4),cmd.mid(10,4));

            }

            isRockerValue = false;

            //qDebug()<<dataArray.toHex();
        }
    }

}

//CRC校验
QByteArray Master::crcOk(QByteArray data)
{
    quint16 crc_string = checksum(data.left(data.size() - 2));
    if(crc_string == data.right(2).toHex().toInt(NULL,16))
    {
        return data.toHex();
    }

    return QByteArray("");
}

//写数据
void Master::writeCom(QByteArray data)
{

    if(myCom->write(data))
    {
        //qDebug()<<data.toHex();
    }
    //myCom->EchoCommand(QString(data));
}

//命令解析
void Master::AnalysisCmd()
{

}

//写入摇杆使用者
void Master::writeRocker(QString user)
{
//        qDebug()<<"write rocker";
        //qDebug()<<isBat<<isKey<<isRockerValue;

    if(!isBat && !isKey  && !isRockerValue && !isRocker)
    {



        QString cmd;
        QString crc;
        cmd = SLAVE + WRITE + Y_ADDRESS +  user;
        crc = ToStr(checksum(QByteArray::fromHex(cmd.toLatin1())),4);
        cmd = cmd + crc;
        this->writeCom(QByteArray::fromHex(cmd.toLatin1()));
        dataArray.clear();
        isRocker = true;
    }
}

//写入舵机最大角度(0~1000)
//RUDDER_ADDRESS
void Master::writeRudderMaxValue(int value)
{
    QString cmd;
    QString crc;
    cmd = SLAVE + WRITE + RUDDER_ADDRESS + ToStr(value,4);
    crc = ToStr(checksum(QByteArray::fromHex(cmd.toLatin1())),4);
    cmd = cmd + crc;
    this->writeCom(QByteArray::fromHex(cmd.toLatin1()));
}

//写入LED最大电流(0~500mA)
void Master::writeLedMaxValue(int value)
{
    QString cmd;
    QString crc;
    cmd = SLAVE + WRITE + A_ADDRESS + ToStr(value,4);
    crc = ToStr(checksum(QByteArray::fromHex(cmd.toLatin1())),4);
    cmd = cmd + crc;
    this->writeCom(QByteArray::fromHex(cmd.toLatin1()));
}

//读取电池状态
void Master::readBatStatus(int count)
{
    //isBat = true;
    if(!isBat && !isKey  && !isRockerValue && !isRocker)
    {
        QString cmd;
        QString crc;
        cmd = SLAVE + READ + BAT_ADDRESS + ToStr(count,4);
        crc = ToStr(checksum(QByteArray::fromHex(cmd.toLatin1())),4);
        //crc = crc.right(2) + crc.left(2);
        cmd = cmd + crc;
        this->writeCom(QByteArray::fromHex(cmd.toLatin1()));
        dataArray.clear();
        isBat = true;
    }
}

//读取按键键值/LED亮度
void Master::readKeyValue(int count)
{
    if(!isBat && !isKey  && !isRockerValue && !isRocker)
    {
        QString cmd;
        QString crc;
        cmd = SLAVE + READ + KEY_ADDRES + ToStr(count,4);
        crc = ToStr(checksum(QByteArray::fromHex(cmd.toLatin1())),4);
        cmd = cmd + crc;
        this->writeCom(QByteArray::fromHex(cmd.toLatin1()));
        dataArray.clear();
        isKey = true;
    }

}

//读取X轴坐标
void Master::readXValue(int count)
{
    if(!isBat && !isKey  && !isRockerValue && !isRocker)
    {
        QString cmd;
        QString crc;
        cmd = SLAVE + READ + ROCKER_X_ADDRESS + ToStr(count,4);
        crc = ToStr(checksum(QByteArray::fromHex(cmd.toLatin1())),4);
        cmd = cmd + crc;
        this->writeCom(QByteArray::fromHex(cmd.toLatin1()));
        dataArray.clear();
        isRockerValue = true;
    }
}



//读取电量等信息
void Master::readInfo()
{

    this->readBatStatus(1);
    while(isBat)
    {
        msleep(10);
    }

    this->readKeyValue(2);
    while(isKey)
    {
        msleep(10);
    }


    if(isWriteRocker)
    {
        this->writeRocker(rockerUser);
        while(isRocker)
        {
            msleep(10);
        }
        isWriteRocker = false;
    }


    if(curRockerUser == MASTER_Y)
    {
        this->readXValue(2);
        while(isRockerValue)
        {
            msleep(10);
        }
    }

}


//设置控制摇杆使用者
void Master::setRockerUser(QString user)
{
    rockerUser = user;
}



