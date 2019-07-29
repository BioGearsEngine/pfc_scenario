###############################################################################
# Policy adjustments
###############################################################################
cmake_minimum_required(VERSION 3.12.0)
cmake_policy(VERSION 3.12.0)
###############################################################################
# Options
###############################################################################
option(${ROOT_PROJECT_NAME}_BUILD_SCENARIO_BUILDER "Toggle building of cmd_bio tools" ON)
if(${ROOT_PROJECT_NAME}_BUILD_SCENARIO_BUILDER)
 set(CMAKE_AUTOMOC ON)
 set(CMAKE_INCLUDE_CURRENT_DIR ON)
 set_property(GLOBAL PROPERTY USE_FOLDERS ON)
 set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER AutoMoc)
###############################################################################
# Base Variables
###############################################################################
set(PROJECT_NAME scenario_builder)
set(PREFIX sb)

set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cpp" )
set(${PREFIX}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" )
set(${PREFIX}_UNIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unit" PARENT_SCOPE)

###############################################################################
# Requirments
###############################################################################
find_package(Xerces-c REQUIRED)
find_package(GTest REQUIRED)
###############################################################################
#Code Generation
###############################################################################

###############################################################################
message(STATUS "")
message(STATUS "Configuring ${PROJECT_NAME}")

#Scenario Driver

add_source_files(HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/cpp 
                 REGEX "*.h" "*.hpp" SOURCE_GROUP  "Headers\\Private")
add_source_files(SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/cpp 
                REGEX "*.c" "*.cpp" SOURCE_GROUP  "Source Files")
add_source_files(HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/xsd/cpp 
                REGEX "*.hxx" SOURCE_GROUP  "Headers\\Private\\xsd")
add_source_files(SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/xsd/cpp 
                REGEX "*.cxx" SOURCE_GROUP  "Source Files\\xsd")
add_source_files(RESOURCES LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/qml 
                REGEX "*.qrc"  SOURCE_GROUP  "Resources")
add_source_files(QML_FILES LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/qml 
                REGEX "*.qml" SOURCE_GROUP  "Resources")
add_source_files(UNIT_TEST LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/unit_test
                REGEX "*.cpp" SOURCE_GROUP  "Tests")


set(${PREFIX}_HEADERS ${HDRS} ${PUBLIC_HDRS} ${GEN_HDRS})
set(${PREFIX}_SOURCES ${SRCS} ${GEN_SRCS})

list(FILTER ${PREFIX}_SOURCES  EXCLUDE  REGEX "main.cpp" )

# add the qml.qrc file
qt5_add_resources(qml_QRC ${RESOURCES})

###############################################################################
#Define Logic
###############################################################################
if(WIN32)
  list(APPEND ${PREFIX}_CPPFLAGS_EXPORT )
  list(APPEND ${PREFIX}_CPPFLAGS "-D_SCL_SECURE_NO_WARNINGS" "-D_CRT_SECURE_NO_WARNINGS" $ENV{PARALLEL_COMPILE} )
elseif(CMAKE_COMPILER_IS_GNUCXX)
list(APPEND ${PREFIX}_CPPFLAGS_EXPORT )
  list(APPEND ${PREFIX}_CPPFLAGS  ${CodeSynthesis_CPPFLAGS})
  list(APPEND ${PREFIX}_LDFLAGS "-Wl,--no-as-needed" )
endif()

if (${PREFIX}_BUILD_STATIC)
  add_definitions("-D${PREFIX}_BUILT_AS_STATIC")
endif()

###############################################################################
# COMPILATION & LINKLINKAGE MODIFICATIONS
###############################################################################

list(APPEND ${PREFIX}_INCLUDES
      PUBLIC ${${PREFIX}_INCLUDE_DIR}
      PUBLIC ${${PREFIX}_PRIVATE_INCLUDE_DIR}
      PUBLIC ${${PREFIX}_GENERATED_INCLUDE_DIR}
      PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/xsd/cpp
)



set(${PREFIX}_LIBS
     ${CMAKE_THREAD_LIBS_INIT}
     ${CMAKE_DL_LIBS}
   Xerces::xerces
   Qt5::Core 
   Qt5::Widgets 
   Qt5::Gui 
   Qt5::Sql 
   Qt5::Quick 
   Qt5::QuickControls2 
   Qt5::Qml
)

set(${PREFIX}_LIBS ${${PREFIX}_LIBS} PARENT_SCOPE)
###############################################################################
# TARGETS
###############################################################################
# LIBRARY
###############################################################################
add_library(lib${PROJECT_NAME} STATIC ${${PREFIX}_SOURCES} ${${PREFIX}_HEADERS})
add_library( Builder::lib ALIAS lib${PROJECT_NAME} )
target_link_libraries(lib${PROJECT_NAME} PUBLIC ${${PREFIX}_LIBS} Boost::filesystem Boost::disable_autolinking )
set_target_properties(lib${PROJECT_NAME}
  PROPERTIES
  DEFINE_SYMBOL "SUSTAIN_FRAMEWORK_EXPORTS"
  FOLDER "Libraries"
  OUTPUT_NAME "lib${PROJECT_NAME}"
  COMPILE_PDB_NAME "lib${PROJECT_NAME}"
  PROJECT_LABEL "lib${PROJECT_NAME}"
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
target_compile_definitions(lib${PROJECT_NAME} PUBLIC ${${PREFIX}_CPPFLAGS}    )
target_compile_definitions(lib${PROJECT_NAME} PUBLIC $<$<PLATFORM_ID:Windows>:BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE> )
target_compile_options(lib${PROJECT_NAME} PUBLIC $<$<PLATFORM_ID:Windows>:/bigobj>  PUBLIC $<$<PLATFORM_ID:Windows>:/MP>)
target_include_directories(lib${PROJECT_NAME}
      PUBLIC ${${PREFIX}_INCLUDE_DIR}
      PUBLIC ${${PREFIX}_PRIVATE_INCLUDE_DIR}
      PUBLIC ${${PREFIX}_GENERATED_INCLUDE_DIR}
      PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/xsd/cpp"
)

if(CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(lib${PROJECT_NAME} PROPERTIES LINK_FLAGS ${${PREFIX}_LDFLAGS})
endif()

###############################################################################
# GUI
###############################################################################
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/cpp/main.cpp  ${qml_QRC}  ${QML_FILES})
target_link_libraries(${PROJECT_NAME} Builder::lib Boost::disable_autolinking )
set_target_properties(${PROJECT_NAME}
  PROPERTIES
  DEFINE_SYMBOL "SUSTAIN_FRAMEWORK_EXPORTS"
  FOLDER "Binaries"
  OUTPUT_NAME "${PROJECT_NAME}"
  COMPILE_PDB_NAME "${PROJECT_NAME}"
  PROJECT_LABEL "${PROJECT_NAME}"
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
target_compile_definitions(${PROJECT_NAME} PRIVATE ${${PREFIX}_CPPFLAGS}    )
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<PLATFORM_ID:Windows>:BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE> )
target_compile_options(${PROJECT_NAME} PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP>)
###############################################################################
# Unit Test
###############################################################################
add_executable(unit_${PROJECT_NAME} ${UNIT_TEST})
target_link_libraries(unit_${PROJECT_NAME} Builder::lib GTest::GTest GTest::Main Boost::disable_autolinking )
set_target_properties(unit_${PROJECT_NAME}
  PROPERTIES
  DEFINE_SYMBOL "SUSTAIN_FRAMEWORK_EXPORTS"
  FOLDER "Tests"
  OUTPUT_NAME "unit_${PROJECT_NAME}"
  COMPILE_PDB_NAME "unit_${PROJECT_NAME}"
  PROJECT_LABEL "unit_${PROJECT_NAME}"
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
target_compile_definitions(unit_${PROJECT_NAME} PRIVATE ${${PREFIX}_CPPFLAGS}    )
target_compile_definitions(unit_${PROJECT_NAME} PRIVATE $<$<PLATFORM_ID:Windows>:BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE> )
target_compile_options(unit_${PROJECT_NAME} PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP>)

################################################################################
# Install
################################################################################
 install(TARGETS ${PROJECT_NAME}
     RUNTIME DESTINATION bin
     LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
     ARCHIVE DESTINATION lib
  )


################################################################################
message(STATUS "")
endif()
