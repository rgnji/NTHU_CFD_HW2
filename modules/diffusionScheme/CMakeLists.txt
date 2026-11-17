add_library(diffusionScheme INTERFACE)

target_include_directories(diffusionScheme INTERFACE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(diffusionScheme INTERFACE 
    utilHeader
    field
    utility
)