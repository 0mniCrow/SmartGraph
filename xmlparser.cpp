#include "xmlparser.h"

XMLParser::XMLParser()
{

}


bool XMLParser::loadProject(const QString& file_addr,
                        nest_vert_map& vertices,
                        nest_vert_map& edges,
                        QString& bg_img_addr)
{
    QFile xml_file(file_addr);
    if(!xml_file.open(QFile::ReadOnly|QFile::Text))
    {
        return false;
    }
    QDomDocument xml_doc("xmlDoc");
    if(!xml_doc.setContent(&xml_file))
    {
        xml_file.close();
        return false;
    }
    xml_file.close();
    QDomElement root = xml_doc.documentElement();
    QDomNode cur_node = root.firstChild();
    while(!cur_node.isNull())
    {
        QDomElement cur_elem = cur_node.toElement();
        if(cur_elem.nodeName()=="vertices")
        {
            QDomNode cur_node_obj = cur_elem.firstChild();
            while(!cur_node_obj.isNull())
            {
                QDomElement cur_obj = cur_node_obj.toElement();
                QDomNode cur_node_param = cur_obj.firstChild();
                vert_map * cur_vert = new vert_map;
                while(!cur_node_param.isNull())
                {
                    QDomElement cur_param = cur_node_param.toElement();
                    cur_vert->insert(cur_param.tagName(),cur_param.text());
                    cur_node_param = cur_node_param.nextSibling();
                }
                vertices.append(cur_vert);
                cur_node_obj = cur_node_obj.nextSibling();
            }
        }
        else if(cur_elem.nodeName()=="edges")
        {
            QDomNode cur_node_obj = cur_elem.firstChild();
            while(!cur_node_obj.isNull())
            {
                QDomElement cur_obj = cur_node_obj.toElement();
                QDomNode cur_node_param = cur_obj.firstChild();
                vert_map * cur_edge = new vert_map;
                while(!cur_node_param.isNull())
                {
                    QDomElement cur_param = cur_node_param.toElement();
                    cur_edge->insert(cur_param.tagName(),cur_param.text());
                    cur_node_param = cur_node_param.nextSibling();
                }
                edges.append(cur_edge);
                cur_node_obj = cur_node_obj.nextSibling();
            }
        }
        else if(cur_elem.nodeName()=="bgImage")
        {
            bg_img_addr = cur_elem.text();
        }
        cur_node = cur_node.nextSibling();
    }
    return true;
}

bool XMLParser::saveProject(const QString& file_addr,
                        const nest_vert_map &vertices,
                        const nest_vert_map &edges,
                        const QString &bg_img_addr)
{
    QFile xml_file(file_addr);
    if(!xml_file.open(QFile::WriteOnly|QFile::Text))
    {
        return false;
    }
    QTextStream xml_stream(&xml_file);
    QDomDocument main_doc;
    QDomElement root = main_doc.createElement("project");
    main_doc.appendChild(root);
    if(!vertices.isEmpty())
    {
        QDomElement DOM_vertices  = main_doc.createElement("vertices");
        for(vert_map*vertex:vertices)
        {
            QDomElement DOM_vertex = main_doc.createElement("vertex");
            auto it = vertex->cbegin();
            while(it!=vertex->cend())
            {
                QDomElement DOM_elem = main_doc.createElement(it.key());
                QDomText DOM_val = main_doc.createTextNode(it.value());
                DOM_elem.appendChild(DOM_val);
                DOM_vertex.appendChild(DOM_elem);
                it++;
            }
            DOM_vertices.appendChild(DOM_vertex);
        }
        root.appendChild(DOM_vertices);
        QDomElement DOM_edges = main_doc.createElement("edges");
        for(vert_map*edge:edges)
        {
            QDomElement DOM_edge = main_doc.createElement("edge");
            auto it = edge->cbegin();
            while(it!=edge->cend())
            {
                QDomElement DOM_elem = main_doc.createElement(it.key());
                QDomText DOM_val = main_doc.createTextNode(it.value());
                DOM_elem.appendChild(DOM_val);
                DOM_edge.appendChild(DOM_elem);
                it++;
            }
            DOM_edges.appendChild(DOM_edge);
        }
        root.appendChild(DOM_edges);
    }
    if(!bg_img_addr.isEmpty())
    {
        QDomElement DOM_bg = main_doc.createElement("bgImage");
        QDomText DOM_img_addr = main_doc.createTextNode("imgAddr");
        DOM_img_addr.setData(bg_img_addr);
        DOM_bg.appendChild(DOM_img_addr);
        root.appendChild(DOM_bg);
    }
    xml_stream<<main_doc.toString();
    xml_file.flush();
    xml_file.close();
    return true;
}

bool XMLParser::loadTranslation(const QString& file_addr,
                            QMap<QWidget*,QMap<QString,GViewTranslObj>>& container)
{
    QFile xml_file(file_addr);
    if(!xml_file.open(QFile::ReadOnly|QFile::Text))
    {
        return false;
    }

    return true;
}

bool XMLParser::saveObjectMap(const QString& file_addr,
                          const QMap<QString,QMap<QString,QString>>& object_map)
{
    QFile xml_file(file_addr);
    if(!xml_file.open(QFile::WriteOnly|QFile::Text))
    {
        return false;
    }
    QTextStream xml_stream(&xml_file);
    QDomDocument main_doc;
    QDomElement root = main_doc.createElement("translation");
    main_doc.appendChild(root);
    auto window_it = object_map.cbegin();
    while(window_it!= object_map.cend())
    {
        QDomElement DOM_window = main_doc.createElement("window");
        DOM_window.setAttribute("name",window_it.key());
        auto it = window_it->cbegin();
        while(it!=window_it->cend())
        {
            QDomElement DOM_obj = main_doc.createElement("object");
            QDomElement DOM_obj_name = main_doc.createElement("name");
            QDomText DOM_obj_name_text = main_doc.createTextNode(it.key());
            DOM_obj_name.appendChild(DOM_obj_name_text);
            DOM_obj.appendChild(DOM_obj_name);
            QDomElement DOM_obj_type = main_doc.createElement("type");
            QDomText DOM_obj_type_text = main_doc.createTextNode(it.value());
            DOM_obj_type.appendChild(DOM_obj_type_text);
            DOM_obj.appendChild(DOM_obj_type);
            DOM_window.appendChild(DOM_obj);
            ++it;
        }
        root.appendChild(DOM_window);
        ++window_it;
    }
    xml_stream<<main_doc.toString();
    xml_file.flush();
    xml_file.close();
    return true;
}
