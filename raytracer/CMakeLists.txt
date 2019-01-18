cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE debug)
project(Raytracer)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

set(SOURCES main.c
            Vector.c
            Ray.c
            Matrix.c
            Figure.c
            Solid.c
            Reflect.c
            Shader.c)

add_executable(Raytracer main.c ${SOURCES})
target_link_libraries(Raytracer ${SDL2_LIBRARIES})
target_link_libraries(Raytracer m)
