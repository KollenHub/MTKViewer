cmake_minimum_required(VERSION 3.20)

set(LIB_NAME Core)

file(GLOB_RECURSE SOURCE "*.h" "*.cpp")

add_library(${LIB_NAME} ${SOURCE})
add_library(KN::${LIB_NAME} ALIAS ${LIB_NAME})


target_include_directories(${LIB_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${LIB_NAME} PUBLIC spdlog::spdlog)


