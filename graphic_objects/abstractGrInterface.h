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
    AbstractGrInterface(const item_id_t& id=item_id_t()):_id_(id) {};
    virtual ~AbstractGrInterface(){};
    virtual void setID(item_id_t& id){_id_=id;}
    virtual item_id_t getID() const {return _id_;}
    virtual void setX(coord_real x) = 0;
    virtual void setY(coord_real y) = 0;
    virtual coord_real getX() const = 0;
    virtual coord_real getY() const = 0;
    virtual void setWidth(coord_real width) = 0;
    virtual void setHeight(coord_real height) = 0;
    virtual coord_real getWidth() const = 0;
    virtual coord_real getHeight() const = 0;
    virtual void move(coord_real x, coord_real y) = 0;
    virtual void draw() = 0;
};


#endif // ABSTRACTGRINTERFACE_H
