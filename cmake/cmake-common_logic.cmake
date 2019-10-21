if( NOT CMAKE_DEBUG_POSTFIX )
  set( CMAKE_DEBUG_POSTFIX "_d" CACHE STRING "This string is appended to target names in debug mode." FORCE )
endif()
if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE Debug FORCE )
  set(BUILD_OPTIONS_STRINGS
    "Debug"
    "Release"
  ) 
  if(ANDROID)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Release;Debug" FORCE)
  else()
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Release;Debug" FORCE)
  endif()
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${BUILD_OPTIONS_STRINGS})
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${_ROOT}/usr" CACHE PATH "default install path" FORCE )
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 1)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER "Code Generators" )
set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP  "Generated")

####
#
#  A simple macro to check for PACKAGE_FOUND and warn the user to 
#
####
function(verify_package package)
  cmake_parse_arguments(_l
    REQUIRED 
    ""
    ""
     ${ARGN})
  find_package(${package} CONFIG ${_l_UNPARSED_ARGUMENTS} QUIET)
  if(NOT ${package}_FOUND)
    find_package(${package} ${_l_UNPARSED_ARGUMENTS} QUIET)
    if(NOT ${package}_FOUND)
      message(WARNING "The following packages ${package} were not found."
        "For native compilations setting CMAKE_PREFIX_PATH can solve this problem"
        "For cross complilation try expanding your CMAKE_FIND_ROOT_PATH"
        "")
      if(_l_REQUIRED)
        message(FATAL_ERROR "Unable to find ${package} which is marked REQUIRED")
      endif()
    endif()
  endif()
  set(${package}_FOUND ${${package}_FOUND} PARENT_SCOPE)
endfunction(verify_package)
####
#
#  Simple macro for taking a list of header files and asking them to be included
#
####
function(install_headers header_list out_dir)

    FOREACH(HEADER ${${header_list}})
        STRING(REGEX MATCH "(.\\\*)\\\[/\\\]" DIR ${HEADER})
        INSTALL(FILES ${HEADER} DESTINATION ${out_dir}/${DIR})
    ENDFOREACH(HEADER)

endfunction()
########################################################################################################
# 
# Source File Managment Macros
# 
########################################################################################################
function(CHILDLIST result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist} PARENT_SCOPE)
endfunction()

function(add_source_files var)
  set(__prefix "_l")
  set(__options "DEBUG")
  set(__one_value "LOCATION;SOURCE_GROUP")
  set(__multi_value "REGEX")
  cmake_parse_arguments( "_l" "DEBUG" "LOCATION;SOURCE_GROUP" "REGEX" ${ARGN})
    if(_l_DEBUG)
      message(STATUS "add_source_files( ${var} LOCATION \"${_l_LOCATION}\" REGEX \"${_l_REGEX}\" SOURCE_GROUP \"${_l_SOURCE_GROUP}\")")
    endif()
    foreach(regex IN LISTS _l_REGEX)
      file(GLOB TEMP "${_l_LOCATION}/${regex}")
      list(APPEND __new_entries ${TEMP})
    endforeach()
    if(_l_SOURCE_GROUP)
      source_group("${_l_SOURCE_GROUP}" FILES ${__new_entries})
    endif()
    if(_l_DEBUG)
      message(STATUS "CHILDLIST( result ${_l_LOCATION})")
    endif()
    CHILDLIST( result ${_l_LOCATION})
    

    foreach( dir IN LISTS result)
      if( _l_DEBUG)
        add_source_files( ${var} LOCATION "${_l_LOCATION}/${dir}" REGEX ${_l_REGEX} SOURCE_GROUP "${_l_SOURCE_GROUP}\\${dir}" DEBUG)
      else()
        add_source_files( ${var} LOCATION "${_l_LOCATION}/${dir}" REGEX ${_l_REGEX} SOURCE_GROUP "${_l_SOURCE_GROUP}\\${dir}")
      endif()
    endforeach()

    set(${var} ${${var}} ${__new_entries} PARENT_SCOPE)
endfunction()

