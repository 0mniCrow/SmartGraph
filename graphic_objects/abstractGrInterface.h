#ifndef ABSTRACTGRINTERFACE_H
#define ABSTRACTGRINTERFACE_H

///Тыпы ўбудаваных аб'ектаў:
#define GR_NO_TYPE 0
#define GR_ABSTRACT_ITEM 1
#define GR_ABSTRACT_CONNECTION 2
#define GR_STATIC_ITEM 3
#define GR_SIMPLE_CONNECTION 4
#define GR_ABSTRACT_QT_ITEM 5
#define GR_ITEM_ID_DEF 0

#include <string>
using coord_real = double;
using item_id_t = unsigned int;


class AbstractGrInterface
{
private:
    item_id_t _id_;
public:
    enum GrObjectType{NoType = GR_NO_TYPE};
    AbstractGrInterface(const item_id_t& id=GR_ITEM_ID_DEF):_id_(id) {};
    virtual ~AbstractGrInterface() = default;
    virtual void setGrID(item_id_t& id){_id_=id;}
    virtual item_id_t getGrID() const {return _id_;}
    virtual void drawGr() = 0;
    virtual char grObjectType() const noexcept{return NoType;}
};


#endif // ABSTRACTGRINTERFACE_H
