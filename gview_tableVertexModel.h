#ifndef GVIEW_TABLEVERTEXMODEL_H
#define GVIEW_TABLEVERTEXMODEL_H
#define INACTIVE_PHANTOM_ROW -1
#define VM_COLUMN_COUNT 1
#include "gviewitem.h"
#include <QIODevice>
#include <QAbstractTableModel>
#include <QMimeData>
#include <QDataStream>

class VertexModel:public QAbstractTableModel
{
private:
    QVector<GViewItem*> _vertices_;
    QVector<GViewItem*> _proxy_vector_;
    char _vm_flags_;
    int _dragged_row_;
    int _phantom_row_;
    bool (*_sort_func_)(GViewItem* left, GViewItem* right);
    int getActualSize() const;
    bool isRowValid(int row) const;
    QVariant getData(int row) const;
    QVector<GViewItem*>* getActualList();
    const QVector<GViewItem*>* getActualCList() const;
    bool loadData(int row, const QVariant& data);
public:
    enum VM_FLAGS{VM_NoFlags = 0x00,
                  VM_Proxy_isActive = 0x01,
                  VM_Sorted = 0x02,
                  VM_Filtered = 0x04};
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
    int rowIndex(GViewItem * item) const;
    bool contains(GViewItem* item)const;
    QVector<GViewItem*>::const_iterator begin() const;
    QVector<GViewItem*>::const_iterator end() const;
    QVector<GViewItem*>::const_iterator find(GViewItem* item);
    int extractMimeData(const QMimeData* mimeData) const;
    void setVMFlags(char vm_flags);
    char getVMFlags() const;
    void setPhantomRow(int phantom_row);
    int phantomRow() const;
    void clearPhantomRow();
    void setDraggedRow(int dragged_row);
    void clearDraggedRow();
protected:
    virtual bool sortVM(GViewItem* left, GViewItem* right);
    virtual bool filter(GViewItem* element);
};


#endif // GVIEW_TABLEVERTEXMODEL_H
