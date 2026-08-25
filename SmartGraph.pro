QT       += core gui 3dinput xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Graph_algorithms.cpp \
    graphic_objects/abstractGrItem.cpp \
    graphic_objects/abstractgrconnection.cpp \
    graphic_objects/abstractitemmanager.cpp \
    graphic_objects/basicitemmanager.cpp \
    graphic_objects/itemcommunicator.cpp \
    graphic_objects/simplegrconnection.cpp \
    graphic_objects/staticgritem.cpp \
    gview_localization_controller.cpp \
    gview_tableVertexModel.cpp \
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
    supplement/gview_time_objects.cpp \
    touchform.cpp \
    vectorgraph.cpp \
    widgets/gview_edit_window.cpp \
    widgets/gview_time_slider.cpp \
    widgets/gview_timeline_tool.cpp \
    widgets/gview_tooltip_window.cpp \
    xmlparser.cpp

HEADERS += \
    Graph_algorithms.h \
    Graph_algorithms_supply.h \
    graphic_objects/abstractGrInterface.h \
    graphic_objects/abstractGrItem.h \
    graphic_objects/abstractgrconnection.h \
    graphic_objects/abstractitemmanager.h \
    graphic_objects/basicitemmanager.h \
    graphic_objects/itemcommunicator.h \
    graphic_objects/simplegrconnection.h \
    graphic_objects/staticgritem.h \
    graphic_objects/templateItemData.h \
    gview_localization_controller.h \
    gview_tableVertexModel.h \
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
    supplement/SuperstructForManagableDataModels.h \
    supplement/gview_time_objects.h \
    touchform.h \
    vectorgraph.h \
    widgets/gview_edit_window.h \
    widgets/gview_time_slider.h \
    widgets/gview_timeline_tool.h \
    widgets/gview_tooltip_window.h \
    xmlparser.h

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
