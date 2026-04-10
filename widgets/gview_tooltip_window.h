#ifndef GVIEW_TOOLTIP_WINDOW_H
#define GVIEW_TOOLTIP_WINDOW_H
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

class GViewToolTip:public QWidget
{
    Q_OBJECT
private:
    QLabel * _info_;
public:
    GViewToolTip(const QString& data, QWidget* tata = nullptr);
public slots:
    void updateFields(const QString& new_val);
};

#endif // GVIEW_TOOLTIP_WINDOW_H
