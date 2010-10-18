# File name: itkVideoDashboardDriverScript.cmake
#
# OS: Windows Vista Buisness
# Hardware: 
# GPU: NA

# Note: The specific version and processor type of this machine should be reported in the
# header above. Indeed, this file will be send to the dashboard as a NOTE file.

cmake_minimum_required(VERSION 2.8)

#
# For additional information,
# See http://github.com/bcd123/ITK-Vid-A2D2
#

#
# Dashboard properties
#
set(MY_OPERATING_SYSTEM "Windows") # Windows, Linux, Darwin...
set(MY_COMPILER "MSVS.9.0.30729.1")
set(MY_ITK_VERSION "3.20")
set(CTEST_SITE "Westeros.kitware") # for example: mymachine.kitware, mymachine.dkfz, ...
set(CTEST_DASHBOARD_ROOT "C:/Dashboard")
set(CTEST_CMAKE_COMMAND "C:/Program Files/CMake 2.8/bin/cmake")
set(CTEST_CMAKE_GENERATOR "MSVC Solution files")

#
# Dashboard options
#
set(WITH_KWSTYLE FALSE)
set(WITH_MEMCHECK FALSE)
set(CTEST_MEMORYCHECK_COMMAND "")
set(WITH_COVERAGE FALSE)
set(WITH_DOCUMENTATION FALSE)
#set(DOCUMENTATION_ARCHIVES_OUTPUT_DIRECTORY ) # for example: $ENV{HOME}/Projects/Doxygen
set(CTEST_BUILD_CONFIGURATION "Debug")
set(CTEST_TEST_TIMEOUT 500)
set(CTEST_BUILD_FLAGS "-j4") # Use multiple CPU cores to build

# experimental:
# - run_ctest() macro will be called *ONE* time
# - binary directory will *NOT* be cleaned
# continuous:
# - run_ctest() macro will be called EVERY 5 minutes ...
# - binary directory will *NOT* be cleaned
# - configure/build will be executed *ONLY* if the repository has been updated
# nightly:
# - run_ctest() macro will be called *ONE* time
# - binary directory *WILL BE* cleaned
set(SCRIPT_MODE "experimental") # "experimental", "continuous", "nightly"

#
# Project specific properties
#
set(CTEST_SOURCE_DIRECTORY "${CTEST_DASHBOARD_ROOT}/ITKVideo")
set(CTEST_BINARY_DIRECTORY "${CTEST_DASHBOARD_ROOT}/ITKVideo-${CTEST_BUILD_CONFIGURATION}-${SCRIPT_MODE}")

#
# Additionnal CMakeCache options
#
set(ADDITIONNAL_CMAKECACHE_OPTION "")

# List of test that should be explicitly disabled on this machine
set(TEST_TO_EXCLUDE_REGEX "")

# set any extra environment variables here
set(ENV{DISPLAY} ":0")
set(ENV{CXX} "cl")
set(ENV{CC} "cl")

find_program(CTEST_COVERAGE_COMMAND NAMES gcov)
find_program(CTEST_GIT_COMMAND NAMES git REQUIRED)

##########################################
# WARNING: DO NOT EDIT BEYOND THIS POINT #
##########################################

set(CTEST_NOTES_FILES "${CTEST_SCRIPT_DIRECTORY}/${CTEST_SCRIPT_NAME}")

#
# Project specific properties
#
set(CTEST_PROJECT_NAME "ITKVideo")
set(CTEST_BUILD_NAME "${MY_OPERATING_SYSTEM}-${MY_COMPILER}-ITK${MY_QT_VERSION}-${CTEST_BUILD_CONFIGURATION}")

#
# Display build info
#
message("site name: ${CTEST_SITE}")
message("build name: ${CTEST_BUILD_NAME}")
message("script mode: ${SCRIPT_MODE}")
message("coverage: ${WITH_COVERAGE}, memcheck: ${WITH_MEMCHECK}")

#
# Convenient macro allowing to download a file
#
MACRO(downloadFile url dest)
  FILE(DOWNLOAD ${url} ${dest} STATUS status)
  LIST(GET status 0 error_code)
  LIST(GET status 1 error_msg)
  IF(error_code)
    MESSAGE(FATAL_ERROR "error: Failed to download ${url} - ${error_msg}")
  ENDIF()
ENDMACRO()

#
# Download and include dashboard driver script
#
set(url http://github.com/bcd123/ITK-Vid-A2D2/raw/master/A2D2/itkVideoDashboardDriverScript.cmake)
set(dest ${CTEST_SCRIPT_DIRECTORY}/${CTEST_SCRIPT_NAME}.driver)
downloadFile(${url} ${dest})
INCLUDE(${dest})