#ifndef GVIEW_TIME_OBJECTS_H
#define GVIEW_TIME_OBJECTS_H
#include <QString>
#include <QList>
#include <QMap>
#include <QSet>
#define TO_GVIEW_TIME(val) Q_UINT64_C(val)
using gview_time_t = quint64;


class GViewAbstractTimeObject                               //Абстрактны тып для адзінкі часовага прамежку
{
protected:
    static inline gview_time_t _time_ = TO_GVIEW_TIME(0);
    QString _name_;                                                      //Імя бягучага прамежку
    QSet<GViewAbstractTimeObject*>              _big_relatives_;         //Імя наступнага статычнага прамежку
    QMap<GViewAbstractTimeObject*,gview_time_t> _small_relatives_;
public:
    enum timespace_type{TS_NoType = 0};

    explicit GViewAbstractTimeObject(const QString& name):_name_(name){};
    static void setGeneralTime(gview_time_t time){ _time_ = time;}
    virtual quint64 childTime(const QString& subtime_name =
            QString()) const = 0;
    virtual QList<QString> children() const = 0;

    virtual QString name() const = 0;
    virtual QString parent() const = 0;
    virtual void setParentName(const QString& parent_name) = 0;
    virtual char type() const {return TS_NoType;}
};

/*
class LinearTimeObject:public AbstractTimeObject       //Статычны тып часу для нязьменных аб'ектаў, як гадзіны (60 хвілін)
{                                                       //ці дні (24 гадзіны)
private:
    QString _child_name_;                               //Імя спадкавага прамежку (хвіліна для гадзіны, гадзіна для дня)
    quint64 _child_time_;                                //Колькасць спадкавых прамежкаў у бягучым прамежку. (60 хвілін у адной гадзіне і г.д.)
public:
    LinearTimeObject(const QString& name,
                     const QString& child_name=QString(),
                     qint64 child_time = 0,
                     const QString& parent_name=QString()):
        AbstractTimeObject(name,parent_name),
        _child_name_(child_name),_child_time_(child_time){}
    virtual quint64 childTime(const QString& subtime_name =
            QString()) const override
    {
        Q_UNUSED(subtime_name)
        return _child_time_;
    }
    virtual QList<QString> children() const override
    {
        return QStringList(_child_name_);
    }
    enum timespace_type{TS_StaticType = 1};
    char type() const override {return TS_StaticType;}
};

class VariableTimeObject:public AbstractTimeObject         //Зьменны тып часу для аб'ектаў як месяц (30 ці 31 дзень)
{
private:
    QString _child_name_;
    QList<QPair<QString,quint64>> _name_num_;      //Упарадкаваны спіс імёнаў падтыпу часу й прамежкаў у часе
public:
    VariableTimeObject(const QString& name,
                       const QString& child_name,
                       QList<QPair<QString,quint64>>& names_and_childnum,
                       const QString& parent_name=QString()):
        AbstractTimeObject(name,parent_name),
        _child_name_(child_name),_name_num_(names_and_childnum){}
    virtual quint64 childTime(const QString& subtime_name =
            QString()) const override
    {
        quint64 num = 0;
        if(subtime_name.isEmpty())
        {
            for(const QPair<QString,quint64>& var:_name_num_)
            {
                num+=var.second;
            }
        }
        else
        {
            num = optionChildNum(subtime_name);
        }
        return num;
    }
    virtual QList<QString> children() const override
    {
        return QStringList(_child_name_);
    }
    int optionsCount() const {return _name_num_.size();}
    QString optionName(int number)const
    {
        return number<_name_num_.size()?_name_num_.at(number).first:QString();
    }
    quint64 optionChildNum(int number) const
    {
        return number<_name_num_.size()?_name_num_.at(number).second:Q_UINT64_C(0);
    }
    quint64 optionChildNum(const QString& option_name) const
    {
       auto it = std::find_if(_name_num_.cbegin(),_name_num_.cend(),
                              [option_name](const QPair<QString,quint64>&var)
       {return var.first==option_name;});
       return it==_name_num_.cend()?Q_UINT64_C(0):it->second;
    }
    enum timespace_type{TS_VariedType = 2};
    char type() const override {return TS_VariedType;}
};

class MultiChildTimeObject:public AbstractTimeObject       //Тып часу, які мае некалькі відаў спадкавых прамежкаў (год мае месяцы і дні адначасова)
{
private:
    QString _first_child_;
    QMap<QString,quint64>  _children_;               //Назва спадкавага прамежку і яго колькасць.
public:
    MultiChildTimeObject(const QString& name,
                         QMap<QString,quint64> children,
                         const QString& first_child,
                         const QString& parent_name=QString()):
        AbstractTimeObject(name,parent_name),
        _first_child_(first_child),_children_(children){}
    virtual quint64 childTime(const QString& subtime_name =
            QString()) const override
    {
        quint64 num = 0;
        if(subtime_name.isEmpty())
        {
            num = _children_[_first_child_];
        }
        else
        {
            num = _children_.contains(subtime_name)?_children_[subtime_name]:Q_UINT64_C(0);
        }
        return num;
    }
    virtual QList<QString> children() const override
    {
        return _children_.keys();
    }
    enum timespace_type{TS_MultiChild = 3};
    char type() const override {return TS_MultiChild;}
};
*/
#endif // GVIEW_TIME_OBJECTS_H
