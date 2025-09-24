#include "visualisationgraphform.h"
#include "ui_visualisationgraphform.h"

VisualisationGraphForm::VisualisationGraphForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VisualisationGraphForm)
{
    ui->setupUi(this);
    _model_ = new VertexModel();
    //_proxy_model_ = new PhantomRowProxyModel(this);
    //_proxy_model_->setSourceModel(_model_);
    _view_ = new GViewPort(ui->spin_Radius->value(),_model_);
    _scene_ = new GViewScene(_view_);
    _scene_->setSceneRect(QRectF(-500,500,1000,1000));
    _view_->setScene(_scene_);
    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(_view_);
    ui->group_forGView->setLayout(layout);
    SelectedRow* delegate = new SelectedRow();
    _vert_list_ = new VertexList(delegate);
    _vert_list_->setModel(_model_);
    _vert_list_->setDragDropMode(QAbstractItemView::InternalMove);
    _vert_list_->setSelectionMode(QAbstractItemView::SingleSelection);
    _vert_list_->setDefaultDropAction(Qt::MoveAction);
    _vert_list_->horizontalHeader()->hide();
    _vert_list_->verticalHeader()->hide();
    _vert_list_->setSelectConn();
    //_vert_list_->setItemDelegate(delegate);
    QVBoxLayout* layout2 = new QVBoxLayout();
    layout2->addWidget(_vert_list_);
    ui->group_forTableView->setLayout(layout2);
    connect(ui->button_GView_add,&QPushButton::clicked,this,&VisualisationGraphForm::AddObject);
    connect(ui->button_GView_remove,&QPushButton::clicked,this,&VisualisationGraphForm::RemoveObject);
    connect(ui->button_AddEdge,&QPushButton::clicked,this,&VisualisationGraphForm::CreateEdge);
    connect(ui->button_removeEdge,&QPushButton::clicked,this,&VisualisationGraphForm::RemoveEdge);
    connect(ui->spin_Radius,&QSpinBox::valueChanged,this,&VisualisationGraphForm::ChangeSize);
    connect(_view_,&GViewPort::selectedInfo,this,&VisualisationGraphForm::updateInfo);
    connect(_view_,&GViewPort::viewNewSelect,_vert_list_,&VertexList::outsideNewSelect);
    connect(_vert_list_,&VertexList::listNewSelect,_view_,&GViewPort::outsideNewSelect);
}

VisualisationGraphForm::~VisualisationGraphForm()
{
    _view_->deleteLater();
    _scene_->deleteLater();
    _vert_list_->deleteLater();
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

void VisualisationGraphForm::updateInfo(QString info)
{
    ui->text_GView_info->append(info);
    return;
}
