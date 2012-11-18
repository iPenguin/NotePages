include(DocbookGen)

configure_file(${CMAKE_SOURCE_DIR}/docs/index.docbook.in
            ${CMAKE_CURRENT_BINARY_DIR}/index.docbook)

set(docbookFile "${CMAKE_CURRENT_BINARY_DIR}/index.docbook")

message(STATUS "Createing documentation: html")
docbook_generate(html "${docbookFile}" "${NP_VERSION_SHORT}")
message(STATUS "Createing documentation: pdf")
docbook_generate(pdf "${docbookFile}" "${NP_VERSION_SHORT}")


if(APPLE)
    include("apple.cmake")
elseif(WIN32)
    include("windows.cmake")
else()
    include("linux.cmake")
endif()