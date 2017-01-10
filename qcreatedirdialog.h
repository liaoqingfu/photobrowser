#ifndef QCREATEDIRDIALOG_H
#define QCREATEDIRDIALOG_H

#include <QDialog>
#include <QDir>
#include <QHBoxLayout>
#include<QVBoxLayout>
#include<QLineEdit>
#include <qtimer.h>
#include <QListWidget>
#include <QListWidgetItem>
#include <qimagelistwidget.h>
#include <qwarntipsdialog.h>
#include <keyboardform.h>


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
    QLabel *tittle;
    QPushButton *sureBtn;
    QPushButton *cancelBtn;
    QLineEdit *lineEdit;
    KeyBoardForm *keyboard;

    QImageListWidget *currentlist;

    QTimer *timer;

};

#endif // QCREATEDIRDIALOG_H
