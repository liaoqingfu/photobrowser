#include "qclearsd.h"
#include <iostream>

using namespace std;

QClearSd::QClearSd(QObject *parent) :
    QObject(parent)
{

    nFiles = 0;
    rmFiles = 0;
    percent = 0;
}


/***********************************************
 *
 *               目录文件个数统计
 *
 * *********************************************/
bool QClearSd::FindFile(const QString & path)
{
    QDir dir(path);
    if (!dir.exists())
        return false;
    dir.setFilter(QDir::Dirs|QDir::Files);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    int i=0;
    do{

        QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName()=="." || fileInfo.fileName()=="..")
        {
            i++;
            continue;
        }

        bool bisDir=fileInfo.isDir();

        if(bisDir)
        {
            nFiles++;
            FindFile(fileInfo.filePath());

        }
        else
        {
            nFiles++;
        }
        i++;

      QCoreApplication::processEvents();

    }while(i<list.size());

    cout << "Total Files is：" << nFiles << endl;

    return true;

}

/****************************************************
 *
 *              目录下的文件和子目录的删除
 *
 * *************************************************/
void QClearSd::removefilesindir(const QString& path)
{
    QDir dir(path);
    QFileInfoList info_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::AllDirs);

    if(dir.exists())
    {
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
    }

    foreach(QFileInfo file_info, info_list)
    {
        if (file_info.isDir())
        {
            removefilesindir(file_info.absoluteFilePath());
        }
        else if (file_info.isFile())
        {
            QFile file(file_info.absoluteFilePath());
            file.remove();

            rmFiles ++;
            percent = (int((double)rmFiles / nFiles * 100));

            QString path;
            path = info_list.at(0).absoluteFilePath();
            emit FileChanged(percent, path);
        }

          QCoreApplication::processEvents();
    }

    QDir temp_dir;
    if(temp_dir.rmdir(path))
    {
        rmFiles ++;
        //          QString path;
        //          path = info_list.at(0).absoluteFilePath();
        percent = int((double)(rmFiles) / (nFiles) * 100);
        emit FileChanged(percent, path);
    }

    return;
}


