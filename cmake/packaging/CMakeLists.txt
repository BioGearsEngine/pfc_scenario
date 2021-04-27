cmake_policy(SET CMP0087 NEW)

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "Sustain Scenario Builder")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Interface for developing Sustain Scenario Files")
set(CPACK_PACKAGE_VENDOR "Applied Resaerch Associates")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_HOMEPAGE_URL "http://TheSustainTrainer.com")
set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/sustainlogo_SNo_icon.ico")

set(CPACK_PACKAGE_INSTALL_DIRECTORY "Sustain/${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_MAJOR ${${ROOT_PROJECT_NAME}_VERSION_MAJOR} )      
set(CPACK_PACKAGE_VERSION_MINOR ${${ROOT_PROJECT_NAME}_VERSION_MINOR} )
set(CPACK_PACKAGE_VERSION_PATCH ${${ROOT_PROJECT_NAME}_VERSION_PATCH} )
set(CPACK_PACKAGE_VERSION_TWEAK ${${ROOT_PROJECT_NAME}_VERSION_TWEAK} )

set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/packaging/)

if(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full UNIX paths properly.
  # Make sure there is at least one set of four backlashes.
  
  set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\SustainLogo.png")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "${CMAKE_CURRENT_SOURCE_DIR}/sustainlogo_SNo_icon.ico")
  set(CPACK_NSIS_DISPLAY_NAME "Sustain Scenario Builder")
  set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.TheSustainTrainer.com")
  set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.TheSustainTrainer.com")
  set(CPACK_NSIS_CONTACT "admin@TheSustainTrainer.com")
  set(CPACK_NSIS_MODIFY_PATH ON)
  set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

  set(CPACK_WIX_UPGRADE_GUID "8502aed2-9ff7-4171-aa51-b4efbfee71f3")
  set(CPACK_WIX_PRODUCT_ICON "${CMAKE_CURRENT_SOURCE_DIR}/sustainlogo_SNo_icon.ico")
  set(CPACK_WIX_PROGRAM_MENU_FOLDER "Sustain")
  set(CPACK_WIX_UI_BANNER "${CMAKE_CURRENT_SOURCE_DIR}/WIX_UI_BANNER.png")
  set(CPACK_WIX_UI_DIALOG "${CMAKE_CURRENT_SOURCE_DIR}/WIX_UI_DIALOG.png")
  set(CPACK_WIX_PROPERTY_ARPCOMMENTS "Scenario File Editor ${${ROOT_PROJECT_NAME}_VERSION_TAG}")
  set(CPACK_WIX_PROPERTY_ARPHELPLINK  "https://TheSustainTrainer.com")
  set(ARPURLINFOABOUT  "https://TheSustainTrainer.com")

  get_filename_component(Qt5_ROOT_DIR "${Qt5_DIR}/../../../" ABSOLUTE  )

  install(CODE "
    include(BundleUtilities)

    string(TOUPPER \${CMAKE_INSTALL_CONFIG_NAME} CONFIG)
    if( NOT UNIX )
      set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY \${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_\${CONFIG}} )
      set( CMAKE_LIBRARY_OUTPUT_DIRECTORY \${CMAKE_LIBRARY_OUTPUT_DIRECTORY_\${CONFIG}} )
      set( CMAKE_RUNTIME_OUTPUT_DIRECTORY \${CMAKE_RUNTIME_OUTPUT_DIRECTORY_\${CONFIG}} )
    endif()
    

    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})

    foreach(_dir IN LISTS CMAKE_PREFIX_PATH)
      list(APPEND THIRD_PARTY  \"\${_dir}\")
      list(APPEND THIRD_PARTY_LIB \"\${_dir}/lib\")
      list(APPEND THIRD_PARTY_BIN \"\${_dir}/bin\")
    endforeach()

    message(\" Generating Bundle for \${CMAKE_INSTALL_PREFIX}/bin/ScenarioBuilder.exe\")
    fixup_bundle(\${CMAKE_INSTALL_PREFIX}/bin/$<TARGET_FILE_NAME:ScenarioBuilder>
                    \"\"
                \"\${THIRD_PARTY_LIB};\${THIRD_PARTY_BIN}\"
    )
    if(WIN32)
        message(\" Running windeployqt for \${CMAKE_INSTALL_PREFIX}/bin/ScenarioBuilder.exe\")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E env \"${Qt5_ROOT_DIR}/bin/windeployqt\" --no-compiler-runtime --list mapping \"\${CMAKE_INSTALL_PREFIX}/bin/$<TARGET_FILE_NAME:ScenarioBuilder>\"
            WORKING_DIRECTORY  \"\${CMAKE_INSTALL_PREFIX}/\"
            OUTPUT_VARIABLE _output
            ERROR_VARIABLE  _error
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if(_output)
          message(\"windeployqt OUTPUT : \${_output}\")
        endif()
        if( _error )
          message(\"windeployqt ERROR : \${_error}\")
        endif()
    endif()
  "         
  )

  install(DIRECTORY 
      ${Qt5_ROOT_DIR}/plugins/
      DESTINATION plugins/
      FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
      FILES_MATCHING 
      PATTERN "*.dll"
      )   
install(DIRECTORY 
    ${Qt5_ROOT_DIR}/qml/
    DESTINATION qml/
    FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    )

    install( FILES
    ${Qt5_ROOT_DIR}/bin/qt.conf
    DESTINATION bin/
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    )  

else()
  set(CPACK_STRIP_FILES "")
  set(CPACK_SOURCE_STRIP_FILES "")
endif()
set(CPACK_PACKAGE_EXECUTABLES "ScenarioBuilder" "Scenario Builder")

include(CPack)
