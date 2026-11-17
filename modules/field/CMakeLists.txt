add_library(field INTERFACE)

target_include_directories(field INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(field INTERFACE
    utility
)