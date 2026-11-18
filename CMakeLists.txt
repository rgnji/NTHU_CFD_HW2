cmake_minimum_required(VERSION 4.4.4)
project(NTHU_CFD_HW2 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

add_subdirectory(modules/utility)
add_subdirectory(modules/utilHeader)
add_subdirectory(modules/field)
add_subdirectory(modules/linearSolver)
add_subdirectory(modules/diffusionScheme)
add_subdirectory(modules/convectionScheme)
add_subdirectory(modules/boundaryCondition)

add_subdirectory(src)

target_compile_definitions(NTHU_CFD_HW2 PRIVATE
    PROJECT_DIR="${CMAKE_SOURCE_DIR}"
)