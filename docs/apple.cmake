
message(STATUS "Createing documentation: pages")
    docbook_generate(pages "${docbookFile}" "${VERSION_SHORT}")
    
install(FILES "${CMAKE_BINARY_DIR}/docs/pdf/${EXE_NAME}_User_Guide_${VERSION_SHORT}.pdf" DESTINATION ../MacOS)
