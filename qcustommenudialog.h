#ifndef QCUSTOMMENUDIALOG_H
#define QCUSTOMMENUDIALOG_H

#include <qdialog.h>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <qpushbutton.h>
#include <QListWidget>
#include <QListWidgetItem>
#include <qdebug.h>
#include <photobrowser/qiconwidget.h>
#include <photobrowser/qimagelistwidget.h>

class QCustomMenuDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QCustomMenuDialog(QWidget *parent = 0,QImageListWidget *list = 0);

protected:
    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *);
    void moveLeft();
    void moveRight();

signals:
    void optImageFile(int ,QListWidget *);
    void closeWidget();
public slots:


private:
    QImageListWidget *currentlist;

    int listnum;
    int currentnum;

};

#endif // QCUSTOMMENUDIALOG_H
