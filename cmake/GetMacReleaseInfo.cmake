
#! GetMacReleaseInfo : Read the macos release properties and store the results into output variables.
#
# \arg:output_name_var The name of the output variable that contains the name of the OS release.
# \arg:output_ver_var  The name of the output variable that contains the version of the OS.
#
function(GetMacReleaseInfo output_name_var output_ver_var)
  # Set a defaut values for output
  set(${output_name_var} "unknown" PARENT_SCOPE)
  set(${output_ver_var} "0" PARENT_SCOPE)

  if (APPLE)
    # Apple does not provide a consistent method for getting the version codename.
    # See https://apple.stackexchange.com/questions/333452/how-can-i-find-the-friendly-name-of-the-operating-system-from-the-shell-term
    # and https://apple.stackexchange.com/questions/341551/apples-online-version-function-support-sp-apple-com-sp-productedid-does-not-w
    # The best is to define the os codename from from the version itself.
    # The following list is from https://support.apple.com/en-us/HT201260
    
    # Get the OS version
    execute_process(COMMAND sw_vers -productVersion
        OUTPUT_VARIABLE OS_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(${output_ver_var} "${OS_VERSION}" PARENT_SCOPE)
    
    # Split each digit in a list
    string(REPLACE "." ";" OS_VERSION_LIST ${OS_VERSION})
    list(GET OS_VERSION_LIST 0 OS_VERSION_MAJOR)
    list(GET OS_VERSION_LIST 1 OS_VERSION_MINOR)
    list(GET OS_VERSION_LIST 2 OS_VERSION_PATCH)
    
    #  | macOS                 | Latest version |
    #  |-----------------------|----------------|
    #  | macOS Big Sur         | 11.3.1         |
    #  | macOS Catalina        | 10.15.7        |
    #  | macOS Mojave          | 10.14.6        |
    #  | macOS High Sierra     | 10.13.6        |
    #  | macOS Sierra          | 10.12.6        |
    #  | OS X El Capitan       | 10.11.6        |
    #  | OS X Yosemite         | 10.10.5        |
    #  | OS X Mavericks        | 10.9.5         |
    #  | OS X Mountain Lion    | 10.8.5         |
    #  | OS X Lion             | 10.7.5         |
    #  | Mac OS X Snow Leopard | 10.6.8         |
    #  | Mac OS X Leopard      | 10.5.8         |
    #  | Mac OS X Tiger        | 10.4.11        |
    #  | Mac OS X Panther      | 10.3.9         |
    #  | Mac OS X Jaguar       | 10.2.8         |
    #  | Mac OS X Puma         | 10.1.5         |
    #  | Mac OS X Cheetah      | 10.0.4         |
    
    if     (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "0")
      set(OS_NAME "Mac OS X Cheetah")
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "1")
      set(OS_NAME "Mac OS X Puma")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "2")
      set(OS_NAME "Mac OS X Jaguar")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "3")
      set(OS_NAME "Mac OS X Panther")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "4")
      set(OS_NAME "Mac OS X Tiger")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "5")
      set(OS_NAME "Mac OS X Leopard")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "6")
      set(OS_NAME "Mac OS X Snow Leopard")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "7")
      set(OS_NAME "OS X Lion")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "8")
      set(OS_NAME "OS X Mountain Lion")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "9")
      set(OS_NAME "OS X Mavericks")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "10")
      set(OS_NAME "OS X Yosemite")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "11")
      set(OS_NAME "OS X El Capitan")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "12")
      set(OS_NAME "macOS Sierra")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "13")
      set(OS_NAME "macOS High Sierra")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "14")
      set(OS_NAME "macOS Mojave")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MINOR STREQUAL "15")
      set(OS_NAME "macOS Catalina")  
    elseif (OS_VERSION_MAJOR STREQUAL "11" AND OS_VERSION_MINOR STREQUAL "3")
      set(OS_NAME "macOS Big Sur")  
    else()
      set(OS_NAME "macOS Unknown")
    endif()
    
    # Set output name variable
    set(${output_name_var} "${OS_NAME}" PARENT_SCOPE)
    
  endif()
endfunction()
