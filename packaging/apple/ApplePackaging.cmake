##############################################################################
#
# medInria
#
# Copyright (c) INRIA 2013. All rights reserved.
# See LICENSE.txt for details.
# 
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.
#
################################################################################

set(CPACK_BINARY_TGZ ON)
set(CPACK_BINARY_DRAGNDROP OFF)
set(CPACK_BINARY_PACKAGEMAKER OFF)

## #############################################################################
## Set macOS bundle settings
## #############################################################################

set(CPACK_PACKAGE_FILE_NAME 
  "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_PROCESSOR}"
   )

message("### target ${target} ${${target}_VERSION} or ${${APPLICATION_NAME}_VERSION}")
message("### APPLICATION_NAME ${APPLICATION_NAME}")
message("### MACOS_ICON_PATH ${MACOS_ICON_PATH}")
set(CPACK_BUNDLE_NAME ${APPLICATION_NAME})
set(CPACK_BUNDLE_ICON ${MACOS_ICON_PATH})
set(CPACK_BUNDLE_PLIST "${CMAKE_SOURCE_DIR}/src/cmake/MedInriaOSXBundleInfo.plist.in")

## #############################################################################
## Add Apple packaging script
## #############################################################################

configure_file(${CMAKE_SOURCE_DIR}/packaging/apple/ApplePackScript.cmake.in 
  ${PROJECT_BINARY_DIR}/tmp.in
  )
  
configure_file(
  ${PROJECT_BINARY_DIR}/tmp.in 
  ${PROJECT_BINARY_DIR}/packaging/apple/ApplePackScript.cmake
  )
  
configure_file(${CMAKE_SOURCE_DIR}/packaging/apple/mac_packager.sh.in 
  ${PROJECT_BINARY_DIR}/packaging/apple/mac_packager.sh
  )

set(CPACK_INSTALL_SCRIPT 
  ${PROJECT_BINARY_DIR}/packaging/apple/ApplePackScript.cmake
  )
