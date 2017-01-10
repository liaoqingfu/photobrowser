#-------------------------------------------------
#
# Project created by QtCreator 2016-12-22T15:02:05
#
#-------------------------------------------------

QT       += core gui serialport
QT     += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = photobrowser
TEMPLATE = app


SOURCES += main.cpp\
        qimagebrowser.cpp \
    qimagelistwidget.cpp \
    qiconwidget.cpp \
    videoutil.cpp \
    qcustommenudialog.cpp \
    keyboardform.cpp \
    qkeyboardbtn.cpp \
    qrenamedialog.cpp \
    qdeletedialog.cpp \
    qcreatedirdialog.cpp \
    qwarntipsdialog.cpp \
    qshowimagewidget.cpp \
    videobrowser/PlayWidgetcpy.cpp \
    videobrowser/qshowvieowidget.cpp \
    videobrowser/qvideobrowser.cpp \
    videobrowser/videodec.cpp \
    serial/crc16.cpp \
    serial/master.cpp \
    serial/myserialport.cpp \
    serial/qmasterthread.cpp \
    serial/qmyserial.cpp

HEADERS  += qimagebrowser.h \
    qimagelistwidget.h \
    qiconwidget.h \
    videoutil.h \
    qcustommenudialog.h \
    keyboardform.h \
    qkeyboardbtn.h \
    qrenamedialog.h \
    qdeletedialog.h \
    qcreatedirdialog.h \
    qwarntipsdialog.h \
    qshowimagewidget.h \
    videobrowser/PlayWidgetcpy.h \
    videobrowser/qshowvieowidget.h \
    videobrowser/qvideobrowser.h \
    videobrowser/videodec.h \
    serial/address.h \
    serial/crc16.h \
    serial/master.h \
    serial/myserialport.h \
    serial/qmasterthread.h \
    serial/qmyserial.h

FORMS    += \
    keyboardform.ui

RESOURCES += \
    images.qrc



#INCLUDEPATH += /usr/include
#INCLUDEPATH += /usr/lib/i386-linux-gnu

OTHER_FILES += \
    images/warning.png


INCLUDEPATH += -I/sysroots/cortexa9hf-vfp-neon-phytec-linux-gnueabi/usr/include
#INCLUDEPATH += -I/home/wzk/workspace/SDL/include
#INCLUDEPATH += -I/home/wzk/workspace/turbojpeg/libjpeg-turbo-master-release/include

LIBS += -L/sysroots/cortexa9hf-vfp-neon-phytec-linux-gnueabi/usr/lib -lavcodec \
                -lavformat \
                -lswscale \
                -lavutil \
                -lpng    \
                -ljpeg
#LIBS += -L/home/wzk/workspace/SDL -lSDL-1.2
#LIBS += -L/home/wzk/workspace/turbojpeg/libjpeg-turbo-master-release/lib -ljpeg -lturbojpeg

