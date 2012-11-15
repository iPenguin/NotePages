/*********************************************************************************************\
    Note Pages - the note taking and cross reference application 
    Copyright (c) 2012 Brian C. Milco <bcmilco AT gmail DOT com>       

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; version 2
    of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
\*********************************************************************************************/
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

    int zoomPercent() { return mZoomPercent; }

signals:
    void scrollBarChanged(int dx, int dy);
    void zoomLevelChanged(int percent);

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

    int mZoomPercent;
};

#endif // PAGEVIEW_H
