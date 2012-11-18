
message(STATUS "Createing documentation: pages")
    docbook_generate(pages "${docbookFile}" "${NP_VERSION_SHORT}")
    
install(FILES "${CMAKE_BINARY_DIR}/docs/pdf/${EXE_NAME}_User_Guide_${NP_VERSION_SHORT}.pdf" DESTINATION ../MacOS)