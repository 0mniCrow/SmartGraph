#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QGraphicsSceneMouseEvent>

class GraphNode;

class CustomGScene:public QGraphicsScene
{
public:
    CustomGScene(QObject* tata):QGraphicsScene(tata){}
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* m_event) override
    {
        qDebug()<<m_event;
        QGraphicsItem* item = nullptr;
        item = itemAt(m_event->pos(),views().at(0)->transform());
        qDebug()<< item;
        QGraphicsScene::mouseReleaseEvent(m_event);
    }
};

class GraphWidget: public QGraphicsView
{
    Q_OBJECT
private:
    int _timerID_ = 0;
    GraphNode* _centralNode_;
public:
    GraphWidget(QWidget* tata = nullptr);
    void itemMoved();
public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();
protected:
    void keyPressEvent(QKeyEvent* event) override;
    void timerEvent(QTimerEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent* event) override;
#endif
    void drawBackground(QPainter* painter, const QRectF& rect) override;
    void scaleView(qreal scaleFactor);
};

#endif // GRAPHWIDGET_H
