/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "notecontent.h"

#include "note.h"

NoteContent::NoteContent(QGraphicsItem *parent, QGraphicsScene *scene)
{
    mScene = qobject_cast<PageScene*>(scene);
    mNote = qgraphicsitem_cast<Note*>(parent);

}

