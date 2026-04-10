#include "gview_edit_window.h"
GViewEdit::GViewEdit(const QString& data, QWidget* tata):QWidget(tata)
{
    setWindowModality(Qt::WindowModality::ApplicationModal);
    setWindowFlag(Qt::FramelessWindowHint,true);
    _save_button_ = new QPushButton("Save 'n Close");
    _close_button_ = new QPushButton("Close");
    _apply_button_ = new QPushButton("Apply");
    connect(_save_button_,&QPushButton::clicked,this,&GViewEdit::manualSave);
    connect(_close_button_,&QPushButton::clicked,this,&GViewEdit::manualClose);
    connect(_apply_button_,&QPushButton::clicked,this,&GViewEdit::manualApply);
    QHBoxLayout * button_line = new QHBoxLayout;
    button_line->addWidget(_save_button_);
    button_line->addWidget(_close_button_);
    button_line->addWidget(_apply_button_);

    _text_ = new QTextEdit;
    _text_->setText(data);
    _original_text_=data;
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(_text_);
    layout->addLayout(button_line);
    setLayout(layout);
    setStyleSheet(
                                  "QTextEdit {color : darkRed; "
                                  "background-color:yellow} "
                                  "QPushButton {"
                                  "background-color: red;"
                                  "border-style: outset;"
                                  "border-width: 2px;"
                                  "border-radius: 10px;"
                                  "border-color: beige;"
                                  "font: bold 14px;"
                                  "min-width: 10em;"
                                  "padding: 6px;"
                                  "}"
                                  "QPushButton:pressed {"
                                  "background-color: rgb(224, 12, 77);"
                                  "border-style: inset;}");
    resize(300,150);
    return;
}


void GViewEdit::manualSave()
{
    if(_original_text_!=_text_->toPlainText())
    {
        manualApply();
    }
    close();
    return;
}
void GViewEdit::manualClose()
{
    if(_original_text_!=_text_->toPlainText())
    {
        QMessageBox mbx(this);
        mbx.setText("Зьмяненьні не былі захаваныя.");
        mbx.setInformativeText("Ці жадаеце Вы захаваць зьмяненьні?");
        mbx.setStandardButtons(QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
        mbx.setDefaultButton(QMessageBox::Save);
        int ret = mbx.exec();
        switch(ret)
        {
        case QMessageBox::Save:
        {
            manualApply();
        }
            break;
        case QMessageBox::Discard:
        {
            _text_->setText(_original_text_);
        }
            break;
        case QMessageBox::Cancel:
        {
            return;
        }
            break;
        }
    }
    close();
    return;
}
void GViewEdit::manualApply()
{
    if(_original_text_==_text_->toPlainText())
        return;

    _original_text_ = _text_->toPlainText();
    emit valueChanged(_original_text_);
    return;
}

void GViewEdit::updateFields(const QString& new_val)
{
    _original_text_ = new_val;
    _text_->setText(_original_text_);
    return;
}
