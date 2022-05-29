cmake_minimum_required(VERSION 2.8.12)

project (ThesisCode)

file(GLOB source
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)


IF(NOT WIN32 AND GRAPHICS)
	message(STATUS "Only on windows the graphics is supported!")
ENDIF()

IF(NOT WIN32 OR NOT GRAPHICS)
	set(GRAPHICS 0)
ENDIF()

IF(GRAPHICS)

	find_package(OpenGL REQUIRED)

	find_library(libglfw NAMES glfw3 HINTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/lib-mingw-w64/)
	find_library(libglew32 NAMES glew32 HINTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLEW/lib/Release/Win32/)

ENDIF()

add_executable(ThesisCode ${source})

target_include_directories(ThesisCode PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(ThesisCode PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLEW/include)
target_include_directories(ThesisCode PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include)
target_include_directories(ThesisCode PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLText/include)

target_compile_definitions(ThesisCode PUBLIC GRAPHICS=${GRAPHICS})

IF(GRAPHICS)
	target_link_libraries(ThesisCode PUBLIC pthread ${libglfw} ${libglew32} ${OPENGL_LIBRARY})
ELSE()
	target_link_libraries(ThesisCode PUBLIC pthread)
ENDIF()
