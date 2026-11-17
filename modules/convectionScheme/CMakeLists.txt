add_library(convectionScheme OBJECT
    src/Register.cpp
)

target_include_directories(convectionScheme PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(convectionScheme PUBLIC 
    utility
    field
    utilHeader
)