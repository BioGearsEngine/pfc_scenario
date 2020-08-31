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
 set(CMAKE_INCLUDE_CURRENT_DIR ON)
 set_property(GLOBAL PROPERTY USE_FOLDERS ON)
 set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER AutoMoc)
###############################################################################
# Base Variables
###############################################################################
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
message(STATUS "Configuring ScenarioBuilder ")

#Scenario Driver


add_source_files(SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/cpp
                REGEX "main.cpp" SOURCE_GROUP  "Source Files")
add_source_files(SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/cpp/sql
                REGEX "*.c" "*.cpp" SOURCE_GROUP  "Source Files\\sql")
add_source_files(HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/cpp/sql
                 REGEX "*.h" "*.hpp" SOURCE_GROUP  "Headers\\Private\\sql")

add_source_files(SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/cpp/xml
                REGEX "*.c" "*.cpp" SOURCE_GROUP  "Source Files\\xml")
add_source_files(HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/cpp/xml
                 REGEX "*.h" "*.hpp" SOURCE_GROUP  "Headers\\Private\\xml")

add_source_files(RESOURCES LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/qml 
                REGEX "*.qrc"  SOURCE_GROUP  "Resources")
add_source_files(QML_FILES LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/qml 
                REGEX "*.qml" SOURCE_GROUP  "Resources")
add_source_files(UNIT_TEST LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/unit_test
                REGEX "*.cpp" SOURCE_GROUP  "Tests")

#XSD_HEADER_FILES
add_source_files(XSD_HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/xsd/cpp 
                REGEX "*.hxx" SOURCE_GROUP  "Headers\\Private\\xsd")
add_source_files(XSD_SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/xsd/cpp 
                REGEX "*.cxx" SOURCE_GROUP  "Source Files\\xsd")

set ( XSD_FILES
    "xsd/pfc_scenario_0.2.xsd"
    "xsd/pfc_scenario_complex_types_0.2.xsd"
    "xsd/msdl_simple_types_1.0.0.xsd"
    "xsd/msdl_complex_types_1.0.0.xsd"
    "xsd/msdl_complex_types_1.0.0.xsd"
    "xsd/msdl_codes_1.0.0.xsd"
    "xsd/military_scenario_1.0.0.xsd"
    "xsd/extern/jc3iedm_meterological.xsd"
    "xsd/extern/jc3iedm-3.1-codes-20061208.xsd"
    "xsd/extern/model_id_v2006_final.xsd"
  )

  foreach(schema IN LISTS XSD_FILES)
      message(STATUS "Tracking ${schema}")
	  add_custom_command(
		OUTPUT  ${PROJECT_BINARY_DIR}/${schema}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${schema}
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${schema} ${PROJECT_BINARY_DIR}/${schema}
		COMMAND ${CMAKE_COMMAND} -E echo ${CMAKE_CURRENT_SOURCE_DIR}/${schema} ${PROJECT_BINARY_DIR}/${schema}
	  )	  
  endforeach()

  add_custom_target( prepare_runtime_dir
	  DEPENDS ${XSD_FILES}
	  COMMENT "Scanning for need to stage runtime dir"
  )


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
# TARGETS
###############################################################################

# lib_pfc_xsd
add_library(pfc_serialization STATIC ${XSD_HDRS} ${XSD_SRCS})
target_link_libraries(pfc_serialization Xerces::xerces)
add_library( Builder::serial ALIAS pfc_serialization )
set_target_properties(pfc_serialization
  PROPERTIES
  DEFINE_SYMBOL "SUSTAIN_FRAMEWORK_EXPORTS"
  FOLDER "Libraries"
  OUTPUT_NAME "pfc_serialization"
  COMPILE_PDB_NAME "pfc_serialization"
  PROJECT_LABEL "pfc_serialization"
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
target_compile_options(pfc_serialization PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP>)

# lib_pfc_builder
add_library(libScenarioBuilder STATIC ${${PREFIX}_SOURCES} ${${PREFIX}_HEADERS})
add_library(Builder::lib ALIAS libScenarioBuilder)
target_link_libraries(libScenarioBuilder PUBLIC   
  Qt5::Core 
  Qt5::Widgets 
  Qt5::Gui 
  Qt5::Sql 
  Qt5::Quick 
  Qt5::QuickControls2 
  Qt5::Qml
  minizip::lib
  Builder::serial
)

set_target_properties(libScenarioBuilder
  PROPERTIES
  DEFINE_SYMBOL "SUSTAIN_FRAMEWORK_EXPORTS"
  FOLDER "Libraries"
  OUTPUT_NAME "ScenarioBuilder"
  COMPILE_PDB_NAME "libScenarioBuilder"
  AUTOMOC ON
  PROJECT_LABEL "pfc_builder"
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
target_compile_definitions(libScenarioBuilder PUBLIC ${${PREFIX}_CPPFLAGS} PUBLIC $<$<PLATFORM_ID:Windows>:BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE> )
target_compile_options(libScenarioBuilder PUBLIC $<$<PLATFORM_ID:Windows>:/bigobj>  PUBLIC $<$<PLATFORM_ID:Windows>:/MP>)

target_include_directories(libScenarioBuilder
  PUBLIC ${${PREFIX}_INCLUDE_DIR}
  PUBLIC ${${PREFIX}_PRIVATE_INCLUDE_DIR}
  PUBLIC ${${PREFIX}_GENERATED_INCLUDE_DIR}
  PUBLIC ${Xerces-c_INCLUDE_DIR}
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/xsd/cpp"
)

if(CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(libScenarioBuilder PROPERTIES LINK_FLAGS ${${PREFIX}_LDFLAGS})
endif()

# GUI Executable
add_executable(ScenarioBuilder ${CMAKE_CURRENT_SOURCE_DIR}/cpp/main.cpp  ${qml_QRC}  ${QML_FILES})
target_link_libraries(ScenarioBuilder 
  Builder::lib 
  Builder::serial
  minizip::lib 
  Boost::disable_autolinking 
  Boost::filesystem
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
set_target_properties(ScenarioBuilder
  PROPERTIES
  FOLDER "Binaries"
  OUTPUT_NAME "ScenarioBuilder "
  COMPILE_PDB_NAME "ScenarioBuilder "
  PROJECT_LABEL "ScenarioBuilder "
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
target_compile_definitions(ScenarioBuilder PRIVATE ${${PREFIX}_CPPFLAGS} PRIVATE $<$<PLATFORM_ID:Windows>:BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE> )
target_compile_options(ScenarioBuilder PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP>)

###############################################################################
# Unit Test
###############################################################################
add_executable(unitScenarioBuilder ${UNIT_TEST})
target_link_libraries(unitScenarioBuilder Builder::lib GTest::GTest GTest::Main Boost::disable_autolinking Boost::filesystem minizip::lib)
set_target_properties(unitScenarioBuilder 
  PROPERTIES
  DEFINE_SYMBOL "SUSTAIN_FRAMEWORK_EXPORTS"
  FOLDER "Tests"
  OUTPUT_NAME "unitScenarioBuilder "
  COMPILE_PDB_NAME "unitScenarioBuilder "
  PROJECT_LABEL "unitScenarioBuilder "
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
target_compile_definitions(unitScenarioBuilder PRIVATE ${${PREFIX}_CPPFLAGS}  PRIVATE $<$<PLATFORM_ID:Windows>:BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE> )
target_compile_options(unitScenarioBuilder PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP>)

################################################################################
# Install
################################################################################
install(TARGETS ScenarioBuilder
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
  ARCHIVE DESTINATION lib
)


################################################################################
message(STATUS "")
generate_project_suffix()
endif()
