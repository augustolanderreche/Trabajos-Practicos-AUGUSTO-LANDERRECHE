QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    login.cpp \
    mainwindow.cpp \
    datastore.cpp \
    sessionmanager.cpp \
    job.cpp \
    history.cpp \
    user.cpp \
    addeditjobdialog.cpp \
    noteseditor.cpp

HEADERS += \
    login.h \
    mainwindow.h \
    datastore.h \
    sessionmanager.h \
    job.h \
    history.h \
    user.h \
    addeditjobdialog.h \
    noteseditor.h

FORMS += \
    login.ui \
    mainwindow.ui \
    addeditjobdialog.ui \
    noteseditor.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
