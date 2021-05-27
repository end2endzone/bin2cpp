
#! GetWindowsReleaseInfo : Read the Windows release information and store the results into output variables.
#
# \arg:output_name_var The name of the output variable that contains the marketing name of the Windows version.
# \arg:output_ver_var  The name of the output variable that contains the version of Windows.
#
function(GetWindowsReleaseInfo output_name_var output_ver_var)
  # Set a defaut values for output
  set(${output_name_var} "unknown" PARENT_SCOPE)
  set(${output_ver_var} "0" PARENT_SCOPE)

  if (WIN32)
    # Get OS properties
    execute_process(COMMAND wmic os get Caption,Version,OSArchitecture /value
        OUTPUT_VARIABLE WMIC_OUTPUT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    STRING(REGEX REPLACE ";" "\\\\;" WMIC_OUTPUT "${WMIC_OUTPUT}")
    STRING(REGEX REPLACE "\n" ";" WMIC_OUTPUT "${WMIC_OUTPUT}")
    STRING(REGEX REPLACE "\"" "" WMIC_OUTPUT "${WMIC_OUTPUT}")
    
    # Parse a file in "name=value" format.
    # See https://stackoverflow.com/questions/17165905/how-to-load-variables-in-a-bar-foo-syntax-in-cmake
    foreach(NameAndValue ${WMIC_OUTPUT})
      # Strip leading spaces
      string(REGEX REPLACE "^[ ]+" "" NameAndValue ${NameAndValue})
      # Find variable name
      string(REGEX MATCH "^[^=]+" Name ${NameAndValue})
      # Find the value
      string(REPLACE "${Name}=" "" Value ${NameAndValue})
      # Set the variable
      set(${Name} "${Value}")
    endforeach()
    
    set(${output_name_var} "${Caption}" PARENT_SCOPE)
    set(${output_ver_var} "${Version}" PARENT_SCOPE)
    
  endif()
endfunction()
