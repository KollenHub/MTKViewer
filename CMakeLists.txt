cmake_minimum_required(VERSION 3.20)

project(mtkViewer)

include(cmake/package.cmake)
include(cmake/directory.cmake)

if(MSVC)
    add_compile_options("/utf-8")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(Qt5 CONFIG REQUIRED COMPONENTS Core Widgets)

find_package(DCMTK CONFIG REQUIRED)

if(TARGET dcmdata)
    message("Exsit DCMTK")
endif()

find_package(spdlog CONFIG REQUIRED)

find_package(VTK REQUIRED CONFIG COMPONENTS
    CommonCore
    RenderingOpenGL2
    GUISupportQt
)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_AUTORCC TRUE)
#包含当前生成的路径
include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE SOURCES "src/*.h" "src/*.qrc" "src/*.cpp" "src/*.hpp" "src/*.ui")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Widgets
    DCMTK::dcmdata
    DCMTK::dcmjpeg
    DCMTK::dcmimgle
    DCMTK::dcmimage
    spdlog::spdlog
    ${VTK_LIBRARIES}
)
target_link_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PRIVATE src)

set(BINARIES "")
#获取binary
get_nth_parent_directory(${DCMTK_DIR} MTK_ROOT)
set(MTK_BIN ${MTK_ROOT}/bin/${CMAKE_BUILD_TYPE})
file(GLOB MTK_BINARIES "${MTK_BIN}/*.dll")
list(APPEND BINARIES ${MTK_BINARIES})

get_nth_parent_directory(${spdlog_DIR} 3 SPDLOG_ROOT)
set(SPDLOG_BIN ${SPDLOG_ROOT}/bin)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(GLOB SPDLOG_BINARIES "${SPDLOG_BIN}/*.dll")
    list(APPEND BINARIES ${SPDLOG_BINARIES})
else()
    file(GLOB SPDLOG_BINARIES "${SPDLOG_BIN}/*[^d].dll")
    list(APPEND BINARIES ${SPDLOG_BINARIES})
endif()

get_nth_parent_directory(${VTK_DIR} 3 VTK_ROOT)
set(VTK_BIN ${VTK_ROOT}/bin)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(GLOB VTK_BINARIES "${VTK_BIN}/*d.dll")
    list(APPEND BINARIES ${VTK_BINARIES})
else()
    file(GLOB VTK_BINARIES "${VTK_BIN}/*[^d].dll")
    list(APPEND BINARIES ${VTK_BINARIES})
endif()

#判断
if(NOT EXISTS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/spdlog.dll" AND NOT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/spdlogd.dll")
    file(COPY ${BINARIES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
else()
    message("Exsit Binaries")
endif()

#QT依赖工具
get_nth_parent_directory(${Qt5_DIR} 3 QT_ROOT)
set(QT_DepExe "${QT_ROOT}/bin/windeployqt.exe")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${QT_DepExe} "$<TARGET_FILE:${PROJECT_NAME}>")

vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
)
