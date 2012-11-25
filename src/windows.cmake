#generate the embeded icon for the exe file.
set(CMAKE_RC_COMPILER_INIT windres)
enable_language(RC)
set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
set(np_win "${CMAKE_SOURCE_DIR}/notePages.rc")

set(QT_WIN_LIBS "C://qtsdk//Desktop//Qt//4.8.1//mingw//bin")
set(MINGW_LIBS "C://QtSDK//mingw//bin")

set(QT_LIBS_WINDOWS "${QT_WIN_LIBS}//QtSvg4.dll" "${QT_WIN_LIBS}//QtCore4.dll" "${QT_WIN_LIBS}//QtGui4.dll"
                    "${QT_WIN_LIBS}//QtNetwork4.dll" "${QT_WIN_LIBS}//QtXml4.dll")
set(QT_DEPS_WINDOWS "${MINGW_LIBS}//libgcc_s_dw2-1.dll" "${MINGW_LIBS}//mingwm10.dll"
                    "${CMAKE_SOURCE_DIR}/resources/qt.conf")

set(QT_PLUGINS_WINDOWS "${QT_PLUGINS_DIR}/imageformats" "${QT_PLUGINS_DIR}/accessible" "${QT_PLUGINS_DIR}/iconengines")


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(${EXE_NAME} ${np_srcs} ${np_ui_h} ${np_moc_srcs} ${np_rcc_srcs}
            ${version_file} ${np_win})
else()
    add_executable(${EXE_NAME} WIN32 ${np_srcs} ${np_ui_h} ${np_moc_srcs} ${np_rcc_srcs}
            ${version_file} ${np_win})
endif()

target_link_libraries(${EXE_NAME} ${QT_LIBRARIES})

install(TARGETS ${EXE_NAME} BUNDLE DESTINATION . COMPONENT Runtime RUNTIME DESTINATION bin COMPONENT Runtime)
    
set_property(TARGET ${EXE_NAME} PROPERTY OUTPUT_NAME "${PROJECT_NAME}")

install(PROGRAMS ${QT_LIBS_WINDOWS} ${QT_DEPS_WINDOWS} DESTINATION bin)
install(DIRECTORY ${QT_PLUGINS_WINDOWS} DESTINATION plugins)