cmake_minimum_required(VERSION 3.30)
project(SDS LANGUAGES C)
add_library(${PROJECT_NAME} STATIC
    sds_Window.c
    sds_Memory.c
    sds_string.c
)
target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${PROJECT_NAME} PRIVATE c_std_11)
