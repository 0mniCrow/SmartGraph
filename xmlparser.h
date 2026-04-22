#ifndef XMLPARSER_H
#define XMLPARSER_H
#include "gview_localization_controller.h"
#include "gview_tableVertexModel.h"
#include <QString>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomText>
#include <QFile>

class XMLParser
{
public:
    XMLParser();
    static bool loadProject(const QString& file_addr,
                            nest_vert_map& vertices,
                            nest_vert_map& edges,
                            QString& bg_img_addr);
    static bool saveProject(const QString& file_addr,
                            const nest_vert_map& vertices,
                            const nest_vert_map& edges,
                            const QString& bg_img_addr = QString());
    static bool loadTranslation(const QString& file_addr,
                                QMap<QWidget*,QMap<QString,GViewTranslObj>>& container);
    static bool saveObjectMap(const QString& file_addr,
                              const QMap<QString,QMap<QString,QString>>& object_map);
};

#endif // XMLPARSER_H
