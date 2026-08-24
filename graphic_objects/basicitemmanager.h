#ifndef BASICITEMMANAGER_H
#define BASICITEMMANAGER_H
#include "abstractitemmanager.h"
#include "simplegrconnection.h"
#include "staticgritem.h"
#include <QStringView>

class BasicItemManager:public QObject, public AbstractItemManager
{
    Q_OBJECT
private:
    QMap<uint,AbstractGrItem*>          _items_;
    QMap<uint,AbstractGrConnection*>    _connections_;
    QString                             _last_error_;

    bool checkItemType(char gr_type) const;
    bool checkConnectionType(char gr_type) const;
public:
    BasicItemManager();
    virtual ~BasicItemManager();
    virtual AbstractGrInterface* createItem(qreal x_coord, qreal y_coord, char type, uint id = 0) override;
    virtual bool deleteItem(AbstractGrInterface* item) override;
    virtual bool deleteItem(uint id) override;
    virtual AbstractGrInterface* findItem(uint id) const override;
    virtual AbstractGrInterface* findItem(qreal x_coord, qreal y_coord) override;
    virtual AbstractGrConnection* createConnection(AbstractGrInterface* source,
                                                   AbstractGrInterface* destination,
                                                   char type,
                                                   uint id = 0) override;
    virtual bool deleteConnection(AbstractGrConnection* connection) override;
    virtual bool deleteConnection(uint id) override;
    virtual AbstractGrConnection* findConnection(uint id) const override;
    virtual AbstractGrConnection* findConnection(AbstractGrInterface* source,
                                                 AbstractGrInterface* destination) override;

    virtual QStringView getLastError() const override;
signals:
    void objectUpdated(uint id);
    void objectRemoved(uint id);
};

#endif // BASICITEMMANAGER_H
