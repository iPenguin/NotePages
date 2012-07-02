include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB np_srcs "*.cpp")
file(GLOB np_uis "*.ui")

set(np_qobject_headers  mainwindow.h
                        page.h
                        pagescene.h
                        settings.h
                        settingsui.h
                        pageview.h
                        macmenubar.h
                        notetext.h
                        note.h
                        linkdialog.h
                        )

set(np_resources
    ../notePages.qrc
    )

qt4_add_resources(np_rcc_srcs ${np_resources})
qt4_wrap_ui(np_ui_h ${np_uis})
qt4_wrap_cpp(np_moc_srcs ${np_qobject_headers})

if(WIN32)
    #generate the embeded icon for the exe file.
    set(CMAKE_RC_COMPILER_INIT windres)
    enable_language(RC)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
    set(np_win "${CMAKE_SOURCE_DIR}/notePages.rc")

    set(QT_WIN_LIBS "C://Qt//4.7.4//bin")
    
    set(QT_LIBS_WINDOWS "${QT_WIN_LIBS}//QtSvg4.dll" "${QT_WIN_LIBS}//QtCore4.dll" "${QT_WIN_LIBS}//QtGui4.dll"
                        "${QT_WIN_LIBS}//QtNetwork4.dll" "${QT_WIN_LIBS}//QtXml4.dll")
    set(QT_DEPS_WINDOWS "${QT_WIN_LIBS}//libgcc_s_dw2-1.dll" "${QT_WIN_LIBS}//mingwm10.dll"
                        "${CMAKE_SOURCE_DIR}/resources/qt.conf")

    set(QT_PLUGINS_WINDOWS "${QT_PLUGINS_DIR}/imageformats" "${QT_PLUGINS_DIR}/accessible" "${QT_PLUGINS_DIR}/iconengines")

else()

    set(np_nix  )

endif()

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(${PROJECT_NAME} ${np_srcs} ${np_ui_h} ${np_moc_srcs} ${np_rcc_srcs}
            ${np_version} ${np_win} ${np_mac} ${np_nix})
else()
    add_executable(${PROJECT_NAME} WIN32 ${np_srcs} ${np_ui_h} ${np_moc_srcs} ${np_rcc_srcs}
            ${np_version} ${np_win} ${np_mac} ${np_nix})
endif()

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})

if(APPLE)
    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} DESTINATION ../MacOS)

    install(CODE "
        execute_process(
            COMMAND \"/usr/bin/macdeployqt\"
            \"@CMAKE_BINARY_DIR@/_CPack_Packages/Darwin/Bundle/@PROJECT_NAME@-@NP_VERSION_SHORT@/@PROJECT_NAME@.app/\"
            WORKING_DIRECTORY \"@CMAKE_BINARY_DIR@\"
            OUTPUT_VARIABLE _output)
    " COMPONENT Runtime)
    
    install(CODE "
        execute_process(
            COMMAND \"@CMAKE_SOURCE_DIR@/bin/sign_app @CMAKE_BINARY_DIR@/_CPack_Packages/Darwin/Bundle/@PROJECT_NAME@-@NP_VERSION_SHORT@/@NP_PROJECT_NAME@.app/\"
            OUTPUT_VARIABLE _output)
    " COMPONENT Runtime)

    if(NP_APP)
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
