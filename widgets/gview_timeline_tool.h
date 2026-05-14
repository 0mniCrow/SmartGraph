#ifndef GVIEWTIMETOOL_H
#define GVIEWTIMETOOL_H

#include <QObject>
#include <QSlider>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QGroupBox>
#include <QGridLayout>
#include <QMap>
#include <QLabel>
#include <QStyleOptionSlider>
#include <QPainter>
/*
///______________________________Time Objects__________________________________________________

class AbstractTimeObject                               //Абстрактны тып для адзінкі часовага прамежку
{
private:
    QString _name_;                                     //Імя бягучага прамежку
    QString _parent_name_;                              //Імя наступнага статычнага прамежку
public:
    enum timespace_type{TS_NoType = 0};
    explicit AbstractTimeObject(const QString& name,
                                const QString& parent_name=QString()):
        _name_(name),_parent_name_(parent_name){}
    virtual quint64 childTime(const QString& subtime_name =
            QString()) const = 0;
    virtual QList<QString> children() const = 0;
    QString name() const {return _name_;}
    QString parent() const {return _parent_name_;}
    void setParentName(const QString& parent_name){_parent_name_=parent_name;}
    virtual char type() const {return TS_NoType;}
};

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

///______________________________Time Constructors__________________________________________________

class AbstractTimeDialog
{
public:
    [[nodiscard]] virtual AbstractTimeObject* createTimeObject() const = 0;


};

class LinearTimeDialog:public AbstractTimeDialog
{

};

class VariableTimeDialog:public AbstractTimeDialog
{

};


class BasicTimeController
{
private:
    quint64 _start_time_;
    quint64 _end_time_;
    quint64 _cur_time_;
    QString _minimal_space_name_;
    QMap<QString,AbstractTimeObject*> _time_types_;
public:
    explicit BasicTimeController();
    bool addStaticTimeSpace(const QString& name,
                            const QString& child_name = QString(),
                            quint64 numb_of_child_timespaces = 0,
                            const QString& parent_name = QString());
    bool addVariedTimeSpace(const QString& name,
                            const QString& child_name,
                            const QList<QPair<QString,quint64>>& varied_names_and_num,
                            const QString& parent_name = QString());
    bool addMultiChildTimeSpace(const QString& name,
                                const QMap<QString,quint64>& children_map,
                                const QString& parent_name = QString());
    bool isExist(const QString& name) const;
    bool setStartTime(quint64 time);
    bool setEndTime(quint64 time);
    quint64 currentTime(const QString& scale_name) const;
    quint64 currentReminder(const QString& scale_name) const;
    bool addTime(quint64 time, const QString& scale = QString());
    bool substractTime(quint64 time, const QString& scale = QString());
    BasicTimeController& operator+=(qint64 time);
    BasicTimeController& operator-=(qint64 time);
    const AbstractTimeObject *  currentScaledObject(const QString& scale_name = QString());
};
*/

class TimeSlider: public QSlider
{
    Q_OBJECT
private:
    QStringList _text_;
public:

    explicit TimeSlider(Qt::Orientation orientation = Qt::Horizontal,
                        QWidget*parent = nullptr):QSlider(orientation,parent)
    {
        return;
    }
    void loadTextLabels(QStringList &list);
protected:
    void paintEvent(QPaintEvent* p_event) override;
};

class GViewTimeTool : public QObject
{
    Q_OBJECT
private:
//    QGridLayout* _grid_;
//    QSlider* _slider_;
    TimeSlider* _slider_;
    int _tick_number_;
public:
    explicit GViewTimeTool(int tick_number = 0,QObject *parent = nullptr);
    [[nodiscard]] QSlider* getTimelineWidget();
    void setTickNumber(int tick_number);
signals:
    void timeChanged(int time);
    void widgetRequireUpdate();
public slots:
    void moveForward();
    void moveBack();
    void jump(int new_state);
};

#endif // GVIEWTIMETOOL_H
