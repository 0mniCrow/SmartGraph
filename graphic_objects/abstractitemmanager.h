#ifndef ABSTRACTITEMMANAGER_H
#define ABSTRACTITEMMANAGER_H
#include "abstractGrInterface.h"
#include "abstractgrconnection.h"
#include "abstractGrItem.h"
#include <QSet>

class AbstractItemManager
{
private:
    QSet<uint> _IDs_;
    uint _accumulator_;
protected:
    uint generateID();
    bool isExist(uint id);
    bool addID(uint id);
    bool removeID(uint id);
    virtual void updateStatus() = 0;
public:
    explicit AbstractItemManager();
    virtual ~AbstractItemManager() = default;

    virtual AbstractGrItem* createItem(qreal x_coord, qreal y_coord, qreal z_coord, char type, uint id = 0) = 0;
    virtual bool deleteItem(AbstractGrItem* item) = 0;
    virtual bool deleteItem(uint id) = 0;
    virtual AbstractGrItem* findItem(uint id) const = 0;
    virtual AbstractGrItem* findItem(qreal x_coord, qreal y_coord) = 0;
    virtual AbstractGrConnection* createConnection(AbstractGrItem* source,
                                                   AbstractGrItem* destination,
                                                   char type,
                                                   uint id = 0) = 0;
    virtual bool deleteConnection(AbstractGrConnection* connection) = 0;
    virtual bool deleteConnection(uint id) = 0;
    virtual AbstractGrConnection* findConnection(uint id) const = 0;
    virtual AbstractGrConnection* findConnection(AbstractGrItem* source,
                                                 AbstractGrItem* destination) = 0;

    virtual QStringView getLastError() const = 0;
};

#endif // ABSTRACTITEMMANAGER_H
