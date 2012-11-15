/*********************************************************************************************\
    Note Pages - the note taking and cross reference application 
    Copyright (c) 2012 Brian C. Milco <bcmilco AT gmail DOT com>       

    This file is a part of the test suite for Note Pages.
    
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

//#include "testname.h"
//#include "testname2.h"

int main(int argc, char** argv) 
{

    QApplication app(argc, argv);
    int retval(0);

    QObject* test;
/*
    test = new TestName();
    retval +=QTest::qExec(test, argc, argv);
    delete test;
    test = 0;

    test = new TestName2();
    retval +=QTest::qExec(test, argc, argv);
    delete test;
    test = 0;
*/
    return (retval ? 1 : 0);
}
