#-------------------------------------------------
#
# Project created by QtCreator 2012-12-02T16:12:46
#
#-------------------------------------------------

QT       += core gui

TARGET = translator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scanner.cpp \
    abstractparser.cpp \
    rdparser.cpp \
    auparser.cpp

HEADERS  += mainwindow.h \
    lexeme.h \
    scanner.h \
    abstractparser.h \
    rdparser.h \
    auparser.h

FORMS    += mainwindow.ui

CONFIG+=exceptions
