# CMakeLists.txt
cmake_minimum_required(VERSION 3.4.3 FATAL_ERROR)
project(bin2cpp)

#Find optional packages.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(WIX)
find_package(NSIS)

##############################################################################################################################################
# Standard CMake variables
##############################################################################################################################################

# Set a default build type if none was specified.
# See https://blog.kitware.com/cmake-and-the-default-build-type/
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  mark_as_advanced(CMAKE_BUILD_TYPE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(   LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

##############################################################################################################################################
# CMake properties
##############################################################################################################################################
MESSAGE( STATUS "PROJECT_NAME:                " ${PROJECT_NAME} )
MESSAGE( STATUS "CMAKE_BINARY_DIR:            " ${CMAKE_BINARY_DIR} )
MESSAGE( STATUS "CMAKE_CURRENT_BINARY_DIR:    " ${CMAKE_CURRENT_BINARY_DIR} )
MESSAGE( STATUS "CMAKE_SOURCE_DIR:            " ${CMAKE_SOURCE_DIR} )
MESSAGE( STATUS "CMAKE_CURRENT_SOURCE_DIR:    " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( STATUS "PROJECT_BINARY_DIR:          " ${PROJECT_BINARY_DIR} )
MESSAGE( STATUS "PROJECT_SOURCE_DIR:          " ${PROJECT_SOURCE_DIR} )
MESSAGE( STATUS "EXECUTABLE_OUTPUT_PATH:      " ${EXECUTABLE_OUTPUT_PATH} )
MESSAGE( STATUS "LIBRARY_OUTPUT_PATH:         " ${LIBRARY_OUTPUT_PATH} )
MESSAGE( STATUS "CMAKE_MODULE_PATH:           " ${CMAKE_MODULE_PATH} )
MESSAGE( STATUS "CMAKE_COMMAND:               " ${CMAKE_COMMAND} )
MESSAGE( STATUS "CMAKE_ROOT:                  " ${CMAKE_ROOT} )
MESSAGE( STATUS "CMAKE_CURRENT_LIST_FILE:     " ${CMAKE_CURRENT_LIST_FILE} )
MESSAGE( STATUS "CMAKE_CURRENT_LIST_LINE:     " ${CMAKE_CURRENT_LIST_LINE} )
MESSAGE( STATUS "CMAKE_INCLUDE_PATH:          " ${CMAKE_INCLUDE_PATH} )
MESSAGE( STATUS "CMAKE_LIBRARY_PATH:          " ${CMAKE_LIBRARY_PATH} )
MESSAGE( STATUS "CMAKE_SYSTEM:                " ${CMAKE_SYSTEM} )
MESSAGE( STATUS "CMAKE_SYSTEM_NAME:           " ${CMAKE_SYSTEM_NAME} )
MESSAGE( STATUS "CMAKE_SYSTEM_VERSION:        " ${CMAKE_SYSTEM_VERSION} )
MESSAGE( STATUS "CMAKE_SYSTEM_PROCESSOR:      " ${CMAKE_SYSTEM_PROCESSOR} )
MESSAGE( STATUS "CMAKE_HOST_SYSTEM_PROCESSOR: " ${CMAKE_HOST_SYSTEM_PROCESSOR} )
MESSAGE( STATUS "CMAKE_SYSTEM_INFO_FILE:      " ${CMAKE_SYSTEM_INFO_FILE} )

# Get OS name and version.
include(GetOsNameAndVersion)
GetOsNameAndVersion()
MESSAGE( STATUS "OS_FRIENDLY_NAME:            " ${OS_FRIENDLY_NAME} )
MESSAGE( STATUS "OS_VERSION:                  " ${OS_VERSION} )


##############################################################################################################################################
# Global settings
##############################################################################################################################################

# Product version according to Semantic Versioning v2.0.0 https://semver.org/
SET(BIN2CPP_VERSION_MAJOR 2)
SET(BIN2CPP_VERSION_MINOR 4)
SET(BIN2CPP_VERSION_PATCH 0)
set(BIN2CPP_VERSION ${BIN2CPP_VERSION_MAJOR}.${BIN2CPP_VERSION_MINOR}.${BIN2CPP_VERSION_PATCH})

# Create a c++ file header from the project LICENSE file.
# The c++ header will be added to all generated files.
include(MakeCplusplusHeader)
MakeCplusplusHeader(${CMAKE_SOURCE_DIR}/LICENSE LICENSE_CPLUSPLUS_HEADER)

# version.h file
set(BIN2CPP_VERSION_HEADER ${CMAKE_BINARY_DIR}/include/bin2cpp/version.h)
message("Generating ${BIN2CPP_VERSION_HEADER}...")
configure_file( ${CMAKE_SOURCE_DIR}/src/bin2cpp/version.h.in ${BIN2CPP_VERSION_HEADER} )

# config.h file
set(BIN2CPP_CONFIG_HEADER ${CMAKE_BINARY_DIR}/include/bin2cpp/config.h)
message("Generating ${BIN2CPP_CONFIG_HEADER}...")
if (BUILD_SHARED_LIBS)
  set(BIN2CPP_BUILD_TYPE_CPP_DEFINE "#define BIN2CPP_BUILT_AS_SHARED")
else()
  set(BIN2CPP_BUILD_TYPE_CPP_DEFINE "#define BIN2CPP_BUILT_AS_STATIC")
endif()
configure_file( ${CMAKE_SOURCE_DIR}/src/bin2cpp/config.h.in ${BIN2CPP_CONFIG_HEADER} )
set(BIN2CPP_BUILD_TYPE_CPP_DEFINE)

# Define installation directories
set(BIN2CPP_INSTALL_ROOT_DIR     ".")
set(BIN2CPP_INSTALL_DOC_DIR      "docs")
set(BIN2CPP_INSTALL_BIN_DIR      "bin")
set(BIN2CPP_INSTALL_LIB_DIR      "lib/bin2cpp-${BIN2CPP_VERSION}")
set(BIN2CPP_INSTALL_INCLUDE_DIR  "include/bin2cpp-${BIN2CPP_VERSION}")
set(BIN2CPP_INSTALL_CMAKE_DIR    "cmake/bin2cpp-${BIN2CPP_VERSION}") # CMake files (*.cmake) should have the same destination as the library files. Some also prefers to use "cmake".

##############################################################################################################################################
# Project settings
##############################################################################################################################################

# Build options
option(BIN2CPP_BUILD_TEST "Build all bin2cpp's unit tests" OFF)
option(BIN2CPP_BUILD_DOC "Build documentation" OFF)
option(BIN2CPP_BUILD_SAMPLES "Build bin2cpp samples" OFF)

# Force a debug postfix if none specified.
# This allows publishing both release and debug binaries to the same location
# and it helps to prevent linking with the wrong library on Windows.
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX "-d")
endif()

# Prevents annoying warnings on MSVC
if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Define include directories for source code.
# The specified values will not be exported.
include_directories(${CMAKE_BINARY_DIR}/include     # for ${BIN2CPP_VERSION_HEADER} and ${BIN2CPP_CONFIG_HEADER} generated files.
                    ${CMAKE_SOURCE_DIR}/src/common  # for common files shared by bin2cpp and unit tests.
)

# Define the file extension for bash/batch files on the target platform
if (WIN32)
  set(SCRIPT_FILE_EXTENSION bat)
else()
  set(SCRIPT_FILE_EXTENSION sh)
endif()

##############################################################################################################################################
# Dependencies
##############################################################################################################################################
find_package(GTest REQUIRED) #rapidassist requires GTest
find_package(rapidassist 0.8.1 REQUIRED)

##############################################################################################################################################
# Subprojects
##############################################################################################################################################

add_subdirectory(src/bin2cpp)

# unit tests
if(BIN2CPP_BUILD_TEST)
  add_subdirectory(test/testfilegenerator)
  add_subdirectory(test/bin2cpp_unittest)
endif()

# samples
if(BIN2CPP_BUILD_SAMPLES)
  add_subdirectory(samples/demo_helloworld)
  add_subdirectory(samples/demo_icons)
endif()

##############################################################################################################################################
# Generate doxygen documentation
# See https://vicrucann.github.io/tutorials/quick-cmake-doxygen/
##############################################################################################################################################
if (BIN2CPP_BUILD_DOC)
  # check if Doxygen is installed
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
 
    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")
 
    # note the option ALL which allows to build the docs together with the application
    add_custom_target( bin2cpp_doc ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM )
  else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
  endif (DOXYGEN_FOUND)
endif()

##############################################################################################################################################
# Install
##############################################################################################################################################

# Install locations:   See https://unix.stackexchange.com/a/36874
#   On UNIX, installs to "/usr/local".
#   On Windows, installs to "C:\Program Files (x86)\${PROJECT_NAME}" or to "C:\Program Files\${PROJECT_NAME}" for 64 bit binaries

install(DIRECTORY ${CMAKE_SOURCE_DIR}/licenses DESTINATION ${BIN2CPP_INSTALL_ROOT_DIR})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/samples DESTINATION ${BIN2CPP_INSTALL_ROOT_DIR})

install(FILES ${CMAKE_SOURCE_DIR}/README.md
              ${CMAKE_SOURCE_DIR}/CONTRIBUTING.md
              ${CMAKE_SOURCE_DIR}/AUTHORS
              ${CMAKE_SOURCE_DIR}/CHANGES
              ${CMAKE_SOURCE_DIR}/LICENSE
              DESTINATION ${BIN2CPP_INSTALL_ROOT_DIR})

install(FILES ${CMAKE_SOURCE_DIR}/docs/bin2cpp-splashscreen.png
              ${CMAKE_SOURCE_DIR}/docs/bin2cpp-v2.4.0-sample.png
              ${CMAKE_SOURCE_DIR}/docs/icon.ico
              DESTINATION ${BIN2CPP_INSTALL_DOC_DIR})

##############################################################################################################################################
# Packaging
##############################################################################################################################################

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${BIN2CPP_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR "${BIN2CPP_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${BIN2CPP_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${BIN2CPP_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "bin2cpp: The easiest way to embed small files into a c++ executable. bin2cpp converts text or binary files to C++ files (*.h, *.cpp) for easy access within the code.")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_GENERATOR "")

if (WIN32 OR MINGW)
  ######################
  # Windows packacking
  ######################
  list(APPEND CPACK_GENERATOR "ZIP")
  if (NSIS_FOUND)
    list(APPEND CPACK_GENERATOR "NSIS")
  endif (NSIS_FOUND)
  if (WIX_FOUND)
    list(APPEND CPACK_GENERATOR "WIX")
  endif (WIX_FOUND)
elseif(APPLE)
  ######################
  # macOS packacking
  ######################
  list(APPEND CPACK_GENERATOR "TGZ")
 
  # PackageMaker (Mac OSX) packaging.
  find_program(PACKAGE_MAKER "PackageMaker")
  if(PACKAGE_MAKER)
    list(APPEND CPACK_GENERATOR "PackageMaker")
  endif()
elseif(UNIX)
  ######################
  # Linux packacking
  ######################
  list(APPEND CPACK_GENERATOR "TGZ")
 
  # DEB (Debian, Ubuntu) packaging.
  find_program(DPKG "dpkg")
  if(DPKG)
   list(APPEND CPACK_GENERATOR "DEB")
    if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "i686")
     set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
    elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
     set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    endif()
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER " https://github.com/end2endzone")
    set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE " https://github.com/end2endzone/bin2cpp")
  endif()
 
  # RPM (Red Hat, CentOS) packaging.
  find_program(RPMBUILD "rpmbuild")
  if(RPMBUILD)
   list(APPEND CPACK_GENERATOR "RPM")
    if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "i686")
      set(CPACK_RPM_PACKAGE_ARCHITECTURE "i686")
    elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
      set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
    endif()
    set(CPACK_RPM_PACKAGE_GROUP " Development/Tools/Other")
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
  endif()
