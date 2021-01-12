######################/fin#########################################################
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
 set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER "Code Generators")
 set_property(GLOBAL PROPERTY AUTORCC_SOURCE_GROUP  "Sources\\generated")
 set_property(GLOBAL PROPERTY AUTOMOC_SOURCE_GROUP  "Sources\\generated")
###############################################################################
# Base Variables
###############################################################################
set(PREFIX sb)

set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cpp" )
set(${PREFIX}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated/" )
set(${PREFIX}_UNIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unit" PARENT_SCOPE)

###############################################################################
# Requirments
###############################################################################
find_package(GTest)
find_package(CodeSynthesis REQUIRED)
###############################################################################
#Code Generation
###############################################################################

###############################################################################
message(STATUS "")
message(STATUS "Configuring ScenarioBuilder ")

#Scenario Driver


add_source_files(SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/cpp
                REGEX "main.cpp" SOURCE_GROUP  "Sources")
add_source_files(SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/cpp/sql
                REGEX "*.c" "*.cpp" SOURCE_GROUP  "Sources\\sql")
add_source_files(HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/cpp/sql
                 REGEX "*.h" "*.hpp" SOURCE_GROUP  "Headers\\Private\\sql")

add_source_files(SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/cpp/xml
                REGEX "*.c" "*.cpp" SOURCE_GROUP  "Sources\\xml")
add_source_files(HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/cpp/xml
                 REGEX "*.h" "*.hpp" SOURCE_GROUP  "Headers\\Private\\xml")

add_source_files(RESOURCES LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/qml 
                REGEX "*.qrc"  SOURCE_GROUP  "Resources")
add_source_files(QML_FILES LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/qml 
                REGEX "*.qml" SOURCE_GROUP  "Resources")
add_source_files(XSD_RESOURCES LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/xsd/ver_${PFC_SCHEMA_VERSION}/ 
                 REGEX "*.xsd" SOURCE_GROUP "Resources")

add_source_files(UNIT_TEST LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/unit_test
                REGEX "*.cpp" SOURCE_GROUP  "Tests")

#XSD_HEADER_FILES
set(PFC_SCHEMA_VERSION 0.3 CACHE STRING "Version of Scehema to compile against. DO_NOT_CHANGE")


REGISTER_XSD_DIR(  "${CMAKE_CURRENT_SOURCE_DIR}/xsd/ver_${PFC_SCHEMA_VERSION}"  
                    CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/xsd/ver_${PFC_SCHEMA_VERSION}/pfc_scenario.cfg"
                    PROJECT "generated/pfc" COMPONENT "schema" RESOURCE_FOLDER "resources/schema"
                    TARGETS XSD_TARGETS VERBOSITY 1)
add_source_files(XSD_HDRS LOCATION ${CMAKE_CURRENT_BINARY_DIR}/generated/pfc/schema 
                REGEX "*.hxx" SOURCE_GROUP  "Headers\\Generated\\schema")
add_source_files(XSD_SRCS LOCATION ${CMAKE_CURRENT_BINARY_DIR}/generated/pfc/schema 
                REGEX "*.cxx" SOURCE_GROUP  "Sources\\Generated\\schema")

mark_as_advanced(PFC_SCHEMA_VERSION)

set ( XSD_FILE_PREFIX "xsd/ver_${PFC_SCHEMA_VERSION}/")
set ( XSD_FILES
    "pfc_scenario.xsd"
    "pfc_scenario_complex_types.xsd"
    "msdl_simple_types_1.0.0.xsd"
    "msdl_complex_types_1.0.0.xsd"
    "msdl_codes_1.0.0.xsd"
    "military_scenario_1.0.0.xsd"
    "extern/jc3iedm_meterological.xsd"
    "extern/jc3iedm-3.1-codes-20061208.xsd"
    "extern/model_id_v2006_final.xsd"
  )

  function(generate_hex_header)
    set(options )
    set(oneValueArgs INPUT OUTPUT)
    set(multiValueArgs NAMESPACE)
    cmake_parse_arguments("" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    string (REPLACE ";" "\\;" _NS_SAFE "${_NAMESPACE}")

    set(_header ${_OUTPUT}.hxx)
    set(_source ${_OUTPUT}.cxx)
    if (_INPUT AND _OUTPUT)
      if( NOT EXISTS ${_header} OR NOT EXISTS ${_source} )
        get_filename_component(_source_name ${_OUTPUT} NAME)
        message(STATUS "\tGenerating ${_source_name}.{hxx,cxx}")
        execute_process( WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/
        COMMAND ${CMAKE_COMMAND} -DIN=${_INPUT} -DOUT=${_OUTPUT} -DNS=${_NS_SAFE}  -P ${PROJECT_SOURCE_DIR}/cmake/common/c_encode_file.cmake
                         ERROR_VARIABLE CMAKE_ERROR
                         #OUTPUT_QUIET
                        )
        if(CMAKE_ERROR)
          message(FATAL_ERROR "Failed to generate c_encoding\n"
                              "Error Msg:\n"
                              "${CMAKE_ERROR}\n"
                 )
        endif(CMAKE_ERROR)
     endif()
     add_custom_command(
        OUTPUT  "${_OUTPUT}.hxx" "${_OUTPUT}.cxx"
        DEPENDS ${_INPUT} ${PROJECT_SOURCE_DIR}/cmake/common/c_encode_file.cmake
        COMMAND ${CMAKE_COMMAND} -DIN="${_INPUT}" -DOUT="${_OUTPUT}" -DNS="${_NS_SAFE}"  -P "${PROJECT_SOURCE_DIR}/cmake/common/c_encode_file.cmake"
      )
      get_filename_component( content_name ${_INPUT} NAME_WE)
      
      add_custom_target( generate_${content_name}   DEPENDS ${_OUTPUT})
      set_target_properties(generate_${content_name} PROPERTIES 
        FOLDER "Code Generators"
      )
    else()    
      message(FATAL_ERROR "generate_hex_header requires arguments INPUT <file> and OUTPUT <file> ")
    endif()
  
  endfunction(generate_hex_header)

  foreach(schema IN LISTS XSD_FILES)
    message(STATUS "Tracking ${schema}")

    get_filename_component(_schema_we ${schema} NAME_WE )
    add_custom_command(
      OUTPUT  ${PROJECT_BINARY_DIR}/schema/${_schema_we}_${PFC_SCHEMA_VERSION}.xsd
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${XSD_FILE_PREFIX}${schema}
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}${XSD_FILE_PREFIX}${schema} ${PROJECT_BINARY_DIR}/schema/${_schema_we}_${PFC_SCHEMA_VERSION}.xsd
      COMMAND ${CMAKE_COMMAND} -E echo ${CMAKE_CURRENT_SOURCE_DIR}${XSD_FILE_PREFIX}${schema} ${PROJECT_BINARY_DIR}/schema/${_schema_we}_${PFC_SCHEMA_VERSION}.xsd
    )

    generate_hex_header( NAMESPACE pfc io INPUT ${CMAKE_CURRENT_SOURCE_DIR}/${XSD_FILE_PREFIX}${schema} OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated/pfc/c_encoded/ver_${PFC_SCHEMA_VERSION}/${schema} )
  endforeach()

  add_source_files(GEN_HDRS LOCATION ${CMAKE_CURRENT_BINARY_DIR}/generated/pfc/c_encoded/ver_${PFC_SCHEMA_VERSION}/
                REGEX "*.hxx" SOURCE_GROUP  "Headers\\Generated\\ver_${PFC_SCHEMA_VERSION}\\")
  add_source_files(GEN_SRCS LOCATION ${CMAKE_CURRENT_BINARY_DIR}/generated/pfc/c_encoded/ver_${PFC_SCHEMA_VERSION}/
                REGEX "*.cxx" SOURCE_GROUP  "Sources\\Generated\\ver_${PFC_SCHEMA_VERSION}\\")

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
add_library(pfc_serialization SHARED  ${XSD_SRCS} ${XSD_HDRS} ${XSD_RESOURCES})
target_link_libraries(pfc_serialization Xerces::xerces)
add_library( Builder::serial ALIAS pfc_serialization )
set_target_properties(pfc_serialization
  PROPERTIES
  DEFINE_SYMBOL "SUSTAIN_XSD_EXPORTS"
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
add_library(libScenarioBuilder STATIC ${${PREFIX}_SOURCES} ${${PREFIX}_HEADERS} ${XSD_RESOURCES})
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
 ${CMAKE_DL_LIBS}
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
                ${${PREFIX}_PRIVATE_INCLUDE_DIR}
				${CMAKE_CURRENT_BINARY_DIR}/generated/
				${Xerces-c_INCLUDE_DIR}
                $<INSTALL_INTERFACE:include>
)

if(CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(libScenarioBuilder PROPERTIES LINK_FLAGS ${${PREFIX}_LDFLAGS})
endif()

# GUI Executable
add_executable(ScenarioBuilder ${CMAKE_CURRENT_SOURCE_DIR}/cpp/main.cpp  ${qml_QRC}  ${QML_FILES} )
if(WIN32)
  target_sources(ScenarioBuilder  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Sustain.rc)
endif()
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
  OUTPUT_NAME "ScenarioBuilder"
  COMPILE_PDB_NAME "ScenarioBuilder"
  PROJECT_LABEL "ScenarioBuilder"
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
target_compile_definitions(ScenarioBuilder PRIVATE ${${PREFIX}_CPPFLAGS} PRIVATE $<$<PLATFORM_ID:Windows>:BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE> )
target_compile_options(ScenarioBuilder PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP>)

###############################################################################
# Unit Test
###############################################################################
option(BUILD_UNIT_TEST "Toogle the building of unit test GTest Suite required" ${GTest_Found})
if(BUILD_UNIT_TEST)
  find_package(GTest REQUIRED)
endif()
if (GTest_FOUND)
add_executable(unitScenarioBuilder ${UNIT_TEST})

target_link_libraries(unitScenarioBuilder Builder::lib GTest::GTest GTest::Main Boost::disable_autolinking Boost::filesystem minizip::lib)
set_target_properties(unitScenarioBuilder 
  PROPERTIES
  DEFINE_SYMBOL "SUSTAIN_FRAMEWORK_EXPORTS"
  FOLDER "Tests"
  OUTPUT_NAME "unitScenarioBuilder"
  COMPILE_PDB_NAME "unitScenarioBuildOneer"
  PROJECT_LABEL "unitScenarioBuilder"
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
target_compile_definitions(unitScenarioBuilder PRIVATE ${${PREFIX}_CPPFLAGS}  PRIVATE $<$<PLATFORM_ID:Windows>:BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE> )
target_compile_options(unitScenarioBuilder PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP>)
endif()
################################################################################
# Install
################################################################################
install(TARGETS ScenarioBuilder EXPORT ScenarioBuilder
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
  ARCHIVE DESTINATION lib
)
install(TARGETS libScenarioBuilder
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include
  PRIVATE_HEADER  DESTINATION include
)
install(TARGETS  pfc_serialization EXPORT ScenarioBuilder
  
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include
  PRIVATE_HEADER  DESTINATION include
)
install(DIRECTORY 
    ${${PREFIX}_GENERATED_INCLUDE_DIR}/pfc/schema
    DESTINATION include/pfc/
    FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    FILES_MATCHING 
	  PATTERN "*.hxx"
    )
  

  install(DIRECTORY 
    "${CMAKE_CURRENT_SOURCE_DIR}/xsd/ver_${PFC_SCHEMA_VERSION}"
    DESTINATION share/pfc/ScenarioBuilder/xsd
    FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    FILES_MATCHING 
	  PATTERN "*.xsd"
    )
  
install(DIRECTORY 
    ${${PREFIX}_GENERATED_INCLUDE_DIR}/pfc/c_encoded
    DESTINATION share/pfc/ScenarioBuilder/
    FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    )

################################################################################
message(STATUS "")
generate_project_suffix()
endif()
