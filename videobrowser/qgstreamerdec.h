#ifndef QGSTREAMERDEC_H
#define QGSTREAMERDEC_H

#include <QThread>
#include <QDebug>

#define VGA_WIDTH 640
#define VGA_HEIGHT 480


enum PlayState{ Play,Pause,Finish};


class QGstreamerDec : public QThread
{
    Q_OBJECT
public:
    explicit QGstreamerDec(QObject *parent = 0);

    void startPlay(QString);
    void stopPlay();

    bool getPauseState();

    bool  isRun;

protected:
    void run();

signals:
    void sendFrame( unsigned char* img);
    void updatePlayState(PlayState state);

public slots:

private:

};

#endif // QGSTREAMERDEC_H
