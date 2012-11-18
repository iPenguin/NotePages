
message(STATUS "Createing documentation: htmlhelp")
#docbook_generate(htmlhelp "${docbookFile}" "${NP_VERSION_SHORT}")

install(FILES "${CMAKE_BINARY_DIR}/docs/pdf/${EXE_NAME}_User_Guide_${NP_VERSION_SHORT}.pdf" DESTINATION bin)
#install(FILES "${CMAKE_BINARY_DIR}/docs/htmlhelp/${EXE_NAME}.chm" DESTINATION docs)
install(FILES "${CMAKE_SOURCE_DIR}/docs/homepage.html" DESTINATION docs)