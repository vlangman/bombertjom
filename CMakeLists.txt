cmake_minimum_required (VERSION 3.11 FATAL_ERROR)

project(Bomberman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Bstatic -pthread")

set(FETCHCONTENT_QUIET OFF)
set(AUDIOLIB ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/irrklang/bin/macosx-gcc)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps)

include(FetchContent)

set(AUDIO_INSTALL_DIR ${EXTERNAL_INSTALL_LOCATION}/irrklang)
#start audio 
FetchContent_Declare(
	irrklang
	INSTALL_DIR ${AUDIO_INSTALL_DIR}
	SOURCE_DIR ${AUDIO_INSTALL_DIR}
	URL "http://www.ambiera.at/downloads/irrKlang-64bit-1.5.0.zip"
)

FetchContent_GetProperties(irrklang)

if(NOT irrklang_POPULATED)
	FetchContent_Populate(irrklang)
	# set(IRRKLANG_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	# set(IRRKLANG_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	# set(IRRKLANG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
endif()
#end audio

FetchContent_Declare(
	glfw
	URL "https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.zip"
)

FetchContent_GetProperties(glfw)

if(NOT glfw_POPULATED)
	FetchContent_Populate(glfw)
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

FetchContent_Declare(
	glad
	GIT_REPOSITORY https://github.com/Dav1dde/glad.git
	GIT_TAG origin/c
)

FetchContent_GetProperties(glad)

if(NOT glad_POPULATED)
	FetchContent_Populate(glad)
endif()

FetchContent_Declare(
	stb_image
	GIT_REPOSITORY https://github.com/jschmidt42/stb-cmake.git
)

FetchContent_GetProperties(stb_image)

if(NOT stb_image_POPULATED)
  FetchContent_Populate(stb_image)
endif()

FetchContent_Declare(
	glm
	URL https://github.com/g-truc/glm/archive/0.9.7.0.zip
)

FetchContent_GetProperties(glm)

if(NOT glm_POPULATED)
	FetchContent_Populate(glm)
endif()

FetchContent_Declare(
	assimp
	URL https://github.com/assimp/assimp/archive/master.zip
)

FetchContent_GetProperties(assimp)

if(NOT assimp_POPULATED)
	FetchContent_Populate(assimp)
	set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
	set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR})
endif()

FetchContent_Declare(
	freetype2
	URL https://download.savannah.gnu.org/releases/freetype/freetype-2.9.1.tar.gz
)

FetchContent_GetProperties(freetype2)

if(NOT freetype2)
	FetchContent_Populate(freetype2)
	add_subdirectory(${freetype2_SOURCE_DIR} ${freetype2_BINARY_DIR})
endif()

#---------------------------------------------------------

# - Try to find irrKlang
# Once done this will define
#
#  IRRKLANG_FOUND - system has irrKlang
#  IRRKLANG_INCLUDE_DIRS - the irrKlang include directory
#  IRRKLANG_LIBRARIES - Link these to use irrKlang
#  IRRKLANG_DEFINITIONS - Compiler switches required for using irrKlang
#
#  Copyright (c) 2006 Andreas Schneider <mail@cynapses.org>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


if (IRRKLANG_LIBRARIES AND IRRKLANG_INCLUDE_DIRS)
  # in cache already
  set(IRRKLANG_FOUND TRUE)
else (IRRKLANG_LIBRARIES AND IRRKLANG_INCLUDE_DIRS)
  find_path(IRRKLANG_INCLUDE_DIR
    NAMES
      irrKlang.h
    PATHS
      ${AUDIO_INSTALL_DIR}/include
  )

  find_library(IRRKLANG_LIBRARY
    NAMES
      IrrKlang
    PATHS
      ${AUDIOLIB}
  )

  if (IRRKLANG_LIBRARY)
    set(IRRKLANG_FOUND TRUE)
  endif (IRRKLANG_LIBRARY)

  set(IRRKLANG_INCLUDE_DIRS
    ${IRRKLANG_INCLUDE_DIR}
  )

  if (IRRKLANG_FOUND)
    set(IRRKLANG_LIBRARIES
      ${IRRKLANG_LIBRARIES}
      ${IRRKLANG_LIBRARY}
    )
  endif (IRRKLANG_FOUND)

  if (IRRKLANG_INCLUDE_DIRS AND IRRKLANG_LIBRARIES)
     set(IRRKLANG_FOUND TRUE)
  endif (IRRKLANG_INCLUDE_DIRS AND IRRKLANG_LIBRARIES)

  if (IRRKLANG_FOUND)
    if (NOT IRRKLANG_FIND_QUIETLY)
      message(STATUS "Found irrKlang: ${IRRKLANG_LIBRARIES}: ${IRRKLANG_INCLUDE_DIRS}")
    endif (NOT IRRKLANG_FIND_QUIETLY)
  else (IRRKLANG_FOUND)
    if (IRRKLANG_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find irrKlang")
    endif (IRRKLANG_FIND_REQUIRED)
  endif (IRRKLANG_FOUND)

  # show the IRRKLANG_INCLUDE_DIRS and IRRKLANG_LIBRARIES variables only in the advanced view
  mark_as_advanced(IRRKLANG_INCLUDE_DIRS IRRKLANG_LIBRARIES)

endif (IRRKLANG_LIBRARIES AND IRRKLANG_INCLUDE_DIRS)

#---------------------------------------------------------


include_directories(
	${glad_SOURCE_DIR}/include 
	${glfw_SOURCE_DIR}/include 
	${std_image_SOURCE_DIR} 
	${glm_SOURCE_DIR}
	${assimp_SOURCE_DIR}/include
	${freetype2_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/include
	${IRRKLANG_INCLUDE_DIRS}
)

file(GLOB SOURCES src/*.cpp)

add_executable(
	Bomberman

	GameEngine/graphics/src/Functions.cpp
	GameEngine/graphics/src/Shaders.cpp
	# GameEngine/graphics/src/Textures.cpp
	GameEngine/graphics/src/Camera.cpp
	GameEngine/graphics/src/Mesh.cpp
	GameEngine/graphics/src/Models.cpp
	GameEngine/utils/src/fileutils.cpp
	GameEngine/utils/src/timer.cpp
	GameEngine/utils/src/Skybox.cpp
	${SOURCES}
	${glad_SOURCE_DIR}/src/glad.c
)

# target_link_libraries(Bomberman ${AUDIO_INSTALL_DIR}/libIrrKlang.so)
# target_link_libraries(Bomberman ${AUDIOLIB}/ikpFlac.so)
# target_link_libraries(Bomberman ${AUDIOLIB}/ikpMP3.so)


find_package(OpenGL REQUIRED)
target_link_libraries(Bomberman ${OPENGL_gl_LIBRARY})
if (OPENGL_GLU_FOUND)
	target_link_libraries(Bomberman ${OPENGL_glu_LIBRARY})
endif()

message(${IRRKLANG_LIBRARIES})

target_link_libraries(Bomberman glfw assimp ${IRRKLANG_LIBRARIES} freetype)