#ifndef QCLEARSD_H
#define QCLEARSD_H

#include <QObject>
#include <QDir>
#include <QApplication>
#include <QFileInfoList>
#include <QFileInfo>
#include <QFile>
#include <QtDebug>
#include <QString>
#include<vector>
#include<iostream>
#include <QDialog>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/statfs.h>


class QClearSd : public QObject
{
    Q_OBJECT

public:
    explicit QClearSd(QObject *parent = 0);
    bool FindFile(const QString & path);
    void removefilesindir(const QString& path);

signals:
    void FileChanged(int, QString);

private:
    int nFiles;
    int rmFiles;
    int percent;
};

#endif // QCLEARSD_H
