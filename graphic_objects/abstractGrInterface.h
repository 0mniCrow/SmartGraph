#ifndef ABSTRACTGRINTERFACE_H
#define ABSTRACTGRINTERFACE_H

#include <string>
using coord_real = double;
using item_id_t = std::string;

class AbstractGrInterface
{
private:
    item_id_t _id_;
public:
    enum GraphicItemType{NoType = 0};
    AbstractGrInterface(const item_id_t& id=item_id_t()):_id_(id) {};
    virtual ~AbstractGrInterface(){};
    virtual void setGrID(item_id_t& id){_id_=id;}
    virtual item_id_t getGrID() const {return _id_;}
    virtual void setGrX(coord_real x) = 0;
    virtual void setGrY(coord_real y) = 0;
    virtual coord_real getGrX() const = 0;
    virtual coord_real getGrY() const = 0;
    virtual void setGrWidth(coord_real width) = 0;
    virtual void setGrHeight(coord_real height) = 0;
    virtual coord_real getGrWidth() const = 0;
    virtual coord_real getGrHeight() const = 0;
    virtual void moveGr(coord_real x, coord_real y) = 0;
    virtual void drawGr() = 0;
    virtual char graphicType() const noexcept{return NoType;}
};


#endif // ABSTRACTGRINTERFACE_H
