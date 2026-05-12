#ifndef GVIEWTIMETOOL_H
#define GVIEWTIMETOOL_H

#include <QObject>
#include <QSlider>
#include <QString>
#include <QVariant>
#include <QGroupBox>
#include <QGridLayout>
#include <QMap>

struct AbstractTimeSpace                            //Абстрактны тып для адзінкі часовага прамежку
{
    QString _name_;                                 //Імя бягучага прамежку
    QString _parent_name_;                          //Імя наступнага статычнага прамежку
    enum timespace_type{TS_NoType = 0};
    virtual char type() const {return TS_NoType;}
};

struct StaticTimeSpace:public AbstractTimeSpace     //Статычны тып часу для нязьменных аб'ектаў, як гадзіны (60 хвілін)
{                                                   //ці дні (24 гадзіны)
    QString _child_name_;                           //Імя спадкавага прамежку (хвіліна для гадзіны, гадзіна для дня)
    quint64 _child_num_;                                //Колькасць спадкавых прамежкаў у бягучым прамежку. (60 хвілін у адной гадзіне і г.д.)
    enum timespace_type{TS_StaticType = 1};
    char type() const override {return TS_StaticType;}
};

struct VariedTimeSpace:public AbstractTimeSpace     //Зьменны тып часу для аб'ектаў як месяц (30 ці 31 дзень)
{
    QString _child_name_;
    QList<QPair<QString,quint64>> _varied_name_num_;   //Упарадкаваны спіс імёнаў падтыпу часу й прамежкаў у часе
    enum timespace_type{TS_VariedType = 2};
    char type() const override {return TS_VariedType;}
};

struct MultiChildTimeSpace:public AbstractTimeSpace     //Тып часу, які мае некалькі відаў спадкавых прамежкаў (год мае месяцы і дні адначасова)
{
    QMap<QString,quint64>  _child_names_num_;               //Назва спадкавага прамежку і яго колькасць.
    enum timespace_type{TS_MultiChild = 3};
    char type() const override {return TS_MultiChild;}
};

class AbstractTimeLine
{
private:
    quint64 _start_time_;
    quint64 _end_time_;
    quint64 _cur_time_;
    QString _minimal_space_name_;
    QMap<QString,AbstractTimeSpace*> _time_types_;

public:
    explicit AbstractTimeLine();
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
    quint64 currentTime(const QString& scale_name) const;
    bool addTime(quint64 time, const QString& scale = QString());
    bool substractTime(quint64 time, const QString& scale = QString());
    AbstractTimeLine& operator+=(qint64 time);
    AbstractTimeLine& operator-=(qint64 time);
    const AbstractTimeSpace *  currentScaledObject(const QString& scale_name = QString());

};


class GViewTimeTool : public QObject
{
    Q_OBJECT
private:
    QGridLayout* _grid_;
    QSlider* _slider_;

public:
    explicit GViewTimeTool(QObject *parent = nullptr);
    [[nodiscard]] QGridLayout* getTimelineWidget();

signals:
    void timeChanged();
    void widgetUpdated();
};

#endif // GVIEWTIMETOOL_H
