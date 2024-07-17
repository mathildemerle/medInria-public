function(mmg_project)

set(ep mmg)

## #############################################################################
## List the dependencies of the project
## #############################################################################

list(APPEND ${ep}_dependencies "")
  
## #############################################################################
## Prepare the project
## ############################################################################# 

EP_Initialisation(${ep}
  USE_SYSTEM OFF 
  BUILD_SHARED_LIBS OFF
  REQUIRED_FOR_PLUGINS ON
  )

if (NOT USE_SYSTEM_${ep})

## #############################################################################
## Define repository where get the sources
## #############################################################################

set(git_url ${GITHUB_PREFIX}MmgTools/mmg.git)
set(git_tag v5.5.1)

## #############################################################################
## Add specific cmake arguments for configuration step of the project
## #############################################################################

# set compilation flags
if (UNIX OR APPLE)
  if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_C_COMPILER_VERSION VERSION_LESS 5.4)
    set(${ep}_c_flags "${${ep}_c_flags} -std=c99")
  else()
    set(${ep}_c_flags "${${ep}_c_flags}")
  endif()
  set(${ep}_cxx_flags "${${ep}_cxx_flags}")
endif()

set(cmake_args
    ${ep_common_cache_args}
    -DCMAKE_C_FLAGS:STRING=${${ep}_c_flags}
    -DCMAKE_CXX_FLAGS:STRING=${${ep}_cxx_flags}
    -DCMAKE_SHARED_LINKER_FLAGS:STRING=${${ep}_shared_linker_flags}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE_externals_projects}
    -DBUILD_MMG:BOOL=ON
    -DLIBMMG_SHARED:BOOL=${BUILD_SHARED_LIBS_${ep}}
    -DUSE_ELAS:BOOL=OFF
    -DUSE_SCOTCH:BOOL=OFF
    -DUSE_VTK:BOOL=OFF
  )

## #############################################################################
## Add external-project
## #############################################################################

ep_GeneratePatchCommand(${ep} ${ep}_PATCH_COMMAND mmg.patch)

epComputPath(${ep})

ExternalProject_Add(${ep}
  PREFIX ${EP_PATH_SOURCE}
  SOURCE_DIR ${EP_PATH_SOURCE}/${ep}
  BINARY_DIR ${build_path}
  INSTALL_DIR ${build_path}
  TMP_DIR ${tmp_path}
  STAMP_DIR ${stamp_path}
  GIT_REPOSITORY ${git_url}
  GIT_TAG ${git_tag}
  CMAKE_GENERATOR ${gen}
  CMAKE_GENERATOR_PLATFORM ${CMAKE_GENERATOR_PLATFORM}
  CMAKE_ARGS ${cmake_args}
  DEPENDS ${${ep}_dependencies}
  PATCH_COMMAND ${${ep}_PATCH_COMMAND}
  UPDATE_COMMAND ""
  )

ExternalProject_Add_Step(${ep} post_install
    DEPENDEES install
    COMMAND ${CMAKE_COMMAND} -E rm "<BINARY_DIR>/mmgConfig.cmake"
    )

## #############################################################################
## Set variable to provide infos about the project
## #############################################################################

set(${ep}_ROOT ${build_path} PARENT_SCOPE)

endif() #NOT USE_SYSTEM_ep

endfunction()
