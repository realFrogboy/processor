cmake_minimum_required(VERSION 3.13.4)
project(disassembler)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set (COMPILER_SETTINGS  -g -Wall  -Wextra -Wpedantic)
    
else ()
    set (COMPILER_SETTINGS -Wall  -Wextra -Wpedantic)

endif ()

add_compile_options(${COMPILER_SETTINGS})

add_executable(${PROJECT_NAME}   
  sources/disassembler.cpp sources/disassem.cpp ../utility/sources/oneggin.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE includes)
target_include_directories(${PROJECT_NAME} PRIVATE ../utility/includes)