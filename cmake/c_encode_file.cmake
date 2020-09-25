  function(generate_hex_header)
    set(options )
    set(oneValueArgs INPUT OUTPUT)
    set(multiValueArgs NAMESPACE)
    cmake_parse_arguments("" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    if (_INPUT AND _OUTPUT)
      file (READ ${_INPUT}  _content  HEX)
      get_filename_component( content_name ${_INPUT} NAME_WE)
      string(REPEAT "[0-9a-f]" 32 TOUPLE_LIMIT)

      string(REGEX REPLACE "(${TOUPLE_LIMIT})" "\\1\n\        " c_tuples "${_content}")
      string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " c_tuples ${c_tuples})
      string(REGEX REPLACE ", " "," c_tuples ${c_tuples})
      string(REGEX REPLACE ", $" "" c_tuples ${c_tuples})
      file (WRITE ${_OUTPUT} "#pragma once\n")
      file (APPEND ${_OUTPUT} "\n\n" )

      foreach (_ns IN LISTS _NAMESPACE)
         file(APPEND ${_OUTPUT} "namespace ${_ns} { \n")
      endforeach()

      file (APPEND ${_OUTPUT} "constexpr char  ${content_name}[]={\n        ") 
      file (APPEND ${_OUTPUT} ${c_tuples})
      file (APPEND ${_OUTPUT}  "\n};\n\n")

      foreach (_ns IN LISTS _NAMESPACE)
         file(APPEND ${_OUTPUT} "}\n")
      endforeach()

    else()    
      message(FATAL_ERROR "generate_hex_header requires arguments INPUT <file> and OUTPUT <file> ")
    endif()
  endfunction(generate_hex_header)


  generate_hex_header(INPUT ${IN} OUTPUT ${OUT} NAMESPACE ${NS})