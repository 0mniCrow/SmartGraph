QT       += core gui 3dinput

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Graph_algorithms.cpp \
    graphic_elements/abstractelement.cpp \
    graphic_elements/abstractobjectinfo.cpp \
    graphic_elements/dateelement.cpp \
    graphic_elements/imageelement.cpp \
    graphic_elements/infoelement.cpp \
    graphic_elements/infowidget.cpp \
    graphic_elements/nameelement.cpp \
    graphic_elements/nodeobjectinfo.cpp \
    gview_tableVertexModel.cpp \
    gview_tableVertexTableDelegate.cpp \
    gview_tableVertexView.cpp \
    gviewedge.cpp \
    gviewitem.cpp \
    gviewport.cpp \
    gviewscene.cpp \
    imagecropwindow.cpp \
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
    graphic_elements/abstractelement.h \
    graphic_elements/abstractobjectinfo.h \
    graphic_elements/dateelement.h \
    graphic_elements/imageelement.h \
    graphic_elements/infoelement.h \
    graphic_elements/infowidget.h \
    graphic_elements/nameelement.h \
    graphic_elements/nodeobjectinfo.h \
    gview_tableVertexModel.h \
    gview_tableVertexTableDelegate.h \
    gview_tableVertexView.h \
    gviewedge.h \
    gviewitem.h \
    gviewport.h \
    gviewscene.h \
    imagecropwindow.h \
    listgraph.h \
    mainwindow.h \
    matrixmodel.h \
    nodegraph.h \
    touchform.h \
    vectorgraph.h

FORMS += \
    imagecropwindow.ui \
    mainwindow.ui \
    touchform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    gview_resources.qrc
