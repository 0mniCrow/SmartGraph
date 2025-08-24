#include "visualisationgraphform.h"
#include "ui_visualisationgraphform.h"

VisualisationGraphForm::VisualisationGraphForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VisualisationGraphForm)
{
    ui->setupUi(this);
    _view_ = new GViewPort();
    _scene_ = new GViewScene(_view_);
    _scene_->setSceneRect(QRectF(-500,500,1000,1000));
    _view_->setScene(_scene_);
    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(_view_);
    ui->group_forGView->setLayout(layout);
    connect(ui->button_GView_add,&QPushButton::clicked,this,&VisualisationGraphForm::AddObject);
    connect(ui->button_GView_remove,&QPushButton::clicked,this,&VisualisationGraphForm::RemoveObject);
    connect(ui->button_AddEdge,&QPushButton::clicked,this,&VisualisationGraphForm::CreateEdge);
}

VisualisationGraphForm::~VisualisationGraphForm()
{
    _view_->deleteLater();
    _scene_->deleteLater();
    delete ui;
}

void VisualisationGraphForm::AddObject()
{
    _view_->changeAddMode(true);
}

void VisualisationGraphForm::RemoveObject()
{
    _view_->changeDeleteMode(true);
}

void VisualisationGraphForm::CreateEdge()
{

}
