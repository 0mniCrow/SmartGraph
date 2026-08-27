#ifndef SUPERSTRUCTFORMANAGABLEDATAMODELS_H
#define SUPERSTRUCTFORMANAGABLEDATAMODELS_H
#include "QString"
#include "QStringView"
#include <functional>

/*Надбудоўка для класаў як графічная мадэль, мадэль табліцы, г.д.
 *  каб яны выклікаліся ў залежнасьці ад крыніцы зьмяненняў*/
class SuperstructureForManagableDataModels
{
private:
    QString _model_name_;
public:
    SuperstructureForManagableDataModels(const QString& model_name):_model_name_(model_name){return;}
    ~SuperstructureForManagableDataModels() = default;
    void setModelName(const QString& name){_model_name_ = name;}
    QStringView getModelName() const {return _model_name_;}
    virtual void updateFromStructure(uint id, QStringView sender_model) = 0;
    virtual std::function<void(unsigned int, QStringView sender_model)> getCallbackFunction() = 0;
};

using SuperSFMDM = SuperstructureForManagableDataModels;

#endif // SUPERSTRUCTFORMANAGABLEDATAMODELS_H
