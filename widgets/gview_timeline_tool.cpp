#include "gview_timeline_tool.h"

///____________AbstractTimeLine_________________
AbstractTimeLine::AbstractTimeLine():
    _start_time_(Q_UINT64_C(0)),
    _end_time_(Q_UINT64_C(0)),
    _cur_time_(Q_UINT64_C(0))
{
    return;
}

bool AbstractTimeLine::addStaticTimeSpace(const QString& name,
                        const QString& child_name,
                        quint64 numb_of_child_timespaces,
                        const QString& parent_name)
{

}

bool AbstractTimeLine::addVariedTimeSpace(const QString& name,
                        const QString& child_name,
                        const QList<QPair<QString,quint64>>& varied_names_and_num,
                        const QString& parent_name)
{

}

bool AbstractTimeLine::addMultiChildTimeSpace(const QString& name,
                            const QMap<QString,quint64>& children_map,
                            const QString& parent_name)
{

}

bool AbstractTimeLine::isExist(const QString& name) const
{

}

quint64 AbstractTimeLine::currentTime(const QString& scale_name) const
{

}

bool AbstractTimeLine::addTime(quint64 time, const QString& scale)
{

}
bool AbstractTimeLine::substractTime(quint64 time, const QString& scale)
{

}

AbstractTimeLine& AbstractTimeLine::operator+=(qint64 time)
{

}

AbstractTimeLine& AbstractTimeLine::operator-=(qint64 time)
{

}

const AbstractTimeSpace *  AbstractTimeLine::currentScaledObject(const QString& scale_name)
{

}

///____________GViewTimeTool____________________
///
GViewTimeTool::GViewTimeTool(QObject *parent)
    : QObject{parent}
{

}
