#include "visualisationgraphform.h"
#include "ui_visualisationgraphform.h"

VisualisationGraphForm::VisualisationGraphForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VisualisationGraphForm)
{
    ui->setupUi(this);
    _view_ = new GViewPort(ui->spin_Radius->value());
    _scene_ = new GViewScene(_view_);
    _scene_->setSceneRect(QRectF(-500,500,1000,1000));
    _view_->setScene(_scene_);
    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(_view_);
    ui->group_forGView->setLayout(layout);
    connect(ui->button_GView_add,&QPushButton::clicked,this,&VisualisationGraphForm::AddObject);
    connect(ui->button_GView_remove,&QPushButton::clicked,this,&VisualisationGraphForm::RemoveObject);
    connect(ui->button_AddEdge,&QPushButton::clicked,this,&VisualisationGraphForm::CreateEdge);
    connect(ui->button_removeEdge,&QPushButton::clicked,this,&VisualisationGraphForm::RemoveEdge);
    connect(ui->spin_Radius,&QSpinBox::valueChanged,this,&VisualisationGraphForm::ChangeSize);
}

VisualisationGraphForm::~VisualisationGraphForm()
{
    _view_->deleteLater();
    _scene_->deleteLater();
    delete ui;
}

void VisualisationGraphForm::AddObject()
{
    if(_view_->mode()==GViewPort::GPort_add)
    {
        _view_->setMode(GViewPort::GPort_NoMode);
    }
    else
    {
        _view_->setMode(GViewPort::GPort_add);
    }
    //_view_->changeAddMode(true);
}

void VisualisationGraphForm::RemoveObject()
{
    if(_view_->mode()==GViewPort::GPort_delete)
    {
        _view_->setMode(GViewPort::GPort_NoMode);
    }
    else
    {
        _view_->setMode(GViewPort::GPort_delete);
    }
    //_view_->changeDeleteMode(true);
}

void VisualisationGraphForm::CreateEdge()
{
    if(_view_->mode()==GViewPort::GPort_startAddEdge||
            _view_->mode()==GViewPort::GPort_finAddEdge)
    {
        _view_->setMode(GViewPort::GPort_NoMode);
    }
    else
    {
        _view_->setMode(GViewPort::GPort_startAddEdge);
    }
    //_view_->changeAddEdgeMode(true);
}

void VisualisationGraphForm::RemoveEdge()
{
    if(_view_->mode()==GViewPort::GPort_startDelEdge||
            _view_->mode()==GViewPort::GPort_finDelEdge)
    {
        _view_->setMode(GViewPort::GPort_NoMode);
    }
    else
    {
        _view_->setMode(GViewPort::GPort_startDelEdge);
    }
}

void VisualisationGraphForm::ChangeSize(int radius)
{
    _view_->setRadius(radius);
}
