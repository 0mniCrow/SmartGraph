#ifndef GVIEW_TABLEVERTEXVIEW_H
#define GVIEW_TABLEVERTEXVIEW_H

#include "gview_tableVertexModel.h"
#include "gview_tablePhantomRowProxyModel.h"
#include <QTableView>
#include <QStyledItemDelegate>
#include <QDragMoveEvent>
#include <QDropEvent>


class SpacingDelegate: public QStyledItemDelegate
{
    Q_OBJECT
private:
    int _space_size_;
    int _drop_row_;
    bool _drag_move_active_;
public:
    SpacingDelegate(int space_size, QObject* tata = nullptr);
    void setDropRow(int drop_row);
    void setDragActive(bool active);
    QSize sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& index) const override;
    void paint(QPainter* painter, const QStyleOptionViewItem& opt, const QModelIndex& index) const override;
};

class VertexList:public QTableView
{
    Q_OBJECT
private:
    int _def_row_height_;
    QModelIndex _cur_index_;
    VertexModel* vertexModel();
public:
    VertexList( QWidget* tata = nullptr);
protected:
    void dragMoveEvent(QDragMoveEvent* d_event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* d_event) override;
    void startDrag(Qt::DropActions supportedActions) override;
};

#endif // GVIEW_TABLEVERTEXVIEW_H
