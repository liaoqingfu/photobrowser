#include "qvideobrowser.h"

QVideoBrowser::QVideoBrowser(QWidget *parent,QString videopath) :
    QWidget(parent)
{
    this->resize(640,480);
    this->setWindowFlags(this->windowFlags()  | Qt::FramelessWindowHint);

    this->videopath = videopath;
    this->initWidget();
}

QVideoBrowser::~QVideoBrowser()
{
    delete playlogo;
    delete timeLabel;
    delete showvideo;
}

void QVideoBrowser::initWidget()
{
    playlogo = new QLabel(this);
    playlogo->resize(120,120);
    playlogo->move((this->width() - 150)/2,(this->height() - 150)/2);
    playlogo->hide();

    QFont font;
    font.setPixelSize(18);

    timeLabel  = new QLabel(this);
    timeLabel->resize(150,20);
    timeLabel->setText("0.10/1.34");
    timeLabel->setStyleSheet("color:white");
    timeLabel->move(10,450);
    timeLabel->setFont(font);

    showvideo = new QShowVieoWidget(this,videopath);
    showvideo->lower();
    connect(showvideo,SIGNAL(sendEndFlag(PlayState)),this,SLOT(updatePlayState(PlayState)));
    showvideo->move(0,0);

}

void QVideoBrowser::updatePlayState(PlayState state)
{
    if(state == Play)
    {

    }
    else if(state == Pause)
    {


    }
    else if(state == Finish)
    {
//        playlogo->setStyleSheet("background-color:transparent;border-image: url(:/images/play.png)");
//        playlogo->show();
        this->close();
    }

}


void QVideoBrowser::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter)
    {   
            this->close();
    }
    else if(e->key() == Qt::Key_Return)
    {
        if(showvideo->OVideoDec->getPlayState())
        {
            showvideo->OVideoDec->setPauseState(( showvideo->OVideoDec->getPauseState()  ? false : true ));
            if(showvideo->OVideoDec->getPauseState())
            {
                playlogo->setStyleSheet("background-color:transparent;border-image: url(:/images/pause.png)");
                playlogo->show();
            }
            else
            {
                playlogo->hide();
            }

        }
        else
        {
            showvideo->OVideoDec->start();
            playlogo->hide();
        }
    }

    QWidget::keyPressEvent(e);
}
