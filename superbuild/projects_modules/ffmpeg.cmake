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

include_guard(GLOBAL)
if(${USE_FFmpeg})

function(ffmpeg_project)
set(ep ffmpeg)

## #############################################################################
## List the dependencies of the project
## #############################################################################

list(APPEND ${ep}_dependencies)

## #############################################################################
## Prepare the project
## #############################################################################

EP_Initialisation(${ep}
    USE_SYSTEM OFF
    BUILD_SHARED_LIBS ON
    REQUIRED_FOR_PLUGINS OFF
    )

if (NOT USE_SYSTEM_${ep})

## #############################################################################
## Define repository where get the sources
## #############################################################################

if (NOT DEFINED ${ep}_SOURCE_DIR)
    if(UNIX) # is TRUE on all UNIX-like OS's, including Apple OS X and CygWin
        set(tag "n3.4.8") # FFMPEG
        set(location GIT_REPOSITORY "${GITHUB_PREFIX}FFmpeg/FFmpeg.git" GIT_TAG ${tag})
    endif()
endif()

## #############################################################################
## Add specific cmake arguments for configuration step of the project
## #############################################################################

set(cmake_args
    ${ep_common_cache_args}
    -DCMAKE_C_FLAGS:STRING=${${ep}_c_flags}
    -DCMAKE_CXX_FLAGS:STRING=${${ep}_cxx_flags}
    )

## #############################################################################
## Add external-project
## #############################################################################

if (UNIX)
    ExternalProject_Add(${ep}
        PREFIX ${EP_PATH_SOURCE}
        SOURCE_DIR ${EP_PATH_SOURCE}/${ep}
        BINARY_DIR ${build_path}
        TMP_DIR ${tmp_path}
        STAMP_DIR ${stamp_path}

        ${location}
        CONFIGURE_COMMAND ${EP_PATH_SOURCE}/${ep}/configure
                # based on configuration used by ffmpeg apt package
                --prefix=${EP_PATH_BUILD}/${ep} #/usr
                --extra-version=0ubuntu0.2
                --toolchain=hardened 
                --libdir=${EP_PATH_BUILD}/${ep}/lib #/usr/lib/x86_64-linux-gnu 
                --incdir=${EP_PATH_BUILD}/${ep}/include #/usr/include/x86_64-linux-gnu 
                #--enable-gpl --disable-stripping --enable-avresample 
                #--enable-avisynth --enable-gnutls --enable-ladspa 
                #--enable-libass --enable-libbluray --enable-libbs2b 
                #--enable-libcaca --enable-libcdio --enable-libflite 
                #--enable-libfontconfig --enable-libfreetype --enable-libfribidi 
                #--enable-libgme --enable-libgsm --enable-libmp3lame 
                #--enable-libmysofa --enable-libopenjpeg --enable-libopenmpt 
                #--enable-libopus --enable-libpulse --enable-librubberband 
                #--enable-librsvg --enable-libshine --enable-libsnappy 
                #--enable-libsoxr --enable-libspeex --enable-libssh --enable-libtheora 
                #--enable-libtwolame --enable-libvorbis --enable-libvpx 
                #--enable-libwavpack --enable-libwebp --enable-libx265 --enable-libxml2 
                #--enable-libxvid --enable-libzmq --enable-libzvbi --enable-omx 
                #--enable-openal --enable-opengl --enable-sdl2 --enable-libdc1394 
                #--enable-libdrm --enable-libiec61883 --disable-chromaprint
                #--enable-frei0r --enable-libopencv --enable-libx264 --enable-shared
                # from https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu
                --enable-gpl
                --enable-gnutls
                --enable-libaom
                --enable-libass
                --enable-libfdk-aac
                --enable-libfreetype
                --enable-libmp3lame
                --enable-libopus
                --enable-libsvtav1
                --enable-libvorbis
                --enable-libvpx
                --enable-libx264
                --enable-libx265
                --enable-nonfree

        BUILD_COMMAND make install
        )

    install(DIRECTORY ${EP_PATH_BUILD}/${ep}/lib/
        DESTINATION lib
        FILES_MATCHING PATTERN "lib*"
        )
endif()

## #############################################################################
## Set variable to provide infos about the project
## #############################################################################

ExternalProject_Get_Property(${ep} binary_dir)
set(${ep}_DIR ${binary_dir} PARENT_SCOPE)

endif() #NOT USE_SYSTEM_ep

endfunction()
endif()