########################################################################################################
#
# Remove Duplicate LIbraries
#
########################################################################################################
function(remove_duplicate_libraries libraries)
  list(LENGTH ${libraries} LIST_LENGTH)
  while( ${libraries} )
     list(GET ${libraries} 0 item )
     if( item STREQUAL debug)
        list(GET ${libraries} 1 item )
        list(REMOVE_AT ${libraries} 0 1 )
        list(APPEND debug_libraries  ${item})
     elseif( item STREQUAL optimized)
        list(GET ${libraries} 1 item )
        list(REMOVE_AT ${libraries} 0 1 )
        list(APPEND release_libraries  ${item})
     else()
        list(REMOVE_AT ${libraries} 0 )
        list(APPEND common_libraries ${item})
     endif()  
  endwhile()
  if(common_libraries)
    list(REMOVE_DUPLICATES common_libraries)
  endif()
  if(release_libraries)
    list(REMOVE_DUPLICATES release_libraries)
   endif()
  if(debug_libraries)
    list(REMOVE_DUPLICATES debug_libraries)
   endif()
  set( results )
  foreach( item IN LISTS release_libraries)
    list(APPEND results "optimized" ${item})
  endforeach()
  
  foreach( item IN LISTS debug_libraries)
    list(APPEND results "debug" ${item})
  endforeach()
  
  set( ${libraries} ${common_libraries} ${results} PARENT_SCOPE)
endfunction()
########################################################################################################
# 
# Stage Macros
# 
########################################################################################################
function(create_cache_file)
  set(CacheForScript ${CMAKE_BINARY_DIR}/cmake-common_cache.cmake)
  set(OUTPUT_PREFIX ${CMAKE_BINARY_DIR}/outputs)
  file(WRITE ${CacheForScript} "")
  file(APPEND ${CacheForScript} "set(ROOT_PROJECT_NAME ${ROOT_PROJECT_NAME})\n")
  file(APPEND ${CacheForScript} "set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR})\n")
  file(APPEND ${CacheForScript} "set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR})\n")
  file(APPEND ${CacheForScript} "set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})\n")
  file(APPEND ${CacheForScript} "set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})\n")
  file(APPEND ${CacheForScript} "set(CMAKE_EXECUTABLE_SUFFIX ${CMAKE_EXECUTABLE_SUFFIX})\n")
  file(APPEND ${CacheForScript} "set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PREFIX}/lib)\n")
  file(APPEND ${CacheForScript} "set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PREFIX}/lib)\n")
  file(APPEND ${CacheForScript} "set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PREFIX}/bin)\n")
  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER _${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
        file(APPEND ${CacheForScript} "set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} ${OUTPUT_PREFIX}/${OUTPUTCONFIG}/lib)\n")
        file(APPEND ${CacheForScript} "set(CMAKE_LIBRARY_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} ${OUTPUT_PREFIX}/${OUTPUTCONFIG}/lib)\n")
        file(APPEND ${CacheForScript} "set(CMAKE_RUNTIME_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} ${OUTPUT_PREFIX}/${OUTPUTCONFIG}/bin)\n")
  endforeach()
endfunction()

function(create_stage)
  add_custom_target(STAGE 
    ${CMAKE_COMMAND} 
    -DCMAKE_INSTALL_CONFIG_NAME=$<CONFIG> -P ${CMAKE_SOURCE_DIR}/cmake/cmake-common_stage.cmake
    )
  set_target_properties(STAGE
      PROPERTIES
      FOLDER "CMakePredefinedTargets"
      PROJECT_LABEL "STAGE"
  )
endfunction() 

########################################################################################################
# 
# Appends a suffix to the project name to make it easier to tell worktree solutions apart
# 
# Will additionally append the MSVC version to the solution if it is of version 14-16
# Creates the following variabels
#
# ${CMAKE_PROJECT_NAME}_SUFFIX        #The value of this variable is appended to the solution name
#                                     #For Cmake Generators who use the project name in the output files
#                                     It is useful for determining which MSVC sln you are loading from 
#                                     the jump list
#
# __PROJECT_SUFFIX_SET                This control variable is set in PARENT_SCOPE to prevent calling 
#                                     of the function twice
#
########################################################################################################
function(generate_project_suffix)
if(MSVC AND NOT __PROJECT_SUFFIX_SET)
  set(__PROJECT_SUFFIX_SET ON PARENT_SCOPE)
  if(${CMAKE_PROJECT_NAME}_PROJECT_SUFFIX)
    project(${CMAKE_PROJECT_NAME}_${${CMAKE_PROJECT_NAME}_PROJECT_SUFFIX})
  endif()
  if(MSVC_VERSION GREATER_EQUAL 1920)
    project(${CMAKE_PROJECT_NAME}_msvc16)
  elseif(MSVC_VERSION GREATER_EQUAL 1910)
    project(${CMAKE_PROJECT_NAME}_msvc15)
  elseif(MSVC_VERSION GREATER_EQUAL 1900)
    project(${CMAKE_PROJECT_NAME}_msvc14)
    project(cmake-test_msvc15)
  endif()
endif()
endfunction()