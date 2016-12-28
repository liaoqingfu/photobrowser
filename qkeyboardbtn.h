#ifndef QKEYBOARDBTN_H
#define QKEYBOARDBTN_H

#include <QWidget>
#include <QKeyEvent>
#include <QPushButton>

class QKeyBoardBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit QKeyBoardBtn(QWidget *parent = 0);

signals:

public slots:

protected:
    void keyPressEvent(QKeyEvent *event);

};

#endif // QKEYBOARDBTN_H
