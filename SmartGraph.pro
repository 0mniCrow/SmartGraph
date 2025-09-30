QT       += core gui 3dinput

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Graph_algorithms.cpp \
    gview_tableVertexModel.cpp \
    gview_tableVertexView.cpp \
    gviewedge.cpp \
    gviewitem.cpp \
    gviewport.cpp \
    gviewscene.cpp \
    listgraph.cpp \
    main.cpp \
    mainwindow.cpp \
    matrixmodel.cpp \
    nodegraph.cpp \
    touchform.cpp \
    vectorgraph.cpp \
    visualisationgraphform.cpp

HEADERS += \
    Graph_algorithms.h \
    Graph_algorithms_supply.h \
    gview_tableVertexModel.h \
    gview_tableVertexView.h \
    gviewedge.h \
    gviewitem.h \
    gviewport.h \
    gviewscene.h \
    listgraph.h \
    mainwindow.h \
    matrixmodel.h \
    nodegraph.h \
    touchform.h \
    vectorgraph.h \
    visualisationgraphform.h

FORMS += \
    mainwindow.ui \
    touchform.ui \
    visualisationgraphform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
