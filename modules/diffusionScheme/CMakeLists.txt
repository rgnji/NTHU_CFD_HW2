add_library(diffusionScheme STATIC src/*.cpp)

target_include_directories(diffusionScheme PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(diffusionScheme PRIVATE utility)