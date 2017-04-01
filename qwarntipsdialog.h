#ifndef QWARNTIPSDIALOG_H
#define QWARNTIPSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QTimer>

class QWarnTipsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QWarnTipsDialog(QWidget *parent = 0,QString message = 0);
    ~QWarnTipsDialog();

protected:
    void closeEvent(QCloseEvent *);

signals:
    void closeWidget();

private:
    QLabel *warnlogo;
    QLabel *tipsmessage;
    QTimer *timer;
};

#endif // QWARNTIPSDIALOG_H
