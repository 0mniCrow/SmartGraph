#ifndef GVIEW_TABLEVERTEXVIEW_H
#define GVIEW_TABLEVERTEXVIEW_H

#include "gview_tableVertexModel.h"
#include "gview_tablePhantomRowProxyModel.h"
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
public:
    VertexList(SelectedRow* styleDelegate, QWidget* tata = nullptr);
protected:
    void dragMoveEvent(QDragMoveEvent* d_event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* d_event) override;
    void startDrag(Qt::DropActions supportedActions) override;
};

#endif // GVIEW_TABLEVERTEXVIEW_H
