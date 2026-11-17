add_library(utilHeader INTERFACE)

target_include_directories(utilHeader INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(utilHeader INTERFACE
    utility
    linearSolver
    field
)