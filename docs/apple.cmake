
message(STATUS "Createing documentation: pages")
    docbook_generate(pages "${docbookFile}" "${VERSION_SHORT}")
    
install(FILES "${CMAKE_BINARY_DIR}/docs/pdf/${EXE_NAME}_User_Guide_${VERSION_SHORT}.pdf" DESTINATION ../MacOS)
install(CODE "
    file(COPY \"@CMAKE_BINARY_DIR@/docs/pdf/@PROJECT_NAME@_User_Guide_@VERSION_SHORT@.pdf\"
            DESTINATION \"@CMAKE_BINARY_DIR@/_CPack_Packages/Darwin/Bundle/@PROJECT_NAME@ v@VERSION_SHORT@\")
    file(RENAME \"@CMAKE_BINARY_DIR@/_CPack_Packages/Darwin/Bundle/@PROJECT_NAME@ v@VERSION_SHORT@/@PROJECT_NAME@_User_Guide_@VERSION_SHORT@.pdf\"
    \"@CMAKE_BINARY_DIR@/_CPack_Packages/Darwin/Bundle/@PROJECT_NAME@ v@VERSION_SHORT@/User Guide.pdf\")
    " COMPONENT Runtime)
