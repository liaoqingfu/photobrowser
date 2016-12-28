#include "qkeyboardbtn.h"

QKeyBoardBtn::QKeyBoardBtn(QWidget *parent) :
    QPushButton(parent)
{
    this->setStyleSheet("QPushButton{background-color: blue; \
                        color: white; \
                        border-radius: 10px; \
                        border: 2px groove gray;\
                        border-style: outset;}"
                        "QPushButton:focus{background-color:orange; \
                        color: white;}"
                        "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                         border-style: inset; }");
}

void QKeyBoardBtn::keyPressEvent(QKeyEvent *event)
{

    switch(event->key()){

    case Qt::Key_Return:
            if(this->hasFocus())
                emit clicked();
                break;
    default:
            QPushButton::keyPressEvent(event);
            break;

    }
}
