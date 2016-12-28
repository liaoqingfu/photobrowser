#ifndef QDELETEDIALOG_H
#define QDELETEDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <qfileinfo.h>
#include <qfile.h>
#include <qimagelistwidget.h>
#include <qiconwidget.h>
namespace Ui {
class QDeleteDialog;
}

class QDeleteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QDeleteDialog(QWidget *parent = 0,QImageListWidget *list = 0);
    ~QDeleteDialog();

signals:
    void deleteItem();


private slots:
    void on_sureBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::QDeleteDialog *ui;

    QImageListWidget *currentlist;
};

#endif // QDELETEDIALOG_H
