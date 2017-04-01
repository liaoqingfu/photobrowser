#ifndef QCREATEDIRDIALOG_H
#define QCREATEDIRDIALOG_H

#include <QDialog>
#include <QDir>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QHBoxLayout>
#include<QVBoxLayout>
#include<QLineEdit>
#include <qtimer.h>
#include <QListWidget>
#include <QListWidgetItem>
#include <photobrowser/qimagelistwidget.h>
#include <photobrowser/qwarntipsdialog.h>
#include <keyboardform.h>
#include <qkeyboardbtn.h>

class QCreateDirDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QCreateDirDialog(QWidget *parent = 0,QImageListWidget *list = 0);
    ~QCreateDirDialog();

signals:
    void  closeWidget();

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

    bool isCreate;

};

#endif // QCREATEDIRDIALOG_H
