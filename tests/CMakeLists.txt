include_directories(${QT_INCLUDES} ${CMAKE_BINARY_DIR}/src ${CMAKE_SOURCE_DIR}/src
                    ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(QT_USE_QTMAIN true)
set(QT_USE_QTTEST true)

include(${QT_USE_FILE})

file(GLOB notepages_test_srcs "test*.cpp")
file(GLOB notepages_test_qobject_headers "test*.h")

set(notepages_app_h
    ../src/settings.h
    )

set(notepages_app_cpp
    ../src/settings.cpp
    ../src/version.h
    ${CMAKE_BINARY_DIR}/version.cpp
    )

set(notepages_app_resources
    )

qt4_add_resources(notepages_app_rcc_srcs ${notepages_app_resources})
qt4_wrap_cpp(notepages_test_moc_srcs ${notepages_test_qobject_headers} ${notepages_app_h})

add_executable(tests main.cpp ${notepages_test_srcs} ${notepages_test_moc_srcs} ${notepages_app_cpp} ${notepages_app_rcc_srcs})
target_link_libraries(tests ${QT_LIBRARIES})
