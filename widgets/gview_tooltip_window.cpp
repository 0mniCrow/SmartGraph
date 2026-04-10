#include "gview_tooltip_window.h"

GViewToolTip::GViewToolTip(const QString& data, QWidget* tata):QWidget(tata)
{
    _info_ = new QLabel;
    _info_->setText(data);
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(_info_);
    setLayout(layout);
    setStyleSheet(
                                  "QLabel {color : darkRed; "
                                  "background-color:yellow} "
                                  );
    setWindowFlag(Qt::Popup,true);
    resize(200,100);
    return;
}


void GViewToolTip::updateFields(const QString& new_val)
{
    _info_->setText(new_val);
    return;
}
