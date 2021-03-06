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
      string(REGEX REPLACE ",$" "" c_tuples ${c_tuples})
      string(MAKE_C_IDENTIFIER  "${content_name}" safe_content_name)

      #Header File
      set(header_file "${_OUTPUT}.hxx")
      file (WRITE ${header_file} "#pragma once\n")
      file (APPEND ${header_file} "\n" 
                                  "#include <cstddef>" 
                                  "\n"
    )
      foreach (_ns IN LISTS _NAMESPACE)
         file(APPEND ${header_file} "namespace ${_ns} { \n")
      endforeach()
      file (APPEND ${header_file} 
                   "  unsigned char const*  get_${safe_content_name}_text();\n"
                   "  std::size_t  ${safe_content_name}_text_size();\n"
      )
      foreach (_ns IN LISTS _NAMESPACE)
         file(APPEND ${header_file} "}\n")
      endforeach()


      #Source File
      set(source_file "${_OUTPUT}.cxx")
      get_filename_component( import_name ${_INPUT} NAME)
      file (WRITE ${source_file} "#include \"${import_name}.hxx\"\n")
      file (APPEND ${source_file} "\n#include <cstddef>\n")
      file (APPEND ${source_file} "\n\n" )

      foreach (_ns IN LISTS _NAMESPACE)
         file(APPEND ${source_file} "namespace ${_ns} { \n")
      endforeach()
      file (APPEND ${source_file} 
                   " unsigned char const ${safe_content_name}_text[]={\n        "
                   ${c_tuples}
                    "\n};\n"
                   "  constexpr std::size_t  size_of_${safe_content_name}=sizeof(${safe_content_name}_text);\n"
                   "\n\n"
                   "  unsigned char const*  get_${safe_content_name}_text(){ \n"
                   "    return ${safe_content_name}_text;\n"
                   "  }\n\n"
                   "  std::size_t ${safe_content_name}_text_size() {"
                   "    return size_of_${safe_content_name};\n"
                   "  }\n\n"
      )
      foreach (_ns IN LISTS _NAMESPACE)
         file(APPEND ${source_file} "}\n")
      endforeach()


    else()    
      message(FATAL_ERROR "generate_hex_header requires arguments INPUT <file> and OUTPUT <file> ")
    endif()
  endfunction(generate_hex_header)

  generate_hex_header(INPUT "${IN}" OUTPUT "${OUT}" NAMESPACE "${NS}")
