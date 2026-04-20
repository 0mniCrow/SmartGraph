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

struct GViewTranslObj
{
    QObject*                _obj_pointer_;
    QMap<QString,QString>   _obj_text_translations_;
    QMap<QString,QString>   _obj_tooltip_translation_;
    GViewTranslObj(QObject* pointer=nullptr):_obj_pointer_(pointer){}
};

class GviewLangControl : public QObject
{
    Q_OBJECT
private:
    QMap<QWidget*,QMap<QString,GViewTranslObj>> _windows_;
    QSet<QString> _translations_;
    void loadObject(QObject* next_elem, QMap<QString,GViewTranslObj>& cur_dict);
    bool canBeTranslated(const QString& class_name) const;
public:
    explicit GviewLangControl(QObject* tata = nullptr);
    bool loadWindow(QWidget* window);
    bool loadTextTranslations(QWidget* window, const QMap<QString,GViewTranslObj>& window_dict);
    const QSet<QString>&  translations() const;
    bool changeLanguage(const QString& lang);
public slots:
    void objectAboutToBeDestroyed(QWidget* window);


};

#endif // GVIEW_LOCALIZATION_CONTROLLER_H
