#ifndef TEMPLATEITEMDATA_H
#define TEMPLATEITEMDATA_H

template <typename T>
class ItemDataInterface
{
public:
    virtual T getGrData() const = 0;
    virtual void setGrData(const T& data) = 0;
};


#endif // TEMPLATEITEMDATA_H
