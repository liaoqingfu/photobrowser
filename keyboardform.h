#ifndef KEYBOARDFORM_H
#define KEYBOARDFORM_H

#include <QWidget>
#include <QSignalMapper>
#include <qkeyboardbtn.h>

namespace Ui {
class KeyBoardForm;
}

class KeyBoardForm : public QWidget
{
    Q_OBJECT

public:
    explicit KeyBoardForm(QWidget *parent = 0);
    ~KeyBoardForm();

public slots:
    void setCapsLock();

signals:
    void sendWord(const QString &);
    void delWord();

private:
    Ui::KeyBoardForm *ui;

    QSignalMapper* signalMapper;

    bool isCaps;
};

#endif // KEYBOARDFORM_H
