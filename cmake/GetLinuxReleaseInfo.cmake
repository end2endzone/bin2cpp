
#! GetLinuxReleaseInfo : Read the linux distribution info and store the results into output variables.
#
# \arg:output_name_var The name of the output variable that contains the name of the distribution.
# \arg:output_ver_var  The name of the output variable that contains the version of the distribution.
#
function(GetLinuxReleaseInfo output_name_var output_ver_var)
  # Set a defaut values for output
  set(${output_name_var} "unknown" PARENT_SCOPE)
  set(${output_ver_var} "0" PARENT_SCOPE)

  if (UNIX)
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
      
      set(${output_name_var} "${ID}" PARENT_SCOPE)
      set(${output_ver_var} "${VERSION_ID}" PARENT_SCOPE)
    endif()
  endif()
endfunction()
