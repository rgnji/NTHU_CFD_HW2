add_executable(NTHU_CFD_HW2 main.cpp)

target_link_libraries(NTHU_CFD_HW2 PRIVATE
    utility
    linearSolver
    diffusionScheme
    convectionScheme
)