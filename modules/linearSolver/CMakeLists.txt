add_library(linearSolver STATIC
    src/GaussSeidel.cpp
    src/Jacobi.cpp
)

target_include_directories(linearSolver PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(linearSolver PRIVATE utility)