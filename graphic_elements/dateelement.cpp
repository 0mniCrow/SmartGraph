#include "dateelement.h"

QList<DateElement::Loc_WeekDay> DateElement::_weekdays_ = QList<DateElement::Loc_WeekDay>();
QList<DateElement::Loc_Month> DateElement::_months_ = QList<DateElement::Loc_Month>();
uint DateElement::_starting_weekday_ = 0;
uint DateElement::_days_in_year_ = 0;

DateElement::DateElement(const QString &element_name,
                         char element_type):
    AbstractElement(element_name,element_type),
    _cur_month_day_(0),
    _cur_month_(0),
    _cur_year_(0),
    _read_only_(false)
{

}


uint DateElement::calculateWeekDay() const
{
    uint passDays = 0;
    if(_cur_year_)
    {
        passDays = _cur_year_*_days_in_year_;
    }
    const int saveround = std::fegetround();
    std::fesetround(FE_TOWARDZERO);
    for(Loc_Month& month: _months_)
    {
        if(month._leap_period_)
        {
            int leaps = std::rint(static_cast<double>(_cur_year_)/
                    static_cast<double>(month._leap_period_));
            passDays-=leaps;
        }
    }
    std::fesetround(saveround);
    passDays+=dayOfTheYear();
    if(_starting_weekday_)
    {
        passDays-=_weekdays_.size()-_starting_weekday_;
    }
    uint weekday = passDays%_weekdays_.size()+1;
    return weekday;
}

uint DateElement::dayOfTheYear() const
{
    uint passDays = 0;
    for(uint i = 1; i<_cur_month_;i++)
    {
        passDays+=_months_.at(i-1)._length_;
        if(_months_.at(toIndex(i))._leap_period_)
        {
            if(!(_cur_year_%_months_.at(toIndex(i))._leap_period_))
            {
                --passDays;
            }
        }
    }
    passDays+=_cur_month_day_;
    return passDays;
}

bool DateElement::checkDates() const
{
    if(!_cur_month_day_ ||
            !_cur_month_)
    {
        return false;
    }
    if(_cur_month_>_months_.size())
    {
        return false;
    }
    if(_cur_month_day_>_months_.at(_cur_month_-1)._length_)
    {
        return false;
    }
    return true;
}

bool DateElement::setDay(uint day_of_month)
{
    if(!day_of_month)
    {
        return false;
    }
    if(!_cur_month_ || _cur_month_>_months_.size())
    {
        return false;
    }
    if(day_of_month>_months_.at(_cur_month_-1)._length_)
    {
        return false;
    }
    _cur_month_day_ = day_of_month;
    return true;
}

bool DateElement::setMonth(uint month_of_year)
{
    if(!month_of_year || month_of_year > _months_.size())
    {
        return false;
    }
    _cur_month_ = month_of_year;
    return true;
}

bool DateElement::setMonth(const QString& month_name)
{
    int index = findMonthIndex(month_name);
    if(index<0)
    {
        return false;
    }
    _cur_month_ = fromIndex(index);
    return true;
}

void DateElement::setYear(uint year)
{
    _cur_year_ = year;
    return;
}

void DateElement::restoreDates()
{
    if(_cur_month_>_months_.size())
    {
        _cur_month_ = 0;
    }
    if(_cur_month_day_>_months_.at(_cur_month_-1)._length_)
    {
        _cur_month_day_ = 0;
    }
    return;
}

inline bool DateElement::isWeekdayExists(const QString& name)
{
    return findDayIndex(name)>=0;
}

inline bool DateElement::isMonthExists(const QString& name)
{
    return findMonthIndex(name)>=0;
}

int DateElement::findDayIndex(const QString& name)
{
//    auto iter= std::find_if(_weekdays_.cbegin(),_weekdays_.cend(),
//                 [&name](const Loc_WeekDay& day)
//    {
//        return day._name_==name;
//    });
    for(int i = 0; i<_weekdays_.size();i++)
    {
        if(_weekdays_.at(i)._name_==name)
        {
            return i;
        }
    }
    return -1;
}

