#ifndef TEMPLATEITEMDATA_H
#define TEMPLATEITEMDATA_H

template <typename T,
          typename T_F,
          typename TypeStr>
class ItemDataInterface
{
public:
    enum dataChangeType{DC_Internal, DC_External};
    virtual T getGrData() const = 0;
    virtual void setGrData(const T& data,dataChangeType gr_type) = 0;
    virtual int getGrDataFieldCount() const = 0;
    virtual TypeStr getGrDataFieldTypeAt(int field_num) const = 0;
    virtual T_F getGrDataFieldAt(int field_num) const = 0;
};


#endif // TEMPLATEITEMDATA_H
