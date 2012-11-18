project("Note Pages")

set(EXE_NAME ${PROJECT_NAME})

set(build_flags "-g -gdwarf-2 -W -Wall")
set(build_flags_debug "-g -gdwarf-2 -W -Wall")
set(build_flags_release "-gdwarf-2 -W -Wall -O2")
set(build_flags_profile "-g -gdwarf-2 -pg -W -Wall")


###################################################################################
# cpack info
###################################################################################

set(plugin_dest_dir "${PROJECT_NAME}.app/Contents/PlugIns")
set(qtconf_dest_dir "${PROJECT_NAME}.app/Contents/Resources")
set(APPS "@CMAKE_CURRENT_BINARY_DIR@/${PROJECT_NAME}")

set(CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_GENERATOR "Bundle")
set(CPACK_BUNDLE_NAME "${PROJECT_NAME}")
set(CPACK_BUNDLE_PLIST "${CMAKE_BINARY_DIR}/Info.plist")
set(CPACK_BUNDLE_ICON "${CMAKE_SOURCE_DIR}/images/${PROJECT_MACOSX_ICON}")

set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
set(CPACK_DMG_DS_STORE "${CMAKE_SOURCE_DIR}/resources/MacDmgDsStore")
set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_SOURCE_DIR}/images/dmg_background.png")

set(CPACK_OSX_PACKAGE_VERSION "10.6") #min package version

set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} version ${NP_VERSION}")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}")
set(MACOSX_BUNDLE_COPYRIGHT "${PROJECT_COPYRIGHT}. All rights reserved.")

#for more see: http://www.mail-archive.com/cmake@cmake.org/msg05498.html
#and see: http://www.cmake.org/Wiki/CMake:Bundles_And_Frameworks
#plutil command line utility to edit plist files.
#http://rixstep.com/2/20060901,00.shtml
set(MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME} - version ${PROJECT_VERSION}")
set(MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}")
set(MACOSX_BUNDLE_ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/images/${PROJECT_MACOSX_ICON}")
set_source_files_properties("${MACOSX_BUNDLE_ICON_FILE}" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.notePages.notePages")
set(MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}")

configure_file(${CMAKE_SOURCE_DIR}/cmake/modules/MacOSXBundleInfo.plist.in
            ${CMAKE_BINARY_DIR}/Info.plist)

install(CODE "
    set(VERSION_STR \"@NP_VERSION_MAJOR@.@NP_VERSION_MINOR@.@NP_VERSION_PATCH@\")
    file(COPY \"@CMAKE_BINARY_DIR@/docs/pdf/@PROJECT_NAME@_User_Guide_@VERSION_STR@.pdf\" 
            DESTINATION \"@CMAKE_BINARY_DIR@/_CPack_Packages/Darwin/Bundle/@PROJECT_NAME@-@VERSION_STR@\")
    file(RENAME \"@CMAKE_BINARY_DIR@/_CPack_Packages/Darwin/Bundle/@PROJECT_NAME@-@VERSION_STR@/@PROJECT_NAME@_User_Guide_@VERSION_STR@.pdf\" 
    \"@CMAKE_BINARY_DIR@/_CPack_Packages/Darwin/Bundle/@PROJECT_NAME@-@VERSION_STR@/User Guide.pdf\")
    " COMPONENT Runtime)

set(DIRS ${QT_LIBRARY_DIRS})

set(np_mac "${CPACK_BUNDLE_ICON}")

