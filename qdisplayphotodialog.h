#ifndef QDISPLAYPHOTODIALOG_H
#define QDISPLAYPHOTODIALOG_H

#include <QDialog>
#include <qimagelistwidget.h>
#include <QPainter>
#include <QPixmap>

namespace Ui {
class QDisplayPhotoDialog;
}

class QDisplayPhotoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QDisplayPhotoDialog(QWidget *parent = 0,QImageListWidget *list = 0);
    ~QDisplayPhotoDialog();

protected:
    void keyPressEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *e);

private:
    Ui::QDisplayPhotoDialog *ui;

    QLabel *numberlabel;
    QLabel *zoomlabel;
    QLabel *filenamelabel;

    QPixmap backgroundpixmap;

    QImageListWidget *currentlist;

    bool isFilp;
};

#endif // QDISPLAYPHOTODIALOG_H
