#ifndef QMYSERIAL_H
#define QMYSERIAL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtDebug>
#include <QString>
#include <QByteArray>
#include <QThread>
#include <QTimer>
#include <stdio.h>


class QMySerial : public QThread
{
    Q_OBJECT
public:
    QMySerial(QObject *parent = 0);

    void openSerial(QString);
    void closeSerial();

    virtual void run();

public slots:
    void readSerial();
    void writeSerial(QByteArray);
    void sendSerial();

signals:
    void sendRotate();
    void sendLock(QByteArray);
    void sendPhoto();
    void sendLight(QByteArray);
    void sendSet();
    void sendStartRecord();
    void sendStopRecord();

    void sendKey(int);


public:
    QSerialPort *serial;

    bool isRecord;

    QTimer *timer;
};

#endif // QMYSERIAL_H
