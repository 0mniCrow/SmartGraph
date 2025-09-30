#ifndef GVIEW_TABLEVERTEXVIEW_H
#define GVIEW_TABLEVERTEXVIEW_H

#include "gview_tableVertexModel.h"
#include <QTableView>
#include <QStyledItemDelegate>
#include <QItemSelectionModel>
#include <QDragMoveEvent>
#include <QDropEvent>


class SelectedRow: public QStyledItemDelegate
{
    Q_OBJECT
private:
    bool _drag_action_;
public:
    SelectedRow(QObject* tata = nullptr);
    void setDragAction(bool drag_action);
    void clearDragAction();
    void paint(QPainter* painter, const QStyleOptionViewItem& opt, const QModelIndex& index) const override;
};

class VertexList:public QTableView
{
    Q_OBJECT
private:
    SelectedRow* selectedRowDelegate();
    VertexModel* vertexModel();
    void changeSelection(const QModelIndex& index);
public:
    VertexList(SelectedRow* styleDelegate, QWidget* tata = nullptr);
    void setSelectConn();
protected:
    void dragMoveEvent(QDragMoveEvent* d_event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* d_event) override;
    void startDrag(Qt::DropActions supportedActions) override;
signals:
    void listNewSelect(GViewItem* selected_item);
public slots:
    void outsideNewSelect(GViewItem* selected_item);
private slots:
    void newSelection(const QModelIndex& newObject, const QModelIndex& prevObject);
};

#endif // GVIEW_TABLEVERTEXVIEW_H
