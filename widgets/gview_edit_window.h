#ifndef GVIEW_EDIT_WINDOW_H
#define GVIEW_EDIT_WINDOW_H
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

class GViewEdit:public QWidget
{
    Q_OBJECT
private:
    QTextEdit* _text_;
    QString _original_text_;
    QPushButton* _save_button_;
    QPushButton* _close_button_;
    QPushButton* _apply_button_;
public:
    GViewEdit(const QString& data, QWidget* tata = nullptr);
private slots:
    void manualSave();
    void manualClose();
    void manualApply();
public slots:
    void updateFields(const QString& new_val);
signals:
    void valueChanged(QString changed_value);
};
#endif // GVIEW_EDIT_WINDOW_H
