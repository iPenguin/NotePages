Note Pages
===========

Note Pages organizes your notes and images for you. It makes it easy for you to keep your files together with your notes, and link between information stored on different Pages.

Note Pages is written in C++/Qt and supports the following platforms:

* Windows XP, Vista, 7
* Mac OS X 10.6+
* Linux

Use Cases
----------

* You're working on a research project, you have taken notes and placed them into blocks inside Note Pages, you scan in some pictures and text from a book and you drag the files onto the Page with your other notes. Those files are copied into the page with your notes, and can be opened just by clicking on the icons in Note Pages.


* You're writing a book, so you create a nested set of Pages for the plot, characters, and locations. Inside each section you link between related items, so when you're writing you can just click and navigate back and forth between items to refresh your memory about details and the connections between items.

* And many more!

Building Note Pages from Source
--------------------------------

I've put together some helpful scripts that can be used in the building process. You can copy the scripts in the bin/ directory into a bin folder in your $PATH (ie. ~/bin) and edit them as appropriate for your environment. Otherwise you can follow the instructions below.

### Requirements for Building ###
To build Note Pages you will need the following software installed on your computer.

*   To build the application
    *    cmake (<http://cmake.org/cmake/resources/software.html>)
    *    QtSDK (<http://qt-project.org/downloads>)
    *    Hunspell (<http://hunspell.sourceforge.net/>) - not implemented yet.

*   To generate the documentation
    *    docbook-xsl-ns (<http://sourceforge.net/projects/docbook/files/docbook-xsl-ns/>)
    *    xsltproc
    *    fop (<http://xmlgraphics.apache.org/fop/download.html>)

*   To create an installer for **Windows**
    *   NSIS installer (<http://nsis.sourceforge.net/Download>)

On Ubuntu run the following command to make sure you have installed all the programs you will need to build Note Pages and the documentation:

    $ sudo apt-get install build-essential cmake fop xsltproc docbook-xsl-ns libservlet2.4-java libhunspell-dev

### Compiling Note Pages ###

Clone the repo to a folder called "notePages".

    $ cd notePages
    
    $ mkdir build
    
    $ cd build

On **Linux** and **Mac**:

    $ cmake ../ [-DDOCS=ON] [-DAPP_STORE=ON] [-DUNIT_TESTING=ON] [-DDOXYGEN=ON] [-DFORECE_32BIT=ON]
    $ make

On **Windows**:
     
    $ cmake ../ -G "MinGW Makefiles"
    $ mingw32-make    

On **Linux** run ./src/notepages. 

On **Mac** run "Note Pages" under the src/ folder. 

On **Windows** run "Note Pages.exe" under the src/ folder.


*    -DDOCS=ON
 
    Generate the documentation from the docbooks source files. Based on the platform you're using different formats will be used.

*    -DAPP_STORE=ON
    
    Experimental and incomplete! Compile and generate any changes needed to make the software compatible with the Mac OS X App Store.

*    -DUNIT_TESTING=ON

    Turn on and build the unit tests.

*    -DDOXYGEN=ON

    Turn on and generate the Doxygen files.

*    -DFORECE_32BIT=ON

    Pass the -m32 and related flags when building the software. Useful if you're building a 32bit version with distcc and you have 64bit nodes.

On **Windows** make sure the PATH contains the folders containing the exe files for the tools you're going to use.
Right click on "My Computer" and select Properties. 
On the "Advanced" Tab click on "Environmental Variables" and edit the PATH system variable to include the appropriate paths.


### Creating an Installer ###

You can build the packages/installers for the platform you're working on by completing the following steps. On **Linux** this will create a deb file, on **Windows** an NSIS install wizard, and on **Mac** a dmg file.

First build the software (see Building above) include the Documentation (using the -DDOCS=ON). Then run the following commands from the build directory:

**Linux**:

    $ cpack

**Mac**:

    $ cpack -G Bundle

**Windows**:

    $ cpack -G NSIS

