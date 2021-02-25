Repository for the scenario builder. 
---------------------------------------

Functionality
===============
This project represents a QT5 based Scenario Builder for the PFC MSDL_SISO-STD-007-2008_ based Scenario format.


Build Status
-----------------
| Platform | Compiler | Architecture | Status |
|----------|----------|--------------|--------|
| Windows  |  msvc15  | amd64        | ![Windows msvc15 Build  Status](https://biogearsengine.com/content/badges/pfc_windows_msvc15.png) |
| Linux  |  gcc9  | amd64 | ![Linux-gcc9-amd64 Build Status](https://biogearsengine.com/content/badges/pfc_linux-gcc9-amd64.png) |

Folder structure 
=====================
 * projects/ _-- Sub Projects_
   *  Scenario_-- Builder/ _The main UI_
   *   /cpp/ _-- C++ Source_
   *  /qml/ _-- QT5 QML Source_
   *   /unit_test/ _google test based unit test_
   *   /xsd/ _-- MSDL xsd and generate c++ source code_
 * cmake/ _-- Additional C++ Source code_

Build instructions
=======================
Install XSD Code Synthesis
Install QT5 Build Libraries
Install CMake 3.12 or higher
Create a Build Directory
Run CMake from the build directory to generate your build files
Build using the build tools selected in cmake

