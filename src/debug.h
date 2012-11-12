/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>

#define debug(message) \
( \
    (qDebug() << Q_FUNC_INFO << ":" << message), \
    (void)0 \
)

#define warn(message) \
( \
    (qWarning() << Q_FUNC_INFO << ":" << message), \
    (void)0 \
)

#define critical(message) \
( \
    (qCritical() << Q_FUNC_INFO << ":" << message), \
    (void)0 \
)

#define fatal(message) \
( \
    (qFatal() << Q_FUNC_INFO << ":" << message), \
    (void)0 \
)

#endif // DEBUG_H
