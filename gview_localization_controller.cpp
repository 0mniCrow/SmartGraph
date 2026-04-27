#include "gview_localization_controller.h"

GviewLangControl::GviewLangControl(QObject *tata)
    : QObject{tata}
{

}

bool GviewLangControl::canBeTranslated(const QString& class_name) const
{
    return((class_name == QLabel::staticMetaObject.className())||
            (class_name == QLineEdit::staticMetaObject.className())||
            (class_name == QTextEdit::staticMetaObject.className())||
            (class_name == QPushButton::staticMetaObject.className())||
            (class_name == QAction::staticMetaObject.className())||
            (class_name == QMenu::staticMetaObject.className())||
            (class_name == QGroupBox::staticMetaObject.className())||
           (class_name == QCheckBox::staticMetaObject.className())||
           (class_name == QRadioButton::staticMetaObject.className())||
           (class_name == QWidget::staticMetaObject.className())||
           (class_name==QMainWindow::staticMetaObject.className()));
}

bool GviewLangControl::setTranslation(QObject *cur_obj, const QString& text, const QString& tooltip)
{
    if(!cur_obj)
    {
        return false;
    }
    QString class_name(cur_obj->metaObject()->className());

    if(class_name == QLabel::staticMetaObject.className())
    {
        QLabel* elem = qobject_cast<QLabel*>(cur_obj);
        elem->setText(text);
    }
    else if(class_name == QLineEdit::staticMetaObject.className())
    {
        QLineEdit* elem = qobject_cast<QLineEdit*>(cur_obj);
        elem->setPlaceholderText(text);
    }
    else if(class_name == QTextEdit::staticMetaObject.className())
    {
        QTextEdit* elem = qobject_cast<QTextEdit*>(cur_obj);
        elem->setPlaceholderText(text);
    }
    else if(class_name == QPushButton::staticMetaObject.className())
    {
        QPushButton* elem = qobject_cast<QPushButton*>(cur_obj);
        elem->setText(text);
    }
    else if(class_name == QAction::staticMetaObject.className())
    {
        QAction* elem = qobject_cast<QAction*>(cur_obj);
        elem->setText(text);
        if(!tooltip.isEmpty())
        {
            elem->setToolTip(tooltip);
        }
    }
    else if(class_name == QMenu::staticMetaObject.className())
    {
        QMenu* elem = qobject_cast<QMenu*>(cur_obj);
        elem->setTitle(text);
    }
    else if(class_name == QGroupBox::staticMetaObject.className())
    {
        QGroupBox* elem = qobject_cast<QGroupBox*>(cur_obj);
        elem->setTitle(text);
    }
    else if(class_name == QCheckBox::staticMetaObject.className())
    {
        QCheckBox* elem = qobject_cast<QCheckBox*>(cur_obj);
        elem->setText(text);
    }
    else if(class_name == QRadioButton::staticMetaObject.className())
    {
        QRadioButton* elem = qobject_cast<QRadioButton*>(cur_obj);
        elem->setText(text);
    }
    else if(class_name == QWidget::staticMetaObject.className()||
            class_name== QMainWindow::staticMetaObject.className())
    {
        QWidget* window = qobject_cast<QWidget*>(cur_obj);
        window->setWindowTitle(text);
    }
    else
    {
        return false;
    }
    if(!tooltip.isEmpty())
    {
        QWidget* elem = qobject_cast<QWidget*>(cur_obj);
        if(elem)
        {
            elem->setToolTip(tooltip);
        }
    }
    return true;
}

void GviewLangControl::loadObject(QObject* cur_elem,
                                  LangLinkMap&  cur_dict,
                                  LangObjMap*   obj_map)
{
    QObjectList elements = cur_elem->children();
    for(QObject* element:elements)
    {
        if(qobject_cast<QWidget*>(element)||
                qobject_cast<QAction*>(element))
        {
            loadObject(element,cur_dict,obj_map);
        }
    }
    QString obj_name(cur_elem->objectName());
    if(!obj_name.contains("_trsl"))
    {
        return;
    }
    QString class_name(cur_elem->metaObject()->className());
    if(canBeTranslated(class_name))
    {
        cur_dict.insert(obj_name,cur_elem);
        if(obj_map)
        {
            obj_map->insert(obj_name,class_name);
        }
    }
    return;
}

bool GviewLangControl::loadWindow(QWidget* window)
{
    if((!window)||
            (_windows_.contains(window)))
    {
        return false;
    }
    LangLinkMap dictionary;
    dictionary.insert(window->objectName(),window);
    if(_object_map_.contains(window->objectName()))
    {
        loadObject(window,dictionary);
    }
    else
    {
        LangObjMap obj_map;
        obj_map.insert(window->objectName(),window->metaObject()->className());
        loadObject(window,dictionary,&obj_map);
        _object_map_.insert(window->objectName(),std::move(obj_map));
    }
    connect(window,&QObject::destroyed,this,&GviewLangControl::objectAboutToBeDestroyed);
    _windows_.insert(window,dictionary);
    return true;
}

