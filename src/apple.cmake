
add_executable(${EXE_NAME} ${np_srcs} ${np_ui_h} ${np_moc_srcs} ${np_rcc_srcs}
            ${np_version} ${np_mac})
            
            
target_link_libraries(${EXE_NAME} ${QT_LIBRARIES})

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} DESTINATION ../MacOS)

install(CODE "
    file(COPY \"@CMAKE_BINARY_DIR@/docs/pdf/@PROJECT_NAME@_User_Guide_@VERSION_SHORT@.pdf\"
            DESTINATION \"@CMAKE_BINARY_DIR@/_CPack_Packages/Darwin/Bundle/@PROJECT_NAME@-@VERSION_SHORT@\")
    file(RENAME \"@CMAKE_BINARY_DIR@/_CPack_Packages/Darwin/Bundle/@PROJECT_NAME@-@VERSION_SHORT@/@PROJECT_NAME@_User_Guide_@VERSION_SHORT@.pdf\"
    \"@CMAKE_BINARY_DIR@/_CPack_Packages/Darwin/Bundle/@PROJECT_NAME@-@VERSION_SHORT@/User Guide.pdf\")
    " COMPONENT Runtime)

install(CODE "
    execute_process(
        COMMAND \"/usr/bin/macdeployqt\"
        \"@CMAKE_BINARY_DIR@/_CPack_Packages/Darwin/Bundle/@PROJECT_NAME@-@VERSION_SHORT@/@PROJECT_NAME@.app/\"
        WORKING_DIRECTORY \"@CMAKE_BINARY_DIR@\"
        OUTPUT_VARIABLE _output)
" COMPONENT Runtime)

install(CODE "
    execute_process(
        COMMAND \"@CMAKE_SOURCE_DIR@/bin/sign_app @CMAKE_BINARY_DIR@/_CPack_Packages/Darwin/Bundle/@PROJECT_NAME@-@VERSION_SHORT@/@PROJECT_NAME@.app/\"
        OUTPUT_VARIABLE _output)
" COMPONENT Runtime)

if(APP_STORE)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "APPLE_APP_STORE=1")
endif()

