#ifndef GVIEWTIMETOOL_H
#define GVIEWTIMETOOL_H

#include <QObject>
#include <QSlider>
#include <QString>
#include <QVariant>
#include <QGroupBox>
#include <QGridLayout>
#include <QMap>

struct AbstractTimeSpace
{
    QString _name_;                                 //Імя бягучага прамежку
    QString _child_name_;                           //Імя спадкавага прамежку
    int _child_space_num_;                          //Колькасць спадкавых прамежкаў у бягучым
};


class AbstractTimeLine
{
private:
    unsigned long long _start_time_;
    unsigned long long _end_time;
    AbstractTimeSpace _minimal_space_;

public:
    explicit AbstractTimeLine();

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
