cmake_minimum_required(VERSION 3.20)

set(LIB_NAME Extension)

file(GLOB_RECURSE SOURCE "*.h" "*.cpp")

add_library(${LIB_NAME} ${SOURCE})
add_library(KN::${LIB_NAME} ALIAS ${LIB_NAME})

target_link_libraries(${LIB_NAME} PUBLIC
    KN::Core
    Qt5::Core
    Qt5::Widgets
    ${VTK_LIBRARIES})


vtk_module_autoinit(
    TARGETS ${LIB_NAME}
    MODULES ${VTK_LIBRARIES}
)