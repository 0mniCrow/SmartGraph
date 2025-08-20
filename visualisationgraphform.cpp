#include "visualisationgraphform.h"
#include "ui_visualisationgraphform.h"

VisualisationGraphForm::VisualisationGraphForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VisualisationGraphForm)
{
    ui->setupUi(this);
    _view_ = new GViewPort();
    _scene_ = new GViewScene(_view_);
    _view_->setScene(_scene_);
    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(_view_);
    ui->group_forGView->setLayout(layout);
    connect(ui->button_GView_add,&QPushButton::clicked,this,&VisualisationGraphForm::AddObject);
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
