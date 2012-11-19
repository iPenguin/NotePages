###################################################################################
# cpack info
###################################################################################

set(APPS "@CMAKE_INSTALL_PREFIX@/bin/${PROJECT_NAME}.exe")

#set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/images/installer.bmp")
set(CPACK_GENERATOR "NSIS")
set(CPACK_NSIS_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME}")
set(CPACK_NSIS_CONTACT "${CPACK_PACKAGE_CONTACT}")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}")

set(CPACK_CMAKE_MODULES_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

set(CPACK_NSIS_MENU_LINKS "docs/homepage.html" "Homepage for ${PROJECT_VENDOR}"
                            "bin/${PROJECT_NAME}_User_Guide_${VERSION_SHORT}.pdf" "${PROJECT_NAME} Help")
# this doesn't work for the NSIS installer
set(CPACK_CREATE_DESKTOP_LINKS "${PROJECT_NAME}.exe")

#set(CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk' '\$INSTDIR\\\\${PROJECT_NAME}.exe'"
#                                  "CreateShortCut '\$DESKTOP\\\\${PROJECT_NAME}.lnk' '\$INSTDIR\\\\${PROJECT_NAME}.exe'")
# Icon in the add/remove control panel. Must be an .exe file
set(CPACK_NSIS_INSTALLED_ICON_NAME "${PROJECT_NAME}.exe")

set(CPACK_NSIS_URL_INFO_ABOUT "${ORG_WEBSITE}")
set(CPACK_NSIS_HELP_LINK "${ORG_WEBSITE}")

set(CPACK_NSIS_MUI_FINISHPAGE_RUN "${PROJECT_NAME}.exe")
