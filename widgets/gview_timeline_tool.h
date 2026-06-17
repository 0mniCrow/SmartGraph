#ifndef GVIEWTIMETOOL_H
#define GVIEWTIMETOOL_H
#define GVIEW_TIMER_INTERVAL 1000

#include <QObject>
#include <QSlider>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QTimer>
#include "supplement/gview_time_objects.h"
#include "gview_time_slider.h"

using gview_time = gview_time_t;

class GViewTimeInterface: public QObject
{
    Q_OBJECT
    Q_PROPERTY(gview_time _current_time_ READ currentTime WRITE setCurrentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(gview_time _max_time_ READ maxTime WRITE setMaxTime NOTIFY maxTimeChanged)
    Q_PROPERTY(gview_time _min_time_ READ minTime WRITE setMinTime NOTIFY minTimeChanged)
    Q_PROPERTY(int _timer_interval_ READ timerInterval WRITE setTimerInterval NOTIFY timerIntervalChanged)
    Q_CLASSINFO("info","Provides an abstract interface for virtual time control.")
private:
    int _timer_interval_;
    gview_time _current_time_;
    gview_time _max_time_;
    gview_time _min_time_;
protected:
    virtual GViewBaseTObject* getUnitInstance(const QString& obj_name) const  = 0;
public:
    GViewTimeInterface(QObject* tata = nullptr):QObject(tata){}
    GViewTimeInterface(gview_time min_time,
                       gview_time max_time,
                       gview_time cur_time,
                       int timer_interval = GVIEW_TIMER_INTERVAL,
                       QObject* tata = nullptr):
        QObject(tata),_timer_interval_(timer_interval),
        _current_time_(cur_time),
        _max_time_(max_time),_min_time_(min_time){}
    virtual ~GViewTimeInterface(){}
    void setCurrentTime(const gview_time& n_time){ _current_time_ = n_time;
                                                           emit currentTimeChanged(_current_time_);}
    gview_time currentTime() const noexcept {return _current_time_;}
    void setMaxTime(const gview_time& max_time)  {_max_time_ = max_time;
                                                        emit maxTimeChanged();}
    gview_time maxTime() const noexcept {return _max_time_;}
    void setMinTime(const gview_time& min_time) {_min_time_ = min_time;
                                                        emit minTimeChanged();}
    gview_time minTime() const noexcept {return _min_time_;}
    int timerInterval()const noexcept {return _timer_interval_;}
    void setTimerInterval(int time_interval){_timer_interval_=time_interval;
                                           emit timerIntervalChanged();}
    virtual QStringList timeUnitNames() const = 0;
    virtual bool getTime(QMap<QString,int>& container) const = 0;               //Атрымаць даведнік з бягучым часам, падзелены па аб'ектах часу (назва/значэнне)
    virtual QString currentUnit() const = 0;
    virtual bool setCurrentUnit(const QString& unit_name) = 0;
signals:
    void currentTimeChanged(gview_time time);
    void sliderStateChanged(int stage);
    void maxTimeChanged();
    void minTimeChanged();
    void timerIntervalChanged();
public slots:
    virtual void stepForward() = 0;
    virtual void stepBack() = 0;
    virtual void jumpForward(int step) = 0;
    virtual void jumpBack(int step) = 0;
    virtual void moveTo(int value) = 0;
    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void setScale(const QString& unit_name) {setCurrentUnit(unit_name);};
    virtual void setTime(const QMap<QString,int>& unitname_val) = 0;        //Усталяваць час па назве кожнага аб'екту часу (назва/значэнне)
    virtual void setTime(const gview_time& time) = 0;                       //Усталяваць час па лічбе.
};



class GViewTimeTool : public GViewTimeInterface
{
    Q_OBJECT
private:
    QTimer _play_timer_;
    TimeSlider* _time_slider_;
    int _tick_number_;
    QStringList _labels_;
    QList<GViewBaseTObject*> _time_units_;
    QString _cur_unit_;
    QPair<gview_time_t,gview_time_t> _time_scale_;
    bool isReadyForWork() const noexcept;                               //Праверка, ці магчыма інструменту працягваць працу
    void setBordersForSlider();                                         //Усталёўвае новыя межы для слайдэра
    void setNewTime(int new_val);                                       //Аднаўляе значэнне бягучага часу згодна з бягучай адзінкай часу
    void updateSliderValue();
    int getCurrentTime();
protected:
    virtual GViewBaseTObject* getUnitInstance(const QString& obj_name) const override;
public:
    explicit GViewTimeTool(int tick_number = 0,QObject *parent = nullptr);
    GViewTimeTool(gview_time min_time,
                  gview_time max_time,
                  gview_time cur_time = 0,
                  int timer_interval = 0,
                  QObject* parent = nullptr);
    ~GViewTimeTool();
    [[nodiscard]] QSlider* getTimelineWidget();
    void setTickNumber(int tick_number);
    void loadValues(const QStringList& values);
    virtual QStringList timeUnitNames() const override;

    virtual QString currentUnit() const override;
    virtual bool setCurrentUnit(const QString& unit_name) override;
    virtual bool getTime(QMap<QString,int>& container) const override;
    bool addTimeUnit(GViewBaseTObject* object);
    bool deleteTimeUnit(const QString& time_object);
signals:
    void stateChanged(int state);
    void widgetRequireUpdate();
public slots:
    void moveForward();
    void moveBack();
    void jump(int new_state);

    virtual void stepForward() override;
    virtual void stepBack() override;
    virtual void jumpForward(int step) override;
    virtual void jumpBack(int step) override;
    virtual void moveTo(int value) override;
    virtual void play() override;
    virtual void stop() override;
    virtual void pause() override;
    virtual void setScale(const QString& sc_name) override;
    virtual void setTime(const QMap<QString,int>& unitname_val) override;
    virtual void setTime(const gview_time& time) override;
};

#endif // GVIEWTIMETOOL_H
