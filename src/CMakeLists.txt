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
                        notecontent.h
                        updater.h
                        )

set(np_resources
    ../notePages.qrc
    )

qt4_add_resources(np_rcc_srcs ${np_resources})
qt4_wrap_ui(np_ui_h ${np_uis})
qt4_wrap_cpp(np_moc_srcs ${np_qobject_headers})

if(APPLE)
    include("apple.cmake")
elseif(WIN32)
    include("windows.cmake")
else()
    include("linux.cmake")
endif()