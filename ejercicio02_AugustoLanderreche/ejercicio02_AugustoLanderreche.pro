QT += core gui network widgets

CONFIG += c++17

TARGET = ejercicio02_AugustoLanderreche
TEMPLATE = app

SOURCES += \
    main.cpp \
    serverhealthmonitor.cpp \
    monitoringdashboard.cpp

HEADERS += \
    serverhealthmonitor.h \
    monitoringdashboard.h

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Configuración de compilador
CONFIG += warn_on
