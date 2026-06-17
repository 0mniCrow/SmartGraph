#ifndef GVIEW_TIME_OBJECTS_H
#define GVIEW_TIME_OBJECTS_H
#include <QString>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QSet>
#define TO_GVIEW_TIME(val) Q_UINT64_C(val)
#define DEFAULT_TIME_UNIT 1
#define DEF_T_INIT TO_GVIEW_TIME(DEFAULT_TIME_UNIT)
using gview_time_t = quint64;


/*
Гэты клас патрэбны як інтэрфэйс і базавы аб'ект часу, які будзе мадыфікаваць
адзінку часу (gview_time_t). Карыстальніку класа time tool трэба захоўваць яе як пераменную-ключ
*/

class GViewBaseTObject                               //Абстрактны тып для адзінкі часовага прамежку
{
protected:
    virtual gview_time_t accumulateUpperVal(const gview_time_t& time) const;
    virtual gview_time_t accumulateLowerVal(const gview_time_t& time) const;
    virtual gview_time_t getUpperReminder(const gview_time_t& time) const;
    virtual gview_time_t getInteger(const gview_time_t& time) const = 0;
    virtual gview_time_t getReminder(const gview_time_t& time) const = 0;
private:
    gview_time_t _modifier_;                                        //Мадыфікатар адзінкі (колькі мінімальных адзінак часу ён атрымлівае)
    uint _min_val_;                                                 //Мінімальная лічба падпарадкаванай адзінкі часу
    uint _max_val_;                                                 //Колькасць падпарадкаваных адзінак часу
    QString _name_;                                                 //Імя бягучай адзінкі
    GViewBaseTObject * _greater_unit_;                              //Залежная адзінка часу
    GViewBaseTObject * _lesser_unit_;                               //Падпарадкаваная адзінка часу
public:
    enum TimeUnit_type{TUnit_NoType = 0};

    explicit GViewBaseTObject(const QString& name,
                              const gview_time_t& modifier = DEF_T_INIT,
                              GViewBaseTObject* greater_unit = nullptr,
                              GViewBaseTObject* lesser_unit = nullptr);
    virtual ~GViewBaseTObject() {}
    virtual QStringList getScaleLabels() const = 0;                                             //Сьпіс значэнняў гэтага прамежку
    virtual gview_time_t scaleUnitToTime(int val, const gview_time_t& time) const = 0;          //Пераўтварыць значэнне бягучага юніту ў час
    virtual int scaleTimeToUnit(const gview_time_t& time) const = 0;                            //Атрымаць значэнне у межах бягучага юніту
    virtual gview_time_t getUnitVal(const gview_time_t& time) const = 0;                        //_modifier_ памножанае на кавалак значэння бягучага часу, адказны за гэты прамежак
    virtual gview_time_t getUnitTime(const gview_time_t& time) const = 0;
    virtual gview_time_t getLowerUnitCount() const;
    virtual gview_time_t getUpperVal(const gview_time_t& time) const;
    virtual gview_time_t getLowerVal(const gview_time_t& time) const;
    QString name() const;
    gview_time_t modifier() const noexcept;                                                              //Атрымаць базавы мадыфікатар
    virtual gview_time_t curModifier(const gview_time_t& time) const = 0;                       //Атрымаць мадыфікатар, залежны на бягучы час
    //virtual gview_time_t upperVal(const gview_time_t& time) const =0;
    GViewBaseTObject* getUpperUnit() const noexcept;
    GViewBaseTObject* getLowerUnit() const noexcept;
    bool isBasicUnit() const noexcept;                                                                   //Мінімальная адзінка часу (не мае падпарадкаваных адзінак)
    bool isTopUnit() const noexcept;                                                                     //Вярхавая адзінка часу (не максімальная, але не мае загаднай адзінкі)
    void setName(const QString& name) ;
    void setModifier(const gview_time_t& modifier) noexcept;
    void setGreaterUnit(GViewBaseTObject* greater_unit) noexcept;
    void setLesserUnit(GViewBaseTObject* lesser_unit) noexcept;
    virtual char type() const noexcept {return TUnit_NoType;}

};


