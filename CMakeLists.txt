cmake_minimum_required (VERSION 3.11 FATAL_ERROR)
project(bombertjom)

include(FetchContent)

file(GLOB SOURCES src/*.cpp)
add_executable(bombertjom ${SOURCES})
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib)


if(UNIX AND APPLE)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11 -fPIC -stdlib=libc++")
	message(STATUS "Compiling for unix osx system")
	

	#set the download directory where sfml will be installed
	set(SDL_INSTALL_DIR ${EXTERNAL_INSTALL_LOCATION}/SDL/build)
	
	#download project SDL install Frameworks and dependancies
	FetchContent_Declare(
		sdl
		INSTALL_DIR ${SDL_INSTALL_DIR}
		SOURCE_DIR ${SDL_INSTALL_DIR}
		URL http://www.libsdl.org/release/SDL2-2.0.8.tar.gz
	)

	message(STATUS "fetching sdl")
	FetchContent_GetProperties(sdl)
	if(NOT sdl_POPULATED)
	  FetchContent_Populate(sdl)
	  set(BUILD_SHARED_LIBS TRUE)
	  add_subdirectory(${sdl_SOURCE_DIR} ${SDL_INSTALL_DIR})
	endif()

	include_directories(${sdl_SOURCE_DIR}/include)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

	
	#build sdl dynamic libraries
	add_custom_target(
		sdlBuild
		COMMAND make
		WORKING_DIRECTORY ${SDL_INSTALL_DIR}
	)
	
	add_dependencies(bombertjom sdlBuild)
	#link my libraries to dylibs
	target_link_libraries(bombertjom ${EXTERNAL_INSTALL_LOCATION}/SDL/build/libSDL2.dylib)


endif()
