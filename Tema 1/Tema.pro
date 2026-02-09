QT += core gui widgets

CONFIG += c++17
TARGET = Tema
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    graph.cpp \
    node.cpp \
    edge.cpp

HEADERS += \
    mainwindow.h \
    graph.h \
    node.h \
    edge.h

FORMS += \
    mainwindow.ui
