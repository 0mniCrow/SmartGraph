QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Graph_algorithms.cpp \
    graphedge.cpp \
    graphnode.cpp \
    graphwidget.cpp \
    listgraph.cpp \
    main.cpp \
    mainwindow.cpp \
    matrixmodel.cpp \
    nodegraph.cpp \
    touchform.cpp \
    vectorgraph.cpp

HEADERS += \
    Graph_algorithms.h \
    Graph_algorithms_supply.h \
    graphedge.h \
    graphnode.h \
    graphwidget.h \
    listgraph.h \
    mainwindow.h \
    matrixmodel.h \
    nodegraph.h \
    touchform.h \
    vectorgraph.h

FORMS += \
    mainwindow.ui \
    touchform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
