#ifndef SUPERSTRUCTFORMANAGABLEDATAMODELS_H
#define SUPERSTRUCTFORMANAGABLEDATAMODELS_H
#include "QString"
#include "QStringView"


/*Надбудоўка для класаў як графічная мадэль, мадэль табліцы, г.д.
 *  каб яны выклікаліся ў залежнасьці ад крыніцы зьмяненняў*/
class SuperstructureForManagableDataModels
{
private:
    QString _model_name_;
public:
    SuperstructureForManagableDataModels(const QString& model_name):_model_name_(model_name){return;}
    ~SuperstructureForManagableDataModels() = default;
    virtual void updateFromStructure(uint id, QStringView sender_model) = 0;
    void setModelName(const QString& name){_model_name_ = name;}
    QStringView getModelName() const {return _model_name_;}
};

using SuperSFMDM = SuperstructureForManagableDataModels;

#endif // SUPERSTRUCTFORMANAGABLEDATAMODELS_H
