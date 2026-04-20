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
            (class_name == QGroupBox::staticMetaObject.className()));

}

void GviewLangControl::loadObject(QObject* cur_elem, QMap<QString,GViewTranslObj>& cur_dict)
{
    QObjectList elements = cur_elem->children();
    for(QObject* element:elements)
    {
        if(qobject_cast<QWidget*>(element)||
                qobject_cast<QAction*>(element))
        {
            loadObject(element,cur_dict);
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
        cur_dict.insert(obj_name,GViewTranslObj(cur_elem));
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
    QMap<QString,GViewTranslObj> dictionary;
    loadObject(window,dictionary);
    return true;
}

bool GviewLangControl::loadTextTranslations(QWidget* window,
                                            const QMap<QString,GViewTranslObj>& window_dict)
{
    if((!window)||
            (!_windows_.contains(window)))
        return false;
    auto cur_window = _windows_.find(window);
    auto cur_dict_it= window_dict.cbegin();
    while(cur_dict_it!=window_dict.cend())
    {
        auto inner_dict_elem = cur_window.value().find(cur_dict_it.key());
        inner_dict_elem->_obj_text_translations_ = std::move(cur_dict_it->_obj_text_translations_);
        inner_dict_elem->_obj_tooltip_translation_=std::move(cur_dict_it->_obj_tooltip_translation_);
        ++cur_dict_it;
    }
    return true;
}
const QSet<QString> &GviewLangControl::translations() const
{
    return _translations_;
}

bool GviewLangControl::changeLanguage(const QString& lang)
{
    if(!_translations_.contains(lang))
    {
        return false;
    }
    return true;
}

void GviewLangControl::objectAboutToBeDestroyed(QWidget* window)
{
    _windows_.remove(window);
    return;
}
