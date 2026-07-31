#ifndef TEMPLATEITEMDATA_H
#define TEMPLATEITEMDATA_H

template <typename T>
class ItemDataInterface
{
public:
    enum dataChangeType{DC_Internal, DC_External};
    virtual T getGrData() const = 0;
    virtual void setGrData(const T& data,dataChangeType gr_type) = 0;
};


#endif // TEMPLATEITEMDATA_H
