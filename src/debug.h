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

#include <QString>
#include <QDebug>

QString debugFunctionName(QString name);

/*!
 * \def debug(message)
 * Outputs a debug message showing the calling function and \a message.
 */
#define debug(message) \
( \
    (qDebug() << debugFunctionName(Q_FUNC_INFO) << ":" << message), \
    (void)0 \
)

/*!
 * \def warn(message)
 * Outputs a warning message showing the calling function and \a message.
 */
#define warn(message) \
( \
    (qWarning() << debugFunctionName(Q_FUNC_INFO) << ":" << message), \
    (void)0 \
)

/*!
 * \def critical(message)
 * Outputs a critical message showing the calling function and \a message.
 */
#define critical(message) \
( \
    (qCritical() << debugFunctionName(Q_FUNC_INFO) << ":" << message), \
    (void)0 \
)

/*!
 * \def fatal(message)
 * Outputs a fatal message showing the calling function and \a message and then core dump.
 */
#define fatal(message) \
( \
    (qFatal("%s : %s", debugFunctionName(Q_FUNC_INFO).toStdString().c_str(), QString(message).toStdString().c_str())), \
    (void)0 \
)

#endif // DEBUG_H
