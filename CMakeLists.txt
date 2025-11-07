cmake_minimum_required(VERSION 3.20)

project(mtkViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include(cmake/package.cmake)
include(cmake/directory.cmake)

if(MSVC)
    add_compile_options("/utf-8" "/std:c++17")
    add_compile_options("/Zc:__cplusplus")
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
    RenderingCore
    RenderingOpenGL2
    GUISupportQt
    InteractionStyle
)
message("VTK_LIBRARIES: ${VTK_LIBRARIES}")
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_AUTORCC TRUE)
#包含当前生成的路径
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#添加子文件夹
add_subdirectory(src/core)
add_subdirectory(src/extension)
add_subdirectory(src/app)


set(BINARIES "")
#获取binary
get_nth_parent_directory(${DCMTK_DIR} MTK_ROOT)
message(STATUS "MTK_ROOT: ${MTK_ROOT}")
# set(MTK_BIN ${MTK_ROOT}/bin/${CMAKE_BUILD_TYPE})
set(MTK_BIN ${MTK_ROOT}/bin)
file(GLOB MTK_BINARIES "${MTK_BIN}/*.dll")
list(APPEND BINARIES ${MTK_BINARIES})

get_nth_parent_directory(${spdlog_DIR} 3 SPDLOG_ROOT)
message(STATUS "SPDLOG_ROOT: ${SPDLOG_ROOT}")
set(SPDLOG_BIN ${SPDLOG_ROOT}/bin)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(GLOB SPDLOG_BINARIES "${SPDLOG_BIN}/*.dll")
    list(APPEND BINARIES ${SPDLOG_BINARIES})
else()
    file(GLOB SPDLOG_BINARIES "${SPDLOG_BIN}/*[^d].dll")
    list(APPEND BINARIES ${SPDLOG_BINARIES})
endif()

get_nth_parent_directory(${VTK_DIR} 3 VTK_ROOT)
message(STATUS "VTK_ROOT: ${VTK_ROOT}")
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

