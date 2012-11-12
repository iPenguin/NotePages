#include "noteimage.h"

#include "pagescene.h"
#include <QImageReader>

NoteImage::NoteImage(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPixmapItem(parent, scene), NoteContent(parent, scene),
      mOldSize(QSizeF()),
      mDiff(QPointF())
{
    setupContextMenu();
}

QRectF NoteImage::boundingRect() const
{
    return QGraphicsPixmapItem::boundingRect();
}

void NoteImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);
    //painter->setPen(Qt::gray);
    //painter->drawRoundedRect(boundingRect(), 5, 5);
}

QSizeF NoteImage::size()
{
    return boundingRect().size();
}

void NoteImage::setSize(QSizeF size)
{
    setPixmap(QPixmap(pageScene()->pagePath() + "/" + file()).scaled(size.toSize()));
}

void NoteImage::setPos(const QPointF &pos)
{
    return QGraphicsPixmapItem::setPos(pos);
}

void NoteImage::setFile(QString f)
{
    NoteContent::setFile(f);
    QImageReader *ireader = new QImageReader(pageScene()->pagePath() + "/" + f);
    setSize(ireader->size());
}

void NoteImage::setImage(QByteArray imageData)
{
    pixmap().loadFromData(imageData);
}

void NoteImage::loadContent(QXmlStreamReader *stream)
{
    setFile(stream->attributes().value("file").toString());
}

void NoteImage::saveContent(QXmlStreamWriter *stream)
{
    stream->writeAttribute("file", file());
}

void NoteImage::deleteContent()
{

    //TODO: prompt to delete any images.
    QString image = file();
    if(QFileInfo(pageScene()->pagePath() + "/" + file()).exists()) {
        QDir d(pageScene()->pagePath());
        d.remove(image);
    }
}

void NoteImage::setupContextMenu()
{

    mContextMenu = new QMenu();
    QAction *delNote = new QAction(QObject::tr("Delete Note"), 0);

    QObject::connect(delNote, SIGNAL(triggered()), pageScene(), SLOT(deleteNote()));

    mContextMenu->addSeparator();
    mContextMenu->addAction(delNote);
}

void NoteImage::mousePressEvent(QGraphicsSceneMouseEvent *e)
{

    mOldSize = pixmap().size();
    QGraphicsPixmapItem::mousePressEvent(e);
}

void NoteImage::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    mDiff = (e->scenePos() - e->buttonDownScenePos(Qt::LeftButton));
    QSizeF newSize = QSizeF(mOldSize.width() + mDiff.x(), mOldSize.height() + mDiff.y());

    qreal ratio = qreal(pixmap().size().width()) / qreal(pixmap().size().height());
    newSize.setWidth(ratio * newSize.height());
    qDebug() << "new size" << newSize;
    setPixmap(pixmap().scaled(newSize.toSize()));

    QGraphicsPixmapItem::mouseMoveEvent(e);
}

void NoteImage::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    mDiff = QPointF(0,0);
    update();
    mOldSize = QSizeF();
    QGraphicsPixmapItem::mouseReleaseEvent(e);
}
