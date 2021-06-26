#! GetOsReleaseInfo :
#    Identifies the Operating System's information and store the results into the given output variables.
#
# \arg:output_name     The name of the output variable that contains the full name of the OS release.
# \arg:output_version  The name of the output variable that contains the version   of the OS release.
#
# For examples:
#   "Microsoft Windows 7 Home Premium","6.1.7601"
#   "Ubuntu","18.04"
#   "macOS Catalina","10.15.7"
#
function(GetOsReleaseInfo output_name output_version)
  # Set a defaut values for output
  set(${output_name}    "unknown" PARENT_SCOPE)
  set(${output_version} "0"       PARENT_SCOPE)

  if (WIN32)
    # Get OS properties
    execute_process(COMMAND wmic os get Caption,Version,OSArchitecture /value
        OUTPUT_VARIABLE WMIC_OUTPUT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    # Split output as a cmake list, each with line in "name=value" format.
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
    
    set(${output_name}    "${Caption}" PARENT_SCOPE)
    set(${output_version} "${Version}" PARENT_SCOPE)
    
  elseif (APPLE)
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
    set(${output_name}    "${OS_NAME}"    PARENT_SCOPE)
    set(${output_version} "${OS_VERSION}" PARENT_SCOPE)
    
  elseif (UNIX)
    # Searching for the OS name as described in the following: 
    # https://unix.stackexchange.com/questions/6345/how-can-i-get-distribution-name-and-version-number-in-a-simple-shell-script
    if(EXISTS "/etc/os-release")
      execute_process(COMMAND cat /etc/os-release
          OUTPUT_VARIABLE OSRELEASE_OUTPUT
          OUTPUT_STRIP_TRAILING_WHITESPACE
      )

      # Split output as a cmake list, each with line in "name=value" format.
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

      # The command "cat /etc/os-release" on a Raspberry Pi 2 outputs the following :
      #   PRETTY_NAME="Raspbian GNU/Linux 9 (stretch)"
      #   NAME="Raspbian GNU/Linux"
      #   VERSION_ID="9"
      #   VERSION="9 (stretch)"
      #   VERSION_CODENAME=stretch
      #   ID=raspbian
      #   ID_LIKE=debian
      #   HOME_URL="http://www.raspbian.org/"
      #   SUPPORT_URL="http://www.raspbian.org/RaspbianForums"
      #   BUG_REPORT_URL="http://www.raspbian.org/RaspbianBugs"
      
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
      
      set(${output_name}    "${ID}"         PARENT_SCOPE)
      set(${output_version} "${VERSION_ID}" PARENT_SCOPE)
      
    elseif (EXISTS "/usr/bin/lsb_release")
      # If it fails, try `/usr/bin/lsb_release`.
    
      # Get the name of the distribution first
      execute_process(COMMAND lsb_release -si
          OUTPUT_VARIABLE LSBRELEASE_OUTPUT
          OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      set(${output_name}    "${LSBRELEASE_OUTPUT}" PARENT_SCOPE)
      
      # Get the version of the distribution first
      execute_process(COMMAND lsb_release -sr
          OUTPUT_VARIABLE LSBRELEASE_OUTPUT
          OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      set(${output_version} "${LSBRELEASE_OUTPUT}" PARENT_SCOPE)
     
    elseif (EXISTS "/etc/lsb-release")
      # If it fails, try `/etc/lsb-release`.
    
      execute_process(COMMAND cat /etc/lsb-release
          OUTPUT_VARIABLE LSBRELEASE_OUTPUT
          OUTPUT_STRIP_TRAILING_WHITESPACE
      )

      # Split output as a cmake list, each with line in "name=value" format.
      STRING(REGEX REPLACE ";" "\\\\;" LSBRELEASE_OUTPUT "${LSBRELEASE_OUTPUT}")
      STRING(REGEX REPLACE "\n" ";" LSBRELEASE_OUTPUT "${LSBRELEASE_OUTPUT}")
      STRING(REGEX REPLACE "\"" "" LSBRELEASE_OUTPUT "${LSBRELEASE_OUTPUT}")
      
      # The command "cat /etc/lsb-release" on Ubuntu outputs the following :
      #   DISTRIB_ID=Ubuntu
      #   DISTRIB_RELEASE=18.04
      #   DISTRIB_CODENAME=bionic
      #   DISTRIB_DESCRIPTION="Ubuntu 18.04.2 LTS"

      # The command "cat /etc/lsb-release" on a Raspberry Pi
      # cat: /etc/lsb-release: No such file or directory
      
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
      
      set(${output_name}    "${DISTRIB_ID}"      PARENT_SCOPE)
      set(${output_version} "${DISTRIB_RELEASE}" PARENT_SCOPE)
      
    else()
      # Fallback to using `uname` and CMAKE_SYSTEM_VERSION.
      
      # The command "uname" on Ubuntu outputs the following :
      #   uname -a --> Linux myubuntu 5.4.0-73-generic #82~18.04.1-Ubuntu SMP Fri Apr 16 15:10:02 UTC 2021 x86_64 x86_64 x86_64 GNU/Linux
      #   uname -s --> Linux  --> CMAKE_SYSTEM_NAME
      #   uname -m --> x86_64 --> CMAKE_HOST_SYSTEM_PROCESSOR
      #   uname -p --> x86_64 --> CMAKE_SYSTEM_PROCESSOR
      
      # The command "uname" on Raspberry Pi 2 outputs the following :
      #   uname -a --> Linux raspberrypi 4.19.66-v7+ #1253 SMP Thu Aug 15 11:49:46 BST 2019 armv7l GNU/Linux
      #   uname -s --> Linux   --> CMAKE_SYSTEM_NAME
      #   uname -m --> armv7l  --> CMAKE_HOST_SYSTEM_PROCESSOR
      #   uname -p --> unknown --> CMAKE_SYSTEM_PROCESSOR
      
      set(${output_name}    "${CMAKE_SYSTEM_NAME}"    PARENT_SCOPE)
      set(${output_version} "${CMAKE_SYSTEM_VERSION}" PARENT_SCOPE)
      
    endif()
  endif()
  
endfunction()
