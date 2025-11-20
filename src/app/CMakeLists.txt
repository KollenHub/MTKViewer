cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE SOURCES "*.h" "*.qrc" "*.cpp" "s*.hpp" "*.ui")

add_executable(${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME} PRIVATE
    DCMTK::dcmdata
    DCMTK::dcmjpeg
    DCMTK::dcmimgle
    DCMTK::dcmimage
    DCMTK::i2d
    KN::Core
    KN::Extension
    ${VTK_LIBRARIES}
)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

#QT依赖工具
get_nth_parent_directory(${Qt5_DIR} 3 QT_ROOT)
set(QT_DepExe "${QT_ROOT}/bin/windeployqt.exe")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${QT_DepExe} "$<TARGET_FILE:${PROJECT_NAME}>")

vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
)
