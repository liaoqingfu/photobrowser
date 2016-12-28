#ifndef QCUSTOMMENUDIALOG_H
#define QCUSTOMMENUDIALOG_H

#include <qdialog.h>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <qpushbutton.h>
#include <QListWidget>
#include <QListWidgetItem>
#include <qdebug.h>
#include <qiconwidget.h>
#include <qimagelistwidget.h>

class QCustomMenuDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QCustomMenuDialog(QWidget *parent = 0,QImageListWidget *list = 0);

protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void optImageFile(int ,QListWidget *);
public slots:


private:
    QImageListWidget *currentlist;

    int listnum;
    int currentnum;

};

#endif // QCUSTOMMENUDIALOG_H
