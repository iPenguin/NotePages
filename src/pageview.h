/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QGraphicsView>

class PageView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PageView(QWidget *parent = 0);
    ~PageView();

    void zoomIn();
    void zoomOut();

    //zoom takes the mouseDelta which is usually in icrements of +/- 120;
    void zoom(int mouseDelta);
    void zoomLevel(int percent);

signals:
    void scrollBarChanged(int dx, int dy);
    void zoomLevelChanged(int percent);

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

    
};

#endif // PAGEVIEW_H
