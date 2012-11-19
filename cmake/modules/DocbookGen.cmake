#
# Copyright (c) 2012 Brian C. Milco <bcmilco AT gmail DOT com>
#
#    This program is free software; you can redistribute it and/or
#    modify it under the terms of the GNU General Public License
#    as published by the Free Software Foundation; version 2
#    of the License.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
# This file contains functions to generate documentaion from docbook files.
# TODO: make sure that docbook-xsl-ns package is installed
#
# OUTPUT FORMATS:
# html: generic html with a folder of images.
# pdf: a PDF
# pages: Mac specific html help pages
# htmlhelp: MS Windows specific html help pages

MACRO(MAKE_WINDOWS_PATH pathname)
  # An extra \\ escape is necessary to get a \ through CMake's processing.
  STRING(REPLACE "/" "\\" ${pathname} "${${pathname}}")
  # Enclose with UNESCAPED quotes.  This means we need to escape our
  # quotes once here, i.e. with \"
  SET(${pathname} ${${pathname}})
ENDMACRO(MAKE_WINDOWS_PATH)

function(DOCBOOK_GENERATE format input version)

    set(working "${CMAKE_CURRENT_BINARY_DIR}/${format}")
    make_directory(${working})

    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/images DESTINATION ${working})

    set(xsltproc "/usr/bin/xsltproc")
    set(docbookBasePath "/usr/share/xml/docbook/stylesheet/docbook-xsl-ns")
    
    if(APPLE)
        set(fop "/opt/fop/fop")
    elseif(WIN32)
        set(xsltproc "xsltproc")
        set(fop "fop.cmd")
        #set(docbookBasePath "/usr/share/xml/docbook/stylesheet/docbook-xsl-ns")
        set(docbookBasePath "C:/cygwin/usr/share/xml/docbook/stylesheet/docbook-xsl-ns")
        set(hhc "hhc")
    else()
        set(fop "/usr/bin/fop")
    endif()

    set(DOCBOOK_XSL_NS_PATH ${docbookBasePath})

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/notePages.xsl.in
                ${CMAKE_CURRENT_BINARY_DIR}/notePages.xsl)

    if(format STREQUAL "html")
            set(xslFile "${docbookBasePath}/html/docbook.xsl")

            #TODO: include a custom CSS Style sheet:
            #file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/notePages.css DESTINATION ${working})
            #--stringparam html.stylesheet notePages.css

            execute_process(
                COMMAND "${xsltproc}" --xinclude -o "${working}/index.html" "${xslFile}" "${input}"
                OUTPUT_VARIABLE _output)
        
    elseif(format STREQUAL "pdf")

            set(outputBaseName "${working}/${EXE_NAME}_User_Guide_${version}")

            set(xslFile "${CMAKE_CURRENT_BINARY_DIR}/notePages.xsl")

            execute_process(
                COMMAND "${xsltproc}" -o "${outputBaseName}.fo" --stringparam fop1.extensions 1 "${xslFile}" "${input}"
                OUTPUT_VARIABLE _output)

            execute_process(
                COMMAND "${fop}" -fo "${outputBaseName}.fo" -pdf "${outputBaseName}.pdf"
                OUTPUT_VARIABLE _output)

    elseif(format STREQUAL "pages")
            set(xslFile "${docbookBasePath}/roundtrip/dbk2pages.xsl")
            set(outputFile "${working}/index.xml")

            execute_process(
                COMMAND "${xsltproc}" -o "${outputFile}" --stringparam pages.template template-pages.xml ${xslFile} "${input}"
                OUTPUT_VARIABLE _output)
            
    elseif(format STREQUAL "htmlhelp")
            set(xslFile "${docbookBasePath}/htmlhelp/htmlhelp.xsl")
            
            execute_process(
                COMMAND "${xsltproc}" "${xslFile}" "${input}"
                WORKING_DIRECTORY "${working}"
                OUTPUT_VARIABLE _output)
           
            #hhc requires a Windows path.
            MAKE_WINDOWS_PATH(working)
            execute_process(
                COMMAND "${hhc}" "${working}\\htmlhelp.hhp"
                WORKING_DIRECTORY "${working}"
                OUTPUT_VARIABLE _output)
            file(RENAME "${working}\\htmlhelp.chm" "${working}\\${EXE_NAME}.chm")
    else()
        message ( FATAL_ERROR "${format} is not supported by this cmake module. Please choose from html, pdf, pages, or htmlhelp" )

    endif()

    set(${outList} ${${outList}} PARENT_SCOPE)

endfunction()
