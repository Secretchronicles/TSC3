#############################################################################
# CMakeLists.txt  -  Build file
#
# Copyright © 2017 The TSC Contributors
#############################################################################
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

########################################
# Configuring CMake

cmake_minimum_required(VERSION 3.0)
project(TSC C CXX)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
include(ExternalProject)
include(GNUInstallDirs)

########################################
# TSC version number

# If you get a merge conflict below, always keep "dev" on the
# "devel" branch. The postfix is only to be altered on release
# branches.
set(TSC_VERSION_MAJOR 3)
set(TSC_VERSION_MINOR 0)
set(TSC_VERSION_PATCH 0)
set(TSC_VERSION_POSTFIX "dev")
string(TIMESTAMP TSC_COMPILE_YEAR "%Y")
string(TIMESTAMP TSC_COMPILE_MONTH "%m")
string(TIMESTAMP TSC_COMPILE_DAY "%d")

# Set TSC_VERSION_GIT if possible
execute_process(COMMAND git rev-parse --short HEAD
  WORKING_DIRECTORY ${TSC_SOURCE_DIR}
  OUTPUT_VARIABLE TSC_VERSION_GIT
  OUTPUT_STRIP_TRAILING_WHITESPACE)

########################################
# Flags & Options

option(ENABLE_NLS "Enable translations and localisations" ON)

########################################
# Compiler config

# Set standard to C++14 and disable compiler-specific
# extensions to ensure maximum portability.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

# General compiler flags
# -Wall: Give warnings
# -Wextra: Give more warnings
# -Wl,--as-needed: Linker should treat static libraries more like shread ones
# -finput-charset=UTF-8: Tell GCC that all source code files are encoded in UTF-8.
#                        This obviates the need to use the "u8" prefix on all the strings.
# -fexec-charset=UTF-8: Tell GCC to store all literal strings as UTF-8 in the executable.
#                       (Would have no effect on u8 prefixed strings as that's required by
#                       the C++ standard for them)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wl,--as-needed -finput-charset=UTF-8 -fexec-charset=UTF-8")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-inline")

# Find out the host triplet if we are crosscompiling.
# This is needed because the mruby build system uses it
# as the output directory name.
if (CMAKE_CROSSCOMPILING)
  get_filename_component(HOST_TRIPLET ${CMAKE_C_COMPILER} NAME)
  string(REGEX REPLACE "(.*)-.*$" "\\1" HOST_TRIPLET "${HOST_TRIPLET}")
  message(STATUS "Detected cross-compilation targetting: ${HOST_TRIPLET}")
endif()

########################################
# Dependencies

add_subdirectory(pathie EXCLUDE_FROM_ALL)

find_package(PNG REQUIRED)
find_package(XercesC REQUIRED)
find_package(SFML COMPONENTS window system REQUIRED)

include_directories(${TSC_SOURCE_DIR}/pathie-include)
include_directories(${SFML_INCLUDE_DIRS})
include_directories(${XercesC_INCLUDE_DIRS})
add_definitions(${SFML_DEFINITIONS})

########################################
# Configuration header

configure_file(${TSC_SOURCE_DIR}/config.hpp.in
  ${TSC_BINARY_DIR}/config.hpp)
include_directories(${TSC_BINARY_DIR})

########################################
# Source files

file(GLOB_RECURSE tsc_sources
  "src/*.cpp"
  "src/*.hpp")

file(GLOB_RECURSE tscproc_sources
  "tscproc/*.cpp"
  "tscproc/*.hpp")

########################################
# Targets

add_executable(tscproc ${tscproc_sources})
add_executable(tsc ${tsc_sources})

target_link_libraries(tsc ${SFML_LIBRARIES} ${XercesC_LIBRARIES} pathie)

# Add tscproc's compilation options only here to not confuse the main
# compilation of the 'tsc' target.
target_include_directories(tscproc PUBLIC ${XercesC_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})
target_compile_definitions(tscproc PUBLIC ${PNG_DEFINITIONS})
target_link_libraries(tscproc ${XercesC_LIBRARIES} ${PNG_LIBRARIES})

########################################
# Installation instructions

install(TARGETS tsc
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT base)
install(PROGRAMS ${TSC_SOURCE_DIR}/tscproc/tscproc-gui.sh
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT base)

########################################
# Print summary

message(STATUS "--------------- Build info summary ----------------")
message(STATUS "TSC version:    ${TSC_VERSION_MAJOR}.${TSC_VERSION_MINOR}.${TSC_VERSION_PATCH}${TSC_VERSION_POSTFIX}")
message(STATUS "Build type:     ${CMAKE_BUILD_TYPE}")
if (CMAKE_CROSSCOMPILING)
  message(STATUS "Crosscompiling: ${HOST_TRIPLET}")
else()
  message(STATUS "Crosscompiling: No")
endif()

message(STATUS "--------------- Configuration summary -------------")
message(STATUS "Enable native language support: ${ENABLE_NLS}")

message(STATUS "--------------- Path configuration -----------------")
message(STATUS "Install prefix:        ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Binary directory:      ${CMAKE_INSTALL_BINDIR}")
message(STATUS "Shared data directory: ${CMAKE_INSTALL_DATADIR}")
message(STATUS "Manpage directory:     ${CMAKE_INSTALL_MANDIR}")
message(STATUS "(relative pathes are relative to the install prefix;")
message(STATUS "the directory /tsc will be appended to the shared")
message(STATUS "data directory indicated above.)")
message(STATUS "----------------------------------------------------")
