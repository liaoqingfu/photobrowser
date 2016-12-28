#ifndef QWARNTIPSDIALOG_H
#define QWARNTIPSDIALOG_H

#include <QDialog>
#include <QTimer>
namespace Ui {
class QWarnTipsDialog;
}

class QWarnTipsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QWarnTipsDialog(QWidget *parent = 0,QString message = 0);
    ~QWarnTipsDialog();

private:
    Ui::QWarnTipsDialog *ui;

    QTimer *timer;
};

#endif // QWARNTIPSDIALOG_H