endif()

# On linux and macOS, change the default package filename to something like includes the system name, 
if (UNIX)
  # uname command outputs for ubuntu 18.04 :
  #   uname -a --> Linux myubuntu 5.4.0-73-generic #82~18.04.1-Ubuntu SMP Fri Apr 16 15:10:02 UTC 2021 x86_64 x86_64 x86_64 GNU/Linux
  #   CMAKE_SYSTEM_NAME,            uname -s --> Linux
  #   CMAKE_HOST_SYSTEM_PROCESSOR,  uname -m --> x86_64
  #   CMAKE_SYSTEM_PROCESSOR,       uname -p --> x86_64
  # cat /etc/os-release:
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
  
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${OS_FRIENDLY_NAME}.${CMAKE_HOST_SYSTEM_PROCESSOR}")
elseif(APPLE)
  # uname command outputs for macOS Catalina 10.15 :
  #   uname -a --> Darwin mymac 19.6.0 Darwin Kernel Version 19.6.0: Mon Aug 31 22:12:52 PDT 2020; root:xnu-6153.141.2~1/RELEASE_X86_64 x86_64
  #   CMAKE_SYSTEM_NAME,            uname -s --> Darwin
  #   CMAKE_HOST_SYSTEM_PROCESSOR,  uname -m --> x86_64
  #   CMAKE_SYSTEM_PROCESSOR,       uname -p --> i386
  # cat /etc/os-release: No such file or directory
  
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${OS_FRIENDLY_NAME}.${CMAKE_HOST_SYSTEM_PROCESSOR}")
endif()


