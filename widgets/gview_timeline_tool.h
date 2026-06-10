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

using gview_time = uint;

class GViewTimeInterface: public QObject
{
    Q_OBJECT
    Q_PROPERTY(gview_time _current_time_ READ currentTime WRITE setCurrentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(gview_time _max_time_ READ maxTime WRITE setMaxTime NOTIFY maxTimeChanged)
    Q_PROPERTY(gview_time _min_time_ READ minTime WRITE setMinTime NOTIFY minTimeChanged)
    Q_CLASSINFO("info","Interface suppose to be inherited to control alternative time with events")
protected:
    gview_time _current_time_;
    gview_time _max_time_;
    gview_time _min_time_;
public:
    GViewTimeInterface(QObject* tata = nullptr):QObject(tata){}
    GViewTimeInterface(gview_time min_time,
                       gview_time max_time,
                       gview_time cur_time,
                       QObject* tata = nullptr):
        QObject(tata),_current_time_(cur_time),_max_time_(max_time),_min_time_(min_time){}
    virtual ~GViewTimeInterface(){}
    void setCurrentTime(const gview_time& n_time){ _current_time_ = n_time;
                                                           emit currentTimeChanged(_current_time_);}
    gview_time currentTime() const noexcept {return _current_time_;}
    void setMaxTime(const gview_time& max_time) noexcept {_max_time_ = max_time;
                                                        emit maxTimeChanged();}
    gview_time maxTime() const noexcept {return _max_time_;}
    void setMinTime(const gview_time& min_time) noexcept {_min_time_ = min_time;
                                                        emit minTimeChanged();}
    gview_time minTime() const noexcept {return _min_time_;}
    virtual QStringList scales() const = 0;
    virtual GViewBaseTObject* getObject(const QString& obj_name) const  = 0;
    virtual bool addTimeObject(GViewBaseTObject* object)  = 0;
    virtual bool deleteTimeObject(const QString& timeObject)  = 0;
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



class GViewTimeTool : public GViewTimeInterface
{
    Q_OBJECT
private:
    QTimer _timer_;
    TimeSlider* _slider_;
    int _tick_number_;
    QStringList _labels_;
    QList<GViewBaseTObject*> _time_units_;
    bool checkWorkState() const noexcept;
public:
    explicit GViewTimeTool(int tick_number = 0,QObject *parent = nullptr);
    GViewTimeTool(gview_time min_time,
                  gview_time max_time,
                  gview_time cur_time = 0,
                  QObject* parent = nullptr);
    ~GViewTimeTool();
    [[nodiscard]] QSlider* getTimelineWidget();
    void setTickNumber(int tick_number);
    void loadValues(const QStringList& values);
    virtual QStringList scales() const override;
    virtual GViewBaseTObject* getObject(const QString& obj_name) const override;
    virtual bool addTimeObject(GViewBaseTObject* object) override;
    virtual bool deleteTimeObject(const QString& time_object) override;
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
};

#endif // GVIEWTIMETOOL_H
