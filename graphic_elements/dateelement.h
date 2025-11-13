#ifndef DATEELEMENT_H
#define DATEELEMENT_H
#include "abstractelement.h"

class DateElement:public AbstractElement
{
private:
    struct Loc_WeekDay{
        QString _name_;
    };

    struct Loc_Month{
        QString _name_;
        uint _length_;
        uint _period_;
    };

    static QList<Loc_WeekDay> _weekdays_;
    static QList<Loc_Month> _months_;
    uint cur_month_day;
    uint cur_year;

public:
    DateElement();
};

#endif // DATEELEMENT_H
