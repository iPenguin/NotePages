#include "note.h"

Note::Note(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsItem(parent, scene)
{
}



int Note::contentType()
{
    return 1;
}
