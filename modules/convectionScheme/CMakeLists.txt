add_library(convectionScheme STATIC src/*.cpp)

target_include_directories(convectionScheme PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(convectionScheme PRIVATE utility)