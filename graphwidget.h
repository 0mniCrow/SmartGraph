#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QObject>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QRandomGenerator>

class GraphNode;

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
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent* event) override;
#endif
    void drawBackground(QPainter* painter, const QRectF& rect) override;
    void scaleView(qreal scaleFactor);
};

#endif // GRAPHWIDGET_H
