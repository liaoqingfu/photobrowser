#-------------------------------------------------
#
# Project created by QtCreator 2016-12-22T15:02:05
#
#-------------------------------------------------

QT       += core gui
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
    qdisplayphotodialog.cpp \
    qshowimagewidget.cpp

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
    qdisplayphotodialog.h \
    qshowimagewidget.h

FORMS    += qimagebrowser.ui \
    keyboardform.ui \
    qrenamedialog.ui \
    qdeletedialog.ui \
    qcreatedirdialog.ui \
    qwarntipsdialog.ui \
    qdisplayphotodialog.ui

RESOURCES += \
    images.qrc


LIBS += -L/usr/lib/i386-linux-gnu/   -lavcodec \
                -lavformat \
                -lswscale \
                -lavutil \
                -lswscale \
                -lavutil \
                -lSDL \
                -lSDL_image \
                -lpng    \
               -L/usr/local/lib/ -ljpeg \


INCLUDEPATH += /usr/include
INCLUDEPATH += /usr/lib/i386-linux-gnu

OTHER_FILES += \
    images/warning.png
