
#! GetOsNameAndVersion :
#    Identifies the Operating System's properties.
#    After executing this function, the following properties will be set : OS_FRIENDLY_NAME, OS_VERSION
#    Examples:
#      OS_FRIENDLY_NAME,OS_VERSION
#      "Microsoft Windows 7 Home Premium","6.1.7601"
#      "Ubuntu","18.04"
#      "macOS Catalina","10.15.7"
#
function(GetOsNameAndVersion)

  if (WIN32)
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
    
    set(OS_FRIENDLY_NAME "${Caption}")
    set(OS_VERSION "${Version}")
    
  elseif (APPLE)
    # Apple does not provide a consistent method for getting the version codename.
    # See https://apple.stackexchange.com/questions/333452/how-can-i-find-the-friendly-name-of-the-operating-system-from-the-shell-term
    # and https://apple.stackexchange.com/questions/341551/apples-online-version-function-support-sp-apple-com-sp-productedid-does-not-w
    # The best is to define the os codename from from the version itself.
    # The following list is from https://support.apple.com/en-us/HT201260
    
    execute_process(COMMAND sw_vers -productVersion
        OUTPUT_VARIABLE OS_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
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
    
    if     (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "0")
      set(OS_FRIENDLY_NAME "Mac OS X Cheetah")
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "1")
      set(OS_FRIENDLY_NAME "Mac OS X Puma")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "2")
      set(OS_FRIENDLY_NAME "Mac OS X Jaguar")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "3")
      set(OS_FRIENDLY_NAME "Mac OS X Panther")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "4")
      set(OS_FRIENDLY_NAME "Mac OS X Tiger")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "5")
      set(OS_FRIENDLY_NAME "Mac OS X Leopard")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "6")
      set(OS_FRIENDLY_NAME "Mac OS X Snow Leopard")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "7")
      set(OS_FRIENDLY_NAME "OS X Lion")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "8")
      set(OS_FRIENDLY_NAME "OS X Mountain Lion")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "9")
      set(OS_FRIENDLY_NAME "OS X Mavericks")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "10")
      set(OS_FRIENDLY_NAME "OS X Yosemite")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "11")
      set(OS_FRIENDLY_NAME "OS X El Capitan")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "12")
      set(OS_FRIENDLY_NAME "macOS Sierra")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "13")
      set(OS_FRIENDLY_NAME "macOS High Sierra")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "14")
      set(OS_FRIENDLY_NAME "macOS Mojave")  
    elseif (OS_VERSION_MAJOR STREQUAL "10" AND OS_VERSION_MAJOR STREQUAL "15")
      set(OS_FRIENDLY_NAME "macOS Catalina")  
    elseif (OS_VERSION_MAJOR STREQUAL "11" AND OS_VERSION_MAJOR STREQUAL "3")
      set(OS_FRIENDLY_NAME "macOS Big Sur")  
    else()
      set(OS_FRIENDLY_NAME "macOS Unknown")
    endif()
  elseif (UNIX)
    # Searching for the OS name as described in the following: 
    # https://unix.stackexchange.com/questions/6345/how-can-i-get-distribution-name-and-version-number-in-a-simple-shell-script
    if(EXISTS "/etc/os-release")
      execute_process(COMMAND cat /etc/os-release
          OUTPUT_VARIABLE OSRELEASE_OUTPUT
          OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      STRING(REGEX REPLACE ";" "\\\\;" OSRELEASE_OUTPUT "${OSRELEASE_OUTPUT}")
      STRING(REGEX REPLACE "\n" ";" OSRELEASE_OUTPUT "${OSRELEASE_OUTPUT}")
      STRING(REGEX REPLACE "\"" "" OSRELEASE_OUTPUT "${OSRELEASE_OUTPUT}")
      
      # The command "cat /etc/os-release" on Ubuntu outputs the following :
      #   NAME="Ubuntu"
      #   VERSION="18.04.2 LTS (Bionic Beaver)"
      #   ID=ubuntu
      #   ID_LIKE=debian
      #   PRETTY_NAME="Ubuntu 18.04.2 LTS"
      #   VERSION_ID="18.04"
      #   HOME_URL="https://www.ubuntu.com/"
      #   SUPPORT_URL="https://help.ubuntu.com/"
      #   BUG_REPORT_URL="https://bugs.launchpad.net/ubuntu/"
      #   PRIVACY_POLICY_URL="https://www.ubuntu.com/legal/terms-and-policies/privacy-policy"
      #   VERSION_CODENAME=bionic
      #   UBUNTU_CODENAME=bionic
      
      # Parse a file in "name=value" format.
      # See https://stackoverflow.com/questions/17165905/how-to-load-variables-in-a-bar-foo-syntax-in-cmake
      foreach(NameAndValue ${OSRELEASE_OUTPUT})
        # Strip leading spaces
        string(REGEX REPLACE "^[ ]+" "" NameAndValue ${NameAndValue})
        # Find variable name
        string(REGEX MATCH "^[^=]+" Name ${NameAndValue})
        # Find the value
        string(REPLACE "${Name}=" "" Value ${NameAndValue})
        # Set the variable
        set(${Name} "${Value}")
      endforeach()
      
      set(OS_FRIENDLY_NAME "${ID}")
      set(OS_VERSION "${VERSION_ID}")
    elseif(EXISTS "/usr/bin/lsb_release")
      execute_process(COMMAND lsb_release -si
          OUTPUT_VARIABLE LSBRELEASE_OUTPUT
          OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      set(OS_FRIENDLY_NAME "${LSBRELEASE_OUTPUT}")
      
      execute_process(COMMAND lsb_release -sr
          OUTPUT_VARIABLE LSBRELEASE_OUTPUT
          OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      set(OS_VERSION "${LSBRELEASE_OUTPUT}")
    elseif(EXISTS "/etc/lsb-release")
      execute_process(COMMAND cat /etc/lsb-release
          OUTPUT_VARIABLE LSBRELEASE_OUTPUT
          OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      STRING(REGEX REPLACE ";" "\\\\;" LSBRELEASE_OUTPUT "${LSBRELEASE_OUTPUT}")
      STRING(REGEX REPLACE "\n" ";" LSBRELEASE_OUTPUT "${LSBRELEASE_OUTPUT}")
      STRING(REGEX REPLACE "\"" "" LSBRELEASE_OUTPUT "${LSBRELEASE_OUTPUT}")
      
      # The command "cat /etc/lsb-release" on Ubuntu outputs the following :
      #   DISTRIB_ID=Ubuntu
      #   DISTRIB_RELEASE=18.04
      #   DISTRIB_CODENAME=bionic
      #   DISTRIB_DESCRIPTION="Ubuntu 18.04.2 LTS"
      
      # Parse a file in "name=value" format.
      # See https://stackoverflow.com/questions/17165905/how-to-load-variables-in-a-bar-foo-syntax-in-cmake
      foreach(NameAndValue ${LSBRELEASE_OUTPUT})
        # Strip leading spaces
        string(REGEX REPLACE "^[ ]+" "" NameAndValue ${NameAndValue})
        # Find variable name
        string(REGEX MATCH "^[^=]+" Name ${NameAndValue})
        # Find the value
        string(REPLACE "${Name}=" "" Value ${NameAndValue})
        # Set the variable
        set(${Name} "${Value}")
      endforeach()
      
      set(OS_FRIENDLY_NAME "${DISTRIB_ID}")
      set(OS_VERSION "${DISTRIB_RELEASE}")
    else()
      # Fallback to using `uname -s` and CMAKE_SYSTEM_VERSION.
      set(OS_FRIENDLY_NAME "${CMAKE_SYSTEM_NAME}")
      set(OS_VERSION "${CMAKE_SYSTEM_VERSION}")
    endif()   
  endif()
  
  # Make both variables visible to the calling script
  set(OS_FRIENDLY_NAME "${OS_FRIENDLY_NAME}" PARENT_SCOPE)
  set(OS_VERSION "${OS_VERSION}" PARENT_SCOPE)
  
  #MESSAGE( "Found OS_FRIENDLY_NAME : " ${OS_FRIENDLY_NAME} )
  #MESSAGE( "Found OS_VERSION :       " ${OS_VERSION} )
  
endfunction()
