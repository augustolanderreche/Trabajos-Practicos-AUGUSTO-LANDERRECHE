QT += core gui widgets sql

CONFIG += c++17

TARGET = ejercicio09-AugustoLanderreche
TEMPLATE = app

SOURCES += \
    main.cpp \
    admindb.cpp \
    login.cpp \
    mainwindow.cpp \
    pintura.cpp

HEADERS += \
    admindb.h \
    login.h \
    mainwindow.h \
    pintura.h

FORMS += \
    login.ui

