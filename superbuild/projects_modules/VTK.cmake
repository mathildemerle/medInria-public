################################################################################
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

function(VTK_project)
set(ep VTK)

## #############################################################################
## List the dependencies of the project
## #############################################################################

list(APPEND ${ep}_dependencies
  ffmpeg
  )
  
## #############################################################################
## Prepare the project
## #############################################################################

EP_Initialisation(${ep} 
  USE_SYSTEM OFF 
  BUILD_SHARED_LIBS ON
  REQUIRED_FOR_PLUGINS ON
  )

if (NOT USE_SYSTEM_${ep})

## #############################################################################
## Set up versioning control.
## #############################################################################

set(git_url ${GITHUB_PREFIX}Kitware/VTK.git)
set(git_tag v9.0.1)

## #############################################################################
## Add specific cmake arguments for configuration step of the project
## #############################################################################

# set compilation flags
if (UNIX)
  set(${ep}_c_flags "${${ep}_c_flags} -w")
  set(${ep}_cxx_flags "${${ep}_cxx_flags} -w")
endif()

# library extension
if (UNIX AND NOT APPLE)
   set(extention so)
elseif(APPLE)
   set(extention dylib)
elseif (WIN32)
   set(extention lib)
endif()

set(cmake_args_generic
  ${ep_common_cache_args}
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE_externals_projects}
  -DCMAKE_C_FLAGS=${${ep}_c_flags}
  -DCMAKE_CXX_FLAGS=${${ep}_cxx_flags}
  -DCMAKE_MACOSX_RPATH:BOOL=OFF
  -DCMAKE_SHARED_LINKER_FLAGS=${${ep}_shared_linker_flags}  
  -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>  
  -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS_${ep}}

  # VTK generic parameters 
  -DVTK_BUILD_TESTING=OFF
  -DVTK_BUILD_DOCUMENTATION=OFF
  -DVTK_BUILD_EXAMPLES=OFF
  -DVTK_RENDERING_BACKEND=OpenGL2

  #  Enabling modules for build
  # Cf. https://vtk.org/doc/nightly/html/md__home_kitware_dashboards_buildbot_vtk_nightly-master-ike-linux-shared-release_doc_nightly_osm749dd663df9981384f2598c108aac3b0.html
  #  YES: The module must be built.
  #  NO: The module must not be built. If a YES module has a NO module in its dependency tree, an error is raised.
  #  WANT: The module should be built. It will not be built, however, if it depends on a NO module.
  #  DONT_WANT: The module doesn't need to be built. It will be built if a YES or WANT module depends on it.
  #  DEFAULT: Look at other metadata to determine the status.

  # TODO: setting to "YES" the modules, as done below in VTK_GROUP_ENABLE_Qt for instance does not work.
  # I need to enter ccmake to change the value from DEFAULT to YES.

  # Qt
  -DVTK_GROUP_ENABLE_Qt=YES
  -DVTK_MODULE_ENABLE_VTK_GUISupportQt=YES
  -DVTK_MODULE_ENABLE_VTK_GUISupportQtOpenGL=YES
  -DVTK_QT_VERSION=5
  -DQt5_DIR=${Qt5_DIR}

  -DModule_vtkRenderingOSPRay:BOOL=${USE_OSPRay}
  # TODO: need to be set according to the value above
  -DVTK_MODULE_ENABLE_VTK_RenderingOSPRay=YES

  # OGV
  -DVTK_USE_OGGTHEORA_ENCODER:BOOL=ON
  # To be removed when upgrading VTK version, TODO: is it ok?
  -DVTK_REQUIRED_OBJCXX_FLAGS:STRING=""
  )

if(USE_OSPRay)
  list(APPEND cmake_args_generic
  -Dospray_DIR=${ospray_DIR}
  -DOSPRAY_INSTALL_DIR=${OSPRAY_INSTALL_DIR})
endif()

# Video Export
if (WIN32)
    list(APPEND cmake_args_generic
        # MPEG2
        -DVTK_USE_MPEG2_ENCODER:BOOL=ON
        -DvtkMPEG2Encode_INCLUDE_PATH:STRINGS=${EP_PATH_SOURCE}/ffmpeg$<SEMICOLON>${EP_PATH_BUILD}/ffmpeg
        -DvtkMPEG2Encode_LIBRARIES:STRING=${EP_PATH_BUILD}/ffmpeg/build/${CMAKE_BUILD_TYPE}/vtkMPEG2Encode.${extention}
        )
else()
    list(APPEND cmake_args_generic
        # FFMPEG
        -DVTK_MODULE_ENABLE_VTK_IOFFMPEG=YES
        -DFFMPEG_INCLUDE_DIR:STRING=${EP_PATH_BUILD}/ffmpeg/include/
        -DFFMPEG_LIBAVCODEC_LIBRARIES:STRING=${EP_PATH_BUILD}/ffmpeg/lib/libavcodec.${extention}
        -DFFMPEG_LIBAVDEVICE_LIBRARIES:STRING=${EP_PATH_BUILD}/ffmpeg/lib/libavdevice.${extention}
        -DFFMPEG_LIBAVFORMAT_LIBRARIES:STRING=${EP_PATH_BUILD}/ffmpeg/lib/libavformat.${extention}
        -DFFMPEG_LIBAVUTIL_LIBRARIES:STRING=${EP_PATH_BUILD}/ffmpeg/lib/libavutil.${extention}
        -DFFMPEG_LIBSWSCALE_LIBRARIES:STRING=${EP_PATH_BUILD}/ffmpeg/lib/libswscale.${extention}
        )
endif()

## #############################################################################
## Add external-project
## #############################################################################

epComputPath(${ep})

ExternalProject_Add(${ep}
  PREFIX ${EP_PATH_SOURCE}
  SOURCE_DIR ${EP_PATH_SOURCE}/${ep}
  BINARY_DIR ${build_path}
  TMP_DIR ${tmp_path}
  STAMP_DIR ${stamp_path}
  
  GIT_REPOSITORY ${git_url}
  GIT_TAG ${git_tag}
  CMAKE_GENERATOR ${gen}
  CMAKE_GENERATOR_PLATFORM ${CMAKE_GENERATOR_PLATFORM}
  CMAKE_ARGS ${cmake_args_generic}
  DEPENDS ${${ep}_dependencies}
  INSTALL_COMMAND ""
  )

## #############################################################################
## Set variable to provide infos about the project
## #############################################################################

ExternalProject_Get_Property(${ep} binary_dir)
set(${ep}_DIR ${binary_dir} PARENT_SCOPE)


endif() #NOT USE_SYSTEM_ep

endfunction()
