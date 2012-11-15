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
