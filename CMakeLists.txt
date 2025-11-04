cmake_minimum_required(VERSION 3.30)
project(SDS VERSION 1.2.1 LANGUAGES C)
add_library(${PROJECT_NAME} STATIC
    src/sds_Memory.c
    src/sds_string.c
    src/sds_vector.c

    src/math/vec2.c
    src/math/vec3.c
    src/math/vec4.c
)

target_include_directories(${PROJECT_NAME} PUBLIC src/)
target_compile_features(${PROJECT_NAME} PRIVATE c_std_11)