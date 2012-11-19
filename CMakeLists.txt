cmake_minimum_required(VERSION 2.8)

if(APPLE)
    project("Note Pages")

    set(EXE_NAME ${PROJECT_NAME})

    set(build_flags "-g -gdwarf-2 -W -Wall")
    set(build_flags_debug "-g -gdwarf-2 -W -Wall")
    set(build_flags_release "-gdwarf-2 -W -Wall -O2")
    set(build_flags_profile "-g -gdwarf-2 -pg -W -Wall")
elseif(WIN32)
    project("Note Pages")

    set(EXE_NAME "Note_Pages")

    set(build_flags "-g -ggdb -W -Wall")
    set(build_flags_debug "-g -ggdb -W -Wall")
    set(build_flags_release "-ggdb -W -Wall -O2 -s")
    set(build_flags_profile "-g -ggdb -pg -W -Wall")
else()
    project("notepages")

    set(EXE_NAME ${PROJECT_NAME})

    set(build_flags "-g -ggdb -W -Wall")
    set(build_flags_debug "-g -ggdb -W -Wall")
    set(build_flags_release "-ggdb -W -Wall -O2 -s")
    set(build_flags_profile "-g -ggdb -pg -W -Wall")
endif()


# Appends the cmake/modules path to MAKE_MODULE_PATH variable...
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

#Forces out of source builds just in case.
include(MacroOutOfSourceBuild)
macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build.")

set(CMAKE_CONFIGURATION_TYPES Debug Release Profile CACHE TYPE INTERNAL FORCE)
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set (HUNSPELL_FIND_QUIETLY TRUE)
include(FindHunSpell)

################################################################################################
# Make a version file containing the current version pulled from git.
#
include(GetGitRevisionDescription)
git_describe(VERSION --tags --dirty=-dirty)

string(REGEX REPLACE "^v([0-9]+)\\..*" "\\1" VERSION_MAJOR "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" VERSION_MINOR "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" VERSION_PATCH "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.[0-9]+(.*)" "\\1" VERSION_SHA1 "${VERSION}")
set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/version.cpp.in
                ${CMAKE_CURRENT_BINARY_DIR}/version.cpp)
set(np_version "${CMAKE_CURRENT_BINARY_DIR}/version.cpp")
################################################################################################

find_package(Qt4 REQUIRED)
#find_package(Qt4 COMPONENTS QTCORE QTGUI)

option(UNIT_TESTING  "Build the test libraries for this software" OFF)
option(FORCE_32BIT "Force 32-bit builds when cross compiling with distcc (needed for 64-bit compile nodes)" OFF)
option(DOCS "Create docbook documentation" OFF)
option(DOXYGEN "Create Doxygen documentation" OFF)
option(APP_STORE "Create an Apple App Store version - WIP" OFF)
set(CMAKE_OSX_ARCHITECTURES "x86_64") #"i386;x86_64")

message("-------------------------------------------------------")
message(STATUS ${PROJECT_NAME} ": " ${VERSION})
message(STATUS "CMake Build Type: " ${CMAKE_BUILD_TYPE})
message(STATUS "Docs:             " ${DOCS})
message(STATUS "Force 32bit:      " ${FORCE_32BIT})
message(STATUS "App Store:        " ${APP_STORE})
message(STATUS "Unit Testing:     " ${UNIT_TESTING})
message(STATUS "Hunspell:         " ${HUNSPELL_FOUND})
message(STATUS "Doxygen:          " ${DOXYGEN})
message("-------------------------------------------------------")

if(FORCE_32BIT)
    set(extra_build_flags "-m32")
    set(extra_link_flags "-m elf_i386")
endif()

set(CMAKE_C_FLAGS "${build_flags} ${extra_build_flags}") #-ansi -pedantic
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})
set(CMAKE_EXE_LINKER_FLAGS "${extra_link_flags} ${CMAKE_EXE_LINKER_FLAGS}")

set(CMAKE_C_FLAGS_DEBUG "${build_flags_debug} ${extra_build_flags}")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${extra_link_flags} ${CMAKE_EXE_LINKER_FLAGS}")

set(CMAKE_C_FLAGS_RELEASE "${build_flags_release} ${extra_build_flags}")
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${extra_link_flags} ${CMAKE_EXE_LINKER_FLAGS}")

set(CMAKE_C_FLAGS_PROFILE "${build_flags_profile} ${extra_build_flags}")
set(CMAKE_CXX_FLAGS_PROFILE ${CMAKE_C_FLAGS_PROFILE})
set(CMAKE_EXE_LINKER_FLAGS_PROFILE "-pg ${extra_link_flags} ${CMAKE_EXE_LINKER_FLAGS}")

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

#More info see: http://cmake.org/cmake/help/cmake2.6docs.html#module:FindQt4

set(QT_USE_QTMAIN true)
set(QT_USE_QTSVG true)
set(QT_USE_QTNETWORK true)
include(${QT_USE_FILE})

#QT4_CREATE_TRANSLATION( qm_files directories ... sources ... ts_files ... OPTIONS ...)
#QT4_ADD_TRANSLATION( qm_files ts_files ... )

add_definitions(${QT_DEFINITIONS})

###################################################################################
#CPack configuration
###################################################################################
include(InstallRequiredSystemLibraries)


set(PROJECT_DESCRIPTION  "A notes and cross reference application")
set(PROJECT_VENDOR       "Brian Milco")
set(PROJECT_LIFE         "2012")
set(ORG_BASE_URL         "notePages.org")
set(PROJECT_CONTACT      "support@${ORG_BASE_URL}")
set(ORG_CONTACT          "http://${ORG_BASE_URL}/contact")
set(ORG_DOWNLOAD         "http://${ORG_BASE_URL}/downloads")
set(ORG_WEBSITE          "www.${ORG_BASE_URL}")
set(PROJECT_VERSION      "${VERSION_SHORT}")
set(PROJECT_COPYRIGHT    "Copyright (c) ${PROJECT_LIFE} ${PROJECT_VENDOR}")
set(PROJECT_MACOSX_ICON  "${PROJECT_NAME}.icns")


set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR ${PROJECT_VENDOR})
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT ${PROJECT_CONTACT})
set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
#set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/images/")  # for the mounted disk image icon.

set(plugin_dest_dir bin)
set(qtconf_dest_dir bin)
set(APPS "@CMAKE_INSTALL_PREFIX@/bin/${PROJECT_NAME}")

if(APPLE)
    include("apple.cmake")
elseif(WIN32)
    include("windows.cmake")
else()
    include("linux.cmake")
endif()

#set(CPACK_BINARY_DRAGNDROP ON)
include(CPack)

###################################################################################
# end cpack configuration
###################################################################################

#FIXME: use the FindDoxygen.cmake module.
if(DOXYGEN)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/Doxyfile.in
                ${CMAKE_BINARY_DIR}/Doxyfile)
    execute_process(
                COMMAND "/usr/bin/doxygen" "${CMAKE_BINARY_DIR}/Doxyfile"
                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
                OUTPUT_VARIABLE _output)
endif()

add_subdirectory(src)

if(DOCS)
    add_subdirectory(docs)
endif()

if(UNIT_TESTING)
    add_subdirectory(tests)
endif()
