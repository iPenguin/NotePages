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
#include "pageview.h"

#include <QWheelEvent>
#include <QDebug>
#include <QScrollBar>

PageView::PageView(QWidget *parent) :
    QGraphicsView(parent),
    mZoomPercent(100)
{
    setInteractive(true);
}

PageView::~PageView()
{
}

void PageView::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);
}

void PageView::mouseMoveEvent(QMouseEvent* event)
{
    int deltaX = 0;
    int deltaY = 0;

    /*
     * When the user drags objects on the scene scroll the view. 
     */
    if(event->buttons() == Qt::LeftButton) {

        /*
         * Scroll horizontally if needed.
         */
        if(event->pos().x() < 5) {
            int diff = horizontalScrollBar()->value() - horizontalScrollBar()->minimum();
            if(diff < deltaX)
                deltaX = -diff;
            else
                deltaX = -5;

        } else if (event->pos().x() > viewport()->width() - 5) {
            int diff = horizontalScrollBar()->maximum() - horizontalScrollBar()->value();
            if(diff < deltaX)
                deltaX = diff;
            else
                deltaX = 5;
        }

        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + deltaX);

        /*
         * Scroll vertically if needed.
         */
        if(event->pos().y() < 5) {
            int diff = verticalScrollBar()->value() - verticalScrollBar()->minimum();
            if(diff < deltaY)
                deltaY = -diff;
            else
                deltaY = -5;

        } else if( event->pos().y() > viewport()->height() - 5) {
            int diff = verticalScrollBar()->maximum() - verticalScrollBar()->value();
            if(diff < deltaY)
                deltaY = diff;
            else
                deltaY = 5;
        }

        verticalScrollBar()->setValue(verticalScrollBar()->value() + deltaY);

        /*
         * If we're not at the limits of the scrollbars update them.
         */
        bool isHorizLimit = false;
        isHorizLimit = (horizontalScrollBar()->value() == horizontalScrollBar()->minimum()) ? true : isHorizLimit;
        isHorizLimit = (horizontalScrollBar()->value() == horizontalScrollBar()->maximum()) ? true : isHorizLimit;

        bool isVertLimit = false;
        isVertLimit = (verticalScrollBar()->value() == verticalScrollBar()->minimum()) ? true : isVertLimit;
        isVertLimit = (verticalScrollBar()->value() == verticalScrollBar()->maximum()) ? true : isVertLimit;

        if((deltaX != 0 && !isHorizLimit) || (deltaY != 0 && !isVertLimit))
            emit scrollBarChanged(deltaX, deltaY);
    }

    QGraphicsView::mouseMoveEvent(event);
}

void PageView::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);
}

void PageView::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() && Qt::CTRL)
        zoom(event->delta());
    else
        QGraphicsView::wheelEvent(event);
}

void PageView::zoomIn()
{
    setZoomLevel((transform().m11()*100) + 5);
    emit zoomLevelChanged(transform().m11()*100);
}

void PageView::zoomOut()
{
    setZoomLevel((transform().m11()*100) - 5);
    emit zoomLevelChanged(transform().m11()*100);
}

void PageView::zoom(int mouseDelta)
{
    double scroll = mouseDelta / 120;
    int delta = 5 * scroll;
    setZoomLevel((transform().m11()*100) + delta);
    emit zoomLevelChanged(transform().m11()*100);
}

void PageView::setZoomLevel(int percent)
{
    qreal pcent = percent / 100.0;
    if(pcent <= 0)
        pcent = 0.01;
    qreal diff = pcent / transform().m11();
    scale(diff, diff);
    mZoomPercent = percent;
}