int DateElement::findMonthIndex(const QString& name)
{
    for(int i = 0; i<_months_.size();i++)
    {
        if(_months_.at(i)._name_==name)
        {
            return i;
        }
    }
    return -1;
}

inline int DateElement::toIndex(uint num)
{
    return num-1;
}

inline uint DateElement::fromIndex(int num)
{
    return num+1;
}

inline bool DateElement::inDaySize(uint num)
{
    return num>0 && num <=_weekdays_.size();
}
inline bool DateElement::inMonthSize(uint num)
{
    return num>0 && num <=_months_.size();
}

bool DateElement::addMonth(const QString& name, uint num_of_days, uint leap_period)
{
    if(num_of_days<1 || isMonthExists(name))
        return false;
    _months_.append(Loc_Month(name,num_of_days,leap_period));
    return true;
}

QString DateElement::monthName(uint num)
{
    QString answer;
    if(inMonthSize(num))
    {
        answer = _months_.at(toIndex(num))._name_;
    }
    return answer;
}

int DateElement::monthNum(const QString& name)
{
    int index = findMonthIndex(name);
    if(index<0)
    {
        return -1;
    }
    return fromIndex(index);
}
int DateElement::monthLen(uint num)
{
    if(!inMonthSize(num))
    {
        return -1;
    }
    return _months_.at(toIndex(num))._length_;
}

int DateElement::monthLen(const QString& name)
{
    int index = findMonthIndex(name);
    if(index<0)
    {
        return -1;
    }
    return _months_.at(index)._length_;
}
int DateElement::monthLeapPeriod(uint num)
{
    if(!inMonthSize(num))
    {
        return -1;
    }
    return _months_.at(toIndex(num))._leap_period_;
}

int DateElement::monthLeapPeriod(const QString& name)
{
    int index = findMonthIndex(name);
    if(index<0)
    {
        return -1;
    }
    return _months_.at(index)._leap_period_;
}

bool DateElement::delMonth(const QString& name)
{
    int index = findMonthIndex(name);
    if(index<0)
    {
        return false;
    }
    _months_.removeAt(index);
    return true;
}

bool DateElement::delMonth(uint num)
{
    if(!inMonthSize(num))
        return false;
    _months_.removeAt(toIndex(num));
    return true;
}

bool DateElement::swapMonths(const QString& first_name,
                  const QString& second_name)
{
    int f_index = findMonthIndex(first_name);
    if(f_index<0)
        return false;
    int s_index = findMonthIndex(second_name);
    if(s_index<0)
        return false;
    if(f_index==s_index)
        return false;
    _months_.swapItemsAt(f_index,s_index);
    return true;
}

bool DateElement::switchMonths(uint first_num, uint second_num)
{
    if((first_num!=second_num)&&
            (inMonthSize(first_num)&&
            inMonthSize(second_num)))
    {
        _months_.swapItemsAt(toIndex(first_num),
                             toIndex(second_num));
        return true;
    }
    return false;
}

uint DateElement::monthCount()
{
    return _months_.size();
}

bool DateElement::setFirstWeekdayOfEra(uint day_num)
{
    if(!inDaySize(day_num))
    {
        return false;
    }
    _starting_weekday_ = day_num;
    return true;
}

bool DateElement::setFirstWeekdayOfEra(const QString& day_name)
{
    int index = findDayIndex(day_name);
    if(index<0)
        return false;
    _starting_weekday_ = fromIndex(index);
    return true;
}

bool DateElement::addWeekDay(const QString& name)
{
    if(isWeekdayExists(name))
        return false;
    _weekdays_.append(Loc_WeekDay(name));
    return true;
}

int DateElement::dayNum(const QString& name)
{
    int index = findDayIndex(name);
    if(index<0)
    {
        return -1;
    }
    return fromIndex(index);
}

