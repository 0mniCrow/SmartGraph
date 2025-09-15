#ifndef GVIEW_TABLEVERTEXMODEL_H
#define GVIEW_TABLEVERTEXMODEL_H

#include "gviewitem.h"
#include <QIODevice>
#include <QAbstractTableModel>
#include <QMimeData>
#include <QDataStream>

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
    int extractMimeData(const QMimeData* mimeData) const;
};


#endif // GVIEW_TABLEVERTEXMODEL_H
