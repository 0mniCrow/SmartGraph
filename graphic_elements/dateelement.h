#ifndef DATEELEMENT_H
#define DATEELEMENT_H
#include "abstractelement.h"

class DateElement:public AbstractElement
{
private:
    struct Loc_WeekDay{
        QString _name_;
        Loc_WeekDay(const QString& name):
            _name_(name){}
    };

    struct Loc_Month{
        QString _name_;
        uint _length_;
        uint _leap_period_;
        Loc_Month(const QString& name,
                  uint len,
                  uint leap):
            _name_(name),_length_(len),
            _leap_period_(leap){}
    };

    static QList<Loc_WeekDay> _weekdays_;
    static uint _starting_weekday_;
    static QList<Loc_Month> _months_;
    static uint _days_in_year_;
    uint _cur_month_day_;
    uint _cur_month_;
    uint _cur_year_;

    uint calculateWeekDay() const;
    uint dayOfTheYear() const;
    bool checkDates() const;
    void restoreDates();
    inline static bool isWeekdayExists(const QString& name);
    inline static bool isMonthExists(const QString& name);
    static int findDayIndex(const QString& name);
    static int findMonthIndex(const QString& name);
    inline static int toIndex(uint num);
    inline static uint fromIndex(int num);
    inline static bool inDaySize(uint num);
    inline static bool inMonthSize(uint num);
public:
    enum ElementType{ ET_DateElement = AbstractElement::ET_End+4};
    DateElement(const QString& element_name, char element_type = ET_Default);
    bool setDay(uint day_of_month);
    bool setMonth(uint month_of_year);
    bool setMonth(const QString& month_name);
    void setYear(uint year);


    static bool addMonth(const QString& name, uint num_of_days, uint leap_period = 0);
    static QString monthName(uint num);
    static int monthNum(const QString& name);
    static int monthLen(uint num);
    static int monthLen(const QString& name);
    static int monthLeapPeriod(uint num);
    static int monthLeapPeriod(const QString& name);
    static bool delMonth(const QString& name);
    static bool delMonth(uint num);
    static bool swapMonths(const QString& first_name,
                             const QString& second_name);
    static bool switchMonths(uint first_num, uint second_num);
    static uint monthCount();
    static bool setFirstWeekdayOfEra(uint day_num);
    static bool setFirstWeekdayOfEra(const QString& day_name);
    static bool addWeekDay(const QString& name);
    static int dayNum(const QString& name);
    static QString dayName(uint num);
    static bool delWeekDay(const QString& name);
    static bool delWeekDay(uint num);
    static bool swapDays(const QString& first_name,
                           const QString& second_name);
    static bool swapDays(uint first_num, uint second_num);
    static uint weekDaysCount();


    QVariant value() const override;
    void setValue(const QVariant& new_val, bool inform_signal = true) override;
    char elementType() const override;
    QWidget* generateWidget() override;
public slots:
    void changeElement(QVariant new_val, bool inform_signal = true) override;
    void setEditable(bool state) override;
};

#endif // DATEELEMENT_H
