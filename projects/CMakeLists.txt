enable_language(CXX)

if(WIN32)
  set(LIBRARY_INSTALL_DIR bin)
else()
  set(LIBRARY_INSTALL_DIR lib)
endif()

set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
set(CMAKE_STATIC_LIBRARY_SUFFIX "_st${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(CMAKE_SHARED_LIBRARY_PREFIX "lib")

  
set(OUTPUT_PREFIX ${CMAKE_BINARY_DIR}/outputs)
if(UNIX)
    foreach(OUTPUTCONFIG Release Debug)
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/lib")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/lib")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/bin")
    endforeach(OUTPUTCONFIG)
else()
    foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/lib")
        set(CMAKE_PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/lib")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/bin")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/bin")
    endforeach(OUTPUTCONFIG)
endif()

set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS system thread program_options filesystem log REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets Gui Sql Quick QuickControls2 Qml REQUIRED)
###############################################################################
# Step 2: Project Includes are generally order dependent.  So manually maintain
#         this list
###############################################################################
add_subdirectory(scenario_builder)
add_subdirectory(zip)
###############################################################################
# Step 3: Global Doxygen configuration 
#         you might need to tweak this if you have multiple doxy files
###############################################################################
