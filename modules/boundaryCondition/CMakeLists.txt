add_library(boundaryCondition SHARED src/Register.cpp)

target_include_directories(boundaryCondition PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(boundaryCondition PRIVATE utility)