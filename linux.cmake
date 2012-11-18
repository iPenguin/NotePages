project("notepages")

set(EXE_NAME ${PROJECT_NAME})

set(build_flags "-g -ggdb -W -Wall")
set(build_flags_debug "-g -ggdb -W -Wall")
set(build_flags_release "-ggdb -W -Wall -O2 -s")
set(build_flags_profile "-g -ggdb -pg -W -Wall")


###################################################################################
# cpack information
###################################################################################
set(CPACK_GENERATOR "DEB") #;RPM;STGZ;TBZ2")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Brian Milco <${PROJECT_CONTACT}>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libqtgui4 (>= 4.7.0), libqtcore4 (>= 4.7.0), libqt4-svg (>= 4.7.0), libqt4-xml (>= 4.7.0), libqt4-network (>= 4.7.0)")
set(CPACK_DEBIAN_PACKAGE_SECTION "Graphics")
set(CPACK_DEBIAN_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION})


set(CPACK_RPM_PACKAGE_LICENSE "Commercial")
set(CPACK_RPM_PACKAGE_GROUP "Applications/Productivity")
set(CPACK_RPM_PACKAGE_VENDOR ${CPACK_PACKAGE_VENDOR})
set(CPACK_RPM_PACKAGE_REQUIRES "requires: libc6, qt >= 4.7, qt-x11 >= 4.7")

if(FORCE_32BIT)
    set(CPACK_RPM_PACKAGE_ARCHITECTURE "i386")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE i386)
else()
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
    set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
endif()
