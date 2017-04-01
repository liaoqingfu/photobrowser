#ifndef QRENAMEDIALOG_H
#define QRENAMEDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include<QVBoxLayout>
#include <QLineEdit>
#include <QCoreApplication>
#include <qtimer.h>
#include <QListWidget>
#include <QListWidgetItem>
#include <photobrowser/qimagelistwidget.h>
#include <photobrowser/qwarntipsdialog.h>
#include "keyboardform.h"
#include "qkeyboardbtn.h"

class QRenameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QRenameDialog(QWidget *parent = 0,QImageListWidget *list = 0);
    ~QRenameDialog();

signals:
    void changeName();
    void closeWidget();


public slots:
    void getKeyEvent(int key);

private slots:
    void on_sureBtn_clicked();

    void on_cancelBtn_clicked();

    void setLineEdit(QString name);

    void delLineEdit();

    void check_lineEdit();

protected:
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *e);

private:

    QLabel *tittle;
    QKeyBoardBtn *sureBtn;
    QKeyBoardBtn *cancelBtn;
    QLineEdit *lineEdit;
    KeyBoardForm *keyboard;

    QImageListWidget *currentlist;

    QTimer *timer;
    QString currentname;
    QString suffix;
    bool isRename;
};

#endif // QRENAMEDIALOG_H
