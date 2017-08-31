#-------------------------------------------------
#
# Project created by QtCreator 2015-01-03T09:13:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = thermapp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    thermapp.c \
    ctmf.cpp \
    violajones.cpp \
    teste.cpp \
    cam.cpp

HEADERS  += mainwindow.h \
    thermapp.h \
    ctmf.h \
    cam.h

FORMS    += mainwindow.ui
INCLUDEPATH += /usr/local/include/opencv2

LIBS += -lrt -lpthread -lusb-1.0 -lm -L/usr/local/lib/  -lopencv_core -lopencv_calib3d -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -lopencv_photo -lopencv_videoio