QString DateElement::dayName(uint num)
{
    if(!inDaySize(num))
        return QString();
    return _weekdays_.at(toIndex(num))._name_;
}

bool DateElement::delWeekDay(const QString& name)
{
    int index = findDayIndex(name);
    if(index<0)
        return false;
    _weekdays_.removeAt(index);
    return true;
}

bool DateElement::delWeekDay(uint num)
{
    if(!inDaySize(num))
        return false;
    _weekdays_.removeAt(toIndex(num));
    return true;
}

bool DateElement::swapDays(const QString& first_name,
                const QString& second_name)
{
    int f_index = findDayIndex(first_name);
    if(f_index<0)
        return false;
    int s_index = findDayIndex(second_name);
    if(s_index<0)
        return false;
    if(f_index==s_index)
        return false;

    _weekdays_.swapItemsAt(f_index,s_index);
    return true;
}

bool DateElement::swapDays(uint first_num, uint second_num)
{
    if((first_num==second_num)||
            !inDaySize(first_num) ||
            !inDaySize(second_num))
    {
        return false;
    }
    _weekdays_.swapItemsAt(toIndex(first_num),toIndex(second_num));
    return true;
}

uint DateElement::weekDaysCount()
{
    return _weekdays_.size();
}


QVariant DateElement::value() const
{
    QString answer;
    answer.append(QString::number(_cur_month_day_)+" "+
                  _months_.at(toIndex(_cur_month_))._name_+" "+
                  QString::number(_cur_year_));
    return QVariant::fromValue(answer);
}

void DateElement::setValue(const QVariant& new_val, bool inform_signal)
{
    if(!new_val.canConvert<QString>())
        return;
    QString date = new_val.toString();
    QStringList list = date.split(" ",Qt::SkipEmptyParts);
    if(list.size()!=3)
        return;
    setDay(list.at(0).toInt());
    setMonth(list.at(1));
    setYear(list.at(2).toInt());
    if(inform_signal)
        emit elementChanged(elementName(),new_val);
    return;
}
char DateElement::elementType() const
{
    return ET_DateElement;
}

[[nodiscard]] QWidget* DateElement::generateWidget()
{
    if(!checkDates())
        return nullptr;

//    if(_read_only_)
//    {
//        QLabel* date = new QLabel();
//        date->setObjectName(elementName());
//        QString info = QString::number(_cur_month_day_)+
//                "'га "+_months_.at(toIndex(_cur_month_))._name_+" "+
//                QString::number(_cur_year_)+" года";
//        date->setText(info);
//        return date;
//    }

    QGroupBox* container = new QGroupBox();
    QHBoxLayout* layout = new QHBoxLayout();
    QSpinBox* date = new QSpinBox();
    QComboBox* month = new QComboBox();
    QSpinBox* year = new QSpinBox();
    container->setObjectName(elementName());
    date->setObjectName(elementName()+QString("_date"));
    month->setObjectName(elementName()+QString("_month"));
    year->setObjectName(elementName()+QString("_year"));
    date->setMinimum(1);
    date->setMaximum(_months_.at(toIndex(_cur_month_))._length_);
    for(Loc_Month& m_name:_months_)
    {
        month->addItem(m_name._name_);
    }
    year->setMinimum(0);
    year->setMaximum(99999);

    date->setValue(_cur_month_day_);
    month->setCurrentIndex(toIndex(_cur_month_));
    year->setValue(_cur_year_);
    layout->addWidget(date);
    layout->addWidget(month);
    layout->addWidget(year);
    if(_read_only_)
    {
//        date->setEnabled(false);
//        month->setEnabled(false);
//        year->setEnabled(false);
        container->setEnabled(false);
    }
    container->setFlat(true);
    container->setLayout(layout);
    return container;
}

void DateElement::changeElement(QVariant new_val, bool inform_signal)
{
    setValue(new_val,inform_signal);
    return;
}

void DateElement::setEditable(bool state)
{
    _read_only_ = state;
    return;
}

QString DateElement::internalDataType() const
{
    return QString("QString");
}
