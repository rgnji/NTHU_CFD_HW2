add_library(utility STATIC
    src/lduMatrix.cpp
    src/orthoSquareMesh.cpp
    src/Patch.cpp
)

target_include_directories(utility PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/third_party"
)