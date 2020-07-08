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

function(ffmpeg_project)
set(ep ffmpeg)

## #############################################################################
## List the dependencies of the project
## #############################################################################

list(APPEND ${ep}_dependencies
    ""
    )

## #############################################################################
## Prepare the project
## #############################################################################

EP_Initialisation(${ep}
    USE_SYSTEM OFF
    BUILD_SHARED_LIBS OFF
    REQUIRED_FOR_PLUGINS OFF
    )

if (NOT USE_SYSTEM_${ep})

## #############################################################################
## Define repository where get the sources
## #############################################################################

if(WIN32) # MPEG2
    set(location URL "http://www.vtk.org/files/support/vtkmpeg2encode.zip")
else() # FFMPEG
    #set(tag "release/0.7")
    set(tag "release/4.1")
    #set(tag "release/4.3")
    set(location GIT_REPOSITORY "${GITHUB_PREFIX}FFmpeg/FFmpeg.git" GIT_TAG ${tag})
endif()

## #############################################################################
## Add specific cmake arguments for configuration step of the project
## #############################################################################

# set compilation flags
if (WIN32) # MPEG2
    set(cmake_args
        ${ep_common_cache_args}
        -DCMAKE_C_FLAGS:STRING=${${ep}_c_flags}
        -DCMAKE_CXX_FLAGS:STRING=${${ep}_cxx_flags}
        -DCMAKE_SHARED_LINKER_FLAGS:STRING=${${ep}_shared_linker_flags}
        )
endif()

## #############################################################################
## Add external-project
## #############################################################################

epComputPath(${ep})

if (WIN32)
    ExternalProject_Add(${ep}
        PREFIX ${EP_PATH_SOURCE}
        SOURCE_DIR ${EP_PATH_SOURCE}/${ep}
        BINARY_DIR ${build_path}
        TMP_DIR ${tmp_path}
        STAMP_DIR ${stamp_path}
        ${location}

        CMAKE_GENERATOR ${gen}
        CMAKE_ARGS ${cmake_args}
        DEPENDS ${${ep}_dependencies}
        INSTALL_COMMAND ""
        )

    # install(DIRECTORY ${EP_PATH_BUILD}/${ep}/${CMAKE_BUILD_TYPE_medInria}/
    #     DESTINATION lib
    #     FILES_MATCHING PATTERN "*lib"
    #     )

else()
    ExternalProject_Add(${ep}
        PREFIX ${EP_PATH_SOURCE}
        SOURCE_DIR ${EP_PATH_SOURCE}/${ep}
        BINARY_DIR ${build_path}
        TMP_DIR ${tmp_path}
        STAMP_DIR ${stamp_path}
        ${location}

        UPDATE_COMMAND ""
        PATCH_COMMAND ""
        DEPENDS ${${ep}_dependencies}
        CONFIGURE_COMMAND ${EP_PATH_SOURCE}/${ep}/configure
            --disable-yasm --disable-static 
            --disable-network --disable-zlib --disable-doc --disable-ffplay --disable-decoders
            --enable-shared
            --prefix=${build_path}
            #
            #--libdir=${build_path}/lib
            #--incdir=${build_path}/include
        BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} install

        #CMAKE_GENERATOR ${gen}
        #CMAKE_ARGS ${cmake_args}
        #DEPENDS ${${ep}_dependencies}
        
        INSTALL_COMMAND ""
        )
        install(DIRECTORY ${EP_PATH_BUILD}/${ep}/lib/
        DESTINATION lib
        FILES_MATCHING PATTERN "lib*")
endif()

## #############################################################################
## Set variable to provide infos about the project
## #############################################################################

set(build_dir ${build_path})
set(${ep}_DIR ${build_dir} PARENT_SCOPE)

endif() #NOT USE_SYSTEM_ep

endfunction()
