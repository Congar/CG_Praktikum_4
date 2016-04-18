#-------------------------------------------------
#
# Project created by QtCreator 2016-04-11T14:25:44
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = P3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MyGLWidget.cpp

HEADERS  += mainwindow.h \
    MyGLWidget.h

FORMS    += mainwindow.ui


LIBS += -lopengl32

RESOURCES += \
    shader.qrc

DISTFILES +=
