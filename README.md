Note Pages
===========

Note Pages organizes your notes and images for you. It makes it easy for you to keep your files together with your notes, and link between information stored on different Pages.

Note Pages is written in C++/Qt and supports the following platforms:

* Windows XP, Vista, 7
* Mac OS X 10.5 -10.7
* Linux

Use Cases
----------

* You're working on a research project, you have taken notes and placed them into blocks inside Note Pages, you scan in some pictures and text from a book and you drag the files onto the Page with your other notes. Those files are copied into the page with your notes, and can be opened just by clicking on the icons in Note Pages.


* You're writing a book, so you create a nested set of Pages for the plot, characters, and locations. Inside each section you link between related items, so when you're writing you can just click and navigate back and forth between items to refresh your memory about details and the connections between items.

* And many more!

Building Note Pages from Source
--------------------------------

### Requirements for Building ###
To build Note Pages you will need the following software installed on your computer.

*   To build the application
    *    cmake (<http://cmake.org/cmake/resources/software.html>)
    *    Qt (<http://qt-project.org/downloads>)

*   To generate the documentation
    *    docbook-xsl-ns (<http://sourceforge.net/projects/docbook/files/docbook-xsl-ns/>)
    *    xsltproc
    *    fop (<http://xmlgraphics.apache.org/fop/download.html>)

*   To create an installer for Windows
    *   NSIS installer (<http://nsis.sourceforge.net/Download>)

On Ubuntu run the following command to make sure you have installed all the programs you will need to build Note Pages and it's documentation:

    $ sudo apt-get install build-essential cmake fop xsltproc docbook-xsl-ns libservlet2.4-java

### Compiling Note Pages ###

Clone the repo to a folder called "notePages". On Windows use a cygwin cmd shell, on Linux and Mac you can use a native shell.

    $ cd notePages
    
    $ mkdir build
    
    $ cd build

On Linux and Mac:

    $ cmake ../
    $ make
    
Make sure the PATH contains the folders containing the exe files for the tools you're going to use.
Right click on "My Computer" and select Properties. 
On the "Advanced" Tab click on "Environmental Variables" and edit the PATH system variable to include the appropriate paths.
On Windows in a native cmd shell:

    $ cmake ../ -G "MinGW Makefiles"
    $ mingw32-make
    
On Linux run ./src/notepages

On Mac run "Note Pages" under the src/ folder

On Windows run "Note Pages.exe" under the src/ folder


### Generating the Documentation ###

You can build the documentation from the docbooks source files into the proper pdf, html files, etc. as needed.

Use the NP_DOCS flag to turn on/off the documentation creation process:

    $ cmake ../ -DNP_DOCS=ON

### Creating an Installer ###

You can build the packages/installers for the platform you're working on by completing the following steps. On Linux this will create a deb file, on Windows an NSIS install wizard, and on Mac a dmg file.

First build the software (see Building above) including the Documentation (see Documentation above). Then run the following command from the build directory:

Linux/Mac OS X:

    $ ../bin/release

Windows:

In a cygwin cmd window run:
    
    $ cd notePages/build/
    $ cmake ../ -DCMAKE_BUILD_TYPE=Release -DNP_DOCS=ON
    $ make

In a Windows cmd shell run:

    $ cpack -G "MinGW Makefiles"