class FixedTObject: public GViewBaseTObject
{
private:
    QList<QString> _text_labels_;
protected:
    virtual gview_time_t getInteger(const gview_time_t& time) const override;
    virtual gview_time_t getReminder(const gview_time_t& time) const override;
public:
    enum TimeUnit_type{TUnit_Fixed = 1};
    explicit FixedTObject(const QString& name,
                          const gview_time_t& modifier = DEF_T_INIT,
                          GViewBaseTObject* greater_unit = nullptr,
                          GViewBaseTObject* lesser_unit = nullptr);
    bool setTextLabels(const QStringList& text_labels);
    virtual QStringList getScaleLabels() const override;
    virtual gview_time_t scaleUnitToTime(int val, const gview_time_t& time) const override;
    virtual int scaleTimeToUnit(const gview_time_t& time) const override;
    virtual gview_time_t curModifier(const gview_time_t& time) const override;
    //virtual gview_time_t upperVal(const gview_time_t& time) const override;
    virtual gview_time_t getUnitVal(const gview_time_t& time) const override;
    virtual gview_time_t getUnitTime(const gview_time_t& time) const override;
    ~FixedTObject(){}
    virtual char type() const noexcept override {return TUnit_Fixed;}
};

class VariantTObject: public GViewBaseTObject
{
private:
    int _leap_cycle_;
    int _leap_length_;
    FixedTObject* _leap_unit_;
    QSet<GViewBaseTObject*> _related_units_;
    QStringList _text_labels_;
    gview_time_t variantInteger(const gview_time_t& time, int& number,bool& is_leap_time) const;
protected:
    virtual gview_time_t getInteger(const gview_time_t& time) const override;
    virtual gview_time_t getReminder(const gview_time_t& time) const override;
    virtual gview_time_t getVariantReminder(const gview_time_t& time, bool& is_leap_time) const;
public:
    enum TimeUnit_type{TUnit_Variant = 2};
    explicit VariantTObject(const QString& name,
                            const gview_time_t& modifier,
                            FixedTObject* leap_unit,
                            int leap_cycle = 4, int leap_length = 1,
                            GViewBaseTObject* greater_unit = nullptr,
                            GViewBaseTObject* lesser_unit = nullptr
                            );
    ~VariantTObject(){}
    bool setTextLabels(const QStringList& text_labels);
    void setLeapCycle(int leap_cycle) noexcept;
    int leapCycle() const noexcept;
    void setLeapLength(int leap_length) noexcept;
    int leapLength()const noexcept;
    void setLeapUnit(FixedTObject* leap_unit) noexcept;
    bool isLeapTime(const gview_time_t& time) const;
    FixedTObject* leapUnit() const noexcept;
    virtual QStringList getScaleLabels() const override;
    virtual gview_time_t scaleUnitToTime(int val, const gview_time_t& time) const override;
    virtual int scaleTimeToUnit(const gview_time_t& time) const override;
    virtual gview_time_t curModifier(const gview_time_t& time) const override;
    virtual gview_time_t getUnitVal(const gview_time_t& time) const override;
    virtual gview_time_t getUnitTime(const gview_time_t& time) const override;
    virtual char type() const noexcept override {return TUnit_Variant;}
};



class MonthTObject:public VariantTObject
{
private:
    QList<QPair<QString,gview_time_t>> _months_;
    QString _leap_month_;
    int getMonthNum(const gview_time_t& time, bool is_leap_time) const;
public:
    explicit MonthTObject(const QString& name,
                            const gview_time_t& modifier,
                            FixedTObject* leap_unit,
                            const QList<QPair<QString,int>>& months,
                            const QString& leap_month,
                            int leap_cycle = 4, int leap_length = 1,
                            GViewBaseTObject* greater_unit = nullptr,
                            GViewBaseTObject* lesser_unit = nullptr
                            );
    ~MonthTObject();
    QString getMonth(const gview_time_t& time) const;
    gview_time_t getMonthLength(const QString& month, const gview_time_t& time) const;
    int getMonthNum(const gview_time_t& time) const;
};

class YearTObject:public VariantTObject
{
private:
    MonthTObject * _month_unit_;
    gview_time_t _month_modifier_;
public:
    explicit YearTObject(const QString& name,
                            const gview_time_t& modifier,
                            FixedTObject* leap_unit,
                            MonthTObject* month_object,
                            const gview_time_t& month_modifier,
                            int leap_cycle = 4, int leap_length = 1,
                            GViewBaseTObject* greater_unit = nullptr,
                            GViewBaseTObject* lesser_unit = nullptr
                            );
    ~YearTObject();
};

//class WeekTObject: public GViewBaseTObject
//{
//private:
//    QList<QString> _week_days_;
//    MonthTObject* _month_unit_;
//    YearTObject* _year_unit_;
//public:

//};
#endif // GVIEW_TIME_OBJECTS_H
