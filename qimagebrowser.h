#ifndef QIMAGEBROWSER_H
#define QIMAGEBROWSER_H

#include <QWidget>
#include <QKeyEvent>
#include <QFutureWatcher>
#include <QHBoxLayout>
#include <QImage>
#include <QFuture>
#include <QMap>
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/qtconcurrentmapkernel.h>
#include <qimagelistwidget.h>
#include <QScrollBar>
#include <videoutil.h>
#include <qiconwidget.h>
#include <QMutex>
#include <keyboardform.h>
#include <qrenamedialog.h>
#include <qdeletedialog.h>
#include <qcreatedirdialog.h>
#include "qcustommenudialog.h"
#include "qdisplayphotodialog.h"
#include "qshowimagewidget.h"


extern  QMutex mutex;

namespace Ui {
class QImageBrowser;
}

class QImageBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit QImageBrowser(QWidget *parent = 0);
    ~QImageBrowser();

    void initLayout();

    void findDirs(QString path,QImageListWidget *list);

    void openFiles(QImageListWidget *listWidget);

    void findFiles(QString path);

protected:
    void keyPressEvent(QKeyEvent *e);
    //bool event(QEvent *e);

public slots:

    void openCustomMenu(QImageListWidget *list);

    void optImageListFile(int mode ,QListWidget *list);

    //year list clicked
    void yearListClicked(QListWidgetItem *yearItem);
    void monthListClicked(QListWidgetItem *monthItem);
    void dayListClicked(QListWidgetItem *dayItem);
    void picListCLicked(QListWidgetItem *picItem);

    void showImage(int num);

protected slots:

private:
    Ui::QImageBrowser *ui;

    QImageListWidget *yearListWidget;
    QImageListWidget *monthListWidget;
    QImageListWidget *dayListWidget;
    QImageListWidget *picListWidget;

    QString rootPath;
    QString secondPath;
    QString thirdPath;
    QString fourPath;

    QFutureWatcher<QImage> *imageWatcher;

};

#endif // QIMAGEBROWSER_H
