cmake_minimum_required(VERSION 3.10)
project(NTHU_CFD_HW1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE LIB_SOURCES
    CONFIGURE_DEPENDS
    "lib/*.cpp"
)

if(MSVC)
    add_compile_options(/utf-8)
endif()

add_executable(NTHU_CFD_HW1
    main.cpp
    ${LIB_SOURCES}
)

target_include_directories(NTHU_CFD_HW1 PRIVATE 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib
)

