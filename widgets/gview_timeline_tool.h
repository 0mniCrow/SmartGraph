#ifndef GVIEWTIMETOOL_H
#define GVIEWTIMETOOL_H

#include <QObject>
#include <QSlider>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QTimer>
#include "supplement/gview_time_objects.h"
#include "gview_time_slider.h"

using gview_time = int;

class GViewTimeInterface: public QObject
{
    Q_OBJECT
    Q_PROPERTY(gview_time _current_time_ READ currentTime WRITE setCurrentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(gview_time _max_time_ READ maxTime WRITE setMaxTime NOTIFY maxTimeChanged)
    Q_PROPERTY(gview_time _min_time_ READ minTime WRITE setMinTime NOTIFY minTimeChanged)
protected:
    gview_time _current_time_;
    gview_time _max_time_;
    gview_time _min_time_;
public:
    GViewTimeInterface(QObject* tata = nullptr):QObject(tata){}
    virtual void setCurrentTime(const gview_time& n_time){ _current_time_ = n_time;
                                                           emit currentTimeChanged(_current_time_);}
    virtual gview_time currentTime() const {return _current_time_;}
    virtual void setMaxTime(const gview_time& max_time) {_max_time_ = max_time;
                                                        emit maxTimeChanged();}
    virtual gview_time maxTime() const {return _max_time_;}
    virtual void setMinTime(const gview_time& min_time) {_min_time_ = min_time;
                                                        emit minTimeChanged();}
    virtual gview_time minTime() const {return _min_time_;}
    virtual QStringList scales() const = 0;
    virtual bool addTimeObject(GViewBaseTObject* object) = 0;
    virtual bool removeTimeObject(const QString& timeObject) = 0;
signals:
    void currentTimeChanged(gview_time time);
    void sliderStateChanged(int stage);
    void maxTimeChanged();
    void minTimeChanged();
public slots:
    virtual void stepForward() = 0;
    virtual void stepBack() = 0;
    virtual void jumpForward(int step) = 0;
    virtual void jumpBack(int step) = 0;
    virtual void moveTo(int value) = 0;
    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void setScale(const QString& sc_name) = 0;
};

class GViewBaseTool: public GViewTimeInterface
{
private:
    QSet<GViewBaseTObject*> _time_units_;
};

class GViewTimeTool : public QObject
{
    Q_OBJECT
private:
    QTimer _timer_;
    TimeSlider* _slider_;
    int _tick_number_;
    QStringList _labels_;
public:
    explicit GViewTimeTool(int tick_number = 0,QObject *parent = nullptr);
    [[nodiscard]] QSlider* getTimelineWidget();
    void setTickNumber(int tick_number);
    void loadValues(const QStringList& values);
signals:
    void stateChanged(int state);
    void widgetRequireUpdate();
public slots:
    void moveForward();
    void moveBack();
    void jump(int new_state);
};

#endif // GVIEWTIMETOOL_H