bool GviewLangControl::loadTextTranslations(const QMap<QString, LangObjMap> &object_map)
{
    bool full_success = true;
    QMap<QString,LangObjMap>::const_iterator it = object_map.cbegin();
    while(it!=object_map.cend())
    {
        auto inner_it = _object_map_.find(it.key());
        if(inner_it==_object_map_.end())
        {
            full_success = false;
        }
        else
        {
            LangObjMap::const_iterator obj_it = it->cbegin();
            while(obj_it!=it->cend())
            {
                auto inner_obj_it = inner_it->find(obj_it.key());
                if(inner_obj_it==inner_it->end())
                {
                    full_success = false;
                }
                else
                {
                    inner_obj_it->_text_translation_ = obj_it->_text_translation_;
                    inner_obj_it->_tooltip_translation_ = obj_it->_tooltip_translation_;
                }
                ++obj_it;
            }
        }
        ++it;
    }
    return full_success;
}
const QSet<QString>& GviewLangControl::translations() const
{
    return _translations_;
}

const QMap<QString, LangObjMap> &GviewLangControl::getObjectMap() const
{
//    QMap<QString,QMap<QString,QString>> windows;
//    auto window_iter = _windows_.cbegin();
//    while(window_iter!=_windows_.cend())
//    {
//        QMap<QString,QString> objects;
//        auto object_iter = window_iter->cbegin();
//        while(object_iter!= window_iter->cend())
//        {
//            if(object_iter->_obj_pointer_)
//            {
//                objects.insert(object_iter->_obj_pointer_->objectName(),
//                           object_iter->_obj_pointer_->metaObject()->className());
//            }
//            else
//            {
//                objects.insert(object_iter.key(),"QString");
//            }
//            ++object_iter;
//        }
//        windows.insert(window_iter.key()->objectName(),objects);
//        ++window_iter;
//    }
    return _object_map_;
}

bool GviewLangControl::changeLanguage(const QString& lang)
{
    if(!_translations_.contains(lang))
    {
        return false;
    }
    auto window_iter = _windows_.begin();
    while(window_iter!=_windows_.end())
    {
        auto obj_iter = _object_map_.find(window_iter.key()->objectName());
        if(obj_iter!=_object_map_.end())
        {
            auto win_elem_iter = window_iter->begin();
            while(win_elem_iter!= window_iter->end())
            {
                auto obj_elem_iter = obj_iter->find(win_elem_iter.key());
                if(obj_elem_iter!=obj_iter->end())
                {
                    setTranslation(win_elem_iter.value(),
                           obj_elem_iter->_text_translation_.value(lang),
                           obj_elem_iter->_tooltip_translation_.value(lang));
                }
                ++win_elem_iter;
            }
        }
        ++window_iter;
    }
    _cur_lang_ = lang;
    return true;
}

void GviewLangControl::objectAboutToBeDestroyed()
{
    QWidget* window = qobject_cast<QWidget*>(sender());
    disconnect(window,&QObject::destroyed,this,&GviewLangControl::objectAboutToBeDestroyed);
    _windows_.remove(window);
    return;
}

bool GviewLangControl::loadStringObj(QWidget* parent_widget, const QString& object_name)
{
    if((!parent_widget)||
            (!_windows_.contains(parent_widget)))
    {
        return false;
    }
    auto obj_it = _object_map_.find(parent_widget->objectName());
    if(obj_it==_object_map_.end())
    {
        return false;
    }
    auto obj_elem_it = obj_it->find(object_name);
    if(obj_elem_it!=obj_it->end())
    {
        return false;
    }
    obj_it->insert(object_name,"QString");
    return true;
}

QString GviewLangControl::stringObjTransl(QWidget* parent_widget, const QString& object_name, const QString& language)
{
    QString answer;
    if((!parent_widget)||
            (!_windows_.contains(parent_widget)))
    {
        return answer;
    }
    QString lang(language.isEmpty()?_cur_lang_:language);
    if(lang.isEmpty()||!_translations_.contains(lang))
    {
        return answer;
    }
    auto it = _object_map_.find(parent_widget->objectName());
    if(it!=_object_map_.end())
    {
        auto elem_it = it->find(object_name);
        if(elem_it!=it->end())
        {
            auto transl_it = elem_it->_text_translation_.find(lang);
            if(transl_it!=elem_it->_text_translation_.end())
            {
                answer = transl_it.value();
            }
        }
    }
    return answer;
}
