#ifndef QRENAMEDIALOG_H
#define QRENAMEDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include<QVBoxLayout>
#include <QLineEdit>
#include <qtimer.h>
#include <QListWidget>
#include <QListWidgetItem>
#include <qimagelistwidget.h>
#include <qwarntipsdialog.h>
#include "keyboardform.h"

class QRenameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QRenameDialog(QWidget *parent = 0,QImageListWidget *list = 0);
    ~QRenameDialog();

signals:
    void changeName();

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

    QString suffix;
};

#endif // QRENAMEDIALOG_H
