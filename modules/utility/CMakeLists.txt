add_library(utility STATIC
    src/lduMatrix.cpp
    src/nodeScalarField.cpp
    src/scalarField.cpp
)

target_include_directories(utility PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
