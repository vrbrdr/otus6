cmake_minimum_required(VERSION 3.7.2)

project(containers)

add_executable(containers
        main.cpp vector2.hpp list2.hpp
        cint.hpp)

set_target_properties(containers PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)        
