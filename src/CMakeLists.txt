include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB dw_srcs "*.cpp")
file(GLOB dw_uis "*.ui")

set(dw_qobject_headers  mainwindow.h
                        page.h
                        pagescene.h
                        settings.h
                        settingsui.h
                        )

set(dw_resources
    ../desktopWiki.qrc
    )

qt4_add_resources(dw_rcc_srcs ${dw_resources})
qt4_wrap_ui(dw_ui_h ${dw_uis})
qt4_wrap_cpp(dw_moc_srcs ${dw_qobject_headers})

if(WIN32)
    #generate the embeded icon for the exe file.
    set(CMAKE_RC_COMPILER_INIT windres)
    enable_language(RC)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
    set(dw_win "${CMAKE_SOURCE_DIR}/desktopWiki.rc")

    set(QT_WIN_LIBS "C://Qt//4.7.4//bin")
    
    set(QT_LIBS_WINDOWS "${QT_WIN_LIBS}//QtSvg4.dll" "${QT_WIN_LIBS}//QtCore4.dll" "${QT_WIN_LIBS}//QtGui4.dll"
                        "${QT_WIN_LIBS}//QtNetwork4.dll" "${QT_WIN_LIBS}//QtXml4.dll")
    set(QT_DEPS_WINDOWS "${QT_WIN_LIBS}//libgcc_s_dw2-1.dll" "${QT_WIN_LIBS}//mingwm10.dll"
                        "${CMAKE_SOURCE_DIR}/resources/qt.conf")

    set(QT_PLUGINS_WINDOWS "${QT_PLUGINS_DIR}/imageformats" "${QT_PLUGINS_DIR}/accessible" "${QT_PLUGINS_DIR}/iconengines")

else()

    set(dw_nix  )

endif()

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(${PROJECT_NAME} ${dw_srcs} ${dw_ui_h} ${dw_moc_srcs} ${dw_rcc_srcs}
            ${dw_version} ${dw_win} ${dw_mac} ${dw_nix})
else()
    add_executable(${PROJECT_NAME} WIN32 ${dw_srcs} ${dw_ui_h} ${dw_moc_srcs} ${dw_rcc_srcs}
            ${dw_version} ${dw_win} ${dw_mac} ${dw_nix})
endif()

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})

if(APPLE)
    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} DESTINATION ../MacOS)

    install(CODE "
        execute_process(
            COMMAND \"/usr/bin/macdeployqt\"
            \"@CMAKE_BINARY_DIR@/_CPack_Packages/Darwin/Bundle/@PROJECT_NAME@-@DW_VERSION_SHORT@/@PROJECT_NAME@.app/\"
            WORKING_DIRECTORY \"@CMAKE_BINARY_DIR@\"
            OUTPUT_VARIABLE _output)
    " COMPONENT Runtime)
    
    install(CODE "
        execute_process(
            COMMAND \"@CMAKE_SOURCE_DIR@/bin/sign_app @CMAKE_BINARY_DIR@/_CPack_Packages/Darwin/Bundle/@PROJECT_NAME@-@DW_VERSION_SHORT@/@DW_PROJECT_NAME@.app/\"
            OUTPUT_VARIABLE _output)
    " COMPONENT Runtime)

    if(DW_APP)
        set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "APPLE_APP_STORE=1")
    endif()

else()
    install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION . COMPONENT Runtime RUNTIME DESTINATION bin COMPONENT Runtime)
endif()

if(WIN32)
    install(PROGRAMS ${QT_LIBS_WINDOWS} ${QT_DEPS_WINDOWS} DESTINATION bin)
    install(DIRECTORY ${QT_PLUGINS_WINDOWS} DESTINATION plugins)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

endif()