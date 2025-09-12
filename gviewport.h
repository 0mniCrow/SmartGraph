#ifndef GVIEWPORT_H
#define GVIEWPORT_H

#include <QWidget>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "gviewscene.h"
#include <QPixmap>
#include <QCursor>
#include <QRandomGenerator>
#include <QMouseEvent>
#include <memory>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>
#include <QMimeData>
#include <QDataStream>

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

class VertexModel:public QAbstractTableModel
{
private:
    QVector<GViewItem*> _vertices_;
public:
    VertexModel(QObject * tata = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex& index,int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QStringList mimeTypes() const override;
    QMimeData* mimeData(const QModelIndexList& indexes) const override;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action,
                      int row, int column, const QModelIndex& parent) override;
    Qt::DropActions supportedDropActions() const override;
    Qt::DropActions supportedDragActions() const override;

    bool moveRows(const QModelIndex& sourceParent,
                  int sourceRow, int count, const
                  QModelIndex& destParent, int destChild) override;
    bool insertRows(int row, int count, const QModelIndex& parent) override;
    bool removeRows(int row, int count, const QModelIndex& parent) override;
    void addItem(GViewItem* item, int row = -1);
    void removeItem(GViewItem* item);
    int size();
    GViewItem* operator[](int num);
    GViewItem* at(int num);
    bool contains(GViewItem* item)const;
    QVector<GViewItem*>::const_iterator begin() const;
    QVector<GViewItem*>::const_iterator end() const;
    QVector<GViewItem*>::const_iterator find(GViewItem* item);
};

class GViewPort:public QGraphicsView
{
    Q_OBJECT
public:
    enum GPort_Mode{GPort_NoMode,
                    GPort_add,
                    GPort_delete,
                    GPort_startAddEdge,
                    GPort_finAddEdge,
                    GPort_startDelEdge,
                    GPort_finDelEdge};
    GViewPort(int vertex_radius, VertexModel* model, QWidget* tata = nullptr);
    void setMode(GPort_Mode mode);
    GPort_Mode mode() const {return _mode_;}
    int vertRadius()const {return _vertex_radius_;}
    void setRadius(int radius);
protected:
    void mouseReleaseEvent(QMouseEvent* m_event) override;
    void mouseMoveEvent(QMouseEvent* m_event) override;
private:
    //QList<GViewItem*> _vertices_;
    VertexModel * _vertices_;
    QList<GViewEdge*> _edges_;
    GViewEdge* _new_edge_;
    GViewItem* _del_edge_;
    GViewItem* _selected_vertex_;
    int _vertex_radius_;
    GPort_Mode _mode_;
    int _counter_;

    void delLinkedEdges(GViewItem* vertex);
    bool addEdge(GViewItem* source, GViewItem* dest,bool directed = true);
    void startAddEdge(GViewItem* src);
    void finishAddEdge(GViewItem* dest);
    void startDelEdge(GViewItem* src);
    void finishDelEdge(GViewItem* dest);
    QGraphicsItem* grabItem(QMouseEvent* m_event);
    GViewItem* grabGItem(QMouseEvent* m_event);
    void addItem(GViewItem* vertex, const QPoint& pos);
    void deleteItem(GViewItem* vertex);
signals:
    void selectedInfo(QString info);
};

#endif // GVIEWPORT_H