#######################
# NSIS specific stuff #
#######################

# NSIS default installation path is :
# x86, "C:\Program Files (x86)\${CPACK_PACKAGE_NAME} ${BIN2CPP_VERSION}"
# x64, "C:\Program Files\${CPACK_PACKAGE_NAME} ${BIN2CPP_VERSION}"

set(CPACK_NSIS_CONTACT "https://github.com/end2endzone")
set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/end2endzone/bin2cpp")
set(CPACK_NSIS_HELP_LINK "https://github.com/end2endzone/bin2cpp")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

# Icon in the add/remove control panel. Must be an .exe file.
# Use the main dll as we do not have main exe.
# The filename will be wrong in debug builds.
set(CPACK_NSIS_INSTALLED_ICON_NAME docs\\\\icon.ico)

# Define our secondaries start menu shortcuts (for files that are not direct target executables).
# See the following for details:
# https://stackoverflow.com/questions/14831739/how-do-i-add-a-start-menu-entry-for-a-text-file-in-cpack-ans-nsis
# https://stackoverflow.com/questions/2640760/creating-windows-desktop-icon-in-cmake-cpack-nsis
# Note: As found with issue #60, the shortcuts created here have the 'Start in' property set to the installation directory. 
#       If a shortcut is pointing to a executable/document that is not stored in the installation root directory,
#       the created shortcut will not work.
set(CPACK_NSIS_MENU_LINKS
  "README.md"       "README.md"
  "CONTRIBUTING.md" "CONTRIBUTING.md"
  "AUTHORS"         "AUTHORS"
  "CHANGES"         "CHANGES"
  "LICENSE"         "LICENSE"
  "licenses"        "Third party licenses"
  "https://github.com/end2endzone/bin2cpp" "Bin2cpp Web Site"
)

