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
/*!
 * \file pageglobals.h
 * Some basic information that is shared by all the content types and the page.
 */
#ifndef PAGEGLOBALS_H
#define PAGEGLOBALS_H

#include <qnamespace.h>

/*!
 * \namespace NoteType
 * The NoteType namespace is used to define the types of notes that can exist on a Page,
 * using the Id.
 */
namespace NoteType
{
    enum Id { Text = Qt::UserRole + 10, Image, Document, Unknown = Qt::UserRole + 255 };
}

#endif // PAGEGLOBALS_H
