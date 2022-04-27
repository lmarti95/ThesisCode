cmake_minimum_required(VERSION 2.8.12)

project (ThesisCode)

file(GLOB source
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

IF(NOT GRAPHICS)
	set(GRAPHICS 0)
ENDIF()

add_executable(ThesisCode ${source})

target_include_directories(ThesisCode PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(ThesisCode PUBLIC GRAPHICS=${GRAPHICS})
target_link_libraries(ThesisCode PUBLIC pthread)

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-sign-compare -Wno-delete-non-virtual-dtor -std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)
