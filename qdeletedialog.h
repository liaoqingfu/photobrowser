#ifndef QDELETEDIALOG_H
#define QDELETEDIALOG_H

#include <QDialog>
#include <QCoreApplication>
#include <QProgressBar>
#include <QListWidget>
#include <QPushButton>
#include <qfileinfo.h>
#include <qfile.h>
#include <photobrowser/qimagelistwidget.h>
#include <photobrowser/qiconwidget.h>
#include <photobrowser/qclearsd.h>
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
    void closeWidget();
    void deleteItem();
private slots:
    void on_sureBtn_clicked();
    void on_cancelBtn_clicked();
    void on_progressBar_valueChanged(int value,QString path);


protected:
    void closeEvent(QCloseEvent *);

    bool FindFile(const QString & path);

public slots:
    void getKeyEvent(int key);

private:
    QLabel *label;
    QPushButton *sureBtn;
    QPushButton *cancelBtn;
    QProgressBar *progressBar;
    QImageListWidget *currentlist;

    bool isDelete;
};

#endif // QDELETEDIALOG_H