# Define more anchor points for NSIS installer.
# SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS   "; CPACK_NSIS_EXTRA_INSTALL_COMMANDS INSERT LOCATION"    )
# SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "; CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS INSERT LOCATION"  )

# Shortcuts that must start in 'bin' directory must be created manually...
# NSIS CreateShortCut documentation: https://nsis.sourceforge.io/Reference/CreateShortCut
# Note: CMake escaping mechanism is complicated. In short, to insert a backslash, use the literal string \\\\ and to insert a double-quote character, use the literal string \\\" .
#       Also, generator expression variable such as `$<TARGET_FILE_NAME:bin2cpp_unittest>` does not work with CPACK_NSIS_CREATE_ICONS_EXTRA.
SET(CPACK_NSIS_CREATE_ICONS_EXTRA "
  SetOutPath \\\"$INSTDIR\\\\bin\\\"
  CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Run Unit Tests.lnk\\\"                 \\\"$INSTDIR\\\\bin\\\\bin2cpp_unittest${CMAKE_DEBUG_POSTFIX}.exe\\\"
  CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Generate test files.lnk\\\"            \\\"$INSTDIR\\\\bin\\\\generate_test_files.${SCRIPT_FILE_EXTENSION}\\\"
  SetOutPath \\\"$INSTDIR\\\"
  ")
SET(CPACK_NSIS_DELETE_ICONS_EXTRA "
  Delete \\\"$SMPROGRAMS\\\\$MUI_TEMP\\\\Run Unit Tests.lnk\\\"
  Delete \\\"$SMPROGRAMS\\\\$MUI_TEMP\\\\Generate test files.lnk\\\"
  ")

#######################
# WIX specific stuff #
#######################

# WIX default installation path is :
# x86, "C:\Program Files (x86)\${CPACK_PACKAGE_NAME} ${BIN2CPP_VERSION}"
# x64, "C:\Program Files\${CPACK_PACKAGE_NAME} ${BIN2CPP_VERSION}"

# Set variables for WIX installer
set(CPACK_WIX_UPGRADE_GUID "2EFE95CD-2DBF-466B-AE5A-8A802328A2F8")
set(CPACK_WIX_LICENSE_RTF  "${CMAKE_SOURCE_DIR}/docs/wix_license_template.rtf")
set(CPACK_WIX_PRODUCT_ICON "${CMAKE_SOURCE_DIR}/docs/icon.ico")
set(CPACK_WIX_HELP_LINK "https://github.com/end2endzone/bin2cpp")
set(CPACK_WIX_PROGRAM_MENU_FOLDER "bin2cpp ${bin2cpp_VERSION}")

# Do not set 'CPACK_WIX_PRODUCT_GUID' to a valid GUID (ie "C532D05F-E319-4A0E-BD41-14B0B26F245B")
# Set CPACK_WIX_UPGRADE_GUID to something fixed while leaving CPACK_WIX_PRODUCT_GUID unset
# (so that you get a unique new product GUID for every package you create).
# This would cause an existing installation (with the same upgrade GUID) to be replaced 
# (files of the old package are removed)
# rather than installed in parallel.
# See https://gitlab.kitware.com/cmake/cmake/-/issues/18081 for details.

# Define start menu shortcuts for WIX
set_property(INSTALL "bin/$<TARGET_FILE_NAME:bin2cpp_unittest>"           PROPERTY CPACK_START_MENU_SHORTCUTS "Run Unit Tests"          )
set_property(INSTALL "bin/generate_test_files.${SCRIPT_FILE_EXTENSION}"   PROPERTY CPACK_START_MENU_SHORTCUTS "Generate test files"     )
set_property(INSTALL "README.md"                                          PROPERTY CPACK_START_MENU_SHORTCUTS "README.md"               )
set_property(INSTALL "CONTRIBUTING.md"                                    PROPERTY CPACK_START_MENU_SHORTCUTS "CONTRIBUTING.md"         )
set_property(INSTALL "AUTHOR"                                             PROPERTY CPACK_START_MENU_SHORTCUTS "AUTHOR"                  )
set_property(INSTALL "CHANGES"                                            PROPERTY CPACK_START_MENU_SHORTCUTS "CHANGES"                 )
set_property(INSTALL "LICENSE"                                            PROPERTY CPACK_START_MENU_SHORTCUTS "LICENSE"                 )
set_property(INSTALL "./licenses"                                         PROPERTY CPACK_START_MENU_SHORTCUTS "Third party licenses"    ) # This one does not work!
set_property(INSTALL "https://github.com/end2endzone/bin2cpp"             PROPERTY CPACK_START_MENU_SHORTCUTS "bin2cpp Web Site"        )

# we don't want to split our program up into several things
set(CPACK_MONOLITHIC_INSTALL 1)

# This must be last
include(CPack)
