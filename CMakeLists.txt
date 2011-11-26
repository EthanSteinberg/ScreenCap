cmake_minimum_required(VERSION 2.6)

project(GLFont)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
   message(FATAL_ERROR "ERROR: Building in source, please build out of it")
endif()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -std=c++0x -g3  -O0 -Werror -Wshadow -std=c++0x -Wextra -lswscale -lavformat -lsnappy  ")



set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")	

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/arch)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(Boost_ADDITIONAL_VERSIONS "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )
set(BOOST_COMPONENTS system thread date_time)

find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_library( AVCODEC_LIBRARY
  NAMES x264
  HINTS
  PATH_SUFFIXES lib64 lib
  PATHS
  /usr/
  /opt/local
)
message("Avcodec is ${AVCODEC_LIBRARY}")

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_PATH})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_PATH})

find_package(GTK3 3.0 COMPONENTS  gtk gtkmm)
include_directories(${GTK3_INCLUDE_DIRS})

add_subdirectory(src) 


