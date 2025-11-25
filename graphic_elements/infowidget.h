#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <QVariant>
#include <QTimer>
#include <QMap>
#include <QSpinBox>
#include <QSet>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QLayout>
#include <QCloseEvent>
#include <QMessageBox>

#define DEF_CLOSING_TIME 3000

struct ObjReinforced
{
    QWidget* _widget_;
    QVariant _orig_value_;
    ObjReinforced(QWidget* widget,
                  const QVariant& orig_value):
        _widget_(widget),_orig_value_(orig_value){}
};

class InfoWidget : public QWidget
{
    Q_OBJECT

protected:
    virtual void closeEvent(QCloseEvent*  cl_event) override;
    virtual void enterEvent(QEnterEvent* ent_event) override;
    virtual void leaveEvent(QEvent* lef_event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* m_event) override;
private:
    enum IW_InternalFlags {IW_NoFlags = 0x00,
                           IW_ReadOnly = 0x01,
                           IW_ImmediateResponce = 0x02,
                           IW_OnTimer = 0x04};
    QTimer _hiding_timer_;
    QMap<QString,ObjReinforced> _elements_;
    QGroupBox* _control_panel_;
    QPushButton* _save_opt_;
    QPushButton* _close_opt_;
    char _flags_;
    void connectElement(const QString& type, QWidget* element);
    void lockElement(const QString& type, QWidget* element, bool lock_status);
    bool loadValue(const QString& type, QWidget* element, const QVariant& value);
    QVariant getValue(QWidget* widget) const;
    bool hasChanged() const;
    static bool isContainerClass(const QString& class_name);
    static bool isEditableClass(const QString& class_name);
    static bool isAllowedClass(const QString& class_name);
public:
    explicit InfoWidget(bool read_only = true, QWidget *parent = nullptr);
    ~InfoWidget();
    void addElement(QWidget* element);
    void setReadOnly(bool mode);
    void setImmediateResponce(bool mode);
    bool setValue(const QString& element_name, const QVariant& value);
    bool updElementDefVal(const QString& element_name);
    QMap<QString,QVariant> getValues(bool changed_only) const;
private slots:
    void catchElementSignal();
    void save();
    void close();
public slots:
    void catchExternalChange(const QString& element_name, const QVariant& value);
    void startClosingTimer(int msec = -1);
signals:
    void elementValueChanged(const QString& element_name, const QVariant& value);
    void saveRequest();
    void closeRequest();

};

#endif // INFOWIDGET_H
