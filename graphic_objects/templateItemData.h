#ifndef TEMPLATEITEMDATA_H
#define TEMPLATEITEMDATA_H

template <typename T>
class itemDataAccessInterface
{
public:
    T getData() const = 0;
    void setData(const T& data) = 0;
};


#endif // TEMPLATEITEMDATA_H
