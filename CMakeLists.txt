cmake_minimum_required(VERSION 3.14)

project(containers)

#set_target_properties(containers PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)       
set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED TRUE)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-g -Wall -Wextra -Werror -pedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/EHsc /W4 /WX)
endif()

set(COMMON_SRCS
    vector2.hpp 
    list2.hpp
)

add_subdirectory(tests)

add_executable(containers main.cpp ${COMMON_SRCS})


