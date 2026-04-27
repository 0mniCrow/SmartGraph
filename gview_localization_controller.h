#ifndef GVIEW_LOCALIZATION_CONTROLLER_H
#define GVIEW_LOCALIZATION_CONTROLLER_H

#include <QObject>
#include <QWidget>
#include <QSet>
#include <QMap>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QMenu>
#include <QAction>
#include <QCheckBox>
#include <QRadioButton>
#include <QMainWindow>

struct GViewTranslObj
{
    QObject*                _obj_pointer_;
    QMap<QString,QString>   _obj_text_translations_;
    QMap<QString,QString>   _obj_tooltip_translation_;
    GViewTranslObj(QObject* pointer=nullptr):_obj_pointer_(pointer){}
};

struct LangObject
{
    QString                 _class_name_;
    QMap<QString,QString>   _text_translation_;
    QMap<QString,QString>   _tooltip_translation_;
    LangObject(const QString& class_name):_class_name_(class_name){}
    LangObject(const char * class_name):_class_name_(class_name){}
};

using LangObjMap = QMap<QString,LangObject>;
using LangLinkMap = QMap<QString,QObject*>;

class GviewLangControl : public QObject
{
    Q_OBJECT
private:
    QMap<QString, LangObjMap>      _object_map_;
    QMap<QWidget*,LangLinkMap>     _windows_;
    QSet<QString>                  _translations_;
    QString                        _cur_lang_;
    void loadObject(QObject* next_elem,
                    LangLinkMap& cur_dict,
                    LangObjMap* obj_map = nullptr);
    bool canBeTranslated(const QString& class_name) const;
    bool setTranslation(QObject* cur_obj, const QString& text, const QString& tooltip = QString());
public:
    explicit GviewLangControl(QObject* tata = nullptr);
    bool loadWindow(QWidget* window);
    bool loadStringObj(QWidget* parent_widget, const QString& object_name);
    QString stringObjTransl(QWidget* parent_widget,
                                    const QString& object_name,
                                    const QString& language = QString());
    bool loadTextTranslations(const QMap<QString,LangObjMap>& object_map);
    const QSet<QString>&  translations() const;
    const QMap<QString,LangObjMap>& getObjectMap() const;
    bool changeLanguage(const QString& lang);
public slots:
    void objectAboutToBeDestroyed();


};

#endif // GVIEW_LOCALIZATION_CONTROLLER_H
