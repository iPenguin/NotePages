Note Pages
===========

#### A notes and cross reference application ####


Note Pages organizes your notes and pictures for you. It makes it easy for you to keep your files together with your notes, and link between information stored on differnet pages.

This can be especially helpful for writers who have to keep track characters, locations, and objects. Or students who have to keep track of research information for classes or papers.

Note Pages is written in C++/Qt and supports the following platforms:

* Windows XP, Vista, 7
* Mac OS X 10.5 -10.7
* Linux

Use Cases
----------

* You're working on a research project, you have taken notes and placed them into text blocks inside Note Pages, you scan in some pictures and text from a book and you drag the files onto the Page with your other notes. Those files are stored with your notes, and can be opened just by clicking on the icons in Note Pages.


* You're writing a book, so you create a nested set of Pages for the plot, characters, and locations. Inside each section you link between related items, so when you're writing you can just click and navigate back and forth between items to refresh your memory about the connections between items.

* And many more!

Building Note Pages
--------------------

To build Note Pages from source:

clone the repo to a folder called "notePages".

    $ cd notePages
    
    $ mkdir build
    
    $ cd build
    
    $ cmake ../
    
    $ make
    
    $ ./src/Note\ Pages

Documentation
--------------

You can build the documentation from the docbooks source files into the proper pdf, html files, etc. as needed.

Use the NP_DOCS flag to turn on/off the document creation process:

    $ cmake ../ -DNP_DOCS=ON

To convert the documentation to the final output you will need to install the docbook-xsl-ns (<http://sourceforge.net/projects/docbook/files/docbook-xsl-ns/>) fop (<http://xmlgraphics.apache.org/fop/download.html>)

On Ubuntu: 
    
    $ sudo apt-get install xsltproc docbook-xsl-ns fop libservlet2.4-java

Making Installers
------------------

You can build the packages/installers for the platform you're working on.
First build the software (see Building above) including the Documentation (see Documentation above). Then run the following command from the build directory:

    $ cpack 

Notes: On Windows you need to install the NSIS installer (<http://nsis.sourceforge.net/Download>) before running the above command.

On Linux the currently supported package formats are: 

* deb

On Mac this process will create a dmg package.

