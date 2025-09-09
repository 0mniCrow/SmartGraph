#ifndef VISUALISATIONGRAPHFORM_H
#define VISUALISATIONGRAPHFORM_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include "gviewport.h"

namespace Ui {
class VisualisationGraphForm;
}

class VisualisationGraphForm : public QWidget
{
    Q_OBJECT
private:
    Ui::VisualisationGraphForm *ui;
    GViewScene* _scene_;
    GViewPort* _view_;
    VertexModel* _model_;
    bool _add_mode_;
public:
    explicit VisualisationGraphForm(QWidget *parent = nullptr);
    ~VisualisationGraphForm();

private slots:
    void AddObject();
    void RemoveObject();
    void CreateEdge();
    void RemoveEdge();
    void ChangeSize(int radius);
    void updateInfo(QString info);
};

#endif // VISUALISATIONGRAPHFORM_H
