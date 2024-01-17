#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H


#include <QtWidgets>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget* parent = nullptr) : QGraphicsView(parent) {}


protected:
    void resizeEvent(QResizeEvent* event) override
    {
        QGraphicsView::resizeEvent(event);

        if (scene())
        {
            scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
            fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
        }
    }

    void paintEvent(QPaintEvent* event) override
    {
        QGraphicsView::paintEvent(event);

        if (scene())
        {
            scene()->update();
        }
    }
};


#endif // GRAPHICSVIEW_H
