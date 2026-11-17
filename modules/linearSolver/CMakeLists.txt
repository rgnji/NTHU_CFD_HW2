add_library(linearSolver OBJECT
    src/linearSolver.cpp
    src/GaussSeidel.cpp
)

target_include_directories(linearSolver PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(linearSolver PUBLIC 
    utility
)