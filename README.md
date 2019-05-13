Repository for the scenario builder. 
---------------------------------------

Functionality
===============
This project represents a QT5 based Scenario Builder for the PFC MSDL_SISO-STD-007-2008_ based Scenario format.

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

