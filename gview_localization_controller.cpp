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
           (class_name == QRadioButton::staticMetaObject.className()));

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
    _windows_.insert(window,dictionary);
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
const QSet<QString>& GviewLangControl::translations() const
{
    return _translations_;
}

const QMap<QWidget*,QMap<QString,GViewTranslObj>>& GviewLangControl::getTranslatableObjectMap()const
{
    return _windows_;
}

QMap<QString,QMap<QString,QString>> GviewLangControl::getObjectMap() const
{
    QMap<QString,QMap<QString,QString>> windows;
    auto window_iter = _windows_.cbegin();
    while(window_iter!=_windows_.cend())
    {
        QMap<QString,QString> objects;
        auto object_iter = window_iter->cbegin();
        while(object_iter!= window_iter->cend())
        {
            objects.insert(object_iter->_obj_pointer_->objectName(),
                           object_iter->_obj_pointer_->metaObject()->className());
            ++object_iter;
        }
        windows.insert(window_iter.key()->objectName(),objects);
        ++window_iter;
    }
    return windows;
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
        auto obj_iter = window_iter.value().begin();
        while(obj_iter!= window_iter.value().end())
        {
            setTranslation(obj_iter->_obj_pointer_,
                           obj_iter->_obj_text_translations_.value(lang),
                           obj_iter->_obj_tooltip_translation_.value(lang));
            ++obj_iter;
        }
        ++window_iter;
    }
    return true;
}

void GviewLangControl::objectAboutToBeDestroyed(QWidget* window)
{
    _windows_.remove(window);
    return;
}

bool GviewLangControl::loadObjectByName(QWidget* parent_widget, const QString& object_name)
{
    if((!parent_widget)||!_windows_.contains(parent_widget))
    {
        return false;
    }
    _windows_.value(parent_widget).insert(object_name,GViewTranslObj());
    return true;
}

QString GviewLangControl::getTranslationForObject(QWidget* parent_widget, const QString& object_name, const QString& language)
{
    QString answer;
    if((!parent_widget)||
            (!_windows_.contains(parent_widget))||
            (!_windows_.value(parent_widget).contains(object_name))||
            (!_translations_.contains(language)))
    {
        return answer;
    }
    answer = _windows_.value(parent_widget).value(object_name)._obj_text_translations_.value(language);
    return answer;
}
