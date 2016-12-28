#ifndef QCREATEDIRDIALOG_H
#define QCREATEDIRDIALOG_H

#include <QDialog>
#include <QDir>
#include <qtimer.h>
#include <QListWidget>
#include <QListWidgetItem>
#include <qimagelistwidget.h>
#include <qwarntipsdialog.h>

namespace Ui {
class QCreateDirDialog;
}

class QCreateDirDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QCreateDirDialog(QWidget *parent = 0,QImageListWidget *list = 0);
    ~QCreateDirDialog();

private slots:
    void on_sureBtn_clicked();

    void on_cancelBtn_clicked();

    void setLineEdit(QString name);

    void delLineEdit();

    void check_lineEdit();

private:
    Ui::QCreateDirDialog *ui;

    QImageListWidget *currentlist;

    QTimer *timer;

};

#endif // QCREATEDIRDIALOG_H
