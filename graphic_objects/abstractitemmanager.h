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
public:
    explicit AbstractItemManager();
    virtual ~AbstractItemManager() = default;

    virtual AbstractGrInterface* createItem(qreal x_coord, qreal y_coord, char type, uint id = 0) = 0;
    virtual bool deleteItem(AbstractGrInterface* item) = 0;
    virtual bool deleteItem(uint id) = 0;
    virtual AbstractGrInterface* findItem(uint id) const = 0;
    virtual AbstractGrInterface* findItem(qreal x_coord, qreal y_coord) = 0;
    virtual AbstractGrConnection* createConnection(AbstractGrInterface* source,
                                                   AbstractGrInterface* destination,
                                                   char type,
                                                   uint id = 0) = 0;
    virtual bool deleteConnection(AbstractGrConnection* connection) = 0;
    virtual bool deleteConnection(uint id) = 0;
    virtual AbstractGrConnection* findConnection(uint id) const = 0;
    virtual AbstractGrConnection* findConnection(AbstractGrInterface* source,
                                                 AbstractGrInterface* destination) = 0;

    virtual QString getLastError() const = 0;

};

#endif // ABSTRACTITEMMANAGER_H
